#include <windows.h>
#include "DxLib.h"
#include "player.h"
#include "map.h"
#include "collision.h"

#pragma warning(disable:4996)

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// DxLib������
	ChangeWindowMode(TRUE);
	SetGraphMode(1440, 900, 16);

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	playerInit();
	mapInit();

	int nowCount, prevCount;
	nowCount = prevCount = GetNowCount();
	// �Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// deltaTime�v��
		float deltaTime;
		nowCount = GetNowCount();
		deltaTime = (nowCount - prevCount) / 1000.0f;
		
		//�X�V����
		playerUpdate(1/120.0f);

		// �v���C���[�̓����蔻���`
		sHitRect playerRect;
		playerRect = GetPlayerHitRect();

		//�ǉ�---10��7��

		//�����Ԃ������Ȃ瓖���蔻��{�b�N�X����v���C���[�ʒu���C��
		if (mapHitCalc(playerRect))
		{
			playerfixColPosition(playerRect);
		}
		//�����`�F�b�N
		playerRect = playerGetGroundCollider();
		playerSetGroundFlg(mapHitCalc(playerRect));

		//����`�F�b�N
		playerRect = playerGetHeadCollider();
		playerSetHeadHitFlg(mapHitCalc(playerRect));

		// �}�b�v�Ɠ������Ă��邩��hit�Ɋi�[
		bool hit;
		hit = mapHitCalc(playerRect);

		//��ʍX�V����
		ClearDrawScreen();

		// �}�b�v�`��
		mapDraw();

		//�v���C���[�`��
		playerDraw();

		// �q�b�g������?����ʕ\��
		if (hit)
		{
			//DrawString(100, 800, "��������", GetColor(255, 255, 255));
			playerfixColPosition(playerRect);
		}

		char buf[64];
		sprintf(buf, "deltaTime(%4.3f) FPS : %4.3f", deltaTime, 1.0f / deltaTime);
		DrawString(100, 800, buf, GetColor(255, 255, 255));

		ScreenFlip();

		prevCount = nowCount;
	}

	playerFinalize();
	mapFinalize();

	DxLib_End();
	return 0;
}

