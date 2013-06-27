#include "igContext.h"

#include <iostream>

static igIdent nullId = GEN_NULL_ID;

void sysSetCliboardString(std::string str);
std::string sysGetCliboardString();

using namespace igSizing;

int gfxCharAt(float x, float y, const char* text, int style, float mouseX);

igContext::igContext()
{
	hotItem = nullId;
	activeItem = nullId;
	keyboardItem = nullId;
	dragItem = nullId;

	mouseWheel = 0;
	leftDown = leftLastDown = false;

	charEntered = 0;
	backspace = false;

	textCharPos = 0;
	textCharPos2 = 0;

	dragPointer = 0;

	shift = false;
	ctrlC = false;
	ctrlX = false;
	ctrlV = false;
	ctrlA = false;
}

bool igContext::MouseInside(float x, float y, float width, float height)
{
	if(MouseClipped())
		return false;

    return mouseX >= x && mouseY >= y && mouseX <= x+width && mouseY <= y+height;
}

bool igContext::MouseClipped()
{
	if(!currentMouseClipping.active)
		return false;

	if(mouseX < currentMouseClipping.x)
		return true;

	if(mouseY < currentMouseClipping.y)
		return true;

	if(mouseX > currentMouseClipping.x+currentMouseClipping.width)
		return true;

	if(mouseY > currentMouseClipping.y+currentMouseClipping.height)
		return true;

	return false;
}

void igContext::Begin()
{
	canDrop = false;
	dragMissing = true;
	hotItem = nullId;
	if(textCharPos < 0)
		textCharPos = 0;
	if(textCharPos2 < 0)
		textCharPos2 = 0;
	currentMouseClipping.active = false;
}

igDragged igContext::End()
{
	if(leftDown == false)
	{
		activeItem = nullId;
	}

	charEntered = 0;
	backspace = false;

	if(dragMissing)
	{
		dragItem = nullId;
		dragPointer = 0;
	}

	igDragged dragged;
	if(dragItem != nullId && dragMoved)
	{
		dragged.canDrop = canDrop;
		dragged.show = true;
		dragged.title = dragTitle;
		dragged.rect = dragRect;
	}

	mouseWheel = 0;
	leftLastDown = leftDown;

	ctrlC = false;
	ctrlX = false;
	ctrlV = false;
	ctrlA = false;

	return dragged;
}

bool igContext::LeftJustUp()
{
	return leftLastDown && !leftDown;
}

igButton igContext::Button(igIdent id, float x, float y,
	float width, float height, const char* title)
{
	igButton button;
	button.rect = igRect(x, y, width, height);
	button.title = title;
    if(MouseInside(x, y, width, height))
    {
		hotItem = id;
		if(leftDown && activeItem == nullId)
			activeItem = id;
		button.hover = true;
	}

	// draw button
	int state = igItemStates::NONE;
	if(activeItem == id) state |= igItemStates::PRESSED;
	if(hotItem == id) state |= igItemStates::HOVER;

	button.clicked = leftDown == false && hotItem == id && activeItem == id;
	button.down = leftDown && activeItem == id;

	return button;
}

igCheckbox igContext::Checkbox(igIdent id, float x, float y, float width, float height, bool value)
{
	igCheckbox checkbox;
	checkbox.rect = igRect(x, y, width, height);
	checkbox.value = value;
	if(MouseInside(x, y, width, height))
	{
		hotItem = id;
		if(leftDown && activeItem == nullId)
			activeItem = id;
		checkbox.hover = true;
	}

	// draw checkbox
	int state = igItemStates::NONE;
	if(activeItem == id) state |= igItemStates::PRESSED;
	if(hotItem == id) state |= igItemStates::HOVER;

	checkbox.clicked = leftDown == false && hotItem == id && activeItem == id;
	checkbox.down = leftDown && activeItem == id;

	return checkbox;
}

igVScroll igContext::VScroll(igIdent id, float x, float y, float width, float height, float aspect, float& value)
{
	float prevValue = value;

	if(MouseInside(x, y, width, height))
	{
		hotItem = id;
		if(leftDown && activeItem == nullId)
		{
			activeItem = id;
		}
	}

	if(leftDown && activeItem == id)
	{
		value = (mouseY - y)/height - aspect/2.0f;
		if(value < 0) value = 0;
		if(value > 1.0f - aspect) value = 1.0f - aspect;
	}

	igVScroll scrollbar;
	scrollbar.changed = prevValue != value;
	scrollbar.rect = igRect(x, y, width, height);
	scrollbar.aspect = aspect;
	scrollbar.value = value;

	return scrollbar;
}

