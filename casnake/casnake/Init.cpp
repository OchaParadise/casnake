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
	if( DxLib_Init() == -1 ) return -1; // DX_Lib����������
	srand((unsigned)time(NULL));

	//SetDrawScreen( DX_SCREEN_BACK );

	return 0;
}

int Init::Load_init_Q()
{
	// �}�b�v�`�b�v�̓ǂݍ���
	GraphHandle_Floorchip = LoadGraph("img\\q_floor16.png");
	GetGraphSize(GraphHandle_Floorchip, &ChipSizeX, &ChipSizeY);

	// �v���C���[�摜�̓ǂݍ���
	GraphHandle_Player = LoadGraph("img\\q_player16.png");

	// �A�C�e���摜�̓ǂݍ���
	GraphHandle_Item = LoadGraph("img\\q_item16.png");

	// tail�摜�̓ǂݍ���
	GraphHandle_Tail = LoadGraph("img\\q_tail16.png");

	return 0;
}

int Init::Load_init()
{
	// �}�b�v�`�b�v�̓ǂݍ���
	GraphHandle_Floorchip = LoadGraph("img\\floor8.png");
	GetGraphSize(GraphHandle_Floorchip, &ChipSizeX, &ChipSizeY);

	// �v���C���[�摜�̓ǂݍ���
	GraphHandle_Player = LoadGraph("img\\player8.png");

	// �A�C�e���摜�̓ǂݍ���
	GraphHandle_Item = LoadGraph("img\\item8.png");

	// tail�摜�̓ǂݍ���
	GraphHandle_Tail = LoadGraph("img\\tail8.png");

	return 0;
}

void Init::Load_init_menu()
{
	int SelectPoint = SPOINT_QUATER;
	int BlankSelectPoint = SPOINT_NORMAL;

	// ���j���[�摜�̓ǂݍ���
	GraphHandle_Menu = LoadGraph("img\\top.png");

	// ���j���[�̉摜�\��
	DrawGraph(0, 0, GraphHandle_Menu, TRUE);

	// �I���|�C���^�摜�ƑI���|�C���^�����p�摜�̓ǂݍ���
	GraphHandle_SelectPointer = LoadGraph("img\\selectpointer.png");
	GraphHandle_PointerMask = LoadGraph("img\\mask.png");

	// ���[�v
	while( ProcessMessage() == 0 )
	{
		// �I���|�C���^�摜�A�I���|�C���^�����摜�̕`��
		DrawGraph(60, SelectPoint, GraphHandle_SelectPointer, TRUE);
		DrawGraph(60, BlankSelectPoint, GraphHandle_PointerMask, TRUE);

		// �L�[����
		{
			// �N�I�[�^�[�r���[���w���Ă��鎞�ɉ��L�[D����
			if(CheckHitKey(KEY_INPUT_S) != 0 || CheckHitKey(KEY_INPUT_DOWN) != 0 && SelectPoint == SPOINT_QUATER){
				SelectPoint = SPOINT_NORMAL;
				BlankSelectPoint = SPOINT_QUATER;
			}
			// �m�[�}���r���[���w���Ă��鎞�ɏ�L�[W����
			if(CheckHitKey(KEY_INPUT_W) != 0 || CheckHitKey(KEY_INPUT_UP) != 0 && SelectPoint == SPOINT_NORMAL){
				SelectPoint = SPOINT_QUATER;
				BlankSelectPoint = SPOINT_NORMAL;
			}

			// ����L�[(�G���^�[)������
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