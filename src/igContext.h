#ifndef __IGCONTEXT_H__
#define __IGCONTEXT_H__

#include "igEvents.h"
#include "igIdent.h"
#include "igRect.h"
#include "igRenderer.h"

#include <string>

#define DEFAULT_CHARSET " QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890~!@#$%^&*()_+=-\\|{}[]:;\"'<>,.?/"

namespace igItemStates
{
	enum eItemState
	{
		NONE			= 0,
		PRESSED			= 1,
		HOVER			= 2,
		TEXT_FOCUS		= 4
	};
}

typedef igItemStates::eItemState igItemState;

typedef bool (*igAcceptDrop)(void* ptr);

static bool igAlwaysAcceptDrop(void* ptr)
{
	return true;
}

static bool igNeverAcceptDrop(void* ptr)
{
	return false;
}

class igDraggable
{
public:
	virtual ~igDraggable() {}
};

struct igContext
{
	float mouseX;
	float mouseY;

	int mouseWheel;
	int leftDown;
	int leftLastDown;

	bool LeftJustUp();

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
	igDraggable* dragPointer;
	bool dragMissing;
	bool canDrop;

	igRenderer* renderer;

	igContext(igRenderer* rend);

    bool MouseInside(float x, float y, float width, float height);

	void Begin();
	void End();
    
	igButton Button(igIdent id, float x, float y, float width, float height, const char* title);
	bool Checkbox(igIdent id, float x, float y, float width, float height, bool value);
	bool VSlider(igIdent id, float x, float y, float width, float height, float aspect, float& value);
	bool HSlider(igIdent id, float x, float y, float width, float height, float aspect, float& value);
	bool TextBox(igIdent id, float x, float y, float width, float height, std::string& value, const std::string& charset=DEFAULT_CHARSET);
	igDraggable* Drag(igIdent id, float x, float y, float width, float height, const char* title, igDraggable* userData, igAcceptDrop fun=igNeverAcceptDrop);
	bool Move(igIdent id, float& x, float& y, float width, float height, const char* title);
	bool Tab(igIdent id, float x, float y, float width, float height, const char* title, bool value);

	// scroll area
	struct
	{
		int startX, startY;
		int currX, currY;
		int width, height;
		igIdent id;
		int* offset;
	} scrollArea;

	void BeginScrollArea(igIdent id, float x, float y, float width, float height, int& offset);
	void EndScrollArea(bool scrollbarRight);

	igButton Button(igIdent id, const char* title);
	bool Checkbox(igIdent id, bool value, const char* title);
	bool TextBox(igIdent id, std::string& value);
	igDraggable* Drag(igIdent id, const char* title, igDraggable* userData, igAcceptDrop fun=igAlwaysAcceptDrop);

	template <class R>
	R* Drag(igIdent id, const char* title, igDraggable& userData, igAcceptDrop fun=igAlwaysAcceptDrop)
	{
		const int marginX = 5;
		const int marginY = 5;
		const int height = 30;
		
		float x = scrollArea.currX+marginX;
		float y = scrollArea.currY;
		float width = scrollArea.width-2*marginX;

		if(MouseInside(x, y, width, height) && dynamic_cast<R*>(dragPointer)!=0)
			canDrop = true;

		igDraggable* result = Drag(id, x, y, width, height, title, &userData, fun);

		scrollArea.currY += height + marginY;

		return dynamic_cast<R*>(result);
	}
};

#endif
