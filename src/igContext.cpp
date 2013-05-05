#include "igContext.h"

#include <iostream>

#include "gfxlib.h"

static igIdent nullId = GEN_NULL_ID;

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

bool igContext::VSlider(igIdent id, float x, float y, float width, float height, float aspect, float& value)
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

	renderer->DrawVSlider(0, 0, x, y, width, height, aspect, value);

	return prevValue != value;
}

bool igContext::HSlider(igIdent id, float x, float y, float width, float height, float aspect, float& value)
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

	// draw vertical slider
	renderer->DrawHSlider(0, 0, x, y, width, height, aspect, value);

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

void igContext::BeginScrollArea( igIdent id, float x, float y, float width, float height, int& offset )
{
	currentMouseClipping.active = true;
	currentMouseClipping.x = x;			currentMouseClipping.y = y;
	currentMouseClipping.width = width;	currentMouseClipping.height = height;

	scrollArea.startX = x; scrollArea.startY = y;
	scrollArea.currX = scrollArea.startX + marginX + scrollArea.indent;
	scrollArea.width = width; scrollArea.height = height;
	scrollArea.id = id;
	scrollArea.offset = &offset;
	scrollArea.indent = 0;

	scrollArea.currY = y - *scrollArea.offset;

	renderer->DrawScrollArea(GFX_STYLE_SCROLL_AREA, x, y, width, height);

	gfxScissor(x, y, width, height);
}

void igContext::EndScrollArea(bool scrollbarRight)
{
	currentMouseClipping.active = false;
	gfxDisableScissor();
	float totalSize = (float)(scrollArea.currY - scrollArea.startY + *scrollArea.offset);
	float aspect = scrollArea.height/totalSize;
	float curr = *scrollArea.offset/totalSize;

	if(aspect < 1.0f)
	{
		float size = 20;
		float posX = scrollArea.startX;
		if(scrollbarRight)
			posX += scrollArea.width;
		else
			posX -= size;

		float value = (*scrollArea.offset + 0.5f)/totalSize;

		VSlider(scrollArea.id, posX, scrollArea.startY,
			size, scrollArea.height, aspect, value);
		*scrollArea.offset = value * totalSize;
		if(MouseInside(scrollArea.startX, scrollArea.startY, scrollArea.width, scrollArea.height))
			*scrollArea.offset -= mouseWheel*30;
	} else
	{
		*scrollArea.offset = 0;
	}
	if(*scrollArea.offset > totalSize - scrollArea.height)
		*scrollArea.offset = totalSize - scrollArea.height;
	if(*scrollArea.offset < 0) *scrollArea.offset = 0;
}



void igContext::NewLine()
{
	scrollArea.currY += newLineSize;
	scrollArea.currX = scrollArea.startX + marginX + scrollArea.indent;
}

igButton igContext::Button( igIdent id, const char* title, int width )
{
	const int height = 30;
	
	const int x = scrollArea.currX;
	const int y = scrollArea.currY+marginY;

	bool maxSize = width == 0;

	if(maxSize)
		width = scrollArea.width - (scrollArea.currX-scrollArea.startX) - marginX;

	igButton button = Button(id, x, y, width, height, title);

	if(maxSize)
		NewLine();
	else 
		scrollArea.currX += width + marginX;

	return button;
}

bool igContext::Checkbox( igIdent id, bool value, const char* title)
{
	const int marginX = 5;
	const int size = 16;
	const int height = 30;
	const int x = scrollArea.currX;
	const int y = scrollArea.currY+height/2-size/2;

	bool result = Checkbox(id, x, y, size, size, value);

	scrollArea.currX += size + marginX;

	return result;
}

bool igContext::TextBox( igIdent id, std::string& value, int width )
{
	const int marginX = 5;
	const int height = 30;
	const int x = scrollArea.currX;
	const int y = scrollArea.currY+marginY;

	bool maxSize = width == 0;

	if(maxSize)
		width = scrollArea.width - (scrollArea.currX-scrollArea.startX) - marginX;

	bool result = TextBox(id, x, y, width, height, value);

	if(maxSize)
		NewLine();
	else 
		scrollArea.currX += width + marginX;

	return result;
}

void igContext::Space( int width )
{
	if(width == 0)
		NewLine();
	else
		scrollArea.currX += width + marginX;
}

const int indentSize = 20;

void igContext::Indent()
{
	scrollArea.indent += indentSize;
	scrollArea.currX = scrollArea.startX + marginX + scrollArea.indent;
}

void igContext::Unindent()
{
	scrollArea.indent -= indentSize;
	scrollArea.currX = scrollArea.startX + marginX + scrollArea.indent;
}

void igContext::Separator()
{
	renderer->DrawSeparator(scrollArea.startX, scrollArea.currY + marginY + newLineSize/2 - 2, scrollArea.width, 4);
	NewLine();
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

