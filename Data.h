#pragma once

#include "game.h"

#define CSV_LOAD_ERR_TITLE TEXT("CSV読み込みエラー")
#define CSV_DATANUM_ERR_TITLE TEXT("CSVデータ数エラー")

#define  CSV_PATH_ENEMY	TEXT(".\\Data\\enemy.csv")

#define CSV_CHARA_FORMAT	TEXT("%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d")

#define ENEMY_MAX	8

//エラーメッセージ
#define SCORE_CREATE_ERR_TITLE	TEXT("スコアデータ作成エラー")
#define SCORE_LOAD_ERR_TITLE	TEXT("スコアデータ読込エラー")
#define SCORE_CREATE_TITLE		TEXT("スコアデータ作成OK!!")
#define SCORE_SAVE_TITLE		TEXT("スコアデータ保存OK!!")
#define SCORE_DELETE_TITLE		TEXT("スコアデータ削除OK!!")
#define SCORE_DELETE_ERR_TITLE	TEXT("スコアデータ削除エラー")

//スコアデータのパス
#define SCORE_DATA_PATH		TEXT(".\\Data\\map.a")

//データの書式指定子
#define SCORE_DATA_FORMAT	TEXT("%[^,],%d,%[^,],%d,%[^,],%d")

//マップチップ画像のファイルパス
#define IMG_PATH_MAP1		TEXT(".\\Map\\mapchip.png")

//マップデータのファイルパス
#define CSV_PATH_MAP1_SHITA			TEXT(".\\Map\\map_shita.csv")
#define CSV_PATH_MAP1_NAKA			TEXT(".\\Map\\map_naka.csv")
#define CSV_PATH_MAP1_NAKA_ATARI	TEXT(".\\Map\\map_naka_atari.csv")
#define CSV_PATH_MAP1_UE			TEXT(".\\Map\\map_ue.csv")

//マップの書式指定子
#define CSV_MAP_FORMAT		TEXT("%d,")

#define MAP1_YOKO_MAX				42	//マップの横サイズ
#define MAP1_TATE_MAX				23	//マップの縦サイズ

#define MAP1_YOKO_DIV				60	//マップの横分割数
#define MAP1_TATE_DIV				60	//縦分割数

#define MAP_MOVE_ID					56	//通れるマップID
#define MAP_STOP_ID					57	//通れないマップ
#define MAP_NONE_ID					58	//何もないマップ

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

//スコアデータ構造体
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

//マップデータ構造体
struct MAP_DATA
{
	//CSVデータ
	int CSV_shita[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_naka[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_naka_atari[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_ue[MAP1_TATE_MAX][MAP1_YOKO_MAX];

	//マップのハンドル
	int handle[MAP1_TATE_DIV * MAP1_YOKO_DIV];

	//マップの場所
	int x[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int y[MAP1_TATE_MAX][MAP1_YOKO_MAX];

	int width;
	int height;

	int DivMax;

	BOOL IsDraw = FALSE;		//描画するか

	//当たり判定
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