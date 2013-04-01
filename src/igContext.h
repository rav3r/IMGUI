#ifndef __IGCONTEXT_H__
#define __IGCONTEXT_H__

#include "igIdent.h"
#include "igRect.h"

#include <string>

#define DEFAULT_CHARSET " QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890~!@#$%^&*()_+=-\\|{}[]:;\"'<>,.?/"

struct igContext
{
	float mouseX;
	float mouseY;

	int leftDown;

	bool backspace;
	wchar_t charEntered;

	int textCharPos;

	igIdent hotItem;
	igIdent activeItem;
	igIdent keyboardItem;
	igIdent dragItem;

	igRect dragRect;
	bool dragMoved;
	float dragX, dragY;
	float dragMouseX, dragMouseY;
	std::string dragTitle;

	igContext();

    bool MouseInside(float x, float y, float width, float height);

	void Begin();
	void End();
    
	bool Button(igIdent id, float x, float y, float width, float height, const char* title);
	bool Checkbox(igIdent id, float x, float y, float width, float height, bool value);
	bool VSlider(igIdent id, float x, float y, float width, float height, float aspect, float& value);
	bool HSlider(igIdent id, float x, float y, float width, float height, float aspect, float& value);
	bool TextBox(igIdent id, float x, float y, float width, float height, std::string& value, const std::string& charset=DEFAULT_CHARSET);
	bool Drag(igIdent id, float x, float y, float width, float height, const char* title);
	bool Move(igIdent id, float& x, float& y, float width, float height, const char* title);
	bool Tab(igIdent id, float x, float y, float width, float height, const char* title, bool value);
};

#endif
