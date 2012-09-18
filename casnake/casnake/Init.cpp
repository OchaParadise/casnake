#include "DxLib.h"
#include "Init.h"
#include "Variable.h"
#include <ctime>
#include <cstdlib>

int Init::Dxlib_init()
{
	ChangeWindowMode(TRUE);	
	SetMainWindowText("casnake");
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);
	SetDrawScreen(DX_SCREEN_BACK);
	SetOutApplicationLogValidFlag(FALSE);
	if( DxLib_Init() == -1 ) return -1; // DX_Lib初期化処理
	srand((unsigned)time(NULL));

	//SetDrawScreen( DX_SCREEN_BACK );

	return 0;
}

int Init::Load_init_Q()
{
	// マップチップの読み込み
	GraphHandle_Floorchip = LoadGraph("img\\q_floor16.png");
	GetGraphSize(GraphHandle_Floorchip, &ChipSizeX, &ChipSizeY);

	// プレイヤー画像の読み込み
	GraphHandle_Player = LoadGraph("img\\q_player16.png");

	// アイテム画像の読み込み
	GraphHandle_Item = LoadGraph("img\\q_item16.png");

	// tail画像の読み込み
	GraphHandle_Tail = LoadGraph("img\\q_tail16.png");

	return 0;
}

int Init::Load_init()
{
	// マップチップの読み込み
	GraphHandle_Floorchip = LoadGraph("img\\floor8.png");
	GetGraphSize(GraphHandle_Floorchip, &ChipSizeX, &ChipSizeY);

	// プレイヤー画像の読み込み
	GraphHandle_Player = LoadGraph("img\\player8.png");

	// アイテム画像の読み込み
	GraphHandle_Item = LoadGraph("img\\item8.png");

	// tail画像の読み込み
	GraphHandle_Tail = LoadGraph("img\\tail8.png");

	return 0;
}

void Init::Load_init_menu()
{
	int SelectPoint = SPOINT_QUATER;
	int BlankSelectPoint = SPOINT_NORMAL;

	// メニュー画像の読み込み
	GraphHandle_Menu = LoadGraph("img\\top.png");

	// メニューの画像表示
	DrawGraph(0, 0, GraphHandle_Menu, TRUE);

	// 選択ポインタ画像と選択ポインタ消去用画像の読み込み
	GraphHandle_SelectPointer = LoadGraph("img\\selectpointer.png");
	GraphHandle_PointerMask = LoadGraph("img\\mask.png");

	// ループ
	while( ProcessMessage() == 0 )
	{
		// 選択ポインタ画像、選択ポインタ消去画像の描画
		DrawGraph(60, SelectPoint, GraphHandle_SelectPointer, TRUE);
		DrawGraph(60, BlankSelectPoint, GraphHandle_PointerMask, TRUE);

		// キー入力
		{
			// クオータービューを指している時に下キーD入力
			if(CheckHitKey(KEY_INPUT_S) != 0 || CheckHitKey(KEY_INPUT_DOWN) != 0 && SelectPoint == SPOINT_QUATER){
				SelectPoint = SPOINT_NORMAL;
				BlankSelectPoint = SPOINT_QUATER;
			}
			// ノーマルビューを指している時に上キーW入力
			if(CheckHitKey(KEY_INPUT_W) != 0 || CheckHitKey(KEY_INPUT_UP) != 0 && SelectPoint == SPOINT_NORMAL){
				SelectPoint = SPOINT_QUATER;
				BlankSelectPoint = SPOINT_NORMAL;
			}

			// 決定キー(エンター)時処理
			if(CheckHitKey(KEY_INPUT_RETURN) != 0)
			{
				if(SelectPoint == SPOINT_QUATER){
					IsQuarter = IDYES;
				}else{
					IsQuarter = IDNO;
				}
				ClearDrawScreen();
				break ;
			}
		}
		//ScreenFlip();
	}

	WaitTimer(100);
}