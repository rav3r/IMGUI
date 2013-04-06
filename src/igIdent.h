#ifndef __IGIDENT_H__
#define __IGIDENT_H__

#ifdef IMGUI_SRC_ID
#define IMGUI_LINE_ID ((IMGUI_SRC_ID) + (__LINE__))
#else
#define IMGUI_LINE_ID (__LINE__)
#endif

#define GEN_ID(var) igIdent((void*)&var, IMGUI_LINE_ID)
#define GEN_IID(i) igIdent(i, IMGUI_LINE_ID)
#define GEN_NULL_ID igIdent(0, IMGUI_LINE_ID)

struct igIdent
{
	void* variable;
	int line;
	int num;

	igIdent(void* v, int l): variable(v), line(l), num(0) {}
	igIdent(int n, int l): variable(0), line(l), num(n) {}
	igIdent(const igIdent& id): variable(id.variable), line(id.line), num(id.num) {}
	igIdent(): variable(0), line(0), num(0) {}
	
};

inline bool operator==(const igIdent& a, const igIdent& b)
{
	return a.variable == b.variable && a.line == b.line && a.num == b.num;
}

inline bool operator!=(const igIdent& a, const igIdent& b)
{
	return a.variable != b.variable || a.line != b.line || a.num != b.num;
}

#endif
