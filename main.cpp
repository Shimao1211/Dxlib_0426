//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"	//DxLib���g�����͕K�v

#include "keyboard.h"	//�L�[�{�[�h�̏���

//�}�N����`
#define GAME_TITLE "�Q�[���^�C�g��"		//�Q�[���^�C�g��
#define GAME_WIDTH  1280				//�Q�[����ʂ̕��i�E�B�h�X�j
#define GAME_HEIGHT 720					//�Q�[����ʂ̍����i�n�C�g�j
#define GAME_COLOR  32					//�Q�[���̐F��

#define GAME_ICON_ID	333				//�Q�[����ICON��ID

#define GAME_WINDOW_BAR 0				//�E�B���h�E�o�[�̎��

// �v���O������ WinMain ����n�܂�܂�
//windows�̃v���O���~���O���@���iWinAPI�j�œ����Ă���I
//DxLib�́ADirectX�Ƃ����A�Q�[���v���O���~���O���ȒP�Ɏg����d�g��
int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//log.txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);								//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText("�Q�[���^�C�g��");				//�E�B���h�E�̃^�C�g���̕���
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);					//�f�t�H���g�̔w�i�̐F
	SetWindowIconID(GAME_ICON_ID);						//�A�C�R���t�@�C����ǂݍ���
	SetWindowStyleMode(GAME_WINDOW_BAR);				//�E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);								//�f�B�X�v���C�̐���������L���ɂ���
	SetAlwaysRunFlag(TRUE);								//�E�B���h�E�������ƃA�N�e�B�u�ɂ���

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	
	//
	SetDrawScreen(DX_SCREEN_BACK);

	//�傫��
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGHT / 2;
	//�~�̔��a
	int radius = 50;

	//�������[�v
	while (1)
	{
		

		//���b�Z�[�W���󂯎�葱����
		if (ProcessMessage() != 0) { break; }	//-1�̂Ƃ��A�G���[��E�B���h�E������ꂽ
		if (ClearDrawScreen() != 0) { break; }
		
		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//�L�[����
		if (KeyDown(KEY_INPUT_W) == TRUE)
		{
			Y--;	//��Ɉړ�
		}
		if (KeyDown(KEY_INPUT_S) == TRUE)
		{
			Y++;	//���Ɉړ�
		}
		if (KeyDown(KEY_INPUT_A) == TRUE)
		{
			X--;	//���Ɉړ�
		}
		if (KeyDown(KEY_INPUT_D) == TRUE)
		{
			X++;	//�E�Ɉړ�
		}

		DrawCircle(X, Y, radius, GetColor(255, 255, 0), TRUE);

		ScreenFlip();		//�_�u���o�b�t�@�����O������ʂ�`��
	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}