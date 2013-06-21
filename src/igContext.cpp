#include "igContext.h"

#include <iostream>

#include "gfxlib.h"

static igIdent nullId = GEN_NULL_ID;

void sysSetCliboardString(std::string str);
std::string sysGetCliboardString();

using namespace igSizing;

igContext::igContext(igRenderer* rend):
	renderer(rend)
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

void igContext::End()
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

	if(dragItem != nullId && dragMoved)
	{
		gfxDrawRectangle(dragRect.x, dragRect.y, dragRect.w, dragRect.h, canDrop ? GFX_STYLE_CAN_DRAG : GFX_STYLE_CAN_NOT_DRAG);
		if(dragTitle.empty() == false)
			gfxPrint(dragRect.x + dragRect.w/2.0f, dragRect.y + dragRect.h/2.0f,
					 dragTitle.c_str(), GFX_STYLE_NONE);
	}

	mouseWheel = 0;
	leftLastDown = leftDown;

	ctrlC = false;
	ctrlX = false;
	ctrlV = false;
	ctrlA = false;
}

bool igContext::LeftJustUp()
{
	return leftLastDown && !leftDown;
}

igButton igContext::Button(igIdent id, float x, float y,
	float width, float height, const char* title)
{
	igButton button;
    if(MouseInside(x, y, width, height))
    {
		hotItem = id;
		if(leftDown && activeItem == nullId)
			activeItem = id;
		button.onHover = true;
	}

	// draw button
	int state = igItemStates::NONE;
	if(activeItem == id) state |= igItemStates::PRESSED;
	if(hotItem == id) state |= igItemStates::HOVER;
	renderer->DrawButton(state, 0, x, y, width, height, title);


	button.onClicked = leftDown == false && hotItem == id && activeItem == id;
	button.onDown = leftDown && activeItem == id;

	return button;
}

bool igContext::Checkbox(igIdent id, float x, float y, float width, float height, bool value)
{
	if(MouseInside(x, y, width, height))
	{
		hotItem = id;
		if(leftDown && activeItem == nullId)
			activeItem = id;
	}

	// draw checkbox
	int state = igItemStates::NONE;
	if(activeItem == id) state |= igItemStates::PRESSED;
	if(hotItem == id) state |= igItemStates::HOVER;
	renderer->DrawCheckbox(state, 0, x, y, width, height, value);

	return leftDown == false && hotItem == id && activeItem == id;
}

bool igContext::VScrollbar(igIdent id, float x, float y, float width, float height, float aspect, float& value)
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

	renderer->DrawVScrollbar(0, 0, x, y, width, height, aspect, value);

	return prevValue != value;
}

bool igContext::HScrollbar(igIdent id, float x, float y, float width, float height, float aspect, float& value)
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
		value = (mouseX - x)/width - aspect/2.0f;

		if(value < 0) value = 0;
		if(value > 1.0f - aspect) value = 1.0f - aspect;
	}

	renderer->DrawHScrollbar(0, 0, x, y, width, height, aspect, value);

	return prevValue != value;
}

