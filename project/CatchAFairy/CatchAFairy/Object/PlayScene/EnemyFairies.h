//=============================================================================
//	EnemyFairies.h
//
//	�G�d���B�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __ENEMY_FAIRIES_H__
#define __ENEMY_FAIRIES_H__

#include "CatchAFairy/Object/UpdateObjectInterface.h"
#include "CatchAFairy/Object/RenderSameObjectInterface.h"
#include "Ishikawa/DirectX/Device/DependentResource/ITDeviceDependentResourceBase.h"
#include "Ishikawa/Math/ITMatrix.h"
#include "Ishikawa/DirectX/Texture/ITFileTexture.h"
#include "CatchAFairy/Data/PointLightsSetting.h"
#include "CatchAFairy/Data/EnemyFairyType.h"
#include "EnemyFairyController.h"

namespace Ishikawa{
	namespace Model{
		namespace ItObject{
			struct VertexData;
		}
	}
}

namespace CatchAFairy{
	namespace Shader{
		namespace User{
			class PlayCameraEyeFirst;
		}
	}

	namespace Object{
		namespace PlayScene{
			class PointLight;
			class Player;

			/// <summary>
			/// �G�d���B
			/// </summary>
			class EnemyFairies final :
				public ::Ishikawa::DirectX::Device::DependentResource::Base,
				public ::CatchAFairy::Object::UpdateInterface,
				public ::CatchAFairy::Object::RenderSameInterface
			{
			private:
				// ���샂�f���f�[�^�t�@�C���p�X
				static const char* const IT_MODEL_DATA_FILEPATH;
				// �g�U���˃e�N�X�`���t�@�C���p�X
				static const wchar_t* const DIFFUSE_TEXTURE_FILEPATH;
				// �@���e�N�X�`���t�@�C���p�X
				static const wchar_t* const NORMAL_TEXTURE_FILEPATH;
				// �G�d�������p�z��
				static const unsigned int ENEMY_FAIRY_GENERATING_COUNT[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)];
				/// <summary>�G�d���̐�</summary>
				static const unsigned int ENEMY_FAIRY_COUNT = ::CatchAFairy::Data::PointLightsSetting::ENEMY_FAIRY_COUNT;


			private:
				/// <summary>���p���Ă���V�F�[�_</summary>
				const ::CatchAFairy::Shader::User::PlayCameraEyeFirst* useShader;
				/// <summary>����I�u�W�F�N�g���_�z��</summary>
				::Ishikawa::Model::ItObject::VertexData* itObjectVertices;
				/// <summary>����I�u�W�F�N�g���_��</summary>
				unsigned int itObjectVertexCount;
				/// <summary>����I�u�W�F�N�g�C���f�b�N�X�z��</summary>
				unsigned long* itObjectIndices;
				/// <summary>����I�u�W�F�N�g�C���f�b�N�X��</summary>
				unsigned int itObjectIndexCount;
				/// <summary>�g�|���W�[�^�C�v</summary>
				::CatchAFairy::Shader::User::VertexTopology topology;
				/// <summary>���_�o�b�t�@</summary>
				ID3D11Buffer* vertexBuffer;
				/// <summary>�C���f�b�N�X�o�b�t�@</summary>
				ID3D11Buffer* indexBuffer;
				/// <summary>�C���f�b�N�X��</summary>
				unsigned int indexCount;
				/// <summary>�O�ډ~���S���W</summary>
				::Ishikawa::Math::Float3 circumscribedCircleCenterPosition;
				/// <summary>�O�ډ~���a���</summary>
				float squareCircumscribedCircleRadius;
				/// <summary>�g�U���˃e�N�X�`��</summary>
				::Ishikawa::DirectX::Texture::FileTexture diffuseTexture;
				/// <summary>�@���e�N�X�`��</summary>
				::Ishikawa::DirectX::Texture::FileTexture normalTexture;
				/// <summary>���[���h�s��z��</summary>
				::Ishikawa::Math::Matrix worldArray[ENEMY_FAIRY_COUNT];
				/// <summary>�R���g���[���z��</summary>
				EnemyFairyController controllerArray[ENEMY_FAIRY_COUNT];
				/// <summary>�v���C��</summary>
				const Player* player;
				/// <summary>�ߊl��</summary>
				unsigned int catchCount;


			public:
				// �f�t�H���g�R���X�g���N�^
				EnemyFairies();
				// �f�X�g���N�^
				virtual ~EnemyFairies() override final;

			public:
				// �g�|���W�[�^�C�v�̎擾
				virtual ::CatchAFairy::Shader::User::VertexTopology GetTopology() const override final;
				// ���_�o�b�t�@�̎擾
				virtual ID3D11Buffer* GetVertexBuffer() const override final;
				// �C���f�b�N�X�o�b�t�@�̎擾
				virtual ID3D11Buffer* GetIndexBuffer() const override final;
				// �C���f�b�N�X���̎擾
				virtual unsigned int GetIndexCount() const override final;
				// �O�ډ~���S���W�̎擾
				virtual const ::Ishikawa::Math::Float3& GetCircumscribedCircleCenterPosition() const override final;
				// �O�ډ~���a���̎擾
				virtual float GetSquareCircumscribedCircleRadius() const override final;
				// �g�U���˃V�F�[�_���\�[�X�r���[�̎擾
				virtual ID3D11ShaderResourceView* GetDiffuseShaderResourceView() const override final;
				// �@���V�F�[�_���\�[�X�r���[�̎擾
				virtual ID3D11ShaderResourceView* GetNormalShaderResourceView() const override final;
				// ���[���h�ϊ��s�񐔂̎擾
				virtual unsigned int GetWorldCount() const override final;
				// ���[���h�ϊ��s��z��̎擾
				virtual const ::Ishikawa::Math::Matrix* GetWorldArray() const override final;
				// �v���C���̎擾
				const Player* GetPlayer() const;
				// �w��d���̎�ނ̎擾
				const CatchAFairy::Data::EnemyFairyType GetFairyType(unsigned int _id) const;

			public:
				//�@�|�C���g���C�g�̐ݒ�
				void SetPointLight(PointLight* const _pointLight);
				// �v���C���̐ݒ�
				void SetPlayer(const Player* const _player);

			public:
				// �f�o�C�X�ɓǂݍ��܂ꂽ
				virtual void OnDeviceLoaded() override final;
				// �f�o�C�X���������ꂽ
				virtual void OnDeviceRestored() override final;
				// �f�o�C�X���j�����ꂽ
				virtual void OnDeviceLost() override final;
				// �ǂꂩ���߂܂���
				void OnCatched(unsigned int _id);

			public:
				// ������
				void Initialize();
				// �X�V
				virtual void Update(float _elapsedTime) override final;

				// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�쐬
				void CreateVertexBufferAndIndexBuffer(
					const ::CatchAFairy::Shader::User::PlayCameraEyeFirst* const _useShader
				);
				// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�J��
				void ReleaseVertexBufferAndIndexBuffer();

			private:
				// ����I�u�W�F�N�g�f�[�^�ǂݍ���
				void LoadItObjectData();
				// ����I�u�W�F�N�g�f�[�^�J��
				void ReleaseItObjectData();
			};
		}
	}
}

#endif
