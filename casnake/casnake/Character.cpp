#include <vector>
#include "Variable.h"
#include "Character.h"
#include "DxLib.h"
#include "Display.h"
#include <Windows.h>

// プレイヤーの配列の初期化
void Character::player_init()
{
	int i, j;

	for(j = 0; j < MAP_H; j++){
		for(i = 0; i < MAP_W; i++){
			chara_map[i][j] = BLANK;
		}
	}
}

// プレイヤーの座標の初期化(一つ目の部屋の左上)
void Character::player_coordinate_init()
{
	// プレイヤー配列の初期化
	player_init();

	vector<_room>::iterator it_room = room_list.begin();

	// とりあえず一番最初の部屋リストの左上にプレイヤーをinit
	player.chara_x = it_room->lx;
	player.chara_y = it_room->ly;
	chara_map[player.chara_x][player.chara_y] = PLAYER;

	IsJustAfterPlayerInit = 0;
}

// 方向キー押下の判定
int Character::player_input()
{
	char Buf[256];

	GetHitKeyStateAll(Buf);

	// ただしtailのある状態で真逆方向は行けない
	if(Buf[KEY_INPUT_W] == 1 || Buf[KEY_INPUT_UP] == 1){
		if(Player_Length != 0 && IsJustAfterPlayerInit == false &&LATEST_HIT_DIRECT == DOWN)
		{
			return LATEST_HIT_DIRECT;
		}
		//IsJustAfterPlayerInit = false;
		return UP;
	}
	else if(Buf[KEY_INPUT_S] == 1 || Buf[KEY_INPUT_DOWN] == 1){
		if(Player_Length != 0 && IsJustAfterPlayerInit == false && LATEST_HIT_DIRECT == UP)
		{
			return LATEST_HIT_DIRECT;
		}
		//IsJustAfterPlayerInit = false;
		return DOWN;
	}
	else if(Buf[KEY_INPUT_A] == 1 || Buf[KEY_INPUT_LEFT] == 1){
		if(Player_Length != 0 && IsJustAfterPlayerInit == false && LATEST_HIT_DIRECT == RIGHT)
		{
			return LATEST_HIT_DIRECT;
		}
		//IsJustAfterPlayerInit = false;
		return LEFT;
	}
	else if(Buf[KEY_INPUT_D] == 1 || Buf[KEY_INPUT_RIGHT] == 1){
		if(Player_Length != 0 && IsJustAfterPlayerInit == false && LATEST_HIT_DIRECT == LEFT)
		{
			return LATEST_HIT_DIRECT;
		}
		//IsJustAfterPlayerInit = false;
		return RIGHT;
	}
	return LATEST_HIT_DIRECT;
}

// プレイヤーキャラを動かす
int Character::player_move()
{
	Display display;

	int Old_playerX = player.chara_x;
	int Old_playerY = player.chara_y;

	// キー入力判定
	LATEST_HIT_DIRECT = player_input();

	if(LATEST_HIT_DIRECT == LEFT)
	{
		// 移動先が空間の時はゲームオーバー
		if(map[player.chara_x - 1][player.chara_y] == BLANK)
		{
			return GAME_OVER;
		}

		player.chara_x -= 1;
	}
	else if(LATEST_HIT_DIRECT == RIGHT)
	{
		// 移動先が空間の時はゲームオーバー
		if(map[player.chara_x + 1][player.chara_y] == BLANK)
		{
			return GAME_OVER;
		}

		player.chara_x += 1;
	}
	else if(LATEST_HIT_DIRECT == UP)
	{
		// 移動先が空間時はゲームオーバー
		if(map[player.chara_x][player.chara_y - 1] == BLANK)
		{
			return GAME_OVER;
		}	

		player.chara_y -= 1;
	}
	else if(LATEST_HIT_DIRECT == DOWN)
	{
		// 移動先が空間の時はゲームオーバー
		if(map[player.chara_x][player.chara_y + 1] == BLANK)
		{
			return GAME_OVER;
		}
		
		player.chara_y += 1;
	}

	chara_map[Old_playerX][Old_playerY] = BLANK;
	chara_map[player.chara_x][player.chara_y] = PLAYER;

	// アイテムとヒットした時
	if(item_map[player.chara_x][player.chara_y] == ITEM){
		hit_item();
		item_map[player.chara_x][player.chara_y] = BLANK;
		LeftItemNum--;
	}
	// しっぽの座標更新
	if(Player_Length > 0){
		tail_rechain(Old_playerX, Old_playerY);
	}

	// tailとプレイヤーが重なったらゲームオーバー
	for(int i = 1; i <= Player_Length; i++){
		if(tail_array[i].chara_x == player.chara_x && tail_array[i].chara_y == player.chara_y)
		{
			return GAME_OVER;
		}
	}

	return 0;
}

// itemにヒットした時tail_arrayに追加する tail_mapにも追加
void Character::hit_item()
{
	int Old_Player_Length = Player_Length;
	Player_Length++;

	if(Old_Player_Length == 0){
		// Player_Lengthが0の時はプレイヤーの座標から座標取得
		tail_array[Player_Length] = player;
	}else{
		// Player_Lengthが0以外(尻尾が一つでもある時)は最後の尻尾から座標取得
		tail_array[Player_Length] = tail_array[Old_Player_Length];
	}
}

// しっぽの座標更新
void Character::tail_rechain(int old_playerX, int old_playerY)
{	
	// tail_arrayの0番目は欠番。
	if(Player_Length > 1){
		for(int tailCount = Player_Length; tailCount >= 2; tailCount--){
			tail_array[tailCount] = tail_array[tailCount-1];
		}
	}
	// 1番目はプレイヤーの配列番号から更新
	tail_array[1].chara_x = old_playerX;
	tail_array[1].chara_y = old_playerY;
}