#pragma once
#include <DxLib.h>
#include "collision.h"

/// <summary>
/// マップ初期化
/// </summary>
void mapInit();

/// <summary>
/// マップ描画
/// </summary>
void mapDraw();

/// <summary>
/// マップ後始末
/// </summary>
void mapFinalize();

/// <summary>
/// マップ衝突
/// </summary>
/// <param name ="checkRect">マップと当たっているか調査したい矩形</param>
/// <returns>マップと衝突したかをtrue/falseで返す</returns>
bool mapHitCalc(sHitRect& checkRect);

/// <summary>
/// マップのインデックス位置からブロック番号を返す
/// </summary>
/// <param name ="ix">マップのインデックス番号x</param>
/// <param name ="iy">マップのインデックス番号y</param>
/// <returns>マップのブロック番号を返す</returns>

unsigned char mapGetBlockType(int ix,int iy);