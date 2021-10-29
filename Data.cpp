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

	//CSVの1行目がヘッダの時
	if (IsHeader == TRUE)
	{
		//1文字ずつ取り出し、改行をヒントに1行目を読み飛ばす
		while (GetChar != '\n') { GetChar = fgetc(fp); }
	}

	//実際にデータを格納する
	while (IsEOF != EOF)	//End Of File (ファイルの最後)　ではないとき繰り返す
	{
		//CSVのデータの1行分を読み取る
		IsEOF = fscanf_s(
			fp,
			SCORE_DATA_FORMAT,	//データが「一行=レコード」で描かれているのがポイント
			&(score)->Name1, STR_MAX,	//文字列を読み込むときは、次の引数で文字列最大数を指定する
			&(score)->Score1,
			&(score)->Name2, STR_MAX,	//文字列を読み込むときは、次の引数で文字列最大数を指定する
			&(score)->Score2,
			&(score)->Name3, STR_MAX,	//文字列を読み込むときは、次の引数で文字列最大数を指定する
			&(score)->Score3
		);

	}

	fclose(fp);	//ファイルを閉じる

	return TRUE;
}

/// <summary>
/// スコアデータ作成
/// </summary>
/// <returns></returns>
BOOL CreateScoreData(VOID)
{
	//なかったらスコアデータを作る
	FILE* creFp;
	errno_t creFileErr = NULL;	//ファイル読み込みエラーを確認
	creFileErr = fopen_s(&creFp, GetScoreDataPath(), "w");	//ファイル書き込みモード(w)で開く(作成する)
	if (creFileErr != NULL)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//ヘッダを書き込み
	fprintf_s(creFp,
		"Name1,Score1,Name2,Score2,Name3,Score3\n"
	);

	//空のスコアデータを書き込み
	fprintf_s(creFp,
		"%s,%d,%s,%d,%s,%d",
		score_data.Name1, score_data.Score1,
		score_data.Name2, score_data.Score2,
		score_data.Name3, score_data.Score3
	);

	fclose(creFp);	//スコアデータを閉じる

	MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_SAVE_TITLE, MB_OK);
	return TRUE;
}

/// <summary>
/// スコアデータ保存
/// </summary>
/// <returns></returns>
BOOL SaveScoreData(VOID)
{
	//なかったらスコアデータを作る
	FILE* creFp;
	errno_t creFileErr = NULL;	//ファイル読み込みエラーを確認
	creFileErr = fopen_s(&creFp, GetScoreDataPath(), "w");	//ファイル書き込みモード(w)で開く(作成する)
	if (creFileErr != NULL)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//ヘッダを書き込み
	fprintf_s(creFp,
		"Name1,Score1,Name2,Score2,Name3,Score3\n"
	);

	//スコアデータを書き込み
	fprintf_s(creFp,
		"%s,%d,%s,%d,%s,%d",
		score_data.Name1, score_data.Score1,
		score_data.Name2, score_data.Score2,
		score_data.Name3, score_data.Score3
	);

	fclose(creFp);	//スコアデータを閉じる

	MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_SAVE_TITLE, MB_OK);
	return TRUE;
}

/// <summary>
/// スコアデータの削除
/// </summary>
/// <param name="path"></param>
BOOL DeleteScoreData(VOID)
{
	//ファイル削除
	if (remove(GetScoreDataPath()) != 0)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_DELETE_ERR_TITLE, MB_OK);
	}
	else
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_DELETE_TITLE, MB_OK);
	}

	//スコアデータを作成
	return CreateScoreData();
}

/// <summary>
/// スコアデータのパスを返す
/// </summary>
/// <param name=""></param>
/// <returns></returns>
const char* GetScoreDataPath(VOID)
{
	return score_data.path;
}

