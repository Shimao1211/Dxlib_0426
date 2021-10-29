#pragma once

#include "game.h"

#define CSV_LOAD_ERR_TITLE TEXT("CSV�ǂݍ��݃G���[")
#define CSV_DATANUM_ERR_TITLE TEXT("CSV�f�[�^���G���[")

#define  CSV_PATH_ENEMY	TEXT(".\\Data\\enemy.csv")

#define CSV_CHARA_FORMAT	TEXT("%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d")

#define ENEMY_MAX	8

//�G���[���b�Z�[�W
#define SCORE_CREATE_ERR_TITLE	TEXT("�X�R�A�f�[�^�쐬�G���[")
#define SCORE_LOAD_ERR_TITLE	TEXT("�X�R�A�f�[�^�Ǎ��G���[")
#define SCORE_CREATE_TITLE		TEXT("�X�R�A�f�[�^�쐬OK!!")
#define SCORE_SAVE_TITLE		TEXT("�X�R�A�f�[�^�ۑ�OK!!")
#define SCORE_DELETE_TITLE		TEXT("�X�R�A�f�[�^�폜OK!!")
#define SCORE_DELETE_ERR_TITLE	TEXT("�X�R�A�f�[�^�폜�G���[")

//�X�R�A�f�[�^�̃p�X
#define SCORE_DATA_PATH		TEXT(".\\Data\\map.a")

//�f�[�^�̏����w��q
#define SCORE_DATA_FORMAT	TEXT("%[^,],%d,%[^,],%d,%[^,],%d")

//�}�b�v�`�b�v�摜�̃t�@�C���p�X
#define IMG_PATH_MAP1		TEXT(".\\Map\\mapchip.png")

//�}�b�v�f�[�^�̃t�@�C���p�X
#define CSV_PATH_MAP1_SHITA			TEXT(".\\Map\\map_shita.csv")
#define CSV_PATH_MAP1_NAKA			TEXT(".\\Map\\map_naka.csv")
#define CSV_PATH_MAP1_NAKA_ATARI	TEXT(".\\Map\\map_naka_atari.csv")
#define CSV_PATH_MAP1_UE			TEXT(".\\Map\\map_ue.csv")

//�}�b�v�̏����w��q
#define CSV_MAP_FORMAT		TEXT("%d,")

#define MAP1_YOKO_MAX				42	//�}�b�v�̉��T�C�Y
#define MAP1_TATE_MAX				23	//�}�b�v�̏c�T�C�Y

#define MAP1_YOKO_DIV				60	//�}�b�v�̉�������
#define MAP1_TATE_DIV				60	//�c������

#define MAP_MOVE_ID					56	//�ʂ��}�b�vID
#define MAP_STOP_ID					57	//�ʂ�Ȃ��}�b�v
#define MAP_NONE_ID					58	//�����Ȃ��}�b�v

struct CHARA_DATA
{
	int No;
	char Name[STR_MAX];

	int HP;
	int ATK;
	int DEF;
	int SPD;

	int MAX_HP;
	int MAX_ATK;
	int MAX_DEF;
	int MAX_SPD;
};

//�X�R�A�f�[�^�\����
struct SCORE_DATA
{
	char path[PATH_MAX];
	char Name1[STR_MAX];
	int Score1;
	char Name2[STR_MAX];
	int Score2;
	char Name3[STR_MAX];
	int Score3;
};

//�}�b�v�f�[�^�\����
struct MAP_DATA
{
	//CSV�f�[�^
	int CSV_shita[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_naka[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_naka_atari[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_ue[MAP1_TATE_MAX][MAP1_YOKO_MAX];

	//�}�b�v�̃n���h��
	int handle[MAP1_TATE_DIV * MAP1_YOKO_DIV];

	//�}�b�v�̏ꏊ
	int x[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int y[MAP1_TATE_MAX][MAP1_YOKO_MAX];

	int width;
	int height;

	int DivMax;

	BOOL IsDraw = FALSE;		//�`�悷�邩

	//�����蔻��
	RECT coll[MAP1_TATE_MAX][MAP1_YOKO_MAX];
};

extern CHARA_DATA enemy[ENEMY_MAX];
extern SCORE_DATA score_data;
extern SCORE_DATA score_dataInit;

extern BOOL LoadCSVChara(const char* path, CHARA_DATA* chara, int DataMax, BOOL IsHeader);
extern BOOL LoadScoreData(const char* path, SCORE_DATA* score, BOOL Iheader);

extern BOOL LoadCSVMap(
	const char* ImgPath,
	const char* ShitaPath,
	const char* NakaPath,
	const char* NakaAtariPath,
	const char* UePath,
	MAP_DATA* map,
	int divYoko,
	int divTate);

extern BOOL CreateScoreData(VOID);
extern BOOL SaveScoreData(VOID);
extern BOOL DeleteScoreData(VOID);
extern const char* GetScoreDataPath(VOID);
extern VOID SetScoreDataPath(const char* path);

extern BOOL CollMap(RECT rect, MAP_DATA map);
extern VOID DrawMap(MAP_DATA map);