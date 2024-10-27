#include "map.h"
#include "collision.h"

//////////////////////////////////////////////
// �萔��`
//////////////////////////////////////////////
const int mapChipSize = 64;  // �}�b�v�`�b�v�P�̑傫��
const int mapXNum = 16;      // �}�b�v�̉������`�b�v��
const int mapYNum = 12;      // �}�b�v�̏c�����`�b�v��
const int mapImgXNum = 2;    // �}�b�v�`�b�v�摜�̉������`�b�v��
const int mapImgYNum = 1;    // �}�b�v�`�b�v�摜�̏c�����`�b�v��

// �}�b�v
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

int mapChipImg[2]; // �摜�n���h���z��


/// <summary>
/// �Ǐ��擾 ����u���b�N�̕ǂ̗L���𒲂� BlockInfo��Ԃ�
/// </summary>
/// <param name="x">�����������u���b�N�̉��C���f�b�N�X</param>
/// <param name="y">�����������u���b�N�̏c�C���f�b�N�X</param>
/// <returns>�ǂ̗L����� BLOCK_LEFT�` BLOCK_DOWN�܂ł̃r�b�g�l���d�˂��킳��ĕԋp�����</returns>
BlockInfo mapGetBlockInfo(int ix, int iy)
{
	BlockInfo ret = 0;

	// �u���b�N��1�����󔒂��H
	if (mapGetBlockType(ix - 1, iy) == 0)
	{
		ret |= BLOCK_LEFT;
	}
	// �u���b�N�̈�E���󔒂��H
	if (mapGetBlockType(ix + 1, iy) == 0)
	{
		ret |= BLOCK_RIGHT;
	}
	// �u���b�N�̈�オ�󔒂��H
	if (mapGetBlockType(ix, iy - 1) == 0)
	{
		ret |= BLOCK_UP;
	}
	//�u���b�N�̈�����󔒂��H
	if (mapGetBlockType(ix, iy + 1) == 0)
	{
		ret |= BLOCK_DOWN;
	}

	// ret�ɂ�4�����̕ǂ̗L���̃r�b�g���d�ˍ��킳��Ă���
	return ret;
}

void mapInit()
{
	// �摜����  ������x�c����,��������,�c������,�`�b�v1���̉��T�C�Y, �`�b�v1���̏c�T�C�Y, �摜�z��擪�|�C���^ 
	LoadDivGraph("img/map.png", mapImgXNum * mapImgYNum, mapImgXNum, mapImgYNum, mapChipSize, mapChipSize, mapChipImg);
}

void mapDraw()
{
	for (int y = 0; y < mapYNum; y++)
	{
		for (int x = 0; x < mapXNum; x++)
		{
			int imgIndex = map[y][x];              // map�z����u���b�N��ނ��擾
			int imgHandle = mapChipImg[imgIndex];  // index�������ĉ摜�n���h���z�񂩂�摜�n���h�����擾

			// �}�b�v�`�b�v���Ńu���b�N�摜��~���l�߂ĕ`�悷��
			// x��0,1,2�E�E�E�ƕω�����B x * mapChipSize �̌v�Z�� 0,64,128,196, ... �ƃu���b�N�����Ƃɑ�����
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

	// ���ׂẴ}�b�v�u���b�N vs �����u���b�N�̏Փ˂𒲂ׂ�
	for (int iy = 0; iy < mapYNum; iy++)
	{
		//�}�b�v�u���b�N��Y���W
		blockRect.worldLY = static_cast<float>(iy * mapChipSize);
		blockRect.worldRY = static_cast<float>((iy + 1) * mapChipSize);

		for (int ix = 0; ix < mapXNum; ix++)
		{
			// ������u���b�N�� 0���ʂ�� 0�ȊO���ʂ�Ȃ� 
			if (map[iy][ix] != 0)
			{
				// �}�b�v�u���b�N��X���W
				blockRect.worldLX = static_cast<float>(ix * mapChipSize);
				blockRect.worldRX = (ix + 1) * static_cast<float>(mapChipSize);

				// �������Ă��邩�H
				if (isHitRect(checkRect, blockRect))
				{
					// ��x�ł��u���b�N�Ɠ���������hitflg��true��
					hitflg = true;

					// �u���b�N���i�㉺���E�ǂ̗L���𒲂ׂ�j
					BlockInfo bi = mapGetBlockInfo(ix, iy);

					// �{�b�N�X�̂߂荞�ݗʂ��v�Z����
					// checkRect�ɂ͂߂荞�ݖ߂��ʂ�����
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
