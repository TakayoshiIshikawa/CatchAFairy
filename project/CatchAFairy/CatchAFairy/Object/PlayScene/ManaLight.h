//=============================================================================
//	ManaLight.h
//
//	�}�i���C�g�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __MANA_LIGHT_H__
#define __MANA_LIGHT_H__

#include "CatchAFairy/Object/UpdateObjectInterface.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "CatchAFairy/Shader/User/CameraEyeReflectionLightShaderUser.h"

namespace CatchAFairy{
	namespace Object{
		namespace PlayScene{
			/// <summary>
			/// �}�i���C�g
			/// </summary>
			class ManaLight final : public ::CatchAFairy::Object::UpdateInterface{
			private:
				// �����x
				static const ::Ishikawa::Math::Float3 ACCELERATION;
				// ���E�����̌����W��
				static const float DECELERATION_FACTOR_XZ;
				// �e�����̍ő叉��������
				static const ::Ishikawa::Math::Float3 MAX_INITIALIZE_SPEED;

			private:
				/// <summary>�Ή����郉�C�g���_</summary>
				::CatchAFairy::Shader::User::CameraEyeReflectionLight::VSInput* lightVertex;
				/// <summary>���x</summary>
				::Ishikawa::Math::Float3 velocity;


			public:
				// �f�t�H���g�R���X�g���N�^
				ManaLight();
				// �f�X�g���N�^
				virtual ~ManaLight() override final;

			public:
				/// <summary>�Ή����郉�C�g���_�̐ݒ�</summary>
				/// <param name="_lightVertex">�Ή����郉�C�g���_</param>
				void ManaLight::SetLightVertex(
					::CatchAFairy::Shader::User::CameraEyeReflectionLight::VSInput* const _lightVertex
				){
					this->lightVertex = _lightVertex;
				}

			public:
				// ����
				void Generate(
					const ::Ishikawa::Math::Float3& _position,
					const ::Ishikawa::Math::Float4& _color,
					const ::Ishikawa::Math::Float2& _radiusAndLumen
				);
				// �X�V
				virtual void Update(float _elapsedTime) override final;

			private:
				// [-1.0f,1.0f)�̗����l���擾
				static inline float Random();
				// ���x�������_�}�C�Y
				void RandomizeVelocity();
			};
		}
	}
}

#endif
