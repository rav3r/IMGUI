#include "igContext.h"

#include "gfxlib.h"

static igIdent nullId = GEN_NULL_ID;

igContext::igContext()
{
	hotItem = nullId;
	activeItem = nullId;

	leftDown = false;
}

bool igContext::MouseInside(float x, float y, float width, float height)
{
    return mouseX >= x && mouseY >= y && mouseX <= x+width && mouseY <= y+height;
}

void igContext::Begin()
{
	hotItem = nullId;
}

void igContext::End()
{
	if(leftDown == false)
	{
		activeItem = nullId;
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

	gfxPrint(x, y, title, GFX_STYLE_NONE); 

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

