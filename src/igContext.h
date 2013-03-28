#ifndef __IGCONTEXT_H__
#define __IGCONTEXT_H__

#include "igIdent.h"

struct igContext
{
	float mouseX;
	float mouseY;

	int leftDown;

	igIdent hotItem;
	igIdent activeItem;

	igContext();

    bool MouseInside(float x, float y, float width, float height);

	void Begin();
	void End();
    
	bool Button(igIdent id, float x, float y, float width, float height, const char* title);
};

#endif
