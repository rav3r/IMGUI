#ifndef __GFXLIB_H__
#define __GFXLIB_H__

#define GFX_STYLE_NONE 0
#define GFX_STYLE_ELEM_PRESSED 1
#define GFX_STYLE_ELEM_HOVER 2
#define GFX_STYLE_CHECKBOX 3
#define GFX_STYLE_SLIDER_THUMB 4
#define GFX_STYLE_SLIDER_BG 5
#define GFX_STYLE_SCROLL_AREA 6
#define GFX_STYLE_TEXTBOX_BG 7
#define GFX_STYLE_CAN_DRAG 8
#define GFX_STYLE_CAN_NOT_DRAG 9
#define GFX_STYLE_SELECTED 10

// External functions used by lib

void gfxDrawRectangle(float x, float y, float width, float height, int style, bool border=true);

void gfxPrint(float x, float y, const char* text, int style, int pipePos = -1, int pipe2Pos = -1);

int gfxCharAt(float x, float y, const char* text, int style, float mouseX);

void gfxScissor(int x, int y, int width, int height);

void gfxDisableScissor();

#endif
