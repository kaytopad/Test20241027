#include "map.h"
#include "collision.h"

//////////////////////////////////////////////
// 定数定義
//////////////////////////////////////////////
const int mapChipSize = 64;  // マップチップ１個の大きさ
const int mapXNum = 16;      // マップの横方向チップ数
const int mapYNum = 12;      // マップの縦方向チップ数
const int mapImgXNum = 2;    // マップチップ画像の横方向チップ数
const int mapImgYNum = 1;    // マップチップ画像の縦方向チップ数

// マップ
const unsigned char map[][mapXNum] =
{
   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
   {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
   {1,0,0,1,1,1,0,0,0,0,0,0,0,1,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

int mapChipImg[2]; // 画像ハンドル配列


/// <summary>
/// 壁情報取得 あるブロックの壁の有無を調べ BlockInfoを返す
/// </summary>
/// <param name="x">調査したいブロックの横インデックス</param>
/// <param name="y">調査したいブロックの縦インデックス</param>
/// <returns>壁の有無情報 BLOCK_LEFT～ BLOCK_DOWNまでのビット値が重ねあわされて返却される</returns>
BlockInfo mapGetBlockInfo(int ix, int iy)
{
	BlockInfo ret = 0;

	// ブロックの1つ左が空白か？
	if (mapGetBlockType(ix - 1, iy) == 0)
	{
		ret |= BLOCK_LEFT;
	}
	// ブロックの一つ右が空白か？
	if (mapGetBlockType(ix + 1, iy) == 0)
	{
		ret |= BLOCK_RIGHT;
	}
	// ブロックの一つ上が空白か？
	if (mapGetBlockType(ix, iy - 1) == 0)
	{
		ret |= BLOCK_UP;
	}
	//ブロックの一つ下が空白か？
	if (mapGetBlockType(ix, iy + 1) == 0)
	{
		ret |= BLOCK_DOWN;
	}

	// retには4方向の壁の有無のビットが重ね合わされている
	return ret;
}

void mapInit()
{
	// 画像総数  横枚数x縦枚数,横方向個数,縦方向個数,チップ1枚の横サイズ, チップ1毎の縦サイズ, 画像配列先頭ポインタ 
	LoadDivGraph("img/map.png", mapImgXNum * mapImgYNum, mapImgXNum, mapImgYNum, mapChipSize, mapChipSize, mapChipImg);
}

void mapDraw()
{
	for (int y = 0; y < mapYNum; y++)
	{
		for (int x = 0; x < mapXNum; x++)
		{
			int imgIndex = map[y][x];              // map配列よりブロック種類を取得
			int imgHandle = mapChipImg[imgIndex];  // indexをつかって画像ハンドル配列から画像ハンドルを取得

			// マップチップ幅でブロック画像を敷き詰めて描画する
			// xは0,1,2・・・と変化する。 x * mapChipSize の計算は 0,64,128,196, ... とブロック幅ごとに増える
			DrawGraph(x * mapChipSize, y * mapChipSize, imgHandle, TRUE);
		}
	}
}

void mapFinalize()
{
	for (int i = 0; i < mapImgXNum * mapImgYNum; i++)
	{
		DeleteGraph(mapChipImg[i]);
	}
}

bool mapHitCalc(sHitRect& checkRect)
{
	bool hitflg = false;
	sHitRect blockRect;

	blockRect.w = mapChipSize;
	blockRect.h = mapChipSize;

	// すべてのマップブロック vs 調査ブロックの衝突を調べる
	for (int iy = 0; iy < mapYNum; iy++)
	{
		//マップブロックのY座標
		blockRect.worldLY = static_cast<float>(iy * mapChipSize);
		blockRect.worldRY = static_cast<float>((iy + 1) * mapChipSize);

		for (int ix = 0; ix < mapXNum; ix++)
		{
			// 当たりブロックか 0→通れる 0以外→通れない 
			if (map[iy][ix] != 0)
			{
				// マップブロックのX座標
				blockRect.worldLX = static_cast<float>(ix * mapChipSize);
				blockRect.worldRX = (ix + 1) * static_cast<float>(mapChipSize);

				// 当たっているか？
				if (isHitRect(checkRect, blockRect))
				{
					// 一度でもブロックと当たったらhitflgをtrueに
					hitflg = true;

					// ブロック情報（上下左右壁の有無を調べる）
					BlockInfo bi = mapGetBlockInfo(ix, iy);

					// ボックスのめり込み量を計算する
					// checkRectにはめり込み戻し量が入る
					calcFixHitRectPosition(checkRect, blockRect, bi);

				}
			}
		}
	}

	return hitflg;
}

unsigned char mapGetBlockType(int ix, int iy)
{
	if (ix < 0 || iy < 0 || ix >= mapXNum || iy >= mapYNum)
	{
		return 0;
	}
	return map[iy][ix];
}
