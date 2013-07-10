#ifndef __IGRECT_H__
#define __IGRECT_H__

struct igRect
{
	int x, y;
	int w, h;

	igRect(int _x, int _y, int _w, int _h): x(_x), y(_y), w(_w), h(_h) {}
	igRect(): w(0), h(0), x(0), y(0) {}
	igRect(const igRect& other): x(other.x), y(other.y), w(other.w), h(other.h) {}
};

#endif