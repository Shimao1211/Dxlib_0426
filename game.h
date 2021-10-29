#pragma once

//ゲーム全体のヘッダファイル

//ヘッダーファイルの読み込み
#include "DxLib.h"		//DxLibを使うときは必要
#include <math.h>		//数学系
#include <string.h>		//文字列系
#include <time.h>		//時間系

//マクロ定義
#define GAME_TITLE	"ゲームタイトル"	//ゲームタイトル
#define GAME_WIDTH	1280				//ゲーム画面の幅(ウィドス)
#define GAME_HEIGHT	720					//ゲーム画面の高さ(ハイト)
#define GAME_COLOR  32					//ゲームの色域

#define GAME_ICON_ID	333				//ゲームのICONのID

#define GAME_WINDOW_BAR	0				//ウィンドウバーの種類

#define GAME_DEBUG	TRUE				//デバッグモード

#define PATH_MAX	255					//パスの長さ
#define STR_MAX		255					//文字の長さ
#define IMGDIV_MAX	128					//ハンドルの最大数(任意)

//列挙型
enum GAME_SCENE {
	GAME_SCENE_TITLE,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
	GAME_SCENE_CHANGE
};	//ゲームのシーン

//画像の構造体
struct IMAGE
{
	int handle = -1;		//画像のハンドル(管理番号)
	char path[PATH_MAX];	//画像の場所(パス)

	int x = -1;				//X位置
	int y = -1;				//Y位置
	int width = -1;			//幅
	int height = -1;		//高さ

	int speed = 1;

	RECT coll;				//当たり判定の領域(四角)

	BOOL IsDraw = FALSE;	//画像が描画できる？
};

//連続する画像の構造体
struct DIVIMAGE
{
	int DivMax;						//分割数
	int handle[IMGDIV_MAX];			//画像ハンドル
	int nowIndex = 0;				//現在のハンドル配列のインデックス
	char path[PATH_MAX];			//パス

	int x = -1;						//X位置
	int y = -1;						//Y位置
	int width = -1;					//幅
	int height = -1;				//高さ

	int DivYoko = -1;				//横の分割数
	int DivTate = -1;				//縦の分割数

	RECT coll;						//当たり判定の領域(四角)

	BOOL IsDraw = FALSE;			//描画できるか？

	int AnimCnt = 0;				//アニメーションカウンタ
	int AnimCntMAX = 2;				//アニメーションカウンタMAX
	BOOL IsAnimLoop = FALSE;		//アニメーションをループさせるか？
};

//動画の構造体
struct MOVIE
{
	int handle = -1;		//動画のハンドル
	char path[PATH_MAX];	//動画のパス

	int x = -1;				//X位置
	int y = -1;				//Y位置
	int width = -1;			//幅
	int height = -1;		//高さ

	int Volume;				//ボリューム（MIN 0 ～ 255 MAX）　
};

//音楽の構造体
struct AUDIO
{
	int handle = -1;		//音楽のハンドル
	char path[PATH_MAX];	//音楽のパス

	int Volume = -1;		//ボリューム（MIN 0 ～ 255 MAX）
	int playType = -1;		//BGM or SE
};

//########## 外部のグローバル変数 ##########
extern DIVIMAGE samplePlayerImg;

//########## プロトタイプ宣言 ##########
//※extern は、外部に関数がありますよ！という目印でつけています。

extern BOOL GameLoad(VOID);	//データを読込
extern VOID GameInit(VOID);	//データの初期化
extern VOID GameDelete(VOID);	//データを削除

extern VOID Title(VOID);		//タイトル画面
extern VOID TitleProc(VOID);	//タイトル画面(処理)
extern VOID TitleDraw(VOID);	//タイトル画面(描画)

extern VOID Play(VOID);		//プレイ画面
extern VOID PlayProc(VOID);	//プレイ画面(処理)
extern VOID PlayDraw(VOID);	//プレイ画面(描画)

extern VOID End(VOID);			//エンド画面
extern VOID EndProc(VOID);		//エンド画面(処理)
extern VOID EndDraw(VOID);		//エンド画面(描画)

extern VOID Change(VOID);		//切り替え画面
extern VOID ChangeProc(VOID);	//切り替え画面(処理)
extern VOID ChangeDraw(VOID);	//切り替え画面(描画)

extern VOID ChangeScene(GAME_SCENE scene);		//シーン切り替え
extern BOOL OnCollRect(RECT a, RECT b);		//矩形と矩形の当たり判定
extern VOID CollUpdateImage(IMAGE* img);		//画像の当たり判定の更新
extern VOID CollUpdateDivImage(DIVIMAGE* div);	//分割画像の当たり判定の更新
extern BOOL LoadImageMem(IMAGE* image, const char* path);										//ゲームの画像を読み込み
extern BOOL LoadAudio(AUDIO* audio, const char* path, int Volume, int playType);				//ゲームの音楽を読み込み
extern BOOL LoadImageDivMem(DIVIMAGE* div, const char* path, int bunkatuYoko, int bunkatuTate);//ゲームの画像の分割読み込み

extern VOID PlayAudio(AUDIO audio);					//音楽再生
extern int GetVolumeAudio(AUDIO audio);				//音楽のボリューム取得
extern VOID SetVolumeAudio(AUDIO* audio, int vol);		//音楽のボリューム設定
extern VOID ChangeVolumeAudio(AUDIO* audio, int vol);	//音楽のボリューム変更
extern VOID StopAudio(AUDIO* audio);					//音楽停止

extern VOID DrawImage(IMAGE image);							//画像描画
extern VOID DrawDivImage(DIVIMAGE* image);					//分割画像の描画
extern VOID DrawDivImageChara(DIVIMAGE* image);				//分割画像の描画