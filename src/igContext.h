#ifndef __IGCONTEXT_H__
#define __IGCONTEXT_H__

#include "igEvents.h"
#include "igIdent.h"
#include "igRect.h"
#include "igRenderer.h"
#include "igSizing.h"

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

namespace igTextAligns
{
	enum eTextAlign
	{
		LEFT,
		RIGHT,
		CENTER
	};
}

typedef igTextAligns::eTextAlign igTextAlign;

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
	bool ctrlV;
	bool ctrlX;
	bool ctrlC;
	bool ctrlA;

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
	bool VScrollbar(igIdent id, float x, float y, float width, float height, float aspect, float& value);
	bool HScrollbar(igIdent id, float x, float y, float width, float height, float aspect, float& value);
	bool TextBox(igIdent id, float x, float y, float width, float height, std::string& value, const std::string& charset=DEFAULT_CHARSET);
	igDraggable* Drag(igIdent id, float x, float y, float width, float height, const char* title, igDraggable* userData, igAcceptDrop fun=igNeverAcceptDrop);
	bool Move(igIdent id, float& x, float& y, float width, float height, const char* title);
	bool Tab(igIdent id, float x, float y, float width, float height, const char* title, bool value);
	void Label(float x, float y, float width, float height, const std::string& text, igTextAlign valign = igTextAligns::CENTER);
	// Scroll area

	void BeginScrollArea(igIdent id, float x, float y, float width, float height, int& offset);
	void EndScrollArea();

	void NewLine(int newLineSize);

	void Indent();
	void Unindent();
	void Separator();

	igButton Button(igIdent id, const char* title, int width=0);
	bool Checkbox(igIdent id, bool value, const char* title);
	bool TextBox(igIdent id, std::string& value, int width=0);
	void Space(int width);
	void Label(const std::string& text, igTextAlign halign=igTextAligns::CENTER, int width=0);

	template <class R>
	R* Drag(igIdent id, const char* title, igDraggable& userData, igAcceptDrop fun=igAlwaysAcceptDrop, int width=0)
	{
		const int x = scrollArea.currX;
		const int y = scrollArea.currY;
		
		bool maxSize = width == 0;

		if(maxSize)
		{
			int currXPos = scrollArea.currX-scrollArea.startX;
			width = scrollArea.width - currXPos - igSizing::SCROLLAREA_MARGIN_X - igSizing::SCROLLBAR_WIDTH;
		}

		if(MouseInside(x, y, width, igSizing::DRAG_HEIGHT) && dragPointer!=&userData &&
			dynamic_cast<R*>(dragPointer)!=0)
			canDrop = true;

		igDraggable* result = Drag(id, x, y, width, igSizing::DRAG_HEIGHT, title, &userData, fun);

		if(maxSize)
			NewLine(igSizing::DRAG_HEIGHT + igSizing::SCROLLAREA_MARGIN_Y);
		else 
			scrollArea.currX += width + igSizing::SCROLLAREA_MARGIN_X;

		return dynamic_cast<R*>(result);
	}
};

#endif
