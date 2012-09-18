#include <vector>
#include "Variable.h"
#include "Item.h"
#include "Calc.h"
#include "DxLib.h"

// �Ƃ����璷���̑�����A�C�e���z��̏�����
void Item::item_init()
{
	int i, j;

	for(j = 0; j < MAP_H; j++){
		for(i = 0; i < MAP_W; i++){
			item_map[i][j] = BLANK;
		}
	}
}

// �A�C�e���̐ݒu
void Item::set_item()
{
	Calc calc;
	int i, j;
	int setItem;

	vector<_room>::iterator it_room = room_list.begin();
	while(it_room != room_list.end()){
		for(i = it_room->lx; i <= it_room->hx; i++){
			for(j = it_room->ly; j <= it_room->hy; j++){
				// 8�}�X�Ɉ���x�ŃA�C�e����ݒu
				setItem = calc.Random(0, 50);
				if(setItem == 0 || setItem == 10 || setItem == 50){
					item_map[i][j] = ITEM;
					LeftItemNum++;
				}
			}
		}
		it_room++;
	}
	FirstItemNum = LeftItemNum;
}