#ifndef __IGWIDGET_H__
#define __IGWIDGET_H__

#include "igRect.h"

#include <string>

// usage
// if(igWidget widget = DrawButton(gui.Button(GEN_ID, "button")) && widget.clicked) {...}

struct igWidget
{
	igRect rect;

	operator bool() const
	{
		return true;
	}
};

struct igButton: public igWidget
{
	igButton(): clicked(false), down(false), hover(false), title(0) {}
	
	bool clicked;
	bool down;
	bool hover;
	const char* title;
};

struct igCheckbox: public igWidget
{
	igCheckbox(): value(false), clicked(false), down(false), hover(false) {}
	
	bool value;
	bool clicked;
	bool down;
	bool hover;
};

struct igTextbox: public igWidget
{
	igTextbox(): down(false), hover(false), textFocus(false) {}

	std::string value;
	bool down;
	bool hover;
	bool textFocus;
	int  pipePos1;
	int  pipePos2;
};

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

struct igLabel: public igWidget
{
	igLabel(): align(igTextAligns::CENTER) {}

	std::string value;
	igTextAlign align;
};

struct igSlider: public igWidget
{
	igSlider(): hover(false), down(false) {}

	float value;
	bool hover;
	bool down;
};

template <class R>
struct igDrag: public igWidget
{
	igDrag(): drop(0), hover(false), down(false) {}
	
	R* drop;
	bool hover;
	bool down;
	std::string title;

	template <class A>
	igDrag<R>& operator=(igDrag<A>& a)
	{
		rect	= a.rect;
		drop	= reinterpret_cast<R*>(a.drop);
		hover	= a.hover;
		down	= a.down;
		title	= a.title;
		
		return *this;
	}
};

struct igMove: public igWidget
{
	igMove(): hover(false), down(false), moved(false) {}

	bool hover;
	bool down;
	bool moved;

	int prevX;
	int prevY;
};

struct igVScroll: public igWidget
{
	igVScroll(): changed(false) {}
	bool changed;
	float aspect;
	float value;
};

struct igSeparator: public igWidget
{

};

struct igAreaBG: public igWidget
{

};

struct igAreaFG: public igWidget
{
	igVScroll scroll;
};

struct igDragged: public igWidget
{
	igDragged(): canDrop(false), show(false) {}
	bool canDrop;
	std::string title;
	bool show;
};

#endif
