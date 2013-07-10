#include "igDrawing.h"
#include "igSizing.h"

#include "gfxlib.h"

extern sf::RenderWindow * gWindow;

// todo
void gfxDrawRectangle(igRect rect, int style, bool border=true, float r=-1, float g=0, float b=0)
{
	gfxDrawRectangle(rect.x, rect.y, rect.w, rect.h, style, border, r, g, b);
}

const igButton& DrawButton(const igButton& button)
{
	igRect r = button.rect;

	if(button.down)
	{
		gfxDrawRectangle(button.rect, GFX_STYLE_ELEM_PRESSED);
	} else if(button.hover)
	{
		gfxDrawRectangle(button.rect, GFX_STYLE_ELEM_HOVER);
	} else
	{
		gfxDrawRectangle(button.rect, GFX_STYLE_NONE);
	}

	gfxPrint(r.x+r.w/2, r.y+r.h/2, button.title, GFX_STYLE_NONE);

	return button;
}

const igCheckbox& DrawCheckbox(const igCheckbox& checkbox, sf::Texture& thickTexture)
{
	const float margin = 5.0f;

	if(checkbox.down)
	{
		gfxDrawRectangle(checkbox.rect, GFX_STYLE_ELEM_PRESSED);
	} else if(checkbox.hover)
	{
		gfxDrawRectangle(checkbox.rect, GFX_STYLE_ELEM_HOVER);
	} else
	{
		gfxDrawRectangle(checkbox.rect, GFX_STYLE_NONE);
	}

	if(checkbox.value)
	{
		int x = checkbox.rect.x; int y = checkbox.rect.y;
		int w = checkbox.rect.w; int h = checkbox.rect.h;
		sf::Sprite thick(thickTexture);
		thick.setPosition((float)x, (float)y);
		gWindow->draw(thick);
	}

	return checkbox;
}

const igTextbox& DrawTextbox(const igTextbox& textbox)
{
	igRect r = textbox.rect;
	// draw text box
	if(textbox.down)
	{
		gfxDrawRectangle(textbox.rect, GFX_STYLE_ELEM_PRESSED);
	} else if(textbox.hover)
	{
		gfxDrawRectangle(textbox.rect, GFX_STYLE_ELEM_HOVER);
	} else
	{
		gfxDrawRectangle(textbox.rect, GFX_STYLE_TEXTBOX_BG);
	}

	int pipePos1 = textbox.textFocus ? textbox.pipePos1 : -1;
	int pipePos2 = textbox.textFocus ? textbox.pipePos2 : -1;

	//if(time % 500 >= 250 && textCharPos == textCharPos2)
	//	pipePos = -1;

	if(pipePos1 > pipePos2)
		std::swap(pipePos1, pipePos2);

	gfxPrint(r.x + r.w/2, r.y + r.h/2, textbox.value.c_str(), GFX_STYLE_NONE, pipePos1, pipePos2); 

	return textbox;
}

const igLabel& DrawLabel(const igLabel& label)
{
	igRect r = label.rect;
	int align = 0;
	if(label.align == igTextAligns::LEFT) align = 1;
	if(label.align == igTextAligns::RIGHT) align = -1;
	gfxPrint(r.x + r.w/2+r.w/2*align, r.y + r.h/2, label.value.c_str(), GFX_STYLE_NONE, -1, -1, align); 
	return label;
}

const igSlider& DrawSlider(const igSlider& slider)
{
	igRect r = slider.rect;
	gfxDrawRectangle(r.x, r.y+r.h/4, r.w, r.h/2, GFX_STYLE_SLIDER_BG, false);
	gfxDrawRectangle((int)(r.x+slider.value*r.w-igSizing::SLIDER_THUMB_SIZE/2), r.y, igSizing::SLIDER_THUMB_SIZE, r.h, GFX_STYLE_SLIDER_THUMB, false);
	return slider;
}

void DrawDrag(const igDrag<void>& drag)
{
	igRect r = drag.rect;
	// draw button
	if(drag.down)
	{
		gfxDrawRectangle(drag.rect, GFX_STYLE_ELEM_PRESSED);
	} else if(drag.hover)
	{
		gfxDrawRectangle(drag.rect, GFX_STYLE_ELEM_HOVER);
	} else
	{
		gfxDrawRectangle(drag.rect, GFX_STYLE_NONE);
	}

	gfxPrint(r.x + r.w/2, r.y + r.h/2, drag.title.c_str(), GFX_STYLE_NONE);
}

const igMove& DrawMove(const igMove& move)
{
	igRect prevRect(move.prevX, move.prevY, move.rect.w, move.rect.h);
	
	if(move.down)
	{
		gfxDrawRectangle(prevRect, GFX_STYLE_ELEM_PRESSED);
	} else if(move.hover)
	{
		gfxDrawRectangle(prevRect, GFX_STYLE_ELEM_HOVER);
	} else
	{
		gfxDrawRectangle(prevRect, GFX_STYLE_NONE);
	}

	return move;
}

const igSeparator& DrawSeparator(const igSeparator& separator)
{
	igRect r = separator.rect;
	gfxDrawRectangle(r.x+5, r.y, r.w - 10, r.h, GFX_STYLE_NONE);
	return separator;
}

const igAreaBG& DrawAreaBG(const igAreaBG& scrollArea)
{
	gfxDrawRectangle(scrollArea.rect, GFX_STYLE_SCROLL_AREA, false, 75/255.0f, 75/255.0f, 75/255.0f);

	gfxScissor(scrollArea.rect.x, scrollArea.rect.y, scrollArea.rect.w, scrollArea.rect.h);

	return scrollArea;
}

const igAreaFG& DrawAreaFG(const igAreaFG& scrollArea)
{
	gfxDisableScissor();

	igVScroll scroll = scrollArea.scroll;
	igRect r = scroll.rect;
	float thumbSize = r.h * scroll.aspect;

	gfxDrawRectangle(scroll.rect, GFX_STYLE_SLIDER_BG, false);
	gfxDrawRectangle(r.x, (int)(r.y+scroll.value*r.h), r.w, (int)thumbSize, GFX_STYLE_SLIDER_THUMB, false);

	return scrollArea;
}

const igDragged& DrawDragged(const igDragged& dragged)
{
	if(!dragged.show)
		return dragged;

	igRect r = dragged.rect;

	gfxDrawRectangle(dragged.rect, dragged.canDrop ? GFX_STYLE_CAN_DRAG : GFX_STYLE_CAN_NOT_DRAG);
	if(dragged.title.empty() == false)
		gfxPrint(r.x + r.w/2, r.y + r.h/2, dragged.title.c_str(), GFX_STYLE_NONE);

	return dragged;
}