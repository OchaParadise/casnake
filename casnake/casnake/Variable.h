#ifndef INCLUDED_VARIABLE
#define INCLUDED_VARIABLE

#include <vector>

using namespace std;

enum{
	LEFT = 1,
	RIGHT = 2,
	UP = 3,
	DOWN = 4,

	cutX = 0,
	cutY = 1,

	BLANK = 0,
	WALL = 1,
	ROOM = 2,
	PATH = 3,
	PATH_FRONT = 4,
	PATH_OF_WALL = 5,
	APPEND_PATH = 6,

	PLAYER = 7,
	ITEM = 8,
	TAIL = 9,

	MAP_W = 90,
	MAP_H = 90,
	MINIMUM_ROOM_SIZE = 4,
	MARGIN_BETWEEN_RECT_ROOM = 3,
	MINIMUM_RECT_SIZE = MINIMUM_ROOM_SIZE + (MARGIN_BETWEEN_RECT_ROOM *2),

	SCREEN_SIZE_X = 640,
	SCREEN_SIZE_Y = 480,

	SPOINT_QUATER = 225,
	SPOINT_NORMAL = 295,

	GAME_OVER = 1
};

typedef struct path{
	int pathX, pathW, pathY, pathH;
	bool isPathVertical;
}_path;

typedef struct room{
	int lx, ly, hx, hy;
	_path path;
	_path path_front;
}_room;

typedef struct rect{
	int lx, ly, hx, hy, cutDirection, precutDirection;
	_room *room;
}_rect;

typedef struct chara{
	int chara_x, chara_y;
}_character;

typedef struct tail{
	int isTail, tailNum;
}_tail;

extern int GraphHandle_Floorchip;
extern int GraphHandle_Player;
extern int GraphHandle_Item;
extern int GraphHandle_Tail;
extern int GraphHandle_ContinueBottun;
extern int GraphHandle_EndBottun;
extern int GraphHandle_Menu;
extern int GraphHandle_SelectPointer;
extern int GraphHandle_PointerMask;
extern int GraphHandle_Introduction;
extern int ChipSizeX, ChipSizeY;

extern int map[MAP_W][MAP_H];
extern int chara_map[MAP_W][MAP_H];
extern int item_map[MAP_W][MAP_H];
extern _character tail_array[MAP_W * MAP_H];

extern vector<_rect> rect_list;
extern vector<_room> room_list;
extern vector<_character> item_list;

extern _character player;

extern int LATEST_HIT_DIRECT;
extern int Player_Length;
extern int LeftItemNum;
extern bool IsJustAfterPlayerInit;
extern int FirstItemNum;

extern int IsQuarter;

class Variable
{
public:
	void VariableClear();
};

#endif