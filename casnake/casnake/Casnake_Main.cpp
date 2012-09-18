#include "DxLib.h"
#include "Variable.h"
#include "Calc.h"
#include "Init.h"
#include "MapMake.h"
#include "Display.h"
#include "Character.h"
#include "Item.h"
#include <Windows.h>


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{
	Init init;
	Display Disp;
	MapMake mapmake;
	Character character;
	Variable variable;
	Item item;

	int IsContinue = IDYES;
	
	///// メニュー画面 /////
	init.Dxlib_init();
	init.Load_init_menu();
	////////////////////////
	
	WaitTimer(100);
	Disp.Introduction();
	
	////// 画像読み込み /////
	if(IsQuarter == IDNO){
		init.Load_init();
	}else{
		init.Load_init_Q();
	}
	/////////////////////////

	// エンターキー入力待ち
	while(CheckHitKey(KEY_INPUT_RETURN) == 0){
		if(ProcessMessage() == -1){
			 break;
		}
	}
	//WaitKey();

	while(IsContinue == IDYES)
	{	
		///// 初期化 /////
		mapmake.map_init();
		item.item_init();
		//////////////////
		
		////// mapを作る /////
		_rect first_split_rect = {0, 0, MAP_W -1, MAP_H -1};
		
		mapmake.rect_split(&first_split_rect, 3);
		mapmake.room_make();
		mapmake.list_to_map();
		//////////////////////
		
		////// アイテム設置 //////
		item.set_item();
		//////////////////////////
		
		////// 入力検知や描画処理(ゲーム本体) //////
		IsContinue = Disp.game_loop(IsContinue);
		////////////////////////////////

		////// コンテナ等の中身をカラに //////
		variable.VariableClear();
		//////////////////////////////////////
	}

	// 感謝！
	Disp.Thanks();
	WaitKey();
	DxLib_End();

	return 0; 
}