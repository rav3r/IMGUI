#ifndef __IGRENDERER_H__
#define __IGRENDERER_H__

#include <string>

class igRenderer
{
public:
	int time;

	virtual void DrawButton(int state, int style, float x, float y,
							float width, float height, const char* title);

	virtual void DrawCheckbox(int state, int style, float x, float y,
							float width, float height, bool value);

	virtual void DrawVSlider(int state, int style, float x, float y,
							float width, float height, float aspect, const float& value);

	virtual void DrawHSlider(int state, int style, float x, float y,
							float width, float height, float aspect, const float& value);

	virtual void DrawTextBox(int state, int style, float x, float y,
							float width, float height, const std::string& value,
							int textCharPos, int textCharPos2);

	virtual void DrawDrag(int state, int style, float x, float y,
							float width, float height, const char* title);

	virtual void DrawMove(int state, int style, float& x, float& y, float prevX, float prevY,
							float width, float height, const char* title);

	virtual void DrawTab(int state, int style, float x, float y,
							float width, float height, const char* title, bool value);

	virtual void DrawScrollArea(int style, float x, float y, float width, float height);
	
	virtual void DrawSeparator(float x, float y, float width, float height);
};

#endif