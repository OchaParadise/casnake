#include "Variable.h"
#include "Character.h"
#include "DxLib.h"
#include <vector>

using namespace std;

int GraphHandle_Floorchip;
int GraphHandle_Player;
int GraphHandle_Item;
int GraphHandle_Tail;
int GraphHandle_ContinueBottun;
int GraphHandle_EndBottun;
int GraphHandle_Menu;
int GraphHandle_SelectPointer;
int GraphHandle_PointerMask;
int GraphHandle_Introduction;
int ChipSizeX, ChipSizeY;

int map[MAP_W][MAP_H];
int chara_map[MAP_W][MAP_H];
int item_map[MAP_W][MAP_H];
_character tail_array[MAP_W * MAP_H];

vector<_rect> rect_list;
vector<_room> room_list;
vector<_character> item_list;

_character player;

int LATEST_HIT_DIRECT = 100;
int Player_Length = 0;
int LeftItemNum = 0;
bool IsJustAfterPlayerInit = true;
int FirstItemNum = 0;

int IsQuarter;

void Variable::VariableClear()
{
	Character character;
	character.player_init();
	rect_list.clear();
	room_list.clear();
	item_list.clear();
	LATEST_HIT_DIRECT = 0;
	Player_Length = 0;
	LeftItemNum = 0;
	FirstItemNum = 0;
	IsJustAfterPlayerInit = true;

	vector<_rect> rect_list_for_swap;
	rect_list.swap(rect_list_for_swap);

	vector<_room> room_list_for_swap;
	room_list.swap(room_list_for_swap);

	vector<_character> item_list_for_swap;
	item_list.swap(item_list_for_swap);
}