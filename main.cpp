#include "game.h"
#include "shape.h"
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "Data.h"
#include "FPS.h"		//FPS�̏���

//�Ǝ��̗񋓌^�̒�`
enum MUKI
{
	muki_none,
	muki_ue,
	muki_hidari,
	muki_shita,
	muki_migi
};


//�L�����N�^�̍\����
struct CHARACTOR
{
	IMAGE img;			//�摜�\����

	int speed = 1;		//�ړ����x

	RECT coll;			//�����蔻��̗̈�i�l�p�j
};


//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���V�[��
GAME_SCENE OldGameScene;	//�O��̃Q�[���V�[��
GAME_SCENE NextGameScene;	//����̃Q�[���V�[��

CHARACTOR player;
CHARACTOR goal;
CHARACTOR yuusha;
CHARACTOR yuusha2;
CHARACTOR yuusha3;

IMAGE TitleLogo;	//�^�C�g�����S
IMAGE TitleEnter;	//�G���^�[�L�[�������Ă�
IMAGE EndClear;		//�N���A���S

//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;	//�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE; //�t�F�[�h�C��

int fadeTimeMill = 2000;	//�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60;	//�~���b���t���[���b�ɕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;				//�����l
int fadeOutCnt = fadeOutCntInit;	//�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCntMax = fadeTimeMax;	//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;				//�����l
int fadeInCnt = fadeInCntInit;		//�t�F�[�h�A�E�g�̃J�E���^
int fadeInCntMax = fadeTimeMax;		//�t�F�[�h�A�E�g�̃J�E���^MAX

//PushEnter�̓_��
int PushEnterCnt = 0;			//�J�E���^
int PushEnterCntMAX = 60;		//�J�E���^MAX�l
BOOL PushEnterBrink = FALSE;	//�_�ł��Ă��邩�H

//�v���g�^�C�v�錾
BOOL GameLoad(VOID);	//�f�[�^��Ǎ�
VOID GameInit(VOID);	//�f�[�^�̏�����
VOID GameDelete(VOID);	//�f�[�^���폜

VOID Title(VOID);	//�^�C�g�����
VOID TitleProc(VOID);	//�^�C�g����ʁi�����j
VOID TitleDraw(VOID);	//�^�C�g����ʁi�`��j

VOID Play(VOID);		//�v���C���
VOID PlayProc(VOID);	//�v���C��ʁi�����j
VOID PlayDraw(VOID);	//�v���C��ʁi�`��j

VOID End(VOID);			//�G���h���
VOID EndProc(VOID);		//�G���h��ʁi�����j
VOID EndDraw(VOID);		//�G���h��ʁi�`��j

VOID Change(VOID);		//�؂�ւ����
VOID ChangeProc(VOID);	//�؂�ւ���ʁi�����j
VOID ChangeDraw(VOID);	//�؂�ւ���ʁi�`��j

VOID ChangeScene(GAME_SCENE scene);		//�V�[���؂�ւ�
VOID CollUpdatePlayer(CHARACTOR* chara);
VOID CollUpdate(CHARACTOR* chara);
BOOL OnCollRect(RECT a, RECT b);		//��`�Ƌ�`�̓����蔻��
VOID CollUpdateImage(IMAGE* img);		//�摜�̓����蔻��̍X�V
VOID CollUpdateDivImage(DIVIMAGE* div);	//�����摜�̓����蔻��̍X�V
BOOL LoadImageMem(IMAGE* image, const char* path);										//�Q�[���̉摜��ǂݍ���
BOOL LoadAudio(AUDIO* audio, const char* path, int Volume, int playType);				//�Q�[���̉��y��ǂݍ���
BOOL LoadImageDivMem(DIVIMAGE* div, const char* path, int bunkatuYoko, int bunkatuTate);//�Q�[���̉摜�̕����ǂݍ���

VOID PlayAudio(AUDIO audio);					//���y�Đ�
int GetVolumeAudio(AUDIO audio);				//���y�̃{�����[���擾
VOID SetVolumeAudio(AUDIO* audio, int vol);		//���y�̃{�����[���ݒ�
VOID ChangeVolumeAudio(AUDIO* audio, int vol);	//���y�̃{�����[���ύX
VOID StopAudio(AUDIO* audio);					//���y��~

VOID DrawImage(IMAGE image);					//�摜�`��
VOID DrawRect(RECT r, unsigned int color, bool b);	//RECT�`��
VOID DrawDivImage(DIVIMAGE* image);				//�����摜�̕`��
VOID DrawDivImageChara(DIVIMAGE* image);		//�����摜�̕`��

//�T���v���̑f��
IMAGE sampleImg;
DIVIMAGE sampleDivImg;
DIVIMAGE samplePlayerImg;
MUKI muki = muki_shita;		//�T���v������

AUDIO TitleBGM;
AUDIO PlayBGM;
AUDIO EndBGM;

IMAGE TitleScene;
IMAGE EndScene;

