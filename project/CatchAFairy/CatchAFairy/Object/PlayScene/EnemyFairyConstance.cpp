//=============================================================================
//	EnemyFairyConstance.cpp
//
//	�G�d���萔�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "EnemyFairyConstance.h"

using namespace ::CatchAFairy::Object::PlayScene;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="_MAX_MOVE_SPEED">�ō��ړ����x</param>
/// <param name="_MAX_ROTATE_SPEED">�ō���]���x</param>
/// <param name="_MAX_SPEED_ROTATION_ANGLE_COS">�ō���]����p�x�̃R�T�C���l</param>
/// <param name="_FAIRY_LIGHT_ANCHOR">�{�̌��̊�_</param>
/// <param name="_FAIRY_LIGHT_COLOR">�{�̌��̐F</param>
/// <param name="_FAIRY_LIGHT_RADIUS">�{�̌��̔��a</param>
/// <param name="_FAIRY_LIGHT_LUMEN">�{�̌��̌��x</param>
/// <param name="_MANA_LIGHT_COLOR">�}�i���C�g�̐F</param>
/// <param name="_MANA_LIGHT_RADIUS">�}�i���C�g�̔��a</param>
/// <param name="_MANA_LIGHT_LUMEN">�}�i���C�g�̌��x</param>
/// <param name="_MANA_LIGHT_GENERATION_INTERVAL">�}�i���C�g�����Ԋu</param>
/// <param name="_MANA_LIGHT_GENERATION_ONCE_COUNT">�}�i���C�g����1�񓖂���̐�����</param>
EnemyFairyConstance::EnemyFairyConstance(
	const float _MAX_MOVE_SPEED,
	const float _MAX_ROTATE_SPEED,
	const float _MAX_SPEED_ROTATION_ANGLE_COS,
	const ::Ishikawa::Math::Float3& _FAIRY_LIGHT_ANCHOR,
	const ::Ishikawa::Math::Float4& _FAIRY_LIGHT_COLOR,
	const float _FAIRY_LIGHT_RADIUS,
	const float _FAIRY_LIGHT_LUMEN,
	const ::Ishikawa::Math::Float4& _MANA_LIGHT_COLOR,
	const float _MANA_LIGHT_RADIUS,
	const float _MANA_LIGHT_LUMEN,
	const float _MANA_LIGHT_GENERATION_INTERVAL,
	const unsigned int _MANA_LIGHT_GENERATION_ONCE_COUNT
) :
	MAX_MOVE_SPEED(_MAX_MOVE_SPEED),
	MAX_ROTATE_SPEED(_MAX_ROTATE_SPEED),
	MAX_SPEED_ROTATION_ANGLE_COS(_MAX_SPEED_ROTATION_ANGLE_COS),
	FAIRY_LIGHT_ANCHOR(_FAIRY_LIGHT_ANCHOR),
	FAIRY_LIGHT_COLOR(_FAIRY_LIGHT_COLOR),
	FAIRY_LIGHT_RADIUS(_FAIRY_LIGHT_RADIUS),
	FAIRY_LIGHT_LUMEN(_FAIRY_LIGHT_LUMEN),
	MANA_LIGHT_COLOR(_MANA_LIGHT_COLOR),
	MANA_LIGHT_RADIUS(_MANA_LIGHT_RADIUS),
	MANA_LIGHT_LUMEN(_MANA_LIGHT_LUMEN),
	MANA_LIGHT_GENERATION_INTERVAL(_MANA_LIGHT_GENERATION_INTERVAL),
	MANA_LIGHT_GENERATION_ONCE_COUNT(_MANA_LIGHT_GENERATION_ONCE_COUNT)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyFairyConstance::~EnemyFairyConstance(){
	// �������Ȃ�
}
