//=============================================================================
//	EnemyFairyController.h
//
//	�G�d���R���g���[���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __ENEMY_FAIRY_CONTROLLER_H__
#define __ENEMY_FAIRY_CONTROLLER_H__

#include "CatchAFairy/Object/UpdateObjectInterface.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "CatchAFairy/Data/EnemyFairyType.h"
#include "EnemyFairyConstance.h"

namespace CatchAFairy{
	namespace Object{
		namespace PlayScene{
			class EnemyFairies;
			class PointLight;
			class EnemyFairyConstance;

			/// <summary>
			/// �G�d���R���g���[��
			/// </summary>
			class EnemyFairyController final :
				public ::CatchAFairy::Object::UpdateInterface
			{
			private:
				/// <summary>
				/// �X�V���
				/// </summary>
				enum class UpdateState{
					/// <summary>�x�e</summary>
					Resting,
					/// <summary>�ړI�̃`�F�b�N�|�C���g�Ɉړ�����</summary>
					MoveToTargetCheckpoint
				};

			private:
				// �`�F�b�N�|�C���g�͈̔�
				static const float CHECKPOINT_RANGE;
				// �G�d���萔�z��
				static const EnemyFairyConstance ENEMY_FAIRY_CONSTANCE[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)];


			private:
				/// <summary>���</summary>
				CatchAFairy::Data::EnemyFairyType type;
				/// <summary>�萔�l</summary>
				const EnemyFairyConstance* CONSTANCE;
				/// <summary>�d�����ʗpID</summary>
				unsigned int id;
				/// <summary>���[���h�s��</summary>
				::Ishikawa::Math::Matrix* world;
				/// <summary>���[���h��Ԃł̖ړI�n���W</summary>
				::Ishikawa::Math::Float4 target;
				/// <summary>�G�d���B</summary>
				EnemyFairies* enemyFairies;
				/// <summary>�|�C���g���C�g</summary>
				PointLight* pointLight;
				/// <summary>�}�i���C�g�����p�^�C�}�[</summary>
				float manaLightGenerationTimer;
				/// <summary>���݂̍X�V�֐�</summary>
				void (EnemyFairyController::*currentUpdateFunction)(float);


			public:
				// �f�t�H���g�R���X�g���N�^
				EnemyFairyController();
				// �f�X�g���N�^
				virtual ~EnemyFairyController() override final;

			public:
				/// <summary>��ނ̎擾</summary>
				/// <returns>���</returns>
				CatchAFairy::Data::EnemyFairyType GetType() const { return this->type; }

			public:
				// ��ނ̐ݒ�
				void SetType(CatchAFairy::Data::EnemyFairyType _type);
				/// <summary>�d�����ʗpID�̐ݒ�</summary>
				/// <param name="_id">�d�����ʗpID</param>
				void SetId(unsigned int _id){ this->id = _id; }
				/// <summary>���[���h�s��̐ݒ�</summary>
				/// <param name="_world">���[���h�s��</param>
				void SetWorld(::Ishikawa::Math::Matrix* const _world){ this->world = _world; }
				/// <summary>�G�d���B�̐ݒ�</summary>
				/// <param name="_enemyFairies">�G�d���B</param>
				void SetEnemyFairies(EnemyFairies* const _enemyFairies){ this->enemyFairies = _enemyFairies; }
				//�@�|�C���g���C�g�̐ݒ�
				void SetPointLight(PointLight* const _pointLight);

			private:
				// �|�C���g���C�g�ݒ�̕ύX
				void ChangePointLightSetting();

			public:
				// ���Z�b�g
				void Reset();
				// �X�V
				virtual void Update(float _elapsedTime) override final;

			private:
				// �J�n�n�_�̑I��
				::Ishikawa::Math::Float4 SelectStartPoint();
				// �|�C���g���C�g�̍X�V
				void UpdatePointLight(float _elapsedTime);
				// �x�e����
				void UpdateOfRest(float _elapsedTime);
				// �ړI�n�Ɉړ�����
				void UpdateOfMoveToTargetCheckpoint(float _elapsedTime);
				// �ړI�n��ύX
				void ChangeTarget();
				// �v���C����T��
				bool SearchPlayer(::Ishikawa::Math::Matrix* const _playerWorld);
			};
		}
	}
}

#endif
