#include "player.h"
#include "collision.h"

const int animPatternNum = 3;			// �A�j���[�V�����̃p�^�[����
const int animDown       = 0;			// ������
const int animLeft       = 1;			// ������
const int animRight      = 2;			// �E����
const int animUp         = 3;			// �����
const int animTypeNum    = 4;			// �A�j���[�V�����̎��
const float animationFPS = 6.0f;		// �b�� 6�R�}�̃A�j���[�V����
const float moveSpeed    = 200.0f;		// �ړ����x (�����Ŏw�肵���l���̃s�N�Z��/�b�i��) 
//�ǉ�---9��30��
const float maxMoveSpeed = 300.0f;		// �ő剡�ړ����x
const float moveAccell = 1.5f;			// �ړ������x
const float frictionRatio = 0.98f;		// ���C�W��
const float brakeRation = 0.95f;		// �i�s�����ɑ΂��ē��͋t�����ɓ��ꂽ�Ƃ��̃u���[�L�W��
const float inAirMoveAccelRatio = 0.8f;	// �󒆉��ړ�������
const float jumpInitalVelocity = 5.5;	// �W�����v�����x
const float jumoUpSpeed = 9.0f;			// �W�����v���������㏸���x
const float hitHeadBrakeRatio = -0.1f;	// ����Փˎ��̌�����


const int   imageSizeX   = 64;				// �摜���T�C�Y
const int   imageSizeY   = 96;				// �摜�c�T�C�Y
const int   hitSizeX     = 40;				// �����蔻��T�C�Y��
const int   hitSizeY     = 60;				// �����蔻��T�C�Y����
//�ǉ�---10��7��
const int colliderOffset = 10;				//�R���C�_�[���E���炵��
const float gravity = 15.0f;				//�d��
const float maxFallSpeed = 300.0f;			//�ő嗎�����x
const float jumpButtonAcceptTime = 0.95f;	//�W�����v��������t����


float px, py;                     // �v���C���[�̍��W
float vx, vy;                     // �v���C���[�̑��x
float animTimer;                  // �A�j���[�V�����^�C�}�[
int   animNowType;                // ���݂̃A�j�����
int   animNowPattern ;            // ���݂̃A�j���p�^�[��
int   animNowIndex;               // �A�j���[�V�����摜�z��̓Y���� 0�`11�܂ł̐��l
int   drawOffsetX;                // �����蔻�肩��̉��������炵��
int   drawOffsetY;                // �����蔻�肩��̏c�������炵��
//�ǉ�---10��7��
bool jumpFlg;					//�W�����v�t���O
bool onGround;					//�n�ʂɗ����Ă��邩
bool hitHead;					//�����Ԃ��������H
float jumpTimer;				//�W�����v���������ԃ^�C�}�[

int playerImg[animPatternNum * animTypeNum]; // �v���C���[�̉摜�n���h��
//�ǉ�---10��7��
sHitRect playerHit;				//�v���C���[�̓����蔻��
sHitRect playerFootCollider;	//�ڒn�R���C�_�[
sHitRect playerHeadCollider;	//����R���C�_�[

bool prevJumpButton;			//�O�̃t���[���ŃW�����v�{�^���������ꂽ���H
bool isJumpPush;				//���W�����v�{�^�����������ꂽ���H

bool inputMove(float deltaTime);

void playerInit()
{
	px = 100.0f;
	py = 100.0f;	//�ύX670��100
	vx = 0.0f;
	vy = 0.0f;
	animTimer = 0.0f;
	//�ǉ�---10��7��
	jumpFlg = false;

	animNowType = animDown;     // ���ʌ����A�j���[�V�������
	animNowPattern = 0;         // 
	
	animNowIndex = 0;

	// �����蔻�菉����
	initRect(playerHit, hitSizeX, hitSizeY);
	//�ǉ�---10��7��
	initRect(playerFootCollider,hitSizeX - colliderOffset,1);
	initRect(playerHeadCollider,hitSizeX - colliderOffset,1);

	// ��3 �c4 �v12���̉摜�t�@�C���̓ǂݍ���
	LoadDivGraph("img/chara.png", animPatternNum * animTypeNum, animPatternNum, animTypeNum, imageSizeX, imageSizeY, playerImg);

	// �`��ʒu�̃I�t�Z�b�g�l���v�Z
	// ���E���猩�ăZ���^�[�A�㉺�����͒�ӊ�ƂȂ�悤�Ɍv�Z
	drawOffsetX = (hitSizeX - imageSizeX) / 2;
	drawOffsetY = (hitSizeY - imageSizeY);

	//�ǉ�---10��7��
	onGround = false;
	hitHead = false;
	jumpTimer = jumpButtonAcceptTime;

	isJumpPush = false;
	prevJumpButton = false;
}