/// <summary>
/// スコアデータのパスを設定する
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
	//マップチップを読み込み
	{
		int IsLoad = -1;	//画像が読み込めたか？

		//一時的に画像のハンドルを用意する
		int TempHandle = LoadGraph(ImgPath);

		//読み込みエラー
		if (TempHandle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),	//ウィンドウハンドル
				ImgPath,				//本文
				"画像読み込みエラー",	//タイトル
				MB_OK					//ボタン
			);

			return FALSE;	//読み込み失敗
		}

		//画像の幅と高さを取得
		int Width = -1;		//幅
		int Height = -1;	//高さ
		GetGraphSize(TempHandle, &Width, &Height);

		//分割して読み込み
		IsLoad = LoadDivGraph(
			ImgPath,							//画像のパス
			divYoko * divTate,					//分割総数
			divYoko, divTate,					//横分割、縦分割
			Width / divYoko, Height / divTate,	//画像1つ分の幅、高さ
			map->handle							//連続で管理する配列の先頭アドレス
		);

		//分割エラー
		if (IsLoad == -1)
		{
			MessageBox(
				GetMainWindowHandle(),	//ウィンドウハンドル
				ImgPath,				//本文
				"画像分割エラー",	//タイトル
				MB_OK					//ボタン
			);

			return FALSE;	//読み込み失敗
		}

		//情報を設定
		map->DivMax = divYoko * divTate;
		GetGraphSize(map->handle[0], &map->width, &map->height);

		//画像を描画する
		map->IsDraw = TRUE;

		//1次的に読み込んだハンドルを解放
		DeleteGraph(TempHandle);
	}

	FILE* fp;				//ファイルポインタ
	char GetChar = '\0';	//取得する文字
	int LoopCnt = 0;		//ループカウンタ
	errno_t FileErr = NULL;	//ファイル読み込みエラーを確認する
	int IsEOF = NULL;		//ファイルの終わりかチェック

	//下CSVを読み取り
	{
		FileErr = fopen_s(&fp, ShitaPath, "r");	//ファイル読み込みモード（ｒ）で開く
		if (FileErr != NULL)
		{
			MessageBox(GetMainWindowHandle(), ShitaPath, CSV_LOAD_ERR_TITLE, MB_OK);	//読み込みエラー
			return FALSE;
		}

		//ココから正常に読み込めた時の処理

		//実際にデータを格納する
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File（ファイルの最後）ではないとき繰り返す
		{
			//ファイルから数値を1つ読み込み(%d,)、配列に格納する
			//[LoopCnt / MAP_YKO_MAX] [LoopCnt % MAP_YOKO_MAX]は、
			//例えば、[3][3]の配列の時、LoopCntが5であれば、
			//[5 / 3 = 1][5 % 3 = 2]となり、1次元配列の位置を2次元配列として変換できる
			IsEOF = fscanf_s(
				fp,
				CSV_MAP_FORMAT,	//データが[XX,]で描かれているのがポイント
				&map->CSV_shita[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//次のデータへ
			LoopCnt++;
		}

		fclose(fp);	//ファイルを閉じる
	}

	//中CSVを読み取り
	{
		FileErr = fopen_s(&fp, NakaPath, "r");	//ファイル読み込みモード（ｒ）で開く
		if (FileErr != NULL)
		{
			MessageBox(GetMainWindowHandle(), NakaPath, CSV_LOAD_ERR_TITLE, MB_OK);	//
			return FALSE;
		}

		//ココから正常に読み込めた時の処理

				//実際にデータを格納する
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File（ファイルの最後）ではないとき繰り返す
		{
			//ファイルから数値を1つ読み込み(%d,)、配列に格納する
			//[LoopCnt / MAP_YKO_MAX] [LoopCnt % MAP_YOKO_MAX]は、
			//例えば、[3][3]の配列の時、LoopCntが5であれば、
			//[5 / 3 = 1][5 % 3 = 2]となり、1次元配列の位置を2次元配列として変換できる
			IsEOF = fscanf_s(
				fp,
				CSV_MAP_FORMAT,	//データが[XX,]で描かれているのがポイント
				&map->CSV_naka[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//次のデータへ
			LoopCnt++;
		}

		fclose(fp);	//ファイルを閉じる
	}

	//中当たりCSVを読み取り
	{
		FileErr = fopen_s(&fp, NakaAtariPath, "r");	//ファイル読み込みモード（ｒ）で開く
		if (FileErr != NULL)
		{
			MessageBox(GetMainWindowHandle(), NakaAtariPath, CSV_LOAD_ERR_TITLE, MB_OK);	//
			return FALSE;
		}

		//ココから正常に読み込めた時の処理

				//実際にデータを格納する
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File（ファイルの最後）ではないとき繰り返す
		{
			//ファイルから数値を1つ読み込み(%d,)、配列に格納する
			//[LoopCnt / MAP_YKO_MAX] [LoopCnt % MAP_YOKO_MAX]は、
			//例えば、[3][3]の配列の時、LoopCntが5であれば、
			//[5 / 3 = 1][5 % 3 = 2]となり、1次元配列の位置を2次元配列として変換できる
			IsEOF = fscanf_s(
				fp,
				CSV_MAP_FORMAT,	//データが[XX,]で描かれているのがポイント
				&map->CSV_naka_atari[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//次のデータへ
			LoopCnt++;
		}

		fclose(fp);	//ファイルを閉じる
	}

	//上CSV
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