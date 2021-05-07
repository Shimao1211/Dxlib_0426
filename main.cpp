//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"	//DxLib���g�����͕K�v

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

	//�l�p�̈ʒu�����߂�
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGHT / 2;

	//�l�p�̑傫�������߂�
	int width = 32;		//��
	int height = 32;	//����

	int radius = 40;

	//���x
	int speed = 1;
	int Xspeed = 1;
	int Yspeed = 1;

	SetDrawScreen(DX_SCREEN_BACK);

	//�������[�v
	while (1)
	{
		//��������L�[�������ꂽ�Ƃ�
		if (CheckHitKeyAll() != 0)	//�O�̂Ƃ��ɁA�����L�[�������ꂽ
		{
			break;	//�������[�v�𔲂���
		}

		//���b�Z�[�W���󂯎�葱����
		if (ProcessMessage() != 0)	//-1�̂Ƃ��A�G���[��E�B���h�E������ꂽ
		{
			break;	//�������[�v�𔲂���
		}

		//��ʂ���������
		if (ClearDrawScreen() != 0) { break; }

		//�l�p��`��
		DrawBox(
			X, Y, X + width, Y + height,
			GetColor(255, 0, 0),		//�F���擾
			TRUE						//�h��Ԃ��H
		);

		//�~��`��
		DrawCircle(
			X, Y, radius,
			GetColor(0, 255, 0),
				FALSE, 5
			);

		//�l�p���E�ɓ�����
		X += Xspeed;
		Y += Yspeed;

		if (X < 0 || X + width > GAME_WIDTH)	//��ʂ̉�����o���Ƃ�
		{ 
			//+1�Ȃ�-1�ɂ���@/ -1�Ȃ�+1�ɂ���
			Xspeed = -Xspeed; //�ړ���������𔽓]

			//�ǂɓ�����Ƒ����Ȃ�
			if (Xspeed > 0) { Xspeed += 2; }
			else if (Xspeed < 0) { Xspeed -= 2; }
		}

		if (Y < 0 || Y + height > GAME_HEIGHT)	//��ʂ̏c����o���Ƃ�
		{
			Yspeed = -Yspeed;

			//�ǂɓ�����Ƒ����Ȃ�
			if (Yspeed > 0) { Yspeed += 2; }
			else if (Yspeed < 0) { Yspeed -= 2; }
		}


		ScreenFlip();		//�_�u���o�b�t�@�����O������ʂ�`��
	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}