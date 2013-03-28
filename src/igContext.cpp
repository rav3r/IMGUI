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

