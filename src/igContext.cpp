#include "igContext.h"

#include <iostream>

#include "gfxlib.h"

static igIdent nullId = GEN_NULL_ID;

igContext::igContext()
{
	hotItem = nullId;
	activeItem = nullId;
	keyboardItem = nullId;
	dragItem = nullId;

	leftDown = false;

	charEntered = 0;
	backspace = false;

	textCharPos = 0;
}

bool igContext::MouseInside(float x, float y, float width, float height)
{
    return mouseX >= x && mouseY >= y && mouseX <= x+width && mouseY <= y+height;
}

void igContext::Begin()
{
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

	if(dragItem != nullId && dragMoved)
	{
		gfxDrawRectangle(dragRect.x, dragRect.y, dragRect.w, dragRect.h, GFX_STYLE_NONE);
		if(dragTitle.empty() == false)
			gfxPrint(dragRect.x + dragRect.w/2.0f, dragRect.y + dragRect.h/2.0f,
					 dragTitle.c_str(), GFX_STYLE_NONE);
	}
}

bool igContext::Button(igIdent id, float x, float y,
	float width, float height, const char* title)
{
    if(MouseInside(x, y, width, height))
    {
		hotItem = id;
		if(leftDown && activeItem == nullId)
			activeItem = id;
	}

	// draw button
	if(activeItem == id)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_PRESSED);
	} else if(hotItem == id)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_HOVER);
	} else
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_NONE);
	}

	gfxPrint(x+width/2.0f, y+height/2.0f, title, GFX_STYLE_NONE); 

	return leftDown == false && hotItem == id && activeItem == id;
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
	const float margin = 2.0f;

	if(activeItem == id)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_PRESSED);
	} else if(hotItem == id)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_HOVER);
	} else
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_NONE);
	}
	
	if(value)
	{
		gfxDrawRectangle(x+margin, y+margin, width-margin*2, height-margin*2, GFX_STYLE_CHECKBOX);
	}

	return leftDown == false && hotItem == id && activeItem == id;
}

bool igContext::VSlider(igIdent id, float x, float y, float width, float height, float aspect, float& value)
{
	float prevValue = value;
	float thumbSize = height * aspect;

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

	// draw vertical slider
	const float margin = 2.0f;

	gfxDrawRectangle(x, y, width, height, GFX_STYLE_NONE);
	gfxDrawRectangle(x+margin, y+value*height, width-margin*2, thumbSize, GFX_STYLE_SLIDER_THUMB);

	return prevValue != value;
}

bool igContext::HSlider(igIdent id, float x, float y, float width, float height, float aspect, float& value)
{
	float prevValue = value;
	float thumbSize = width * aspect;

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
	const float margin = 2.0f;

	gfxDrawRectangle(x, y, width, height, GFX_STYLE_NONE);
	gfxDrawRectangle(x+value*width, y+margin, thumbSize, height-margin*2.0f, GFX_STYLE_SLIDER_THUMB);

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

	// draw text box
	if(keyboardItem == id)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_PRESSED);
	} else if(hotItem == id)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_HOVER);
	} else
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_NONE);
	}

	gfxPrint(x + width/2.0f, y + height/2.0f, value.c_str(), GFX_STYLE_NONE, keyboardItem == id ? textCharPos : -1); 

	return prevValue != value;
}

bool igContext::Drag(igIdent id, float x, float y, float width, float height, const char* title)
{
	bool result = false;

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
				
				dragRect.x = x;
				dragRect.y = y;
				dragRect.w = width;
				dragRect.h = height;

				result = true;
				dragMoved = false;
			}
		}
	}

	// draw button
	if(activeItem == id)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_PRESSED);
	} else if(hotItem == id)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_HOVER);
	} else
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_NONE);
	}

	gfxPrint(x + width/2.0f, y + height/2.0f, title, GFX_STYLE_NONE); 

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

	// draw button
	if(activeItem == id)
	{
		gfxDrawRectangle(prevX, prevY, width, height, GFX_STYLE_ELEM_PRESSED);
	} else if(hotItem == id)
	{
		gfxDrawRectangle(prevX, prevY, width, height, GFX_STYLE_ELEM_HOVER);
	} else
	{
		gfxDrawRectangle(prevX, prevY, width, height, GFX_STYLE_NONE);
	}

	gfxPrint(prevX + width/2.0f, prevY + height/2.0f, title, GFX_STYLE_NONE);
	
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

	// draw tab
	if(activeItem == id || value)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_PRESSED);
	} else if(hotItem == id)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_HOVER);
	} else
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_NONE);
	}

	gfxPrint(x+width/2.0f, y+height/2.0f, title, GFX_STYLE_NONE); 

	return (leftDown == false && hotItem == id && activeItem == id) || value;
}
