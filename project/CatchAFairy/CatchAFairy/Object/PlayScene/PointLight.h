//=============================================================================
//	PointLight.h
//
//	�|�C���g���C�g�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__

struct ID3D11Buffer;
#include "CatchAFairy/Object/UpdateObjectInterface.h"
#include "CatchAFairy/Object/PointLightObjectInterface.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "CatchAFairy/Shader/User/CameraEyeReflectionLightShaderUser.h"
#include "CatchAFairy/Object/PlayScene/ManaLight.h"
#include "CatchAFairy/Data/PointLightsSetting.h"

namespace CatchAFairy{
	namespace Object{
		namespace PlayScene{
			/// <summary>
			/// �|�C���g���C�g
			/// </summary>
			class PointLight final :
				public ::CatchAFairy::Object::UpdateInterface,
				public ::CatchAFairy::Object::PointLightInterface
			{
			private:
				/// <summary>�d��1�̖̂{�̂ɂ����C�g�̐�</summary>
				static const unsigned int ONE_FAIRY_BODY_LIGHT_COUNT = ::CatchAFairy::Data::PointLightsSetting::ONE_FAIRY_BODY_LIGHT_COUNT;
				/// <summary>�d��1�̂���o��}�i���C�g�̐�</summary>
				static const unsigned int ONE_FAIRY_MANA_LIGHT_COUNT = ::CatchAFairy::Data::PointLightsSetting::ONE_FAIRY_MANA_LIGHT_COUNT;
				/// <summary>�d��1�̂�����̃��C�g�̑���</summary>
				static const unsigned int ONE_FAIRY_LIGHT_ALL_COUNT = ::CatchAFairy::Data::PointLightsSetting::ONE_FAIRY_LIGHT_ALL_COUNT;
				/// <summary>�G�d���̐�</summary>
				static const unsigned int ENEMY_FAIRY_COUNT = ::CatchAFairy::Data::PointLightsSetting::ENEMY_FAIRY_COUNT;
				/// <summary>�d���̑���</summary>
				static const unsigned int FAIRY_COUNT = ::CatchAFairy::Data::PointLightsSetting::FAIRY_COUNT;

				/// <summary>���C�g�̍ő吔</summary>
				static const unsigned int MAX_COUNT = ::CatchAFairy::Data::PointLightsSetting::MAX_COUNT;

			private:
				/// <summary>���_�z��</summary>
				::CatchAFairy::Shader::User::CameraEyeReflectionLight::VSInput vertexArray[MAX_COUNT];
				/// <summary>���_�o�b�t�@</summary>
				ID3D11Buffer* vertexBuffer;
				/// <summary>�C���f�b�N�X�o�b�t�@</summary>
				ID3D11Buffer* indexBuffer;
				/// <summary>�C���f�b�N�X��</summary>
				unsigned int indexCount;
				/// <summary>���ˌ��V�F�[�_���[�U</summary>
				::CatchAFairy::Shader::User::CameraEyeReflectionLight* reflectionLightShaderUser;
				/// <summary>�}�i���C�g�F</summary>
				::Ishikawa::Math::Float4 manaLightColor[FAIRY_COUNT];
				/// <summary>�}�i���C�g���a�ƌ��x</summary>
				::Ishikawa::Math::Float2 manaLightRadiusAndLumen[FAIRY_COUNT];
				/// <summary>�}�i���C�g�����C���f�b�N�X</summary>
				unsigned int manaLightCreateIndex[FAIRY_COUNT];
				/// <summary>�}�i���C�g�z��</summary>
				ManaLight manaLightArray[FAIRY_COUNT][ONE_FAIRY_MANA_LIGHT_COUNT];


			public:
				// �f�t�H���g�R���X�g���N�^
				PointLight();
				// �f�X�g���N�^
				virtual ~PointLight() override final;

			public:
				/// <summary>���_�o�b�t�@�̎擾</summary>
				/// <returns>���_�o�b�t�@</returns>
				virtual ID3D11Buffer* GetVertexBuffer() const override final { return this->vertexBuffer; }
				/// <summary>�C���f�b�N�X�o�b�t�@�̎擾</summary>
				/// <returns>�C���f�b�N�X�o�b�t�@</returns>
				virtual ID3D11Buffer* GetIndexBuffer() const override final { return this->indexBuffer; }
				/// <summary>�C���f�b�N�X���̎擾</summary>
				/// <returns>�C���f�b�N�X��</returns>
				virtual unsigned int GetIndexCount() const override final { return this->indexCount; }

			public:
				/// <summary>���ˌ��V�F�[�_���[�U�̐ݒ�</summary>
				/// <param name="_reflectionLightShaderUser">���ˌ��V�F�[�_���[�U</param>
				virtual void SetReflectionLightShaderUser(
					::CatchAFairy::Shader::User::CameraEyeReflectionLight* const _reflectionLightShaderUser
				) override final {
					this->reflectionLightShaderUser = _reflectionLightShaderUser;
				}

				// �d���{�̂ɂ����C�g�̍��W�̐ݒ�
				void SetFairyBodyLightPosition(unsigned int _id, float _x, float _y, float _z);
				// �d���{�̂ɂ����C�g�̐F�̐ݒ�
				void SetFairyBodyLightColor(unsigned int _id, float _red, float _green, float _blue, float _alpha);
				// �d���{�̂ɂ����C�g�̔��a�̐ݒ�
				void SetFairyBodyLightRadius(unsigned int _id, float _radius);
				// �d���{�̂ɂ����C�g�̌��x�̐ݒ�
				void SetFairyBodyLightLumen(unsigned int _id, float _lumen);
				// �}�i���C�g�F�̐ݒ�
				void SetFairyManaLightColor(unsigned int _id, float _red, float _green, float _blue, float _alpha);
				// �}�i���C�g���a�̐ݒ�
				void SetFairyManaLightRadius(unsigned int _id, float _radius);
				// �}�i���C�g���x�̐ݒ�
				void SetFairyManaLightLumen(unsigned int _id, float _lumen);

			public:
				// ������
				void Initialize();
				// �X�V
				virtual void Update(float _elapsedTime) override final;

				// �}�i���C�g����
				void GenerateFairyManaLight(unsigned int _id, float _x, float _y, float _z);

				// �f�o�C�X�ˑ����\�[�X�쐬
				void CreateDeviceDependentResources();
				// �f�o�C�X�ˑ����\�[�X�J��
				void ReleaseDeviceDependentResources();

			private:
				// ���_�z��̏�����
				void InitializeVertexArray();

				// ���_�o�b�t�@�쐬
				void CreateVertexBuffer();
				// �C���f�b�N�X�o�b�t�@�쐬
				void CreateIndexBuffer();
				// ���_�o�b�t�@�J��
				void ReleaseVertexBuffer();
				// �C���f�b�N�X�o�b�t�@�J��
				void ReleaseIndexBuffer();
			};
		}
	}
}

#endif
