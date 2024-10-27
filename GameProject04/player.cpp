#include "player.h"
#include "collision.h"

const int animPatternNum = 3;			// アニメーションのパターン数
const int animDown       = 0;			// 下方向
const int animLeft       = 1;			// 左方向
const int animRight      = 2;			// 右方向
const int animUp         = 3;			// 上方向
const int animTypeNum    = 4;			// アニメーションの種類
const float animationFPS = 6.0f;		// 秒間 6コマのアニメーション
const float moveSpeed    = 200.0f;		// 移動速度 (ここで指定した値分のピクセル/秒進む) 
//追加---9月30日
const float maxMoveSpeed = 300.0f;		// 最大横移動速度
const float moveAccell = 1.5f;			// 移動加速度
const float frictionRatio = 0.98f;		// 摩擦係数
const float brakeRation = 0.95f;		// 進行方向に対して入力逆方向に入れたときのブレーキ係数
const float inAirMoveAccelRatio = 0.8f;	// 空中横移動加速率
const float jumpInitalVelocity = 5.5;	// ジャンプ初速度
const float jumoUpSpeed = 9.0f;			// ジャンプ長押し中上昇速度
const float hitHeadBrakeRatio = -0.1f;	// 頭上衝突時の減速率


const int   imageSizeX   = 64;				// 画像横サイズ
const int   imageSizeY   = 96;				// 画像縦サイズ
const int   hitSizeX     = 40;				// 当たり判定サイズ幅
const int   hitSizeY     = 60;				// 当たり判定サイズ高さ
//追加---10月7日
const int colliderOffset = 10;				//コライダー左右ずらし量
const float gravity = 15.0f;				//重力
const float maxFallSpeed = 300.0f;			//最大落下速度
const float jumpButtonAcceptTime = 0.95f;	//ジャンプ長押し受付時間


float px, py;                     // プレイヤーの座標
float vx, vy;                     // プレイヤーの速度
float animTimer;                  // アニメーションタイマー
int   animNowType;                // 現在のアニメ種類
int   animNowPattern ;            // 現在のアニメパターン
int   animNowIndex;               // アニメーション画像配列の添え字 0～11までの数値
int   drawOffsetX;                // 当たり判定からの横方向ずらし量
int   drawOffsetY;                // 当たり判定からの縦方向ずらし量
//追加---10月7日
bool jumpFlg;					//ジャンプフラグ
bool onGround;					//地面に立っているか
bool hitHead;					//頭がぶつかったか？
float jumpTimer;				//ジャンプ長押し時間タイマー

int playerImg[animPatternNum * animTypeNum]; // プレイヤーの画像ハンドル
//追加---10月7日
sHitRect playerHit;				//プレイヤーの当たり判定
sHitRect playerFootCollider;	//接地コライダー
sHitRect playerHeadCollider;	//頭上コライダー

bool prevJumpButton;			//前のフレームでジャンプボタンが押されたか？
bool isJumpPush;				//今ジャンプボタンが押下されたか？

bool inputMove(float deltaTime);

void playerInit()
{
	px = 100.0f;
	py = 100.0f;	//変更670→100
	vx = 0.0f;
	vy = 0.0f;
	animTimer = 0.0f;
	//追加---10月7日
	jumpFlg = false;

	animNowType = animDown;     // 正面向きアニメーション種類
	animNowPattern = 0;         // 
	
	animNowIndex = 0;

	// 当たり判定初期化
	initRect(playerHit, hitSizeX, hitSizeY);
	//追加---10月7日
	initRect(playerFootCollider,hitSizeX - colliderOffset,1);
	initRect(playerHeadCollider,hitSizeX - colliderOffset,1);

	// 横3 縦4 計12枚の画像ファイルの読み込み
	LoadDivGraph("img/chara.png", animPatternNum * animTypeNum, animPatternNum, animTypeNum, imageSizeX, imageSizeY, playerImg);

	// 描画位置のオフセット値を計算
	// 左右から見てセンター、上下方向は底辺基準となるように計算
	drawOffsetX = (hitSizeX - imageSizeX) / 2;
	drawOffsetY = (hitSizeY - imageSizeY);

	//追加---10月7日
	onGround = false;
	hitHead = false;
	jumpTimer = jumpButtonAcceptTime;

	isJumpPush = false;
	prevJumpButton = false;
}

