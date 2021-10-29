#include "game.h"
#include "Data.h"
#include "shape.h"

CHARA_DATA enemy[ENEMY_MAX];

SCORE_DATA score_data;
SCORE_DATA score_dataInit = { SCORE_DATA_PATH,"NAME1",-1,"NAME2",-1, "NAME3",-1 };

/// <summary>
/// 
/// </summary>
/// <param name="path"></param>
/// <param name="chara"></param>
/// <param name="DataMax"></param>
/// <param name="IsHeader"></param>
/// <returns></returns>
BOOL LoadCSVChara(const char* path, CHARA_DATA* chara, int DataMax, BOOL IsHeader = FALSE)
{
	FILE* fp;
	char GetChar = '\0';
	int LoopCnt = 0;
	errno_t FileErr = NULL;
	int IsEOF = NULL;

	FileErr = fopen_s(&fp, path, "r");
	if (FileErr != NULL)
	{
		MessageBox(GetMainWindowHandle(), path, CSV_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	if (IsHeader == TRUE)
	{
		while (GetChar != '\n') { GetChar = fgetc(fp); }
	}

	while (IsEOF != EOF)
	{
		IsEOF = fscanf_s(
			fp,
			CSV_CHARA_FORMAT,
			&(chara + LoopCnt)->No,
			&(chara + LoopCnt)->Name[0], STR_MAX,
			&(chara + LoopCnt)->HP,
			&(chara + LoopCnt)->ATK,
			&(chara + LoopCnt)->DEF,
			&(chara + LoopCnt)->SPD,
			&(chara + LoopCnt)->MAX_HP,
			&(chara + LoopCnt)->MAX_ATK,
			&(chara + LoopCnt)->MAX_DEF,
			&(chara + LoopCnt)->MAX_SPD
		);

		LoopCnt++;
	}

	if (LoopCnt - 1 != DataMax)
	{
		MessageBox(GetMainWindowHandle(), path, CSV_DATANUM_ERR_TITLE, MB_OK);
		return FALSE;
	}
	
	fclose(fp);

	return TRUE;
}

/// <summary>
/// 
/// </summary>
/// <param name="path"></param>
/// <param name="score"></param>
/// <param name="IsHeader"></param>
/// <returns></returns>
BOOL LoadScoreData(const char* path, SCORE_DATA* score, BOOL IsHeader = FALSE)
{
	FILE* fp;
	char GetChar = '\0';
	errno_t FileErr = NULL;
	int IsEOF = NULL;

	SetScoreDataPath(path);

	FileErr = fopen_s(&fp, path, "r");
	if (FileErr != NULL)
	{
		//
		if (CreateScoreData() == FALSE) { return FALSE; }

		//

		FileErr = fopen_s(&fp, path, "r");
		if (FileErr != NULL)
		{
			MessageBox(GetMainWindowHandle(), path, SCORE_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}
	}

	//CSV��1�s�ڂ��w�b�_�̎�
	if (IsHeader == TRUE)
	{
		//1���������o���A���s���q���g��1�s�ڂ�ǂݔ�΂�
		while (GetChar != '\n') { GetChar = fgetc(fp); }
	}

	//���ۂɃf�[�^���i�[����
	while (IsEOF != EOF)	//End Of File (�t�@�C���̍Ō�)�@�ł͂Ȃ��Ƃ��J��Ԃ�
	{
		//CSV�̃f�[�^��1�s����ǂݎ��
		IsEOF = fscanf_s(
			fp,
			SCORE_DATA_FORMAT,	//�f�[�^���u��s=���R�[�h�v�ŕ`����Ă���̂��|�C���g
			&(score)->Name1, STR_MAX,	//�������ǂݍ��ނƂ��́A���̈����ŕ�����ő吔���w�肷��
			&(score)->Score1,
			&(score)->Name2, STR_MAX,	//�������ǂݍ��ނƂ��́A���̈����ŕ�����ő吔���w�肷��
			&(score)->Score2,
			&(score)->Name3, STR_MAX,	//�������ǂݍ��ނƂ��́A���̈����ŕ�����ő吔���w�肷��
			&(score)->Score3
		);

	}

	fclose(fp);	//�t�@�C�������

	return TRUE;
}

/// <summary>
/// �X�R�A�f�[�^�쐬
/// </summary>
/// <returns></returns>
BOOL CreateScoreData(VOID)
{
	//�Ȃ�������X�R�A�f�[�^�����
	FILE* creFp;
	errno_t creFileErr = NULL;	//�t�@�C���ǂݍ��݃G���[���m�F
	creFileErr = fopen_s(&creFp, GetScoreDataPath(), "w");	//�t�@�C���������݃��[�h(w)�ŊJ��(�쐬����)
	if (creFileErr != NULL)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//�w�b�_����������
	fprintf_s(creFp,
		"Name1,Score1,Name2,Score2,Name3,Score3\n"
	);

	//��̃X�R�A�f�[�^����������
	fprintf_s(creFp,
		"%s,%d,%s,%d,%s,%d",
		score_data.Name1, score_data.Score1,
		score_data.Name2, score_data.Score2,
		score_data.Name3, score_data.Score3
	);

	fclose(creFp);	//�X�R�A�f�[�^�����

	MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_SAVE_TITLE, MB_OK);
	return TRUE;
}

/// <summary>
/// �X�R�A�f�[�^�ۑ�
/// </summary>
/// <returns></returns>
BOOL SaveScoreData(VOID)
{
	//�Ȃ�������X�R�A�f�[�^�����
	FILE* creFp;
	errno_t creFileErr = NULL;	//�t�@�C���ǂݍ��݃G���[���m�F
	creFileErr = fopen_s(&creFp, GetScoreDataPath(), "w");	//�t�@�C���������݃��[�h(w)�ŊJ��(�쐬����)
	if (creFileErr != NULL)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//�w�b�_����������
	fprintf_s(creFp,
		"Name1,Score1,Name2,Score2,Name3,Score3\n"
	);

	//�X�R�A�f�[�^����������
	fprintf_s(creFp,
		"%s,%d,%s,%d,%s,%d",
		score_data.Name1, score_data.Score1,
		score_data.Name2, score_data.Score2,
		score_data.Name3, score_data.Score3
	);

	fclose(creFp);	//�X�R�A�f�[�^�����

	MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_SAVE_TITLE, MB_OK);
	return TRUE;
}

/// <summary>
/// �X�R�A�f�[�^�̍폜
/// </summary>
/// <param name="path"></param>
BOOL DeleteScoreData(VOID)
{
	//�t�@�C���폜
	if (remove(GetScoreDataPath()) != 0)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_DELETE_ERR_TITLE, MB_OK);
	}
	else
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_DELETE_TITLE, MB_OK);
	}

	//�X�R�A�f�[�^���쐬
	return CreateScoreData();
}

