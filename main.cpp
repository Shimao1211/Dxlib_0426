//ヘッダーファイルの読み込み
#include "DxLib.h"	//DxLibを使う時は必要

//マクロ定義
#define GAME_TITLE "ゲームタイトル"		//ゲームタイトル
#define GAME_WIDTH  1280				//ゲーム画面の幅（ウィドス）
#define GAME_HEIGHT 720					//ゲーム画面の高さ（ハイト）
#define GAME_COLOR  32					//ゲームの色域

#define GAME_ICON_ID	333				//ゲームのICONのID

#define GAME_WINDOW_BAR 0				//ウィンドウバーの種類

// プログラムは WinMain から始まります
//windowsのプログラミング方法＝（WinAPI）で動いている！
//DxLibは、DirectXという、ゲームプログラミングを簡単に使える仕組み
int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//log.txtを出力しない
	ChangeWindowMode(TRUE);								//ウィンドウモードに設定
	SetMainWindowText("ゲームタイトル");				//ウィンドウのタイトルの文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度を設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);					//デフォルトの背景の色
	SetWindowIconID(GAME_ICON_ID);						//アイコンファイルを読み込み
	SetWindowStyleMode(GAME_WINDOW_BAR);				//ウィンドウバーの状態
	SetWaitVSyncFlag(TRUE);								//ディスプレイの垂直同期を有効にする
	SetAlwaysRunFlag(TRUE);								//ウィンドウをずっとアクティブにする

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//四角の位置を決める
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGHT / 2;

	//四角の大きさを決める
	int width = 32;		//幅
	int height = 32;	//高さ

	int radius = 40;

	//速度
	int speed = 1;
	int Xspeed = 1;
	int Yspeed = 1;

	SetDrawScreen(DX_SCREEN_BACK);

	//無限ループ
	while (1)
	{
		//何かしらキーが押されたとき
		if (CheckHitKeyAll() != 0)	//０のときに、何かキーが押された
		{
			break;	//無限ループを抜ける
		}

		//メッセージを受け取り続ける
		if (ProcessMessage() != 0)	//-1のとき、エラーやウィンドウが閉じられた
		{
			break;	//無限ループを抜ける
		}

		//画面を消去する
		if (ClearDrawScreen() != 0) { break; }

		//四角を描画
		DrawBox(
			X, Y, X + width, Y + height,
			GetColor(255, 0, 0),		//色を取得
			TRUE						//塗りつぶし？
		);

		//円を描画
		DrawCircle(
			X, Y, radius,
			GetColor(0, 255, 0),
				FALSE, 5
			);

		//四角を右に動かす
		X += Xspeed;
		Y += Yspeed;

		if (X < 0 || X + width > GAME_WIDTH)	//画面の横から出たとき
		{ 
			//+1なら-1にする　/ -1なら+1にする
			Xspeed = -Xspeed; //移動する向きを反転

			//壁に当たると速くなる
			if (Xspeed > 0) { Xspeed += 2; }
			else if (Xspeed < 0) { Xspeed -= 2; }
		}

		if (Y < 0 || Y + height > GAME_HEIGHT)	//画面の縦から出たとき
		{
			Yspeed = -Yspeed;

			//壁に当たると速くなる
			if (Yspeed > 0) { Yspeed += 2; }
			else if (Yspeed < 0) { Yspeed -= 2; }
		}


		ScreenFlip();		//ダブルバッファリングした画面を描画
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}