MAP_DATA map1;

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
	SetMainWindowText(GAME_TITLE);						//�E�B���h�E�̃^�C�g���̕���
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);					//�f�t�H���g�̔w�i�̐F
	SetWindowIconID(GAME_ICON_ID);						//�A�C�R���t�@�C����ǂݍ���
	SetWindowStyleMode(GAME_WINDOW_BAR);				//�E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);								//�f�B�X�v���C�̐���������L���ɂ���
	SetAlwaysRunFlag(TRUE);								//�E�B���h�E�������ƃA�N�e�B�u�ɂ���

	//�c�w���C�u��������������
	if (DxLib_Init() == -1)
	{
		// �G���[���N�����璼���ɏI��
		return -1;
	}

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);


	//�ŏ��̃V�[���́A�^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����
	if (!GameLoad())
	{
		//�f�[�^�̓ǂݍ��݂Ɏ��s�����Ƃ�
		GameDelete();	//�f�[�^�폜
		DxLib_End();	//DxLib�I��
		return -1;		//�ُ�I��

	}

	//�Q�[���̏�����
	GameInit();

	//�������[�v
	while (1)
	{
		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; }	//��ʂ���������

		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();				//�v���C���
			break;
		case GAME_SCENE_END:
			End();				//�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();			//�؂�ւ����
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		//FPS�l��`��
		FPSDraw();

		//FPS�l��҂�
		FPSWait();

		ScreenFlip();		//�_�u���o�b�t�@�����O������ʂ�`��
	}

	GameDelete;
	
	//DX���C�u�����g�p�̏I������
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}

/// <summary>
/// �Q�[���f�[�^�̓ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���TRUE�@�^�@�ǂݍ��߂Ȃ�������FALSE</returns>
BOOL GameLoad(VOID)
{
	//�T���v�������摜��ǂݍ���
	if (!LoadImageMem(&goal.img, ".\\Image\\goal.\png")) { return FALSE; }
	if (!LoadImageMem(&yuusha.img, ".\\Image\\yuusha.\png")) { return FALSE; }
	if (!LoadImageMem(&yuusha2.img, ".\\Image\\yuusha2.\png")) { return FALSE; }
	if (!LoadImageMem(&yuusha3.img, ".\\Image\\yuusha3.\png")) { return FALSE; }

	if (LoadImageDivMem(&samplePlayerImg, ".\\Image\\R.png", 3, 4) == FALSE) { return FALSE; }

	//���S��ǂݍ���
	if (!LoadImageMem(&TitleLogo, ".\\Image\\�^�C�g�����S.\png")) { return FALSE; }
	if (!LoadImageMem(&TitleEnter, ".\\Image\\PushEnter.\png")) { return FALSE; }
	if (!LoadImageMem(&EndClear, ".\\Image\\Clear.\png")) { return FALSE; }

	if (LoadImageMem(&TitleScene, ".\\Image\\TitleScene.jpg") == FALSE) { return FALSE; }
	TitleScene.x = 0;
	TitleScene.y = 0;
	TitleScene.IsDraw = TRUE;	//�`�悷��

	if (LoadImageMem(&EndScene, ".\\Image\\EndScene.png") == FALSE) { return FALSE; }
	EndScene.x = 0;
	EndScene.y = 0;
	EndScene.IsDraw = TRUE;	//�`�悷��

	//���y��ǂݍ���
	if (!LoadAudio(&TitleBGM, ".\\Audio\\TitleBGM.mp3", 128, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&PlayBGM, ".\\Audio\\PlayBGM.mp3", 128, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&EndBGM, ".\\Audio\\EndBGM.mp3", 128, DX_PLAYTYPE_LOOP)) { return FALSE; }

	if (LoadCSVMap(
		IMG_PATH_MAP1,
		CSV_PATH_MAP1_SHITA,
		CSV_PATH_MAP1_NAKA,
		CSV_PATH_MAP1_NAKA_ATARI,
		CSV_PATH_MAP1_UE,
		&map1,
		MAP1_YOKO_DIV, MAP1_TATE_DIV
	) == FALSE) {
		return FALSE;
	}

	return TRUE;	//�S�ēǂݍ��݂��I
}

/// <summary>
/// �Q�[���̃f�[�^���폜
/// </summary>
VOID GameDelete(VOID)
{
	//�T���v���摜���폜
	DeleteGraph(sampleImg.handle);

	//�T���v�������摜���폜
	for (int i = 0; i < sampleDivImg.DivMax; i++) { DeleteGraph(sampleDivImg.handle[i]); }

	//�T���v�������摜���폜
	for (int i = 0; i < samplePlayerImg.DivMax; i++) { DeleteGraph(samplePlayerImg.handle[i]); }

	//�T���v�����y���폜
	DeleteMusicMem(TitleBGM.handle);

	DeleteMusicMem(PlayBGM.handle);

	DeleteMusicMem(EndBGM.handle);

	DeleteGraph(TitleScene.handle);

	DeleteGraph(EndScene.handle);

	return;
}

