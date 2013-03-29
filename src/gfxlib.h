#ifndef __GFXLIB_H__
#define __GFXLIB_H__

#define GFX_STYLE_NONE 0
#define GFX_STYLE_ELEM_PRESSED 1
#define GFX_STYLE_ELEM_HOVER 2
#define GFX_STYLE_CHECKBOX 3
#define GFX_STYLE_SLIDER_THUMB 4

// External functions used by lib

void gfxDrawRectangle(float x, float y, float width, float height, int style);

void gfxPrint(float x, float y, const char* text, int style, int pipePos = -1);

#endif
