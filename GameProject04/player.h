#pragma once
#include "dxlib.h"
#include "collision.h"

/// <summary>
/// プレイヤー初期化
/// </summary>
void playerInit();

/// <summary>
/// プレイヤー更新
/// </summary>
/// <param name="deltaTime">1フレーム分の処理時間</param>
void playerUpdate(float deltaTime);

/// <summary>
/// プレイヤー描画
/// </summary>
void playerDraw();

/// <summary>
/// プレイヤー後始末
/// </summary>
void playerFinalize();

/// <summary>
/// プレイヤーの当たり判定矩形を返す
/// </summary>
/// <returns>プレイヤーの当たり判定矩形</returns>
sHitRect GetPlayerHitRect();

/// <summary>
/// 当たり判定矩形からプレイヤー位置を修正する
/// </summary>
/// <param name="hitRect">当たり判定矩形</param>
void playerfixColPosition(sHitRect& hitRect);

//追加---10月7日

/// <summary>
/// 地面に立っているか？
/// </summary>
/// <param name="groundFlg">足元の当たり判定結果</param>
void playerSetGroundFlg(bool groundFlg);

/// <summary>
/// 頭を壁にぶつけたか
/// </summary>
/// <param name="headHitFlg">頭の当たり判定結果</param>
void playerSetHeadHitFlg(bool headHitFlg);

/// <summary>
/// プレイヤーの足元コライダーのゲット
/// </summary>
/// <returns>足元コライダーの矩形情報</returns>
sHitRect playerGetGroundCollider();

/// <summary>
/// プレイヤーの頭上コライダーのゲット
/// </summary>
/// <returns>頭上コライダーの矩形情報</returns>
sHitRect playerGetHeadCollider();