igTextbox igContext::TextBox(igIdent id, float x, float y, float width, float height,
						std::string& value, const std::string& charset)
{
	igTextbox textbox;
	std::string prevValue = value;

	if(MouseInside(x, y, width, height))
	{
		hotItem = id;
		textbox.hover = true;
		if(leftDown && activeItem == nullId)
		{
			activeItem = id;
			keyboardItem = id;
			textCharPos2 = textCharPos = gfxCharAt(x+width/2.0f, y + height/2.0f, value.c_str(), 0, mouseX);
		}
	} else if(leftDown && !leftLastDown)
	{
		if(keyboardItem == id)
			keyboardItem = nullId;
		if(activeItem == id)
			activeItem = nullId;
	}
	if(leftDown && activeItem == id)
	{
		textCharPos2 = gfxCharAt(x+width/2.0f, y + height/2.0f, value.c_str(), 0, mouseX);
	}

	if(keyboardItem == id)
	{
		if(textCharPos > value.size())
			textCharPos = value.size();

		if(textCharPos2 > value.size())
			textCharPos2 = value.size();

		if(charEntered)
		{
			for(int i=0; i<charset.size(); i++)
				if(charset[i] == charEntered)
				{
					int pipePos1 = textCharPos;
					int pipePos2 = textCharPos2;

					if(pipePos1 > pipePos2)
						std::swap(pipePos1, pipePos2);

					value.erase(pipePos1, pipePos2-pipePos1);
					value.insert(pipePos1, 1, (char)charEntered);
					pipePos1++;
					textCharPos2 = textCharPos = pipePos1;
					break;
				}
		}

		if(charEntered == 8 && (textCharPos > 0 || textCharPos!=textCharPos2)) // backspace
		{
			if(textCharPos != textCharPos2)
			{
				int pipePos1 = textCharPos;
				int pipePos2 = textCharPos2;

				if(pipePos1 > pipePos2)
					std::swap(pipePos1, pipePos2);

				value.erase(pipePos1, pipePos2-pipePos1);
				textCharPos2 = textCharPos = pipePos1;
			} else
			{
				textCharPos--;
				textCharPos2 = textCharPos;
				value.erase(textCharPos, 1);
			}
		}

		if(charEntered == 127) // delete
		{
			if(textCharPos != textCharPos2)
			{
				int pipePos1 = textCharPos;
				int pipePos2 = textCharPos2;

				if(pipePos1 > pipePos2)
					std::swap(pipePos1, pipePos2);

				value.erase(pipePos1, pipePos2-pipePos1);
				textCharPos2 = textCharPos = pipePos1;
			} else
				value.erase(textCharPos, 1);
		}

		if(ctrlC)
		{
			int textPos1 = textCharPos < textCharPos2 ? textCharPos : textCharPos2;
			int textPos2 = textCharPos > textCharPos2 ? textCharPos : textCharPos2;
			if(textPos1 != textPos2)
				sysSetCliboardString(value.substr(textPos1, textPos2-textPos1));
			else
				sysSetCliboardString(value);
		}

		if(ctrlX)
		{
			int textPos1 = textCharPos < textCharPos2 ? textCharPos : textCharPos2;
			int textPos2 = textCharPos > textCharPos2 ? textCharPos : textCharPos2;
			if(textPos1 != textPos2)
			{
				sysSetCliboardString(value.substr(textPos1, textPos2-textPos1));
				value.erase(textPos1, textPos2-textPos1);
				textCharPos2 = textCharPos = textPos1;
			}
		}

		if(ctrlV)
		{
			int textPos1 = textCharPos < textCharPos2 ? textCharPos : textCharPos2;
			int textPos2 = textCharPos > textCharPos2 ? textCharPos : textCharPos2;
			if(textPos1 != textPos2)
			{
				value.erase(textPos1, textPos2-textPos1);
				textCharPos2 = textCharPos = textPos1;
			}

			std::string clipboardText = sysGetCliboardString();
			std::string allowedChars;
			if(clipboardText.empty() == false) allowedChars.reserve(clipboardText.size());
			for(int i=0; i<clipboardText.size(); i++)
			{
				for(int j=0; j<charset.size(); j++)
					if(charset[j] == clipboardText[i])
						allowedChars += clipboardText[i];
			}

			value = value.substr(0, textPos1) + allowedChars + value.substr(textPos1);
			textCharPos2 = textCharPos = textPos1 + allowedChars.size();
		}

		if(ctrlA)
		{
			textCharPos = 0;
			textCharPos2 = value.size();
		}
	}

	int state = igItemStates::NONE;
	if(keyboardItem == id) state |= igItemStates::TEXT_FOCUS;

	if(activeItem == id && leftDown) textbox.down = true;

	textbox.rect = igRect(x, y, width, height);
	textbox.pipePos1 = textCharPos;
	textbox.pipePos2 = textCharPos2;
	textbox.textFocus = keyboardItem == id;
	textbox.value = value;

	return textbox;
}

