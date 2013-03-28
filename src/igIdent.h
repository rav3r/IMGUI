#ifndef __IGIDENT_H__
#define __IGIDENT_H__

#ifdef IMGUI_SRC_ID
#define IMGUI_LINE_ID ((IMGUI_SRC_ID) + (__LINE__))
#else
#define IMGUI_LINE_ID (__LINE__)
#endif

#define GEN_ID(var) igIdent((void*)&var, IMGUI_LINE_ID)
#define GEN_NULL_ID igIdent(0, IMGUI_LINE_ID)

struct igIdent
{
	void* variable;
	int line;

	igIdent(void* v, int l): variable(v), line(l) {}	
	igIdent(const igIdent& id): variable(id.variable), line(id.line) {}
	igIdent(): variable(0), line(0) {}
	
};

#endif
