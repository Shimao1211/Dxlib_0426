#pragma once

//ヘッダファイルの読み込み
#include "game.h"	//ゲーム全体のヘッダファイル

//マクロ定義
#define GAME_FPS		60	//FPSの標準
#define GAME_FPS_MAX	240	//FPSの最大値
#define GAME_FPS_MIN	60	//FPSの最小値

//FPS構造体
struct FPS
{
	BOOL IsInitFlg = FALSE;	//最初の測定開始フラグ
	LONGLONG StartTime = 0;	//測定開始時刻
	LONGLONG NowTime = 0;	//現在の時刻
	LONGLONG OldTime = 0;	//以前の時刻

	int Value = GAME_FPS;		//FPSの値

	float DeltaTime = 0.000001;	//0が５個 経過時間
	int Count = 1;				//現在のフレーム数
	float DrawValue = 0.0f;		//計算結果を描画
	int SampleRate = GAME_FPS;	//平均を取るサンプル値

	float GameTime = 0.0f;		//ゲーム内時間
	DATEDATA NowDataTime;		//現在の日付時刻
};

//グローバル変数の外部参照
extern FPS fps;	//FPSの管理

//プロトタイプ宣言
extern VOID FPSUpdate(VOID);	//FPS値を更新する
extern VOID FPSDraw(VOID);		//FPS値を描画
extern VOID FPSWait(VOID);		//FPS値を待つ
extern VOID ResetGameTime(VOID);//ゲーム内時間リセット
extern float GetGameTime(VOID);	//ゲーム内時間を取得