bool igContext::TextBox(igIdent id, float x, float y, float width, float height,
						std::string& value, const std::string& charset)
{
	std::string prevValue = value;

	if(MouseInside(x, y, width, height))
	{
		hotItem = id;
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
	renderer->DrawTextBox(state, 0, x, y, width, height, value, textCharPos, textCharPos2);

	return prevValue != value;
}

igDraggable* igContext::Drag(igIdent id, float x, float y, float width, float height,
					  const char* title, igDraggable* userData, igAcceptDrop fun)
{
	igDraggable* result = 0;

	if(dragPointer && userData==dragPointer)
		dragMissing = false;

	if(dragItem == id)
	{
		dragRect.x = mouseX - dragX;
		dragRect.y = mouseY - dragY;

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
			hotItem = id;
			if(leftDown && dragItem == nullId && activeItem == nullId)
			{
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

	renderer->DrawDrag(state, 0, x, y, width, height, title);

	return result;
}

bool igContext::Move(igIdent id, float& x, float& y, float width, float height, const char* title)
{
	float prevX = x, prevY = y;
	bool result = false;

	if(dragItem == id)
	{
		x = mouseX - dragX;
		y = mouseY - dragY;

		if(leftDown == false)
		{
			dragItem = nullId;
		}
		result = true;
	} else
	{
		if(MouseInside(x, y, width, height))
		{
			hotItem = id;
			if(leftDown && dragItem == nullId && activeItem == nullId)
			{
				activeItem = dragItem = id;

				dragX = mouseX - x;
				dragY = mouseY - y;
				
				dragMoved = false;

				result = true;
			}
		}
	}

	int state = igItemStates::NONE;
	if(activeItem == id) state |= igItemStates::PRESSED;
	if(hotItem == id) state |= igItemStates::HOVER;
	renderer->DrawMove(state, 0, x, y, prevX, prevY, width, height, title);
	
	return prevX != x || prevY != y;
}

bool igContext::Tab(igIdent id, float x, float y, float width, float height, const char* title, bool value)
{
	if(MouseInside(x, y, width, height))
	{
		hotItem = id;
		if(leftDown && activeItem == nullId)
			activeItem = id;
	}

	int state = igItemStates::NONE;
	if(activeItem == id) state |= igItemStates::PRESSED;
	if(hotItem == id) state |= igItemStates::HOVER;
	renderer->DrawTab(state, 0, x, y, width, height, title, value);
	
	return (leftDown == false && hotItem == id && activeItem == id) || value;
}

void igContext::Label(float x, float y, float width, float height, const std::string& text, igTextAlign halign)
{
	int hint = 0;
	if(halign == igTextAligns::LEFT)
		hint = -1;
	if(halign == igTextAligns::RIGHT)
		hint = 1;
	renderer->DrawLabel(GFX_STYLE_NONE, x, y, width, height, text, hint);
}

void igContext::BeginScrollArea( igIdent id, float x, float y, float width, float height, int& offset )
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

	scrollArea.currY = y - *scrollArea.offset;

	renderer->DrawScrollArea(GFX_STYLE_SCROLL_AREA, x, y, width, height);

	gfxScissor(x, y, width, height);
}

void igContext::EndScrollArea()
{
	currentMouseClipping.active = false;
	gfxDisableScissor();
	float totalSize = (float)(scrollArea.currY - scrollArea.startY + *scrollArea.offset);
	float aspect = scrollArea.height/totalSize;
	float curr = *scrollArea.offset/totalSize;

	float posX = scrollArea.startX + scrollArea.width - SCROLLBAR_WIDTH;

	float value = *scrollArea.offset/totalSize;

	float newAspect = aspect;
	if(aspect > 1) newAspect = 1;
	VScrollbar(scrollArea.id, posX, scrollArea.startY, SCROLLBAR_WIDTH, scrollArea.height, newAspect, value);
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



void igContext::NewLine(int newLineSize)
{
	scrollArea.currY += newLineSize;
	scrollArea.currX = scrollArea.startX + SCROLLAREA_MARGIN_X + scrollArea.indent;
}

igButton igContext::Button( igIdent id, const char* title, int width )
{	
	const int x = scrollArea.currX;
	const int y = scrollArea.currY;

	bool maxSize = width == 0;

	if(maxSize)
	{
		int currXPos = scrollArea.currX-scrollArea.startX;
		width = scrollArea.width - currXPos - SCROLLAREA_MARGIN_X - SCROLLBAR_WIDTH;
	}

	igButton button = Button(id, x, y, width, BUTTON_HEIGHT, title);

	if(maxSize)
		NewLine(BUTTON_HEIGHT+SCROLLAREA_MARGIN_Y);
	else 
		scrollArea.currX += width + SCROLLAREA_MARGIN_X;

	return button;
}

bool igContext::Checkbox( igIdent id, bool value, const char* title)
{
	const int x = scrollArea.currX;
	const int y = scrollArea.currY+CHECKBOX_HEIGHT/2-CHECKBOX_SIZE/2;

	bool result = Checkbox(id, x, y, CHECKBOX_SIZE, CHECKBOX_SIZE, value);

	scrollArea.currX += CHECKBOX_SIZE + SCROLLAREA_MARGIN_X;

	return result;
}

bool igContext::TextBox( igIdent id, std::string& value, int width )
{
	const int x = scrollArea.currX;
	const int y = scrollArea.currY;

	bool maxSize = width == 0;

	if(maxSize)
	{
		int currXPos = scrollArea.currX-scrollArea.startX;
		width = scrollArea.width - currXPos - SCROLLAREA_MARGIN_X - SCROLLBAR_WIDTH;
	}

	bool result = TextBox(id, x, y, width, TEXTBOX_HEIGHT, value);

	if(maxSize)
		NewLine(TEXTBOX_HEIGHT + SCROLLAREA_MARGIN_Y);
	else 
		scrollArea.currX += width + SCROLLAREA_MARGIN_X;

	return result;
}

void igContext::Space( int width )
{
	if(width == 0)
		NewLine(30);
	else
		scrollArea.currX += width + SCROLLAREA_MARGIN_X;
}

void igContext::Label( const std::string& text, igTextAlign halign, int width)
{
	const int x = scrollArea.currX;
	const int y = scrollArea.currY+SCROLLAREA_MARGIN_Y;

	bool maxSize = width == 0;

	if(maxSize)
	{
		int currXPos = scrollArea.currX-scrollArea.startX;
		width = scrollArea.width - currXPos - SCROLLAREA_MARGIN_X - SCROLLBAR_WIDTH;
	}

	Label(x, y, width, LABEL_HEIGHT, text, halign);

	if(maxSize)
		NewLine(LABEL_HEIGHT + SCROLLAREA_MARGIN_Y);
	else 
		scrollArea.currX += width + SCROLLAREA_MARGIN_X;
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

void igContext::Separator()
{
	float y = scrollArea.currY + SCROLLAREA_MARGIN_Y + SEPARATOR_HEIGHT/2 - SEPARATOR_SIZE/2;
	renderer->DrawSeparator(scrollArea.startX, y, scrollArea.width - SCROLLBAR_WIDTH, SEPARATOR_SIZE);
	NewLine(SEPARATOR_HEIGHT+SCROLLAREA_MARGIN_Y);
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


