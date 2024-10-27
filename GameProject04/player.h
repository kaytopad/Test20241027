#pragma once
#include "dxlib.h"
#include "collision.h"

/// <summary>
/// �v���C���[������
/// </summary>
void playerInit();

/// <summary>
/// �v���C���[�X�V
/// </summary>
/// <param name="deltaTime">1�t���[�����̏�������</param>
void playerUpdate(float deltaTime);

/// <summary>
/// �v���C���[�`��
/// </summary>
void playerDraw();

/// <summary>
/// �v���C���[��n��
/// </summary>
void playerFinalize();

/// <summary>
/// �v���C���[�̓����蔻���`��Ԃ�
/// </summary>
/// <returns>�v���C���[�̓����蔻���`</returns>
sHitRect GetPlayerHitRect();

/// <summary>
/// �����蔻���`����v���C���[�ʒu���C������
/// </summary>
/// <param name="hitRect">�����蔻���`</param>
void playerfixColPosition(sHitRect& hitRect);

//�ǉ�---10��7��

/// <summary>
/// �n�ʂɗ����Ă��邩�H
/// </summary>
/// <param name="groundFlg">�����̓����蔻�茋��</param>
void playerSetGroundFlg(bool groundFlg);

/// <summary>
/// ����ǂɂԂ�����
/// </summary>
/// <param name="headHitFlg">���̓����蔻�茋��</param>
void playerSetHeadHitFlg(bool headHitFlg);

/// <summary>
/// �v���C���[�̑����R���C�_�[�̃Q�b�g
/// </summary>
/// <returns>�����R���C�_�[�̋�`���</returns>
sHitRect playerGetGroundCollider();

/// <summary>
/// �v���C���[�̓���R���C�_�[�̃Q�b�g
/// </summary>
/// <returns>����R���C�_�[�̋�`���</returns>
sHitRect playerGetHeadCollider();