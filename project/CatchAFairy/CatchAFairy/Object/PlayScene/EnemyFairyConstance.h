//=============================================================================
//	EnemyFairyConstance.h
//
//	�G�d���萔�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __ENEMY_FAIRY_CONSTANCE_H__
#define __ENEMY_FAIRY_CONSTANCE_H__

#include "Ishikawa/Math/ITFloatN.h"

namespace CatchAFairy{
	namespace Object{
		namespace PlayScene{
			/// <summary>
			/// �G�d���萔�l
			/// </summary>
			class EnemyFairyConstance final {
			public:
				// �ō��ړ����x
				const float MAX_MOVE_SPEED;
				// �ō���]���x
				const float MAX_ROTATE_SPEED;
				// �ō���]����p�x�̃R�T�C���l
				const float MAX_SPEED_ROTATION_ANGLE_COS;
				// �{�̌��̊�_
				const ::Ishikawa::Math::Float3 FAIRY_LIGHT_ANCHOR;
				// �{�̌��̐F
				const ::Ishikawa::Math::Float4 FAIRY_LIGHT_COLOR;
				// �{�̌��̔��a
				const float FAIRY_LIGHT_RADIUS;
				// �{�̌��̌��x
				const float FAIRY_LIGHT_LUMEN;
				// �}�i���C�g�̐F
				const ::Ishikawa::Math::Float4 MANA_LIGHT_COLOR;
				// �}�i���C�g�̔��a
				const float MANA_LIGHT_RADIUS;
				// �}�i���C�g�̌��x
				const float MANA_LIGHT_LUMEN;
				// �}�i���C�g�����Ԋu
				const float MANA_LIGHT_GENERATION_INTERVAL;
				// �}�i���C�g����1�񓖂���̐�����
				const unsigned int MANA_LIGHT_GENERATION_ONCE_COUNT;

			public:
				// �R���X�g���N�^
				EnemyFairyConstance(
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
				);
				// �f�X�g���N�^
				~EnemyFairyConstance();
			private:
				/// <summary>������Z�q [�폜]</summary>
				EnemyFairyConstance& operator=(const EnemyFairyConstance&) = delete;
			};
		}
	}
}

#endif
