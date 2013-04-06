#ifndef __IGRENDERER_H__
#define __IGRENDERER_H__

#include <string>

class igRenderer
{
public:
	virtual void DrawButton(int state, int style, float x, float y,
							float width, float height, const char* title) = 0;

	virtual void DrawCheckbox(int state, int style, float x, float y,
							float width, float height, bool value) = 0;

	virtual void DrawVSlider(int state, int style, float x, float y,
							float width, float height, float aspect, const float& value) = 0;

	virtual void DrawHSlider(int state, int style, float x, float y,
							float width, float height, float aspect, const float& value) = 0;

	virtual void DrawTextBox(int state, int style, float x, float y,
							float width, float height, const std::string& value, int textCharPos) = 0;

	virtual void DrawDrag(int state, int style, float x, float y,
							float width, float height, const char* title) = 0;

	virtual void DrawMove(int state, int style, float& x, float& y, float prevX, float prevY,
							float width, float height, const char* title) = 0;

	virtual void DrawTab(int state, int style, float x, float y,
							float width, float height, const char* title, bool value) = 0;
};

#endif