void playerUpdate(float deltaTime)
{
	bool isMove = inputMove(deltaTime);
	//�ǉ�---10��7��
	//�ڒn���Ă��邩�H
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
	//����́H�H
	if (hitHead)
	{
		vy = hitHeadBrakeRatio * vy;
	}



	// �����蔻��ʒu�X�V
	updateWorldRect(playerHit, px, py);
	updateWorldRect(playerFootCollider, px + colliderOffset / 2, py + playerHit.h);
	updateWorldRect(playerHeadCollider, px + colliderOffset / 2, py - playerHeadCollider.h);


	////////////////////////////
	// �A�j���[�V�����v�Z
	////////////////////////////
	if (isMove)
	{
		// animTimer �Ŏ��Ԍo�߂𑪂�A 1.0 / animationFPS �����̃A�j���[�V�����p�^�[���ɐi��ł悢�b����
		// �Ȃ����玟�̃R�}�ɐi�߂�
		animTimer += deltaTime;
		if (animTimer > 1.0f / animationFPS)
		{
			// �^�C�}�[��0�� ���̃p�^�[���ɐi�߂邪�AanimPetternNum�𒴂��Ȃ��悤�Ƀ��[�v������
			animTimer = 0.0f;
			animNowPattern++;
			animNowPattern %= animPatternNum; // �]��Ȃ̂� 0,1,2�̂����ꂩ�ɂȂ�
		}
	}
	else
	{
		animNowPattern = 1; // �Î~���͑������낦���p�^�[���ɋ�������
	}

	// �ŏI�I�ȓY�������v�Z 
	// �Y���� = ��̃p�^�[���̃R�}�� + �A�j���[�V������� * �P�̃A�j���[�V�����̃p�^�[������
	animNowIndex = animNowPattern + animNowType * animPatternNum;

}

void playerDraw()
{
	// �`��ʒu��
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
	// ���͏���
	///////////////////////////////////////////

	//�ǉ�---10��7��

	//�W�����v�{�^�����������u��
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		//���̑O�ɃW�����v�{�^����������Ă��邩�H
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
	//���Ɉړ�
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		if (!jumpFlg)
		{
			//�����]�������Ƃ�
			if (vx > 0.0f)
			{
				vx *= brakeRation;
			}

			//�n������x
			vx += -moveAccell * deltaTime;
		}
		else
		{
			//�󒆉����x
			vx += -moveAccell * inAirMoveAccelRatio *deltaTime;
		}

		//���x
		if (vx < -maxMoveSpeed)
		{
			vx = -maxMoveSpeed;
		}

		animNowType = animLeft;
		isMove = true;

	}

	//�E�Ɉړ�
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		if (!jumpFlg)
		{
			//�����]�������Ƃ�
			if (vx < 0.0f)
			{
				vx *= brakeRation;
			}

			//�n������x
			vx += +moveAccell * deltaTime;
		}
		else
		{
			//�󒆉����x
			vx += +moveAccell * inAirMoveAccelRatio * deltaTime;
		}

		//���x
		if (vx > maxMoveSpeed)
		{
			vx = +maxMoveSpeed;
		}

		animNowType = animRight;
		isMove = true;

	}

	//�ǉ�---10��7��
	//�W�����v�\�ŃW�����v�L�[�������ꂽ
	if (isJumpPush && !jumpFlg && jumpTimer > 0.0f)
	{
		vy -= jumpInitalVelocity;
		jumpFlg = true;
		onGround = false;
	}
	//�W�����v���������ŏ㏸�^�C�}�[���ԂȂ�㏸
	if (prevJumpButton && jumpTimer > 0.0f)
	{
		vy -= jumpInitalVelocity * deltaTime;
	}
	//�W�����v���Ȃ�d�͂𔭐�������
	if (jumpFlg)
	{
		jumpTimer -= deltaTime;
		vy += gravity * deltaTime;
	}
	//�L�[���͂Ȃ��Ƃ��n��ł̌���
	if (!isMove && !jumpFlg)
	{
		vx *= frictionRatio;
	}
	//�������x�}��
	if (vy * deltaTime > maxFallSpeed)
	{
		vy = maxFallSpeed;
	}


	//���͂��Ȃ��Ƃ���
	if (!isMove)
	{
		vx *= frictionRatio;
	}

	px += vx;
	py += vy;

	return isMove;
}

//�n�ʂɗ����Ă��邩�H
void playerSetGroundFlg(bool groundFlg)
{
	onGround = groundFlg;
}
//����ǂɂԂ������H
void playerSetHeadHitFlg(bool headHitFlg)
{
	hitHead = headHitFlg;
}
//�v���C���[�̑����R���C�_�[�̃Q�b�g
sHitRect playerGetGroundCollider()
{
	return playerFootCollider;
}
//�v���C���[�̓���R���C�_�[�̃Q�b�g
sHitRect playerGetHeadCollider()
{
	return playerHeadCollider;
}