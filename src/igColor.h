#ifndef __IGCOLOR_H__
#define __IGCOLOR_H__

struct igColor
{
	igColor(): r(1), g(1), b(1), a(1) {}
	igColor(float R, float G, float B, float A=1): r(R), g(G), b(B), a(A) {}
	igColor(const igColor& c): r(c.r), g(c.g), b(c.b), a(c.a) {}
	float r, g, b, a;
};

#endif