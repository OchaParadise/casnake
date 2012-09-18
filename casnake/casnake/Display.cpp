#include "Display.h"
#include "Variable.h"
#include "DxLib.h"
#include "Character.h"
#include <stdio.h>
#include <Windows.h>
#include "IsometricView.h"

// マップとプレイヤーの描画(クォータービューでない時)
void Display::disp_map()
{	
	int MapDrawPointX , MapDrawPointY ;      // 描画するマップ座標値
	int DrawMapChipNumX , DrawMapChipNumY ;	 // 描画するマップチップの数

	// 描画するマップチップの数(量)をセット
	DrawMapChipNumX = SCREEN_SIZE_X / ChipSizeX + 1 ;
	DrawMapChipNumY = SCREEN_SIZE_Y / ChipSizeY + 1 ;

	// 画面左上に描画するmap配列の番号をセット
	MapDrawPointX = player.chara_x - ((SCREEN_SIZE_X/2)/ChipSizeX);
	MapDrawPointY = player.chara_y - ((SCREEN_SIZE_Y/2)/ChipSizeY);

	// 描画
	for(int j = MapDrawPointY; j < (MapDrawPointY + DrawMapChipNumY); j++)
	{
		for(int i = MapDrawPointX; i < (MapDrawPointX + DrawMapChipNumX); i++)
		{
			// 画面からはみ出た位置は描画しない
			if(i < 0 || j < 0 || i >= MAP_W || j >= MAP_H )
			{
				continue;
			}
			// マップチップの描画
			if(map[i][j] == ROOM || map[i][j] == PATH || map[i][j] == PATH_FRONT || map[i][j] == PATH_OF_WALL || map[i][j] == APPEND_PATH){
				DrawGraph((i - MapDrawPointX)*ChipSizeX, (j - MapDrawPointY)*ChipSizeY, GraphHandle_Floorchip, TRUE);
			}
			// アイテムの描画
			if(item_map[i][j] == ITEM){
				DrawGraph((i - MapDrawPointX)*ChipSizeX, (j - MapDrawPointY)*ChipSizeY, GraphHandle_Item, TRUE);
			}
			// 残りアイテム数表示
			int Color = GetColor(255, 255, 255);
			DrawFormatString(0, 0, Color, "残り:%d", LeftItemNum);
			DrawFormatString(0, 20, Color, "長さ:%d", Player_Length);
		}
	}

	// キャラクターだけ描画
	for(int j = MapDrawPointY; j < (MapDrawPointY + DrawMapChipNumY); j++)
	{
		for(int i = MapDrawPointX; i < (MapDrawPointX + DrawMapChipNumX); i++)
		{
			// プレイヤーの描画
			if(chara_map[i][j] == PLAYER){
				DrawGraph((i - MapDrawPointX)*ChipSizeX, (j - MapDrawPointY)*ChipSizeY, GraphHandle_Player, TRUE);
				// しっぽの描画
				if(Player_Length != 0){
					tail_disp(i, j, MapDrawPointX, MapDrawPointY);
				}
			}
		}
	}

	WaitTimer(100);
}

// 入力検知から画面の描画までのループ(ゲーム本体に近い)
int Display::game_loop(int IsContinue)
{
	Character character;
	IsometricView iso;

	// プレイヤーの初期位置は一つ目の部屋の左上
	character.player_coordinate_init();
	LATEST_HIT_DIRECT = character.player_input();
	// IsometricView表示
	if(IsQuarter == IDYES){
		//iso.DisplayIsometricView();

		IsContinue = IDYES;
		while(ProcessMessage() == 0 /*&& CheckHitKey(KEY_INPUT_ESCAPE) == 0*/)
		{
			//ClearDrawScreen();
			LATEST_HIT_DIRECT = character.player_input();

			if(character.player_move() == GAME_OVER){
				// プレイヤーの移動先が空間かtailの時ゲームオーバー
				return MessageGameOver();
			}

			ClearDrawScreen();
			iso.DisplayIsometricView();

			LATEST_HIT_DIRECT = character.player_input();
			//ScreenFlip(); 
			// 残りのアイテムが無くなったらクリア
			if(LeftItemNum == 0){
				return MessageEnding();
			}
		}
	}

	// IsometricViewでない時
	if(IsQuarter == IDNO){
		//disp_map();
		LATEST_HIT_DIRECT = character.player_input();
		IsContinue = IDYES;
		while(ProcessMessage() == 0 /*&& CheckHitKey(KEY_INPUT_ESCAPE) == 0*/)
		{
			//ClearDrawScreen();
			LATEST_HIT_DIRECT = character.player_input();

			character.player_input();
			if(character.player_move() == GAME_OVER){
				// プレイヤーの移動先が空間かtailの時ゲームオーバー
				return MessageGameOver();
			}

			ClearDrawScreen();
			disp_map();
			
			LATEST_HIT_DIRECT = character.player_input();
			//ScreenFlip(); 
			// 残りのアイテムが無くなったらクリア
			if(LeftItemNum == 0){
				return MessageEnding();
			}
		}
	}

	return 0;
}

// しっぽの描画
void Display::tail_disp(int numX, int numY, int DrawPointX, int DrawPointY)
{
	// tail_arrayの0番目は欠番。尻尾の座標情報は1番目から
	for(int tailCount = 1; tailCount <= Player_Length; tailCount++){
		DrawGraph((tail_array[tailCount].chara_x - DrawPointX)*ChipSizeX, (tail_array[tailCount].chara_y - DrawPointY)*ChipSizeY, GraphHandle_Tail, TRUE);
	}
}

// クリア時のメッセージ
int Display::MessageEnding()
{
	TCHAR msg[100] = {NULL};
	wsprintf(msg, "%d個のアイテムを入手し、クリアしました! \n もう一度プレイしますか?", FirstItemNum);
	return MessageBox(NULL, msg , "おめでとうございます!", MB_ICONQUESTION | MB_YESNO);
}

// ゲームオーバーのメッセージ
int Display::MessageGameOver()
{
	return MessageBox(NULL, "GameOver \n もう一度プレイしますか?", "GameOver" , MB_ICONEXCLAMATION | MB_YESNO);
}

// クォータービューかノーマルビューか選択メッセージ
int Display::MessageSelectMode()
{
	return MessageBox(NULL, "クォータービューでプレイしますか?", "SelectView" , MB_ICONQUESTION | MB_YESNO);
}

// 説明画面表示
void Display::Introduction()
{
	// 説明画面画像の読み込みと表示
	if(IsQuarter == IDYES){
		GraphHandle_Introduction = LoadGraph("img\\q_introduction.png");
		DrawGraph(0, 0, GraphHandle_Introduction, TRUE);
	}else{
		GraphHandle_Introduction = LoadGraph("img\\n_introduction.png");
		DrawGraph(0, 0, GraphHandle_Introduction, TRUE);
	}
	//ScreenFlip();
}

// 感謝！
void Display::Thanks()
{
	int Color = GetColor(255, 255, 255);
	//ClearDrawScreen();
	SetFontSize(30);
	DrawFormatString(155, 200, Color, "Thank you for playing!");
	//ScreenFlip();
}