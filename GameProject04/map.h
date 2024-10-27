#pragma once
#include <DxLib.h>
#include "collision.h"

/// <summary>
/// �}�b�v������
/// </summary>
void mapInit();

/// <summary>
/// �}�b�v�`��
/// </summary>
void mapDraw();

/// <summary>
/// �}�b�v��n��
/// </summary>
void mapFinalize();

/// <summary>
/// �}�b�v�Փ�
/// </summary>
/// <param name ="checkRect">�}�b�v�Ɠ������Ă��邩������������`</param>
/// <returns>�}�b�v�ƏՓ˂�������true/false�ŕԂ�</returns>
bool mapHitCalc(sHitRect& checkRect);

/// <summary>
/// �}�b�v�̃C���f�b�N�X�ʒu����u���b�N�ԍ���Ԃ�
/// </summary>
/// <param name ="ix">�}�b�v�̃C���f�b�N�X�ԍ�x</param>
/// <param name ="iy">�}�b�v�̃C���f�b�N�X�ԍ�y</param>
/// <returns>�}�b�v�̃u���b�N�ԍ���Ԃ�</returns>

unsigned char mapGetBlockType(int ix,int iy);