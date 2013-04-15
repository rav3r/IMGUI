#ifndef __IGEVENTS_H__
#define __IGEVENTS_H__

struct igButton
{
	bool onClicked;
	bool onDown;
	bool onHover;

	operator bool() const
	{
		return onClicked || onDown || onHover;
	}
};

#endif