void playerUpdate(float deltaTime)
{
	bool isMove = inputMove(deltaTime);
	//追加---10月7日
	//接地しているか？
	if (onGround)
	{
		jumpFlg = false; 
		vy = 0.0f;
		jumpTimer = jumpButtonAcceptTime;
	}
	else
	{
		jumpFlg = true;
	}
	//頭上は？？
	if (hitHead)
	{
		vy = hitHeadBrakeRatio * vy;
	}



	// 当たり判定位置更新
	updateWorldRect(playerHit, px, py);
	updateWorldRect(playerFootCollider, px + colliderOffset / 2, py + playerHit.h);
	updateWorldRect(playerHeadCollider, px + colliderOffset / 2, py - playerHeadCollider.h);


	////////////////////////////
	// アニメーション計算
	////////////////////////////
	if (isMove)
	{
		// animTimer で時間経過を測り、 1.0 / animationFPS →次のアニメーションパターンに進んでよい秒数に
		// なったら次のコマに進める
		animTimer += deltaTime;
		if (animTimer > 1.0f / animationFPS)
		{
			// タイマーを0に 次のパターンに進めるが、animPetternNumを超えないようにループさせる
			animTimer = 0.0f;
			animNowPattern++;
			animNowPattern %= animPatternNum; // 余りなので 0,1,2のいずれかになる
		}
	}
	else
	{
		animNowPattern = 1; // 静止時は足をそろえたパターンに強制する
	}

	// 最終的な添え字を計算 
	// 添え字 = 一つのパターンのコマ数 + アニメーション種類 * １つのアニメーションのパターン枚数
	animNowIndex = animNowPattern + animNowType * animPatternNum;

}

void playerDraw()
{
	// 描画位置は
	DrawGraph(static_cast<int>(px) + drawOffsetX, static_cast<int>(py) + drawOffsetY, playerImg[animNowIndex], TRUE);
	drawRect(playerHit);
}


void playerFinalize()
{
	if (playerImg[0] != -1)
	{
		for (int i = 0; i < animPatternNum * animTypeNum; i++)
		{
			DeleteGraph(playerImg[i]);
		}
	}
}

sHitRect GetPlayerHitRect()
{
	return playerHit;
}


void playerfixColPosition(sHitRect& hitRect)
{
	px = hitRect.worldLX;
	py = hitRect.worldLY;

	playerHit = hitRect;
}

bool inputMove(float deltaTime)
{
	///////////////////////////////////////////
	// 入力処理
	///////////////////////////////////////////

	//追加---10月7日

	//ジャンプボタンを押した瞬間
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		//その前にジャンプボタンが押されているか？
		if (!prevJumpButton)
		{
			isJumpPush = true;
		}
		else
		{
			isJumpPush = false;
		}
		prevJumpButton = true;
	}
	else
	{
		prevJumpButton = false;
	}


	bool isMove = false;
	//左に移動
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		if (!jumpFlg)
		{
			//方向転換したとき
			if (vx > 0.0f)
			{
				vx *= brakeRation;
			}

			//地上加速度
			vx += -moveAccell * deltaTime;
		}
		else
		{
			//空中加速度
			vx += -moveAccell * inAirMoveAccelRatio *deltaTime;
		}

		//速度
		if (vx < -maxMoveSpeed)
		{
			vx = -maxMoveSpeed;
		}

		animNowType = animLeft;
		isMove = true;

	}

	//右に移動
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		if (!jumpFlg)
		{
			//方向転換したとき
			if (vx < 0.0f)
			{
				vx *= brakeRation;
			}

			//地上加速度
			vx += +moveAccell * deltaTime;
		}
		else
		{
			//空中加速度
			vx += +moveAccell * inAirMoveAccelRatio * deltaTime;
		}

		//速度
		if (vx > maxMoveSpeed)
		{
			vx = +maxMoveSpeed;
		}

		animNowType = animRight;
		isMove = true;

	}

	//追加---10月7日
	//ジャンプ可能でジャンプキーが押された
	if (isJumpPush && !jumpFlg && jumpTimer > 0.0f)
	{
		vy -= jumpInitalVelocity;
		jumpFlg = true;
		onGround = false;
	}
	//ジャンプ長押し中で上昇タイマー期間なら上昇
	if (prevJumpButton && jumpTimer > 0.0f)
	{
		vy -= jumpInitalVelocity * deltaTime;
	}
	//ジャンプ中なら重力を発生させる
	if (jumpFlg)
	{
		jumpTimer -= deltaTime;
		vy += gravity * deltaTime;
	}
	//キー入力ないとき地上での減速
	if (!isMove && !jumpFlg)
	{
		vx *= frictionRatio;
	}
	//落下速度抑制
	if (vy * deltaTime > maxFallSpeed)
	{
		vy = maxFallSpeed;
	}


	//入力がないときは
	if (!isMove)
	{
		vx *= frictionRatio;
	}

	px += vx;
	py += vy;

	return isMove;
}

//地面に立っているか？
void playerSetGroundFlg(bool groundFlg)
{
	onGround = groundFlg;
}
//頭を壁にぶつけたか？
void playerSetHeadHitFlg(bool headHitFlg)
{
	hitHead = headHitFlg;
}
//プレイヤーの足元コライダーのゲット
sHitRect playerGetGroundCollider()
{
	return playerFootCollider;
}
//プレイヤーの頭上コライダーのゲット
sHitRect playerGetHeadCollider()
{
	return playerHeadCollider;
}