igDrag<igDraggable> igContext::Drag(igIdent id, float x, float y, float width, float height,
					  const char* title, igDraggable* userData, igAcceptDrop fun)
{
	igDraggable* result = 0;
	igDrag<igDraggable> drag;

	if(dragPointer && userData==dragPointer)
		dragMissing = false;

	if(dragItem == id)
	{
		dragRect.x = mouseX - dragX;
		dragRect.y = mouseY - dragY;
		drag.down = true;

		if(abs(x - dragRect.x) > 1 || abs(y - dragRect.y) > 1)
			dragMoved = true;

		dragTitle = title;
		if(leftDown == false)
		{
			dragItem = nullId;
		}
	} else
	{
		if(MouseInside(x, y, width, height))
		{
			drag.hover = true;
			hotItem = id;
			if(leftDown && dragItem == nullId && activeItem == nullId)
			{
				drag.down = true;
				activeItem = dragItem = id;

				dragX = mouseX - x;
				dragY = mouseY - y;
				
				dragRect.x = x;
				dragRect.y = y;
				dragRect.w = width;
				dragRect.h = height;
				dragPointer = userData;

				dragMoved = false;
				dragMissing = false;
			}
			if(LeftJustUp() && dragPointer!=0)
			{
				result = dragPointer;
				if(fun(result) == false) result = 0;
				dragPointer = 0;
			}
		}
	}

	int state = igItemStates::NONE;
	if(activeItem == id) state |= igItemStates::PRESSED;
	if(hotItem == id) state |= igItemStates::HOVER;

	drag.drop = result;
	drag.rect = igRect(x, y, width, height);
	drag.title = title;

	return drag;
}

igMove igContext::Move(igIdent id, float& x, float& y, float width, float height, const char* title)
{
	float prevX = x, prevY = y;
	bool result = false;

	igMove move;

	if(dragItem == id)
	{
		dragMissing = false;

		x = mouseX - dragX;
		y = mouseY - dragY;

		if(leftDown == false)
		{
			dragItem = nullId;
		} else
		{
			move.down = true;
		}
		result = true;
	} else
	{
		if(MouseInside(x, y, width, height))
		{
			move.hover = true;
			hotItem = id;
			if(leftDown && dragItem == nullId && activeItem == nullId)
			{
				activeItem = dragItem = id;

				dragX = mouseX - x;
				dragY = mouseY - y;
				
				dragMoved = false;

				result = true;
				dragMissing = false;
				move.down = true;
			}
		}
	}

	int state = igItemStates::NONE;
	if(activeItem == id) state |= igItemStates::PRESSED;
	if(hotItem == id) state |= igItemStates::HOVER;

	move.rect = igRect(x, y, width, height);
	move.moved = prevX != x || prevY != y;
	move.prevX = prevX;
	move.prevY = prevY;

	return move;
}

// this is really stupid function :)
igLabel igContext::Label(float x, float y, float width, float height, const std::string& text, igTextAlign halign)
{
	igLabel label;

	label.rect = igRect(x, y, width, height);
	label.align = halign;
	label.value = text;

	return label;

}

