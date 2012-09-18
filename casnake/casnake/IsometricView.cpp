#include "Variable.h"
#include "IsometricView.h"
#include "Character.h"
#include "DxLib.h"

// クォータービューでのマップ描画
void IsometricView::DisplayIsometricView_Map()
{
	int LoopCount = 0;

	for(int j = 0; j < MAP_H; j++ )
	{
		for(int i = 0; i < MAP_W; i++)
		{
			int StartX_OffSet = LoopCount * ChipSizeX/2;
			int StartY_OffSet = LoopCount * ChipSizeY/4;

			if(map[i][j] == ROOM || map[i][j] == PATH || map[i][j] == PATH_FRONT || map[i][j] == PATH_OF_WALL || map[i][j] == APPEND_PATH)
			{
				DrawGraph((StartX_OffSet + SCREEN_SIZE_X/2) - i*ChipSizeX/2, StartY_OffSet + i*ChipSizeY/4, GraphHandle_Floorchip, TRUE);
			}
		}
		LoopCount++;
	}
}

// クォータービューでのキャラクター描画
void IsometricView::DisplayIsometricView_Character()
{
	int LoopCount = 0;

	for(int j = 0; j < MAP_H; j++ )
	{
		for(int i = 0; i < MAP_W; i++)
		{
			int StartX_OffSet = j * ChipSizeX/2;
			int StartY_OffSet = j * ChipSizeY/4;

			if(chara_map[i][j] == PLAYER)
			{
				DrawGraph((StartX_OffSet + SCREEN_SIZE_X/2) - i*ChipSizeX/2, StartY_OffSet + i*ChipSizeY/4, GraphHandle_Player, TRUE);
			}
		}
		LoopCount++;
	}
}

