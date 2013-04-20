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

	dragPointer = 0;
}

bool igContext::MouseInside(float x, float y, float width, float height)
{
    return mouseX >= x && mouseY >= y && mouseX <= x+width && mouseY <= y+height;
}

void igContext::Begin()
{
	canDrop = false;
	dragMissing = true;
	hotItem = nullId;
	if(textCharPos < 0)
		textCharPos = 0;
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
			textCharPos = value.size();
		}
	}

	if(keyboardItem == id)
	{
		if(textCharPos > value.size())
			textCharPos = value.size();

		if(charEntered)
		{
			for(int i=0; i<charset.size(); i++)
				if(charset[i] == charEntered)
				{
					value.insert(textCharPos, 1, (char)charEntered);
					textCharPos++;
					break;
				}
		}

		if(charEntered == 8 && textCharPos > 0) // backspace
		{
			textCharPos--;
			value.erase(textCharPos, 1);
		}

		if(charEntered == 127) // delete
		{
			value.erase(textCharPos, 1);
		}
	}

	int state = igItemStates::NONE;
	if(keyboardItem == id) state |= igItemStates::TEXT_FOCUS;
	renderer->DrawTextBox(state, 0, x, y, width, height, value, textCharPos);

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
	scrollArea.startX = scrollArea.currX = x; scrollArea.startY = y;
	scrollArea.width = width; scrollArea.height = height;
	scrollArea.id = id;
	scrollArea.offset = &offset;

	scrollArea.currY = y - *scrollArea.offset;

	renderer->DrawScrollArea(GFX_STYLE_SCROLL_AREA, x, y, width, height);

	gfxScissor(x, y, width, height);
}

void igContext::EndScrollArea(bool scrollbarRight)
{
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

igButton igContext::Button( igIdent id, const char* title )
{
	const int marginX = 5;
	const int marginY = 5;
	const int height = 30;

	igButton button = Button(id, scrollArea.currX+marginX, scrollArea.currY, scrollArea.width-2*marginX, height, title);

	scrollArea.currY += height + marginY;

	return button;
}

bool igContext::Checkbox( igIdent id, bool value, const char* title )
{
	const int marginX = 5;
	const int marginY = 5;
	const int size = 16;
	const int height = 30;
	bool result = Checkbox(id, scrollArea.currX+marginX, scrollArea.currY+height/2-size/2, size, size, value);

	scrollArea.currY += height;

	return result;
}

bool igContext::TextBox( igIdent id, std::string& value )
{
	const int marginX = 5;
	const int marginY = 5;
	const int height = 30;

	bool result = TextBox(id, scrollArea.currX+marginX, scrollArea.currY, scrollArea.width-2*marginX, height, value);

	scrollArea.currY += height + marginY;

	return result;
}

igDraggable* igContext::Drag( igIdent id, const char* title, igDraggable* userData, igAcceptDrop fun )
{
	const int marginX = 5;
	const int marginY = 5;
	const int height = 30;

	igDraggable* result = Drag(id, scrollArea.currX+marginX, scrollArea.currY, scrollArea.width-2*marginX, height, title, userData, fun);

	scrollArea.currY += height + marginY;

	return result;
}
