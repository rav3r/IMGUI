#include "gfxRenderer.h"

#include "IMGUI.h"
#include "gfxlib.h"

void gfxRenderer::DrawButton( int state, int style, float x, float y, float width, float height, const char* title )
{
	if(state & igItemStates::PRESSED)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_PRESSED);
	} else if(state & igItemStates::HOVER)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_HOVER);
	} else
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_NONE);
	}

	gfxPrint(x+width/2.0f, y+height/2.0f, title, GFX_STYLE_NONE); 
}

void gfxRenderer::DrawCheckbox( int state, int style, float x, float y, float width, float height, bool value )
{
	const float margin = 2.0f;

	if(state & igItemStates::PRESSED)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_PRESSED);
	} else if(state & igItemStates::HOVER)
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
}

void gfxRenderer::DrawVSlider( int state, int style, float x, float y, float width, float height, float aspect, const float& value )
{
	// draw vertical slider
	const float margin = 2.0f;
	float thumbSize = height * aspect;

	gfxDrawRectangle(x, y, width, height, GFX_STYLE_NONE);
	gfxDrawRectangle(x+margin, y+value*height, width-margin*2, thumbSize, GFX_STYLE_SLIDER_THUMB);
}

void gfxRenderer::DrawHSlider( int state, int style, float x, float y, float width, float height, float aspect, const float& value )
{
	const float margin = 2.0f;
	float thumbSize = width * aspect;

	gfxDrawRectangle(x, y, width, height, GFX_STYLE_NONE);
	gfxDrawRectangle(x+value*width, y+margin, thumbSize, height-margin*2.0f, GFX_STYLE_SLIDER_THUMB);
}

void gfxRenderer::DrawTextBox( int state, int style, float x, float y, float width, float height, const std::string& value, int textCharPos )
{
	// draw text box
	if(state & igItemStates::PRESSED)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_PRESSED);
	} else if(state & igItemStates::HOVER)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_HOVER);
	} else
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_NONE);
	}

	gfxPrint(x + width/2.0f, y + height/2.0f, value.c_str(), GFX_STYLE_NONE, state & igItemStates::TEXT_FOCUS ? textCharPos : -1); 
}

void gfxRenderer::DrawDrag( int state, int style, float x, float y, float width, float height, const char* title )
{
	// draw button
	if(state & igItemStates::PRESSED)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_PRESSED);
	} else if(state & igItemStates::HOVER)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_HOVER);
	} else
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_NONE);
	}

	gfxPrint(x + width/2.0f, y + height/2.0f, title, GFX_STYLE_NONE);
}

void gfxRenderer::DrawMove( int state, int style, float& x, float& y, float prevX, float prevY, float width, float height, const char* title )
{
	// draw button
	if(state & igItemStates::PRESSED)
	{
		gfxDrawRectangle(prevX, prevY, width, height, GFX_STYLE_ELEM_PRESSED);
	} else if(state & igItemStates::HOVER)
	{
		gfxDrawRectangle(prevX, prevY, width, height, GFX_STYLE_ELEM_HOVER);
	} else
	{
		gfxDrawRectangle(prevX, prevY, width, height, GFX_STYLE_NONE);
	}

	gfxPrint(prevX + width/2.0f, prevY + height/2.0f, title, GFX_STYLE_NONE);
}

void gfxRenderer::DrawTab( int state, int style, float x, float y, float width, float height, const char* title, bool value )
{
	if(state & igItemStates::PRESSED || value)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_PRESSED);
	} else if(state & igItemStates::HOVER)
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_ELEM_HOVER);
	} else
	{
		gfxDrawRectangle(x, y, width, height, GFX_STYLE_NONE);
	}

	gfxPrint(x+width/2.0f, y+height/2.0f, title, GFX_STYLE_NONE); 

}