/// <summary>
/// �摜���������ɓǂݍ���
/// </summary>
/// <param name="image">�摜�\���̂̃A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{
	//�S�[���̓ǂݍ���
	strcpyDx(image->path, path);			//�p�X�̃R�s�[
	image->handle = LoadGraph(image->path);		//�摜�̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			image->path,			//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(image->handle, &image->width, &image->height);

	//�ǂݍ��߂�
	return TRUE;
}


	/// <summary>
	/// �Q�[���f�[�^��������
	/// </summary>
	/// <param name=""></param>
	VOID GameInit(VOID)
	{
		//�v���C���[�̏�����
		player.img.x = GAME_WIDTH / 2 - player.img.width / 2;	//������
		player.img.y = GAME_HEIGHT / 2 - player.img.height / 2;	//������
		player.speed = 500;			//�X�s�[�h
		player.img.IsDraw = TRUE;	//�`��ł���I

		//�����蔻����X�V����
		CollUpdatePlayer(&player);	//�v���C���[�̓����蔻��̍X�V:

		//�S�[���̏�����
		goal.img.x = 1200;
		goal.img.y = 355;
		goal.img.speed = 500;
		goal.img.IsDraw = TRUE;	//�`��ł���I

		//�E�҂̏�����
		yuusha.img.x = 1100;
		yuusha.img.y = 260;
		yuusha.img.speed = 500;
		yuusha.img.IsDraw = TRUE;	//�`��ł���I

		//�E��2�̏�����
		yuusha2.img.x = 580;
		yuusha2.img.y = 350;
		yuusha2.img.speed = 500;
		yuusha2.img.IsDraw = TRUE;	//�`��ł���I

		//�E��3�̏�����
		yuusha3.img.x = 270;
		yuusha3.img.y = 100;
		yuusha3.img.speed = 500;
		yuusha3.img.IsDraw = TRUE;	//�`��ł���I

		//�����蔻����X�V����
		CollUpdate(&goal);	//�v���C���[�̓����蔻��̃A�h���X

		TitleScene.x = 0;
		TitleScene.y = 0;
		TitleScene.IsDraw = TRUE;	//�`�悷��

		EndScene.x = 0;
		EndScene.y = 0;
		EndScene.IsDraw = TRUE;	//�`�悷��

		//�^�C�g�����S�̈ʒu�����߂�
		TitleLogo.x = GAME_WIDTH / 2 - TitleLogo.width / 2;	//��������
		TitleEnter.y = 100;

		//PushEnter�̈ʒu�����߂�
		TitleEnter.x = GAME_WIDTH / 2 - TitleEnter.width / 2;	//��������
		TitleEnter.y = GAME_HEIGHT - TitleEnter.height - 100;

		//PushEnter�̓_��
		PushEnterCnt = 0;
		PushEnterCntMAX = 60;
		PushEnterBrink = FALSE;

		//�N���A���S�̈ʒu�����߂�
		EndClear.x = GAME_WIDTH / 2 - EndClear.width / 2;	//��������
		EndClear.y = GAME_HEIGHT / 2 - EndClear.height / 2;	//��������

	}

	/// <summary>
	/// �V�[����؂�ւ���֐�
	/// </summary>
	/// <param name="scene"></param>
	VOID ChangeScene(GAME_SCENE scene)
	{
		GameScene = scene;	//�V�[����؂�ւ�
		IsFadeIn = FALSE;	//�t�F�[�h�C�����Ȃ�
		IsFadeOut = TRUE;	//�t�F�[�h�A�E�g����

		return;
	}

	/// <summary>
	///  �^�C�g�����
	/// </summary>
	VOID Title(VOID)
	{
		TitleProc();	//����
		TitleDraw();	//�`��

		return;
	}

	/// <summary>
	/// �^�C�g����ʂ̏���
	/// </summary>
	VOID TitleProc(VOID)
	{

		if (KeyClick(KEY_INPUT_RETURN) == TRUE)
		{
			//BGM���~�߂�
			StopSoundMem(TitleBGM.handle);

			//�V�[���؂�ւ�
			//���̃V�[���̏������������ōs���Ɗy

			//�Q�[���̏�����
			GameInit();

			//�v���C��ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_PLAY);

			return;
		}
		
		//BGM������Ă��Ȃ��Ƃ�
		if (CheckSoundMem(TitleBGM.handle) == 0)
		{
			//BGM�𗬂�
			PlaySoundMem(TitleBGM.handle, TitleBGM.playType);
		}

		return;
	}

	/// <summary>
	/// �^�C�g����ʂ̕`��
	/// </summary>
	VOID TitleDraw(VOID)
	{
		DrawGraph(TitleScene.x, TitleScene.y, TitleScene.handle, TRUE);

		//�^�C�g�����S�̕`��
		DrawGraph(TitleLogo.x, TitleLogo.y, TitleLogo.handle, TRUE);

		//MAX�l�܂ő҂�
		if (PushEnterCnt < PushEnterCntMAX) { PushEnterCnt++; }
		else
		{
			if (PushEnterBrink == TRUE)PushEnterBrink = FALSE;
			else if (PushEnterBrink == FALSE)PushEnterBrink = TRUE;

			PushEnterCnt = 0;	//�J�E���^������
		}

		//PushEnter��_��
		if (PushEnterBrink == TRUE)
		{
			//�������ɂ���
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)(PushEnterCntMAX - PushEnterCnt) / PushEnterCntMAX) * 255);

			//PushEnter�̕`��
			DrawGraph(TitleEnter.x, TitleEnter.y, TitleEnter.handle, TRUE);

			//�������I��
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		if (PushEnterBrink == FALSE)
		{
			//�������ɂ���
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)(PushEnterCntMAX - PushEnterCnt) / PushEnterCntMAX) * 255);

			//PushEnter�̕`��
			DrawGraph(TitleEnter.x, TitleEnter.y, TitleEnter.handle, TRUE);

			//�������I��
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		

		
		
		DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
		return;
	}

	/// <summary>
	///  �v���C���
	///  </summary>
	VOID Play(VOID)
	{
		PlayProc();	//����
		PlayDraw();	//�`��

		return;
	}

	/// <summary>
	/// �v���C��ʂ̏���
	/// </summary>
	VOID PlayProc(VOID)
	{
		/*
		if (KeyClick(KEY_INPUT_RETURN) == TRUE)
		{
			//�V�[���؂�ւ�
			//���̃V�[���̏������������ōs���Ɗy

			//�G���h��ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_END);
		}
		*/

		//BGM������Ă��Ȃ��Ƃ�
		if (CheckSoundMem(PlayBGM.handle) == 0)
		{
			//BGM�𗬂�
			PlaySoundMem(PlayBGM.handle, PlayBGM.playType);
		}

		//�v���C���[�̑���
		if (KeyDown(KEY_INPUT_UP) == TRUE)
		{
			player.img.y -= player.speed * fps.DeltaTime;
		}
		if (KeyDown(KEY_INPUT_DOWN) == TRUE)
		{
			player.img.y += player.speed * fps.DeltaTime;
		}

		if (KeyDown(KEY_INPUT_LEFT) == TRUE)
		{
			player.img.x -= player.speed * fps.DeltaTime;
		}
		if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
		{
			player.img.x += player.speed * fps.DeltaTime;
		}

		{
			muki = muki_none;	//�ŏ��͌����𖳂��ɂ���
			DIVIMAGE dummy = samplePlayerImg;

			if (KeyDown(KEY_INPUT_W))
			{
				muki = muki_ue;
				dummy.y--;
			}
			else if (KeyDown(KEY_INPUT_S))
			{
				muki = muki_shita;
				dummy.y++;
			}
			if (KeyDown(KEY_INPUT_A))
			{
				muki = muki_hidari;
				dummy.x--;
			}
			else if (KeyDown(KEY_INPUT_D))
			{
				muki = muki_migi;
				dummy.x++;
			}
			CollUpdateDivImage(&dummy);	//�����蔻��̍X�V

			if (CollMap(dummy.coll, map1) == FALSE)
			{
				samplePlayerImg = dummy;
			}
		}

		//�����蔻����X�V����
		CollUpdatePlayer(&player);

		//�S�[���̓����蔻����X�V����
		CollUpdate(&goal);

		CollUpdate(&yuusha);
		CollUpdate(&yuusha2);
		CollUpdate(&yuusha3);

		//�v���C���[���S�[���ɓ��������Ƃ���
		if (OnCollRect(samplePlayerImg.coll, goal.coll) == TRUE)
		{
			//BGM���~�߂�
			StopSoundMem(PlayBGM.handle);

			//�G���h��ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_END);

			//�����������I��
			return;
		}

		//�v���C���[���S�[���ɓ��������Ƃ���
		if (OnCollRect(samplePlayerImg.coll, yuusha.coll) == TRUE)
		{
			//BGM���~�߂�
			StopSoundMem(PlayBGM.handle);

			//�G���h��ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_END);

			//�����������I��
			return;
		}

		//�v���C���[���S�[���ɓ��������Ƃ���
		if (OnCollRect(samplePlayerImg.coll, yuusha2.coll) == TRUE)
		{
			//BGM���~�߂�
			StopSoundMem(PlayBGM.handle);

			//�G���h��ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_END);

			//�����������I��
			return;
		}

		//�v���C���[���S�[���ɓ��������Ƃ���
		if (OnCollRect(samplePlayerImg.coll, yuusha3.coll) == TRUE)
		{
			//BGM���~�߂�
			StopSoundMem(PlayBGM.handle);

			//�G���h��ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_END);

			//�����������I��
			return;
		}

		return;
	}

	/// <summary>
	/// �v���C��ʂ̕`��
	/// </summary>
	VOID PlayDraw(VOID)
	{
		DrawImage(sampleImg);				//�T���v���摜�̕`��
		DrawDivImage(&sampleDivImg);		//�T���v�������摜�̕`��

		//�v���C���[�̓���T���v��
		{
			DrawDivImageChara(&samplePlayerImg);//�T���v�������摜�̕`��
		}

		DrawMap(map1);

		//�v���C���[��`��
		if (player.img.IsDraw == TRUE)
		{
			//�摜��`��
			DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

			////�f�o�b�O�̂Ƃ��́A�����蔻��̗̈��`��
			//if (GAME_DEBUG == TRUE)
			//{
			//	//�l�p��`��
			//	DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
			//		GetColor(255, 0, 0), FALSE);
			//}
		}

		//�S�[����`��
		if (goal.img.IsDraw == TRUE)
		{
			//�摜��`��
			DrawGraph(goal.img.x, goal.img.y, goal.img.handle, TRUE);

			////�f�o�b�O�̂Ƃ��́A�����蔻��̗̈��`��
			//if (GAME_DEBUG == TRUE)
			//{
			//	//
			//	DrawBox(goal.coll.left, goal.coll.top, goal.coll.right, goal.coll.bottom,
			//		GetColor(255, 0, 0), FALSE);
			//}
		}

		//�E�҂�`��
		if (yuusha.img.IsDraw == TRUE)
		{

			//�摜��`��
			DrawGraph(yuusha.img.x, yuusha.img.y, yuusha.img.handle, TRUE);

			//////�f�o�b�O�̂Ƃ��́A�����蔻��̗̈��`��
			//if (GAME_DEBUG == TRUE)
			//{
			//	//
			//	DrawBox(yuusha.coll.left, yuusha.coll.top, yuusha.coll.right, yuusha.coll.bottom,
			//		GetColor(255, 0, 0), FALSE);
			//}
		}

		//�E�҂�`��
		if (yuusha2.img.IsDraw == TRUE)
		{

			//�摜��`��
			DrawGraph(yuusha2.img.x, yuusha2.img.y, yuusha2.img.handle, TRUE);

			//////�f�o�b�O�̂Ƃ��́A�����蔻��̗̈��`��
			//if (GAME_DEBUG == TRUE)
			//{
			//	//
			//	DrawBox(yuusha2.coll.left, yuusha2.coll.top, yuusha2.coll.right, yuusha2.coll.bottom,
			//		GetColor(255, 0, 0), FALSE);
			//}
		}

		//�E�҂�`��
		if (yuusha3.img.IsDraw == TRUE)
		{

			//�摜��`��
			DrawGraph(yuusha3.img.x, yuusha3.img.y, yuusha3.img.handle, TRUE);

			//////�f�o�b�O�̂Ƃ��́A�����蔻��̗̈��`��
			//if (GAME_DEBUG == TRUE)
			//{
			//	//
			//	DrawBox(yuusha3.coll.left, yuusha3.coll.top, yuusha3.coll.right, yuusha3.coll.bottom,
			//		GetColor(255, 0, 0), FALSE);
			//}
		}


		

		DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
		return;
	}

	/// <summary>
	///  �G���h���
	/// </summary>
	VOID End(VOID)
	{
		EndProc();	//����
		EndDraw();	//�`��

		return;
	}

	/// <summary>
	/// �G���h��ʂ̏���
	/// </summary>
	VOID EndProc(VOID)
	{
		if (KeyClick(KEY_INPUT_RETURN) == TRUE)
		{
			//�V�[���؂�ւ�
			//���̃V�[���̏������������ōs���Ɗy

			//BGM���~�߂�
			StopSoundMem(EndBGM.handle);

			//�^�C�g����ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_TITLE);

			return;
		}

		//BGM������Ă��Ȃ��Ƃ�
		if (CheckSoundMem(EndBGM.handle) == 0)
		{
			//BGM�𗬂�
			PlaySoundMem(EndBGM.handle, EndBGM.playType);
		}

		return;
	}

	/// <summary>
	/// �G���h��ʂ̕`��
	/// </summary>
	VOID EndDraw(VOID)
	{
		DrawGraph(EndScene.x, EndScene.y, EndScene.handle, TRUE);

		//EndClear�̕`��
		DrawGraph(EndClear.x, EndClear.y, EndClear.handle, TRUE);

		DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
		return;
	}

	/// <summary>
	///  �؂�ւ����
	/// </summary>
	VOID Change(VOID)
	{
		ChangeProc();	//����
		ChangeDraw();	//�`��

		return;
	}

	/// <summary>
	///�@�؂�ւ���ʂ̏���
	/// </summary>
	VOID ChangeProc(VOID)
	{

		//�t�F�[�h�C��
		if (IsFadeIn == TRUE);
		{
			if (fadeInCnt > fadeInCntMax)
			{
				fadeInCnt--;	//�J�E���^�����炷
			}
			else
			{
				//�t�F�[�h�C���������I�����

				fadeInCnt = fadeInCntInit;	//�J�E���^������
				IsFadeIn = FALSE;			//�ӂ��[�����񏈗��I��
			}
		}

		//�t�F�[�h�A�E�g
		if (IsFadeOut == TRUE)
		{
			if (fadeOutCnt < fadeOutCntMax)
			{
				fadeOutCnt++;	//�J�E���^�����炷
			}
			else
			{
				//�t�F�[�h�C���������I�����

				fadeOutCnt = fadeOutCntInit;	//�J�E���^������
				IsFadeOut = FALSE;			//�ӂ��[�����񏈗��I��
			}
		}

		//�؂�ւ������I��
		if (IsFadeIn == FALSE && IsFadeOut == FALSE)
		{
			//�t�F�[�h�C�����Ă��Ȃ��A�t�F�[�h�A�E�g�����Ă��Ȃ��Ƃ�
			GameScene = NextGameScene;	//���̃V�[���ɐ؂�ւ�
			OldGameScene = GameScene;	//�ȑO�̃Q�[���V�[���X�V
		}


		return;
	}

	/// <summary>
	/// �؂�ւ���ʂ̕`��
	/// </summary>
	VOID ChangeDraw(VOID)
	{
		//�ȑO�̃V�[����`��
		switch (OldGameScene)
		{
		case GAME_SCENE_TITLE:
			TitleDraw();		//�^�C�g����ʂ̕`��
			break;
		case GAME_SCENE_PLAY:
			PlayDraw();		//�v���C��ʂ̕`��
			break;
		case GAME_SCENE_END:
			EndDraw();		//�G���h��ʂ̕`��
			break;
		default:
			break;
		}

		//�t�F�[�h�C��
		if (IsFadeIn == TRUE)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
		}

		//�t�F�[�h�A�E�g
		if (IsFadeOut == TRUE)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
		}

		//�l�p��`��
		DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

		//�������I��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));
		return;
	}

	/// <summary>
	/// �����蔻��̗̈�X�V�i�v���C���[�j
	/// </summary>
	/// <param name="Coll">�����蔻��̗̈�X�V</param>
	VOID CollUpdatePlayer(CHARACTOR * chara)
	{
		chara->coll.left = chara->img.x;					//�����蔻��������
		chara->coll.top = chara->img.y;						//�����蔻��������

		chara->coll.right = chara->img.x + chara->img.width;	//�����蔻��������
		chara->coll.bottom = chara->img.y + chara->img.height;	//�����蔻��������

		return;
	}

	/// <summary>
	/// �����蔻��̗̈�X�V�i�S�[���j
	/// </summary>
	/// <param name="Coll">�����蔻��̗̈�X�V</param>
	VOID CollUpdate(CHARACTOR * chara)
	{
		chara->coll.left = chara->img.x;					//�����蔻��������
		chara->coll.top = chara->img.y;						//�����蔻��������

		chara->coll.right = chara->img.x + chara->img.width;	//�����蔻��������
		chara->coll.bottom = chara->img.y + chara->img.height;	//�����蔻��������

		return;
	}