igSlider igContext::HSlider( igIdent id, float x, float y, float width, float height, float& value )
{
	float prevValue = value;

	igSlider slider;

	if(MouseInside(x, y, width, height))
	{
		slider.hover = true;
		hotItem = id;
		if(leftDown && activeItem == nullId)
		{
			activeItem = id;
		}
	}

	if(leftDown && activeItem == id)
	{
		slider.down = true;
		value = (mouseX - x)/width;

		if(value < 0) value = 0;
		if(value > 1.0f) value = 1.0f;
	}

	slider.value = value;
	slider.rect = igRect(x, y, width, height);

	return slider;
}

igAreaBG igContext::BeginScrollArea( igIdent id, float x, float y, float width, float height, int& offset, bool scrollbar, igColor color )
{
	currentMouseClipping.active = true;
	currentMouseClipping.x = x;			currentMouseClipping.y = y;
	currentMouseClipping.width = width;	currentMouseClipping.height = height;

	scrollArea.startX = x; scrollArea.startY = y;
	scrollArea.currX = scrollArea.startX + SCROLLAREA_MARGIN_X + scrollArea.indent;
	scrollArea.width = width; scrollArea.height = height;
	scrollArea.id = id;
	scrollArea.offset = &offset;
	scrollArea.indent = 0;
	scrollArea.currMaxHeight = 0;
	scrollArea.scrollbar = scrollbar ? 1 : 0;

	scrollArea.currY = y - *scrollArea.offset;

	igAreaBG scrollArea;
	scrollArea.rect = igRect(x, y, width, height);
	return scrollArea;
}

igAreaFG igContext::EndScrollArea()
{
	currentMouseClipping.active = false;
	float totalSize = (float)(scrollArea.currY - scrollArea.startY + *scrollArea.offset);
	float aspect = scrollArea.height/totalSize;
	float curr = *scrollArea.offset/totalSize;

	igAreaFG areaFG;
	if(scrollArea.scrollbar != 0)
	{
		float posX = scrollArea.startX + scrollArea.width - SCROLLBAR_WIDTH;

		float value = *scrollArea.offset/totalSize;
		
		float newAspect = aspect;
		if(aspect > 1) newAspect = 1;
		areaFG.scroll = VScroll(scrollArea.id, posX, scrollArea.startY, SCROLLBAR_WIDTH, scrollArea.height, newAspect, value);
		*scrollArea.offset = value * totalSize+0.5f;
		if(MouseInside(scrollArea.startX, scrollArea.startY, scrollArea.width, scrollArea.height))
			*scrollArea.offset -= mouseWheel*30;
		if(aspect >= 1.0f)
		{
			*scrollArea.offset = 0;
		}
		if(*scrollArea.offset > totalSize - scrollArea.height)
			*scrollArea.offset = totalSize - scrollArea.height;
		if(*scrollArea.offset < 0) *scrollArea.offset = 0;
	}
	return areaFG;
}

void igContext::AdjustNewScrollAreaHeight( int height )
{
	if(scrollArea.currMaxHeight < height)
		scrollArea.currMaxHeight = height;
}

void igContext::NewLine()
{
	scrollArea.currY += scrollArea.currMaxHeight + SCROLLAREA_MARGIN_Y;
	scrollArea.currX = scrollArea.startX + SCROLLAREA_MARGIN_X + scrollArea.indent;
	scrollArea.currMaxHeight = 0;
}

igButton igContext::Button( igIdent id, const char* title, int width )
{	
	const int x = scrollArea.currX;
	const int y = scrollArea.currY;

	bool maxSize = width == 0;

	if(maxSize)
	{
		int currXPos = scrollArea.currX-scrollArea.startX;
		width = scrollArea.width - currXPos - SCROLLAREA_MARGIN_X - SCROLLBAR_WIDTH*scrollArea.scrollbar;
	}

	igButton button = Button(id, x, y, width, BUTTON_HEIGHT, title);

	AdjustNewScrollAreaHeight(BUTTON_HEIGHT);

	if(maxSize)
		NewLine();
	else 
		scrollArea.currX += width + SCROLLAREA_MARGIN_X;

	return button;
}

igCheckbox igContext::Checkbox( igIdent id, bool value, const char* title)
{
	const int x = scrollArea.currX;
	const int y = scrollArea.currY+CHECKBOX_HEIGHT/2-CHECKBOX_SIZE/2;

	igCheckbox result = Checkbox(id, x, y, CHECKBOX_SIZE, CHECKBOX_SIZE, value);

	scrollArea.currX += CHECKBOX_SIZE + SCROLLAREA_MARGIN_X;

	return result;
}

