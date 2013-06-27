#ifndef __IGCONTEXT_H__
#define __IGCONTEXT_H__

#include "igWidget.h"
#include "igColor.h"
#include "igIdent.h"
#include "igRect.h"
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
		int currMaxHeight;
		igIdent id;
		int* offset;
		int scrollbar;
	} scrollArea;

	void AdjustNewScrollAreaHeight(int height);
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

	igContext();

	void ArrowLeftDown();
	void ArrowRightDown();

    bool MouseInside(float x, float y, float width, float height);

	bool MouseClipped();

	void Begin();
	igDragged End();
    
	igButton	Button(igIdent id, float x, float y, float width, float height, const char* title);
	igCheckbox	Checkbox(igIdent id, float x, float y, float width, float height, bool value);
	igTextbox	TextBox(igIdent id, float x, float y, float width, float height, std::string& value, const std::string& charset=DEFAULT_CHARSET);
	igLabel		Label(float x, float y, float width, float height, const std::string& text, igTextAlign valign = igTextAligns::CENTER);
	igSlider	HSlider(igIdent id, float x, float y, float width, float height, float& value);
	igMove		Move(igIdent id, float& x, float& y, float width, float height, const char* title);
	igVScroll	VScroll(igIdent id, float x, float y, float width, float height, float aspect, float& value);

	igDrag<igDraggable> Drag(igIdent id, float x, float y, float width, float height, const char* title, igDraggable* userData, igAcceptDrop fun=igNeverAcceptDrop);

	// scroll area

	igAreaBG BeginScrollArea(igIdent id, float x, float y, float width, float height, int& offset, bool scrollbar=true, igColor color=igColor(0.5f, 0.5f, 0.5f));
	igAreaFG EndScrollArea();

	void NewLine();

	void Indent();
	void Unindent();
	void Space(int width);

	// widgets

	igButton	Button(igIdent id, const char* title, int width=0);
	igCheckbox	Checkbox(igIdent id, bool value, const char* title);
	igLabel		Label(const std::string& text, igTextAlign halign=igTextAligns::CENTER, int width=0);
	igSlider	Slider(igIdent id, float& val, float minVal, float maxVal, int width=0);
	igTextbox	TextBox(igIdent id, std::string& value, int width=0);
	igSeparator	Separator();

	template <class R>
	igDrag<R> Drag(igIdent id, const char* title, igDraggable& userData, igAcceptDrop fun=igAlwaysAcceptDrop, int width=0)
	{
		const int x = scrollArea.currX;
		const int y = scrollArea.currY;
		
		bool maxSize = width == 0;

		if(maxSize)
		{
			int currXPos = scrollArea.currX-scrollArea.startX;
			width = scrollArea.width - currXPos - igSizing::SCROLLAREA_MARGIN_X - igSizing::SCROLLBAR_WIDTH*scrollArea.scrollbar;
		}

		if(MouseInside((float)x, (float)y, (float)width, igSizing::DRAG_HEIGHT) && dragPointer!=&userData &&
			dynamic_cast<R*>(dragPointer)!=0)
			canDrop = true;

		igDrag<igDraggable>& result = Drag(id, x, y, width, (float)igSizing::DRAG_HEIGHT, title, &userData, fun);

		if(maxSize)
		{
			AdjustNewScrollAreaHeight(igSizing::DRAG_HEIGHT);
			NewLine();
		} else 
			scrollArea.currX += width + igSizing::SCROLLAREA_MARGIN_X;

		igDrag<R> drag;
		drag = result;
		return drag;
	}
};

#endif