/// <summary>
/// �����蔻��̗̈�X�V�i�摜�j
/// </summary>
/// <param name="img">�摜�\���̂̃|�C���^</param>
	VOID CollUpdateImage(IMAGE* img)
	{
		img->coll.left = img->x;
		img->coll.top = img->y;

		img->coll.right = img->x + img->width;
		img->coll.bottom = img->y + img->height;

		return;
	}

	/// <summary>
	/// �����蔻��̗̈�X�V�i�����摜�j
	/// </summary>
	/// <param name="img">�摜�\���̂̃|�C���^</param>
	VOID CollUpdateDivImage(DIVIMAGE* div)
	{
		div->coll.left = div->x;
		div->coll.top = div->y;

		div->coll.right = div->x + div->width;
		div->coll.bottom = div->y + div->height;

		return;
	}

	/// <summary>
	/// ��`�Ƌ�`�̓����蔻��
	/// </summary>
	/// <param name="a">��`A</param>
	/// <param name="b">��`B</param>
	/// <returns>��������TRUE�^������Ȃ��Ȃ�FALSE</returns>
	BOOL OnCollRect(RECT a, RECT b)
	{
		if (
			a.left < b.right &&//��`A�̍���X���W < ��`B�̉E��X���W�@����
			a.right > b.left &&//��`A�̉E��X���W > ��`B�̍���X���W�@����
			a.top < b.bottom &&//��`A�̏��Y���W < ��`B�̉���Y���W�@����
			a.bottom > b.top//��`A�̉���Y���W > ��`B�̏��Y���W
			)
		{
			//�������Ă���Ƃ�
			return TRUE;
		}
		else
		{
			//�������Ă��Ȃ��Ƃ�
			return FALSE;
		}
	}

	/// <summary>