igTextbox igContext::TextBox( igIdent id, std::string& value, int width )
{
	const int x = scrollArea.currX;
	const int y = scrollArea.currY;

	bool maxSize = width == 0;

	if(maxSize)
	{
		int currXPos = scrollArea.currX-scrollArea.startX;
		width = scrollArea.width - currXPos - SCROLLAREA_MARGIN_X - SCROLLBAR_WIDTH*scrollArea.scrollbar;
	}

	igTextbox& result = TextBox(id, x, y, width, TEXTBOX_HEIGHT, value);

	AdjustNewScrollAreaHeight(TEXTBOX_HEIGHT);

	if(maxSize)
		NewLine();
	else 
		scrollArea.currX += width + SCROLLAREA_MARGIN_X;

	return result;
}

void igContext::Space( int width )
{
	if(width == 0)
		NewLine();
	else
		scrollArea.currX += width + SCROLLAREA_MARGIN_X;
}

igLabel igContext::Label( const std::string& text, igTextAlign halign, int width)
{
	const int x = scrollArea.currX;
	const int y = scrollArea.currY+SCROLLAREA_MARGIN_Y;

	bool maxSize = width == 0;

	if(maxSize)
	{
		int currXPos = scrollArea.currX-scrollArea.startX;
		width = scrollArea.width - currXPos - SCROLLAREA_MARGIN_X - SCROLLBAR_WIDTH*scrollArea.scrollbar;
	}

	igLabel& label = Label(x, y, width, LABEL_HEIGHT, text, halign);

	AdjustNewScrollAreaHeight(LABEL_HEIGHT);

	if(maxSize)
		NewLine();
	else 
		scrollArea.currX += width + SCROLLAREA_MARGIN_X;

	return label;
}

igSlider igContext::Slider( igIdent id, float& value, float minVal, float maxVal, int width/*=0*/ )
{
	const int x = scrollArea.currX + SLIDER_THUMB_SIZE/2.0f;
	const int y = scrollArea.currY+SCROLLAREA_MARGIN_Y;

	bool maxSize = width == 0;

	if(maxSize)
	{
		int currXPos = scrollArea.currX-scrollArea.startX;
		width = scrollArea.width - currXPos - SCROLLAREA_MARGIN_X - SCROLLBAR_WIDTH*scrollArea.scrollbar - SLIDER_THUMB_SIZE;
	}

	igSlider& result = HSlider(id, x, y, width, 20, value);

	AdjustNewScrollAreaHeight(SLIDER_HEIGHT);

	if(maxSize)
		NewLine();
	else 
		scrollArea.currX += width + SLIDER_THUMB_SIZE + SCROLLAREA_MARGIN_X;

	return result;
}

const int indentSize = 20;

void igContext::Indent()
{
	scrollArea.indent += indentSize;
	scrollArea.currX = scrollArea.startX + SCROLLAREA_MARGIN_X + scrollArea.indent;
}

void igContext::Unindent()
{
	scrollArea.indent -= indentSize;
	scrollArea.currX = scrollArea.startX + SCROLLAREA_MARGIN_X + scrollArea.indent;
}

igSeparator igContext::Separator()
{
	igSeparator separator;

	float x = scrollArea.startX;
	float y = scrollArea.currY + SCROLLAREA_MARGIN_Y + SEPARATOR_HEIGHT/2 - SEPARATOR_SIZE/2;
	float width = scrollArea.width - SCROLLBAR_WIDTH*scrollArea.scrollbar;
	float height = SEPARATOR_SIZE;

	separator.rect = igRect(x, y, width, height);

	AdjustNewScrollAreaHeight(SEPARATOR_HEIGHT);
	NewLine();
	
	return separator;
}

void igContext::ArrowLeftDown()
{
	if(shift)
	{
		textCharPos--;
	} else if(textCharPos == textCharPos2)
	{
		textCharPos--;
		textCharPos2 = textCharPos;
	} else 
	{
		textCharPos2 = textCharPos;
	}
}

void igContext::ArrowRightDown()
{
	if(shift)
	{
		textCharPos++;
	} else if(textCharPos == textCharPos2)
	{
		textCharPos++;
		textCharPos2 = textCharPos;
	} else
	{
		textCharPos2 = textCharPos;
	}
}

