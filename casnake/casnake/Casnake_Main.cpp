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
	
	///// ���j���[��� /////
	init.Dxlib_init();
	init.Load_init_menu();
	////////////////////////
	
	WaitTimer(100);
	Disp.Introduction();
	
	////// �摜�ǂݍ��� /////
	if(IsQuarter == IDNO){
		init.Load_init();
	}else{
		init.Load_init_Q();
	}
	/////////////////////////

	// �G���^�[�L�[���͑҂�
	while(CheckHitKey(KEY_INPUT_RETURN) == 0){
		if(ProcessMessage() == -1){
			 break;
		}
	}
	//WaitKey();

	while(IsContinue == IDYES)
	{	
		///// ������ /////
		mapmake.map_init();
		item.item_init();
		//////////////////
		
		////// map����� /////
		_rect first_split_rect = {0, 0, MAP_W -1, MAP_H -1};
		
		mapmake.rect_split(&first_split_rect, 3);
		mapmake.room_make();
		mapmake.list_to_map();
		//////////////////////
		
		////// �A�C�e���ݒu //////
		item.set_item();
		//////////////////////////
		
		////// ���͌��m��`�揈��(�Q�[���{��) //////
		IsContinue = Disp.game_loop(IsContinue);
		////////////////////////////////

		////// �R���e�i���̒��g���J���� //////
		variable.VariableClear();
		//////////////////////////////////////
	}

	// ���ӁI
	Disp.Thanks();
	WaitKey();
	DxLib_End();

	return 0; 
}