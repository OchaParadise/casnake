#include <vector>
#include "Variable.h"
#include "Character.h"
#include "DxLib.h"
#include "Display.h"
#include <Windows.h>

// �v���C���[�̔z��̏�����
void Character::player_init()
{
	int i, j;

	for(j = 0; j < MAP_H; j++){
		for(i = 0; i < MAP_W; i++){
			chara_map[i][j] = BLANK;
		}
	}
}

// �v���C���[�̍��W�̏�����(��ڂ̕����̍���)
void Character::player_coordinate_init()
{
	// �v���C���[�z��̏�����
	player_init();

	vector<_room>::iterator it_room = room_list.begin();

	// �Ƃ肠������ԍŏ��̕������X�g�̍���Ƀv���C���[��init
	player.chara_x = it_room->lx;
	player.chara_y = it_room->ly;
	chara_map[player.chara_x][player.chara_y] = PLAYER;

	IsJustAfterPlayerInit = 0;
}

// �����L�[�����̔���
int Character::player_input()
{
	char Buf[256];

	GetHitKeyStateAll(Buf);

	// ������tail�̂����ԂŐ^�t�����͍s���Ȃ�
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

// �v���C���[�L�����𓮂���
int Character::player_move()
{
	Display display;

	int Old_playerX = player.chara_x;
	int Old_playerY = player.chara_y;

	// �L�[���͔���
	LATEST_HIT_DIRECT = player_input();

	if(LATEST_HIT_DIRECT == LEFT)
	{
		// �ړ��悪��Ԃ̎��̓Q�[���I�[�o�[
		if(map[player.chara_x - 1][player.chara_y] == BLANK)
		{
			return GAME_OVER;
		}

		player.chara_x -= 1;
	}
	else if(LATEST_HIT_DIRECT == RIGHT)
	{
		// �ړ��悪��Ԃ̎��̓Q�[���I�[�o�[
		if(map[player.chara_x + 1][player.chara_y] == BLANK)
		{
			return GAME_OVER;
		}

		player.chara_x += 1;
	}
	else if(LATEST_HIT_DIRECT == UP)
	{
		// �ړ��悪��Ԏ��̓Q�[���I�[�o�[
		if(map[player.chara_x][player.chara_y - 1] == BLANK)
		{
			return GAME_OVER;
		}	

		player.chara_y -= 1;
	}
	else if(LATEST_HIT_DIRECT == DOWN)
	{
		// �ړ��悪��Ԃ̎��̓Q�[���I�[�o�[
		if(map[player.chara_x][player.chara_y + 1] == BLANK)
		{
			return GAME_OVER;
		}
		
		player.chara_y += 1;
	}

	chara_map[Old_playerX][Old_playerY] = BLANK;
	chara_map[player.chara_x][player.chara_y] = PLAYER;

	// �A�C�e���ƃq�b�g������
	if(item_map[player.chara_x][player.chara_y] == ITEM){
		hit_item();
		item_map[player.chara_x][player.chara_y] = BLANK;
		LeftItemNum--;
	}
	// �����ۂ̍��W�X�V
	if(Player_Length > 0){
		tail_rechain(Old_playerX, Old_playerY);
	}

	// tail�ƃv���C���[���d�Ȃ�����Q�[���I�[�o�[
	for(int i = 1; i <= Player_Length; i++){
		if(tail_array[i].chara_x == player.chara_x && tail_array[i].chara_y == player.chara_y)
		{
			return GAME_OVER;
		}
	}

	return 0;
}

// item�Ƀq�b�g������tail_array�ɒǉ����� tail_map�ɂ��ǉ�
void Character::hit_item()
{
	int Old_Player_Length = Player_Length;
	Player_Length++;

	if(Old_Player_Length == 0){
		// Player_Length��0�̎��̓v���C���[�̍��W������W�擾
		tail_array[Player_Length] = player;
	}else{
		// Player_Length��0�ȊO(�K������ł����鎞)�͍Ō�̐K��������W�擾
		tail_array[Player_Length] = tail_array[Old_Player_Length];
	}
}

// �����ۂ̍��W�X�V
void Character::tail_rechain(int old_playerX, int old_playerY)
{	
	// tail_array��0�Ԗڂ͌��ԁB
	if(Player_Length > 1){
		for(int tailCount = Player_Length; tailCount >= 2; tailCount--){
			tail_array[tailCount] = tail_array[tailCount-1];
		}
	}
	// 1�Ԗڂ̓v���C���[�̔z��ԍ�����X�V
	tail_array[1].chara_x = old_playerX;
	tail_array[1].chara_y = old_playerY;
}