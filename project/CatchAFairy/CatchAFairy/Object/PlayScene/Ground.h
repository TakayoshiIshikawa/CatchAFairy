//=============================================================================
//	Ground.h
//
//	�n�ʂ̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __GROUND_H__
#define __GROUND_H__

#include "CatchAFairy/Object/RenderObjectInterface.h"
#include "Ishikawa/DirectX/Device/DependentResource/ITDeviceDependentResourceBase.h"
#include "Ishikawa/Math/ITMatrix.h"
#include "Ishikawa/DirectX/Texture/ITFileTexture.h"

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
			/// <summary>
			/// �n��
			/// </summary>
			class Ground final :
				public ::CatchAFairy::Object::RenderInterface,
				public ::Ishikawa::DirectX::Device::DependentResource::Base
			{
			private:
				// ���샂�f���f�[�^�t�@�C���p�X
				static const char* const IT_MODEL_DATA_FILEPATH;
				// �g�U���˃e�N�X�`���t�@�C���p�X
				static const wchar_t* const DIFFUSE_TEXTURE_FILEPATH;
				// �@���e�N�X�`���t�@�C���p�X
				static const wchar_t* const NORMAL_TEXTURE_FILEPATH;

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
				/// <summary>���[���h�s��</summary>
				::Ishikawa::Math::Matrix world;
				/// <summary>�g�U���˃e�N�X�`��</summary>
				::Ishikawa::DirectX::Texture::FileTexture diffuseTexture;
				/// <summary>�@���e�N�X�`��</summary>
				::Ishikawa::DirectX::Texture::FileTexture normalTexture;


			public:
				// �f�t�H���g�R���X�g���N�^
				Ground();
				// �f�X�g���N�^
				virtual ~Ground() override final;

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
				// ���[���h�ϊ��s��̎擾
				virtual const ::Ishikawa::Math::Matrix& GetWorld() const override final;

			public:
				// �f�o�C�X�ɓǂݍ��܂ꂽ
				virtual void OnDeviceLoaded() override final;
				// �f�o�C�X���������ꂽ
				virtual void OnDeviceRestored() override final;
				// �f�o�C�X���j�����ꂽ
				virtual void OnDeviceLost() override final;

			public:
				// ������
				void Initialize();
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