// クォータービューでの描画
void IsometricView::DisplayIsometricView()
{
	Character character;

	int LoopCount = 0;

	int OffSetX;
	int OffSetY;

	int tailCount;
	int tail_i, tail_j;
	int Tail_ScreenX, Tail_ScreenY;
	int tail_StartX_OffSet, tail_StartY_OffSet;

/*	// プレイヤーを中心に置くためのオフセットを計算
	for(int j = 0; j < MAP_H; j++ )
	{
		for(int i = 0; i < MAP_W; i++)
		{
			int StartX_OffSet = j * ChipSizeX/2;
			int StartY_OffSet = j * ChipSizeY/4;

			int ScreenX  = -StartX_OffSet + i*ChipSizeX/2;
			int ScreenY = StartY_OffSet + i*ChipSizeY/4;

			if(chara_map[i][j] == PLAYER)
			{
				OffSetX = SCREEN_SIZE_X/2 - ScreenX;
				OffSetY = SCREEN_SIZE_Y/2 - ScreenY;
				break;
			}
		}
	}
*/
	// プレイヤーを中心に置くためのオフセットを計算
	int StartX_OffSet = player.chara_y * ChipSizeX/2;
	int StartY_OffSet = player.chara_y * ChipSizeY/4;
	
	int ScreenX  = -StartX_OffSet + player.chara_x*ChipSizeX/2;
	int ScreenY = StartY_OffSet + player.chara_x*ChipSizeY/4;

	OffSetX = SCREEN_SIZE_X/2 - ScreenX;
	OffSetY = SCREEN_SIZE_Y/2 - ScreenY;


	LATEST_HIT_DIRECT = character.player_input();
	
	for(int j = 0; j < MAP_H; j++ )
	{
		for(int i = 0; i < MAP_W; i++)
		{
			int StartX_OffSet = LoopCount * ChipSizeX/2;
			int StartY_OffSet = LoopCount * ChipSizeY/4;

			int ScreenX  = -StartX_OffSet + i*ChipSizeX/2;
			int ScreenY = StartY_OffSet + i*ChipSizeY/4;
			
			if(OffSetX + ScreenX < 0 || OffSetY + ScreenY < 0 || OffSetX + ScreenX > SCREEN_SIZE_X || OffSetY + ScreenY < 0){
				continue;
			}

			// マップチップの描画
			if(map[i][j] == ROOM || map[i][j] == PATH || map[i][j] == PATH_FRONT || map[i][j] == PATH_OF_WALL || map[i][j] == APPEND_PATH)
			{
				DrawGraph(OffSetX + ScreenX, OffSetY + ScreenY, GraphHandle_Floorchip, TRUE);
			}

			// アイテムの描画
			if(item_map[i][j] == ITEM)
			{
				DrawGraph(OffSetX + ScreenX, OffSetY + ScreenY, GraphHandle_Item, TRUE);
			}

			// 尻尾の描画
			tail_StartX_OffSet = j * ChipSizeX/2;
			tail_StartY_OffSet = j * ChipSizeY/4;

			if(Player_Length != 0){
				for(tailCount = 1; tailCount <= Player_Length; tailCount++){
					if(tail_array[tailCount].chara_x == i && tail_array[tailCount].chara_y == j)
					{
						Tail_ScreenX  = -tail_StartX_OffSet + tail_array[tailCount].chara_x*(ChipSizeX/2);
						Tail_ScreenY = tail_StartY_OffSet + tail_array[tailCount].chara_x*(ChipSizeY/4);
						
						DrawGraph(OffSetX + Tail_ScreenX, OffSetY + Tail_ScreenY, GraphHandle_Tail, TRUE);
					}
					LATEST_HIT_DIRECT = character.player_input();
				}
			}

			// プレイヤーの描画
			if(chara_map[i][j] == PLAYER)
			{
				DrawGraph(OffSetX + ScreenX, OffSetY + ScreenY, GraphHandle_Player, TRUE);
			}	

			// 残りアイテム数表示
			int Color = GetColor(255, 255, 255);
			DrawFormatString(0, 0, Color, "残り:%d", LeftItemNum);
			DrawFormatString(0, 20, Color, "長さ:%d", Player_Length);
			LATEST_HIT_DIRECT = character.player_input();
		}

		LATEST_HIT_DIRECT = character.player_input();
		LoopCount++;
	}

/*
	//int tailCount;
	//int tail_i, tail_j;
	//int tail_StartX_OffSet, tail_StartY_OffSet;
	//int Tail_ScreenX, Tail_ScreenY;
	
	// 尻尾の描画
	if(Player_Length != 0)
	{
		for(tailCount = 1; tailCount <= Player_Length; tailCount++)
		{
			for(tail_j = 0; tail_j < MAP_H; tail_j++ )
			{
				for(tail_i = 0; tail_i < MAP_W; tail_i++)
				{
					tail_StartX_OffSet = tail_j * ChipSizeX/2;
					tail_StartY_OffSet = tail_j * ChipSizeY/4;
					
					//int ScreenX  = -tail_StartX_OffSet + tail_i*ChipSizeX/2;
					//int ScreenY = tail_StartY_OffSet + tail_i*ChipSizeY/4;
							
					if(tail_array[tailCount].chara_x == tail_i && tail_array[tailCount].chara_y == tail_j)
					{
						Tail_ScreenX  = -tail_StartX_OffSet + tail_array[tailCount].chara_x*(ChipSizeX/2);
						Tail_ScreenY = tail_StartY_OffSet + tail_array[tailCount].chara_x*(ChipSizeY/4);
						
						DrawGraph(OffSetX + Tail_ScreenX, OffSetY + Tail_ScreenY, GraphHandle_Tail, TRUE);
					}
				}
			}
		}
	}
*/
/*	
	// プレイヤーの描画
	for(int j = 0; j < MAP_H; j++ )
	{
		for(int i = 0; i < MAP_W; i++)
		{
			int StartX_OffSet = j * ChipSizeX/2;
			int StartY_OffSet = j * ChipSizeY/4;

			int ScreenX  = -StartX_OffSet + i*ChipSizeX/2;
			int ScreenY = StartY_OffSet + i*ChipSizeY/4;
			
			if(chara_map[i][j] == PLAYER)
			{
				DrawGraph(OffSetX + ScreenX, OffSetY + ScreenY, GraphHandle_Player, TRUE);
			}	
		}
	}
*/
	for(int waittime = 0; waittime < 15; waittime++){
		LATEST_HIT_DIRECT = character.player_input();
		WaitTimer(15);
	}
	//WaitTimer(200);
}