/// �摜�𕪊����ă������ɓǂݍ���
/// </summary>
/// <param name="handle">�n���h���z��̐擪�A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
/// <param name="bunkatuYoko">��������Ƃ��̉��̐�</param>
/// <param name="bunkatuTate">��������Ƃ��̏c�̐�</param>
/// <returns></returns>
	BOOL LoadImageDivMem(DIVIMAGE* div, const char* path, int bunkatuYoko, int bunkatuTate)
	{
		int IsLoad = -1;	//�摜���ǂݍ��݂����H

		//�ꎞ�I�ɉ摜�̃n���h����p�ӂ���
		int TempHandle = LoadGraph(path);

		//�ǂݍ��݃G���[
		if (TempHandle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),	//�E�B���h�E�n���h��
				path,					//�{��
				"�摜�ǂݍ��݃G���[",	//�^�C�g��
				MB_OK					//�{�^��
			);

			return FALSE;	//�ǂݍ��ݎ��s
		}

		//�摜�̕��ƍ������擾
		int Width = -1;		//��
		int Height = -1;	//����
		GetGraphSize(TempHandle, &Width, &Height);

		//�������ēǂݍ���
		IsLoad = LoadDivGraph(
			path,							//�摜�̃p�X
			bunkatuYoko * bunkatuTate,		//��������
			bunkatuYoko, bunkatuTate,					//���̕���,�c�̕���
			Width / bunkatuYoko, Height / bunkatuTate,	//�摜1���̕�,����
			div->handle									//�A���ŊǗ�����z��̐擪�A�h���X
		);

		//�����G���[
		if (IsLoad == -1)
		{
			MessageBox(
				GetMainWindowHandle(),	//�E�B���h�E�n���h��
				path,					//�{��
				"�摜�����G���[",		//�^�C�g��
				MB_OK					//�{�^��
			);

			return FALSE;	//�ǂݍ��ݎ��s
		}

		//����ݒ�
		div->DivYoko = bunkatuYoko;
		div->DivTate = bunkatuTate;
		div->DivMax = div->DivYoko * div->DivTate;
		div->x = 0;
		div->y = 0;
		GetGraphSize(div->handle[0], &div->width, &div->height);
		div->AnimCnt = 0;
		div->IsAnimLoop = FALSE;
		div->nowIndex = 0;

		//�����蔻��̍X�V
		CollUpdateDivImage(div);

		//�摜��`�悷��
		div->IsDraw = TRUE;

		//�ꎞ�I�ɓǂݍ��񂾃n���h�������
		DeleteGraph(TempHandle);

		return TRUE;
	}

	///// <summary>
	///// �摜���������ɓǂݍ���
	///// </summary>
	///// <param name="image">�摜�\���̂̃A�h���X</param>
	///// <param name="path">�摜�̃p�X</param>
	///// <returns></returns>
	//BOOL LoadImageMem(IMAGE* image, const char* path)
	//{
	//	//�S�[���̉摜��ǂݍ���
	//	strcpyDx(image->path, path);			//�p�X�̃R�s�[
	//	image->handle = LoadGraph(image->path);	//�摜�̓ǂݍ���

	//	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
	//	if (image->handle == -1)
	//	{
	//		MessageBox(
	//			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
	//			image->path,			//���b�Z�[�W�{��
	//			"�摜�ǂݍ��݃G���[�I",	//���b�Z�[�W�^�C�g��
	//			MB_OK					//�{�^��
	//		);

	//		return FALSE;	//�ǂݍ��ݎ��s
	//	}

	//	//�摜�̕��ƍ������擾
	//	GetGraphSize(image->handle, &image->width, &image->height);

	//	//�摜�̈ʒu��������
	//	image->x = 0; image->y = 0;

	//	//�����蔻��̍X�V
	//	CollUpdateImage(image);

	//	//�摜��`�悷��
	//	image->IsDraw = TRUE;

	//	//�ǂݍ��߂�
	//	return TRUE;
	//}

	/// <summary>
	/// �摜�`��
	/// </summary>
	/// <param name="image">�`�悷��摜�\����</param>
	/// <returns></returns>
	/// <returns></returns>
	VOID DrawImage(IMAGE image)
	{
		//�摜���`��ł���Ƃ���
		if (image.IsDraw == TRUE)
		{
			DrawGraph(image.x, image.y, image.handle, TRUE);
		}

		////�f�o�b�O���[�h�̂Ƃ���
		//if (GAME_DEBUG == TRUE)
		//{
		//	//�����蔻���`��
		//	DrawRect(image.coll, GetColor(255, 255, 255), FALSE);
		//}
		//return;
	}

	/// <summary>
	/// �����摜�̕`��
	/// </summary>
	/// <param name="image">�`�悷��摜�\����</param>
	/// <returns></returns>
	/// <returns></returns>
	VOID DrawDivImage(DIVIMAGE* image)
	{
		//�摜���`��ł���Ƃ���
		if (image->IsDraw == TRUE)
		{
			DrawGraph(image->x, image->y, image->handle[image->nowIndex], TRUE);

			//�A�j���[�V�����̃J�E���g�A�b�v
			if (image->AnimCnt < image->AnimCntMAX) { image->AnimCnt++; }
			else
			{
				//���ׂẴA�j���[�V������`�悵�I�������
				if (image->nowIndex == image->DivMax - 1)
				{
					//�A�j���[�V�������J��Ԃ��Ȃ��Ȃ��
					if (image->IsAnimLoop == FALSE)
					{
						image->IsDraw = FALSE;	//�`����~�߂�
					}
					image->AnimCnt = 0;		//�J�E���^0�N���A
					image->nowIndex = 0;	//�擪�ɖ߂�
				}
				//���̕����摜������Ƃ���
				else if (image->nowIndex < image->DivMax - 1)
				{
					image->nowIndex++; //���̉摜��
				}
				image->AnimCnt = 0;	//�J�E���^0�N���A
			}
		}

		//�f�o�b�O���[�h�̂Ƃ���
		if (GAME_DEBUG == TRUE)
		{
			//�����蔻���`��
			DrawRect(image->coll, GetColor(255, 255, 255), FALSE);
		}
		return;
	}

	/// <summary>
	/// �����摜�̕`��(�L�����Ȃ�)
	/// </summary>
	/// <param name="image">�`�悷��摜�\����</param>
	/// <returns></returns>
	/// <returns></returns>
	VOID DrawDivImageChara(DIVIMAGE* image)
	{
		//�摜���`��ł���Ƃ���
		if (image->IsDraw == TRUE)
		{
			DrawGraph(image->x, image->y, image->handle[image->nowIndex], TRUE);

			//�A�j���[�V�����̃J�E���g�A�b�v
			if (image->AnimCnt < image->AnimCntMAX) { image->AnimCnt++; }
			else
			{

				//�E�����̂Ƃ�(�摜�ɂ���Đ������Ⴄ�̂ŁA���킹�邱�ƁI)
				if (muki == muki_migi)
				{
					if (image->nowIndex >= 6 && image->nowIndex < 8)
					{
						image->nowIndex++;
					}
					else
					{
						image->nowIndex = 6;
					}
				}

				//�������̂Ƃ�(�摜�ɂ���Đ������Ⴄ�̂ŁA���킹�邱�ƁI)
				if (muki == muki_hidari)
				{
					if (image->nowIndex >= 3 && image->nowIndex < 5)
					{
						image->nowIndex++;
					}
					else
					{
						image->nowIndex = 3;
					}
				}

				//������̂Ƃ�(�摜�ɂ���Đ������Ⴄ�̂ŁA���킹�邱�ƁI)
				if (muki == muki_ue)
				{
					if (image->nowIndex >= 9 && image->nowIndex < 11)
					{
						image->nowIndex++;
					}
					else
					{
						image->nowIndex = 9;
					}
				}

				//�������̂Ƃ�(�摜�ɂ���Đ������Ⴄ�̂ŁA���킹�邱�ƁI)
				if (muki == muki_shita)
				{
					if (image->nowIndex >= 0 && image->nowIndex < 2)
					{
						image->nowIndex++;
					}
					else
					{
						image->nowIndex = 0;
					}
				}

				//���������̂Ƃ��́A���O�̌����̐^�񒆂̉摜�ɂ���(�摜�ɍ��킹�Č��߂ăl)
				if (muki == muki_none)
				{
					if (image->nowIndex >= 6 && image->nowIndex <= 8) { image->nowIndex = 7; }
					if (image->nowIndex >= 3 && image->nowIndex <= 5) { image->nowIndex = 4; }
					if (image->nowIndex >= 9 && image->nowIndex <= 11) { image->nowIndex = 10; }
					if (image->nowIndex >= 0 && image->nowIndex <= 2) { image->nowIndex = 1; }
				}

				image->AnimCnt = 0;	//�J�E���^0�N���A
			}
		}

		////�f�o�b�O���[�h�̂Ƃ���
		//if (GAME_DEBUG == TRUE)
		//{
		//	//�����蔻���`��
		//	DrawRect(image->coll, GetColor(255, 255, 255), FALSE);
		//}
		return;
	}

	/// <summary>
	/// ���y���������ɓǂݍ���
	/// </summary>
	/// <param name="audio">Audio�\���̕ϐ��̃A�h���X</param>
	/// <param name="path">Audio�̉��y�p�X</param>
	/// <param name="volume">�{�����[��</param>
	/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
	/// <returns></returns>
	BOOL LoadAudio(AUDIO* audio, const char* path, int Volume, int playType)
	{
		//���y�̓ǂݍ���
		strcpyDx(audio->path, path);					//�p�X�̃R�s�[
		audio->handle = LoadSoundMem(audio->path);		//���y�̓ǂݍ���

		//���y���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
		if (audio->handle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
				audio->path,			//���b�Z�[�W�{��
				"���y�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
				MB_OK					//�{�^��
			);

			return FALSE;	//�ǂݍ��ݎ��s
		}

		//���̑��̐ݒ�
		audio->Volume = Volume;
		audio->playType = playType;

		return TRUE;
	}

	/// <summary>
	/// ���y�Đ�
	/// </summary>
	/// <param name="audio">�Đ����鉹�y�\����</param>
	/// <returns></returns>
	VOID PlayAudio(AUDIO audio)
	{
		//���[�v�Đ��łȂ���΁A���̂܂܍Đ�����
		if (audio.playType == DX_PLAYTYPE_BACK)
		{
			PlaySoundMem(audio.handle, audio.playType);//���y�̍Đ�
		}
		//���[�v�Đ��̂Ƃ��́A���y���Đ�����Ă��Ȃ���΁A�Đ�����
		else if (audio.playType == DX_PLAYTYPE_LOOP)
		{
			if (CheckSoundMem(audio.handle) == 0)
			{
				PlaySoundMem(audio.handle, audio.playType);//���y�̍Đ�
			}
		}

		return;
	}

	/// <summary>
	/// ���y�̃{�����[���擾
	/// </summary>
	/// <param name="audio">�擾���鉹�y�\����</param>
	/// <param name = "volume">�{�����[��(0�`255)</param>
	/// <returns></returns>
	int GetVolumeAudio(AUDIO audio)
	{
		//���ʂ��擾
		return audio.Volume;
	}

	/// <summary>
	/// ���y�̃{�����[���ݒ�
	/// </summary>
	/// <param name="audio">�ݒ肷�鉹�y�\����</param>
	/// <param name = "volume">�{�����[��(0�`255)</param>
	/// <returns></returns>
	VOID SetVolumeAudio(AUDIO* audio, int vol)
	{
		//���ʂ�ݒ�
		audio->Volume = vol;
		ChangeVolumeSoundMem(audio->Volume, audio->handle);

		return;
	}

	/// <summary>
	/// ���y�̃{�����[���ύX
	/// </summary>
	/// <param name="audio">�ύX���鉹�y�\����</param>
	/// <param name = "volume">�{�����[��(0�`255)</param>
	/// <returns></returns>
	VOID ChangeVolumeAudio(AUDIO* audio, int vol)
	{
		//���ʂ�ݒ�i�\���̂̃{�����[���͕ύX���Ȃ��j
		ChangeVolumeSoundMem(vol, audio->handle);

		return;
	}

	/// <summary>
	/// ���y��~
	/// </summary>
	/// <param name="audio">��~���鉹�y�\����</param>
	/// <returns></returns>
	VOID StopAudio(AUDIO* audio)
	{
		//���y�̒�~
		StopSoundMem(audio->handle);

		return;
	}