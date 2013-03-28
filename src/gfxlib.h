#ifndef __GFXLIB_H__
#define __GFXLIB_H__

#define GFX_STYLE_NONE 0
#define GFX_STYLE_ELEM_PRESSED 1

// External functions used by lib

void gfxDrawRectangle(float x, float y, float width, float height, int style);

void gfxPrint(float x, float y, const char* text, int style);

#endif
