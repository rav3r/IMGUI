#ifndef __GFXRENDERER_H__
#define __GFXRENDERER_H__

#include "igRenderer.h"

class gfxRenderer: public igRenderer
{
public:
	void DrawButton(int state, int style, float x, float y,
					float width, float height, const char* title);

	void DrawCheckbox(int state, int style, float x, float y,
					float width, float height, bool value);

	void DrawVSlider(int state, int style, float x, float y,
					float width, float height, float aspect, const float& value);

	void DrawHSlider(int state, int style, float x, float y,
					float width, float height, float aspect, const float& value);

	void DrawTextBox(int state, int style, float x, float y,
					float width, float height, const std::string& value, int textCharPos);

	void DrawDrag(int state, int style, float x, float y,
					float width, float height, const char* title);

	void DrawMove(int state, int style, float& x, float& y, float prevX, float prevY,
					float width, float height, const char* title);

	void DrawTab(int state, int style, float x, float y,
					float width, float height, const char* title, bool value);
};

#endif