#include "Display.h"
#include "Variable.h"
#include "DxLib.h"
#include "Character.h"
#include <stdio.h>
#include <Windows.h>
#include "IsometricView.h"

// �}�b�v�ƃv���C���[�̕`��(�N�H�[�^�[�r���[�łȂ���)
void Display::disp_map()
{	
	int MapDrawPointX , MapDrawPointY ;      // �`�悷��}�b�v���W�l
	int DrawMapChipNumX , DrawMapChipNumY ;	 // �`�悷��}�b�v�`�b�v�̐�

	// �`�悷��}�b�v�`�b�v�̐�(��)���Z�b�g
	DrawMapChipNumX = SCREEN_SIZE_X / ChipSizeX + 1 ;
	DrawMapChipNumY = SCREEN_SIZE_Y / ChipSizeY + 1 ;

	// ��ʍ���ɕ`�悷��map�z��̔ԍ����Z�b�g
	MapDrawPointX = player.chara_x - ((SCREEN_SIZE_X/2)/ChipSizeX);
	MapDrawPointY = player.chara_y - ((SCREEN_SIZE_Y/2)/ChipSizeY);

	// �`��
	for(int j = MapDrawPointY; j < (MapDrawPointY + DrawMapChipNumY); j++)
	{
		for(int i = MapDrawPointX; i < (MapDrawPointX + DrawMapChipNumX); i++)
		{
			// ��ʂ���͂ݏo���ʒu�͕`�悵�Ȃ�
			if(i < 0 || j < 0 || i >= MAP_W || j >= MAP_H )
			{
				continue;
			}
			// �}�b�v�`�b�v�̕`��
			if(map[i][j] == ROOM || map[i][j] == PATH || map[i][j] == PATH_FRONT || map[i][j] == PATH_OF_WALL || map[i][j] == APPEND_PATH){
				DrawGraph((i - MapDrawPointX)*ChipSizeX, (j - MapDrawPointY)*ChipSizeY, GraphHandle_Floorchip, TRUE);
			}
			// �A�C�e���̕`��
			if(item_map[i][j] == ITEM){
				DrawGraph((i - MapDrawPointX)*ChipSizeX, (j - MapDrawPointY)*ChipSizeY, GraphHandle_Item, TRUE);
			}
			// �c��A�C�e�����\��
			int Color = GetColor(255, 255, 255);
			DrawFormatString(0, 0, Color, "�c��:%d", LeftItemNum);
			DrawFormatString(0, 20, Color, "����:%d", Player_Length);
		}
	}

	// �L�����N�^�[�����`��
	for(int j = MapDrawPointY; j < (MapDrawPointY + DrawMapChipNumY); j++)
	{
		for(int i = MapDrawPointX; i < (MapDrawPointX + DrawMapChipNumX); i++)
		{
			// �v���C���[�̕`��
			if(chara_map[i][j] == PLAYER){
				DrawGraph((i - MapDrawPointX)*ChipSizeX, (j - MapDrawPointY)*ChipSizeY, GraphHandle_Player, TRUE);
				// �����ۂ̕`��
				if(Player_Length != 0){
					tail_disp(i, j, MapDrawPointX, MapDrawPointY);
				}
			}
		}
	}

	WaitTimer(100);
}

// ���͌��m�����ʂ̕`��܂ł̃��[�v(�Q�[���{�̂ɋ߂�)
int Display::game_loop(int IsContinue)
{
	Character character;
	IsometricView iso;

	// �v���C���[�̏����ʒu�͈�ڂ̕����̍���
	character.player_coordinate_init();
	LATEST_HIT_DIRECT = character.player_input();
	// IsometricView�\��
	if(IsQuarter == IDYES){
		//iso.DisplayIsometricView();

		IsContinue = IDYES;
		while(ProcessMessage() == 0 /*&& CheckHitKey(KEY_INPUT_ESCAPE) == 0*/)
		{
			//ClearDrawScreen();
			LATEST_HIT_DIRECT = character.player_input();

			if(character.player_move() == GAME_OVER){
				// �v���C���[�̈ړ��悪��Ԃ�tail�̎��Q�[���I�[�o�[
				return MessageGameOver();
			}

			ClearDrawScreen();
			iso.DisplayIsometricView();

			LATEST_HIT_DIRECT = character.player_input();
			//ScreenFlip(); 
			// �c��̃A�C�e���������Ȃ�����N���A
			if(LeftItemNum == 0){
				return MessageEnding();
			}
		}
	}

	// IsometricView�łȂ���
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
				// �v���C���[�̈ړ��悪��Ԃ�tail�̎��Q�[���I�[�o�[
				return MessageGameOver();
			}

			ClearDrawScreen();
			disp_map();
			
			LATEST_HIT_DIRECT = character.player_input();
			//ScreenFlip(); 
			// �c��̃A�C�e���������Ȃ�����N���A
			if(LeftItemNum == 0){
				return MessageEnding();
			}
		}
	}

	return 0;
}

// �����ۂ̕`��
void Display::tail_disp(int numX, int numY, int DrawPointX, int DrawPointY)
{
	// tail_array��0�Ԗڂ͌��ԁB�K���̍��W����1�Ԗڂ���
	for(int tailCount = 1; tailCount <= Player_Length; tailCount++){
		DrawGraph((tail_array[tailCount].chara_x - DrawPointX)*ChipSizeX, (tail_array[tailCount].chara_y - DrawPointY)*ChipSizeY, GraphHandle_Tail, TRUE);
	}
}

// �N���A���̃��b�Z�[�W
int Display::MessageEnding()
{
	TCHAR msg[100] = {NULL};
	wsprintf(msg, "%d�̃A�C�e������肵�A�N���A���܂���! \n ������x�v���C���܂���?", FirstItemNum);
	return MessageBox(NULL, msg , "���߂łƂ��������܂�!", MB_ICONQUESTION | MB_YESNO);
}

// �Q�[���I�[�o�[�̃��b�Z�[�W
int Display::MessageGameOver()
{
	return MessageBox(NULL, "GameOver \n ������x�v���C���܂���?", "GameOver" , MB_ICONEXCLAMATION | MB_YESNO);
}

// �N�H�[�^�[�r���[���m�[�}���r���[���I�����b�Z�[�W
int Display::MessageSelectMode()
{
	return MessageBox(NULL, "�N�H�[�^�[�r���[�Ńv���C���܂���?", "SelectView" , MB_ICONQUESTION | MB_YESNO);
}

// ������ʕ\��
void Display::Introduction()
{
	// ������ʉ摜�̓ǂݍ��݂ƕ\��
	if(IsQuarter == IDYES){
		GraphHandle_Introduction = LoadGraph("img\\q_introduction.png");
		DrawGraph(0, 0, GraphHandle_Introduction, TRUE);
	}else{
		GraphHandle_Introduction = LoadGraph("img\\n_introduction.png");
		DrawGraph(0, 0, GraphHandle_Introduction, TRUE);
	}
	//ScreenFlip();
}

// ���ӁI
void Display::Thanks()
{
	int Color = GetColor(255, 255, 255);
	//ClearDrawScreen();
	SetFontSize(30);
	DrawFormatString(155, 200, Color, "Thank you for playing!");
	//ScreenFlip();
}