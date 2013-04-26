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

// todo: rename
const int newLineSize = 34;
const int marginY = 2;
const int marginX = 5;

class igContext
{
private:
	// clipping mouse area
	struct
	{
		float x, y;
		float width, height;
		bool active;
	} currentMouseClipping;

	// scroll area
	struct
	{
		int startX, startY;
		int currX, currY;
		int width, height;
		int indent;
		igIdent id;
		int* offset;
	} scrollArea;
public:
	float mouseX;
	float mouseY;

	int mouseWheel;
	int leftDown;
	int leftLastDown;

	bool LeftJustUp();

	bool backspace;
	wchar_t charEntered;

	bool shift;

	int textCharPos;
	int textCharPos2;

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

	void ArrowLeftDown();
	void ArrowRightDown();

    bool MouseInside(float x, float y, float width, float height);

	bool MouseClipped();

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

	// Scroll area

	void BeginScrollArea(igIdent id, float x, float y, float width, float height, int& offset);
	void EndScrollArea(bool scrollbarRight);

	void NewLine();

	void Indent();
	void Unindent();
	void Separator();

	igButton Button(igIdent id, const char* title, int width=0);
	bool Checkbox(igIdent id, bool value, const char* title);
	bool TextBox(igIdent id, std::string& value, int width=0);
	void Space(int width);

	template <class R>
	R* Drag(igIdent id, const char* title, igDraggable& userData, igAcceptDrop fun=igAlwaysAcceptDrop, int width=0)
	{
		const int marginX = 5;
		const int height = 30;
		const int x = scrollArea.currX;
		const int y = scrollArea.currY;
		
		bool maxSize = width == 0;

		if(maxSize)
			width = scrollArea.width - (scrollArea.currX-scrollArea.startX) - marginX;

		if(MouseInside(x, y, width, height) && dragPointer!=&userData &&
			dynamic_cast<R*>(dragPointer)!=0)
			canDrop = true;

		igDraggable* result = Drag(id, x, y, width, height, title, &userData, fun);

		if(maxSize)
			NewLine();
		else 
			scrollArea.currX += width + marginX;

		return dynamic_cast<R*>(result);
	}
};

#endif
