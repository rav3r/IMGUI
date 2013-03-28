#include "igContext.h"

#include "gfxlib.h"

bool igContext::MouseInside(float x, float y, float width, float height)
{
    return mouseX >= x && mouseY >= y && mouseX <= x+width && mouseY <= y+height;
}

bool igContext::Button(igIdent id, float x, float y,
	float width, float height, const char* title)
{
    if(MouseInside(x, y, width, height))
    {
	    gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_PRESSED);
	} else
	{
	    gfxDrawRectangle(x, y, width, height, GFX_STYLE_NONE);
	}
	gfxPrint(x, y, title, GFX_STYLE_NONE); 
	return false;
}
