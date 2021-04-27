//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"	//DxLib���g�����͕K�v

//�}�N����`
#define GAME_TITLE "�Q�[���^�C�g��"		//�Q�[���^�C�g��
#define GAME_WIDTH  1280				//�Q�[����ʂ̕��i�E�B�h�X�j
#define GAME_HEIGHT 720					//�Q�[����ʂ̍����i�n�C�g�j
#define GAME_COLOR  32					//�Q�[���̐F��

#define GAME_ICON_ID	333				//�Q�[����ICON��ID

// �v���O������ WinMain ����n�܂�܂�
//windows�̃v���O���~���O���@���iWinAPI�j�œ����Ă���I
//DxLib�́ADirectX�Ƃ����A�Q�[���v���O���~���O���ȒP�Ɏg����d�g��
int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	ChangeWindowMode(TRUE);					//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText("�Q�[���^�C�g��");	//�E�B���h�E�̃^�C�g���̕���
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);					//�f�t�H���g�̔w�i�̐F
	SetWindowIconID(GAME_ICON_ID);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

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
	
	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}