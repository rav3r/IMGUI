#ifndef __IGRECT_H__
#define __IGRECT_H__

struct igRect
{
	float x, y;
	float w, h;

	igRect(float _x, float _y, float _w, float _h): x(_x), y(_y), w(_w), h(_h) {}
	igRect(): w(0), h(0), x(0), y(0) {}
	igRect(const igRect& other): x(other.x), y(other.y), w(other.w), h(other.h) {}
};

#endif