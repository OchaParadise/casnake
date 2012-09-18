#ifndef INCLUDED_MAPMAKE
#define INCLUDED_MAPMAKE

#include "Variable.h"

class MapMake
{
public:
	void map_init();
	void rect_split(_rect *rect_parent, int precutDirect);
	void room_make();
	void list_to_map();
	_rect *rect_add(int lx, int ly, int hx, int hy, int cutDirection, int precutDirection);
	_room *room_add(int lx, int ly, int hx, int hy, _path path, _path path_front);
};

#endif