/// <summary>
/// �X�R�A�f�[�^�̃p�X��Ԃ�
/// </summary>
/// <param name=""></param>
/// <returns></returns>
const char* GetScoreDataPath(VOID)
{
	return score_data.path;
}

/// <summary>
/// �X�R�A�f�[�^�̃p�X��ݒ肷��
/// </summary>
/// <param name="path"></param>
VOID SetScoreDataPath(const char* path)
{
	strcpyDx(score_data.path, path);
	return;
}

/// <summary>
/// 
/// </summary>
/// <param name="ImgPath"></param>
/// <param name="ShitaPath"></param>
/// <param name="NakaPath"></param>
/// <param name="NakaAtariPath"></param>
/// <param name="UePath"></param>
/// <param name="map"></param>
/// <param name="divYoko"></param>
/// <param name="divTate"></param>
/// <returns></returns>
BOOL LoadCSVMap(const char* ImgPath, const char* ShitaPath, const char* NakaPath, const char*
	NakaAtariPath, const char* UePath, MAP_DATA* map, int divYoko, int divTate)
{
	//�}�b�v�`�b�v��ǂݍ���
	{
		int IsLoad = -1;	//�摜���ǂݍ��߂����H

		//�ꎞ�I�ɉ摜�̃n���h����p�ӂ���
		int TempHandle = LoadGraph(ImgPath);

		//�ǂݍ��݃G���[
		if (TempHandle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),	//�E�B���h�E�n���h��
				ImgPath,				//�{��
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
			ImgPath,							//�摜�̃p�X
			divYoko * divTate,					//��������
			divYoko, divTate,					//�������A�c����
			Width / divYoko, Height / divTate,	//�摜1���̕��A����
			map->handle							//�A���ŊǗ�����z��̐擪�A�h���X
		);

		//�����G���[
		if (IsLoad == -1)
		{
			MessageBox(
				GetMainWindowHandle(),	//�E�B���h�E�n���h��
				ImgPath,				//�{��
				"�摜�����G���[",	//�^�C�g��
				MB_OK					//�{�^��
			);

			return FALSE;	//�ǂݍ��ݎ��s
		}

		//����ݒ�
		map->DivMax = divYoko * divTate;
		GetGraphSize(map->handle[0], &map->width, &map->height);

		//�摜��`�悷��
		map->IsDraw = TRUE;

		//1���I�ɓǂݍ��񂾃n���h�������
		DeleteGraph(TempHandle);
	}

	FILE* fp;				//�t�@�C���|�C���^
	char GetChar = '\0';	//�擾���镶��
	int LoopCnt = 0;		//���[�v�J�E���^
	errno_t FileErr = NULL;	//�t�@�C���ǂݍ��݃G���[���m�F����
	int IsEOF = NULL;		//�t�@�C���̏I��肩�`�F�b�N

	//��CSV��ǂݎ��
	{
		FileErr = fopen_s(&fp, ShitaPath, "r");	//�t�@�C���ǂݍ��݃��[�h�i���j�ŊJ��
		if (FileErr != NULL)
		{
			MessageBox(GetMainWindowHandle(), ShitaPath, CSV_LOAD_ERR_TITLE, MB_OK);	//�ǂݍ��݃G���[
			return FALSE;
		}

		//�R�R���琳��ɓǂݍ��߂����̏���

		//���ۂɃf�[�^���i�[����
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File�i�t�@�C���̍Ō�j�ł͂Ȃ��Ƃ��J��Ԃ�
		{
			//�t�@�C�����琔�l��1�ǂݍ���(%d,)�A�z��Ɋi�[����
			//[LoopCnt / MAP_YKO_MAX] [LoopCnt % MAP_YOKO_MAX]�́A
			//�Ⴆ�΁A[3][3]�̔z��̎��ALoopCnt��5�ł���΁A
			//[5 / 3 = 1][5 % 3 = 2]�ƂȂ�A1�����z��̈ʒu��2�����z��Ƃ��ĕϊ��ł���
			IsEOF = fscanf_s(
				fp,
				CSV_MAP_FORMAT,	//�f�[�^��[XX,]�ŕ`����Ă���̂��|�C���g
				&map->CSV_shita[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//���̃f�[�^��
			LoopCnt++;
		}

		fclose(fp);	//�t�@�C�������
	}

	//��CSV��ǂݎ��
	{
		FileErr = fopen_s(&fp, NakaPath, "r");	//�t�@�C���ǂݍ��݃��[�h�i���j�ŊJ��
		if (FileErr != NULL)
		{
			MessageBox(GetMainWindowHandle(), NakaPath, CSV_LOAD_ERR_TITLE, MB_OK);	//
			return FALSE;
		}

		//�R�R���琳��ɓǂݍ��߂����̏���

				//���ۂɃf�[�^���i�[����
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File�i�t�@�C���̍Ō�j�ł͂Ȃ��Ƃ��J��Ԃ�
		{
			//�t�@�C�����琔�l��1�ǂݍ���(%d,)�A�z��Ɋi�[����
			//[LoopCnt / MAP_YKO_MAX] [LoopCnt % MAP_YOKO_MAX]�́A
			//�Ⴆ�΁A[3][3]�̔z��̎��ALoopCnt��5�ł���΁A
			//[5 / 3 = 1][5 % 3 = 2]�ƂȂ�A1�����z��̈ʒu��2�����z��Ƃ��ĕϊ��ł���
			IsEOF = fscanf_s(
				fp,
				CSV_MAP_FORMAT,	//�f�[�^��[XX,]�ŕ`����Ă���̂��|�C���g
				&map->CSV_naka[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//���̃f�[�^��
			LoopCnt++;
		}

		fclose(fp);	//�t�@�C�������
	}

	//��������CSV��ǂݎ��
	{
		FileErr = fopen_s(&fp, NakaAtariPath, "r");	//�t�@�C���ǂݍ��݃��[�h�i���j�ŊJ��
		if (FileErr != NULL)
		{
			MessageBox(GetMainWindowHandle(), NakaAtariPath, CSV_LOAD_ERR_TITLE, MB_OK);	//
			return FALSE;
		}

		//�R�R���琳��ɓǂݍ��߂����̏���

				//���ۂɃf�[�^���i�[����
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File�i�t�@�C���̍Ō�j�ł͂Ȃ��Ƃ��J��Ԃ�
		{
			//�t�@�C�����琔�l��1�ǂݍ���(%d,)�A�z��Ɋi�[����
			//[LoopCnt / MAP_YKO_MAX] [LoopCnt % MAP_YOKO_MAX]�́A
			//�Ⴆ�΁A[3][3]�̔z��̎��ALoopCnt��5�ł���΁A
			//[5 / 3 = 1][5 % 3 = 2]�ƂȂ�A1�����z��̈ʒu��2�����z��Ƃ��ĕϊ��ł���
			IsEOF = fscanf_s(
				fp,
				CSV_MAP_FORMAT,	//�f�[�^��[XX,]�ŕ`����Ă���̂��|�C���g
				&map->CSV_naka_atari[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//���̃f�[�^��
			LoopCnt++;
		}

		fclose(fp);	//�t�@�C�������
	}

	//��CSV
	{
		FileErr = fopen_s(&fp, UePath, "r");	//
		if (FileErr != NULL)
		{
			MessageBox(GetMainWindowHandle(), UePath, CSV_LOAD_ERR_TITLE, MB_OK);	//
			return FALSE;
		}

		//

		//
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//
		{
			//
			//
			//
			//
			IsEOF = fscanf_s(
				fp,
				CSV_MAP_FORMAT,	//
				&map->CSV_ue[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//
			LoopCnt++;
		}

		fclose(fp);	//
	}

	//
	{
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				//
				if (map->CSV_naka_atari[tate][yoko] == MAP_STOP_ID)
				{
					//
					map->coll[tate][yoko].left = (yoko + 0) * map->width + 1;
					map->coll[tate][yoko].right = (yoko + 1) * map->width - 1;
					map->coll[tate][yoko].top = (tate + 0) * map->height + 1;
					map->coll[tate][yoko].bottom = (tate + 1) * map->height - 1;
				}
				else
				{
					map->coll[tate][yoko].left = 0;
					map->coll[tate][yoko].right = 0;
					map->coll[tate][yoko].top = 0;
					map->coll[tate][yoko].bottom = 0;
				}

				//
				map->x[tate][yoko] = (yoko + 0) * map->width;
				map->y[tate][yoko] = (tate + 0) * map->height;
			}
		}
	}

	return TRUE;
}

/// <summary>
/// 
/// </summary>
/// <param name="rect"></param>
/// <param name="map"></param>
/// <returns></returns>
BOOL CollMap(RECT rect, MAP_DATA map)
{
	for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
		{
			if (CheckCollRectToRect(rect, map.coll[tate][yoko]) == TRUE)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

/// <summary>
/// 
/// </summary>
/// <param name="map"></param>
/// <returns></returns>
VOID DrawMap(MAP_DATA map)
{
	if (map.IsDraw == TRUE)
	{
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(map.x[tate][yoko], map.y[tate][yoko], map.handle[map.CSV_shita[tate][yoko]], TRUE);
			}
		}

		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(map.x[tate][yoko], map.y[tate][yoko], map.handle[map.CSV_naka[tate][yoko]], TRUE);
			}
		}

		DrawDivImageChara(&samplePlayerImg);

		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(map.x[tate][yoko], map.y[tate][yoko], map.handle[map.CSV_ue[tate][yoko]], TRUE);
			}
		}
	}

	if (GAME_DEBUG == TRUE)
	{
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				//DrawRect(map.coll[tate][yoko], GetColor(255, 255, 255), FALSE);
			}
		}
	}
	return;
}