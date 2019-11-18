//=============================================================================
//	Trees.h
//
//	�؁X�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __TREES_H__
#define __TREES_H__

#include "CatchAFairy/Object/UpdateObjectInterface.h"
#include "CatchAFairy/Object/RenderSameObjectInterface.h"
#include "Ishikawa/DirectX/Device/DependentResource/ITDeviceDependentResourceBase.h"
#include "Ishikawa/Math/ITMatrix.h"
#include "Ishikawa/DirectX/Texture/ITFileTexture.h"
#include "CatchAFairy/Data/TreesOffset.h"

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
			/// �؁X[�ړ��͈͓��Ƃ��̋ߕ�]
			/// </summary>
			class Trees final :
				public ::CatchAFairy::Object::UpdateInterface,
				public ::CatchAFairy::Object::RenderSameInterface,
				public ::Ishikawa::DirectX::Device::DependentResource::Base
			{
			private:
				/// <summary>���[���h�ϊ��s�񏉊����p�f�[�^</summary>
				struct WorldOffset{
				public:
					/// <summary>X���W</summary>
					float positionX;
					/// <summary>Y���W</summary>
					float positionY;
					/// <summary>Z���W</summary>
					float positionZ;
					/// <summary>Y����]��</summary>
					float angle;
					/// <summary>���g�嗦</summary>
					float scaleOfWidth;
					/// <summary>�����g�嗦</summary>
					float scaleOfHeight;

				public:
					// �s��ɕϊ�
					void ToMatrix(::Ishikawa::Math::Matrix* const _matrix) const;
				};

			private:
				// ���샂�f���f�[�^�t�@�C���p�X
				static const char* const IT_MODEL_DATA_FILEPATH;
				// �g�U���˃e�N�X�`���t�@�C���p�X
				static const wchar_t* const DIFFUSE_TEXTURE_FILEPATH;
				// �@���e�N�X�`���t�@�C���p�X
				static const wchar_t* const NORMAL_TEXTURE_FILEPATH;
				// �I�t�Z�b�g�f�[�^�t�@�C���p�X
				static const char* const OFFSET_DATA_FILEPATH;
				// ���莲����
				static const float COLLISION_AXIS_HEIGHT;
				// ���蔼�a
				static const float COLLISION_RADIUS;
				// �ő�U��
				static const ::Ishikawa::Math::Float3 MAX_AMPLITUDE;
				// �ŏ��h�ꑬ�x
				static const float MIN_SWING_SPEED;
				// �ő�h�ꑬ�x
				static const float MAX_SWING_SPEED;

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
				/// <summary>�U���z��</summary>
				::Ishikawa::Math::Float3* amplitudeArray;
				/// <summary>�h�ꑬ�x�z��</summary>
				::Ishikawa::Math::Float3* swingSpeedArray;
				/// <summary>�h�ꎞ�Ԕz��</summary>
				::Ishikawa::Math::Float3* swingTimeArray;
				/// <summary>�I�t�Z�b�g�f�[�^</summary>
				::CatchAFairy::Data::TreesOffset treesOffset;
				/// <summary>���[���h�ϊ��s��z��</summary>
				::Ishikawa::Math::Matrix* worldArray;
				/// <summary>�g�U���˃e�N�X�`��</summary>
				::Ishikawa::DirectX::Texture::FileTexture diffuseTexture;
				/// <summary>�@���e�N�X�`��</summary>
				::Ishikawa::DirectX::Texture::FileTexture normalTexture;


			public:
				// �f�t�H���g�R���X�g���N�^
				Trees();
				// �f�X�g���N�^
				virtual ~Trees() override final;

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
				// �X�V
				virtual void Update(float _elapsedTime) override final;
				// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�쐬
				void CreateVertexBufferAndIndexBuffer(
					const ::CatchAFairy::Shader::User::PlayCameraEyeFirst* const _useShader
				);
				// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�J��
				void ReleaseVertexBufferAndIndexBuffer();

				// �x�N�g���Ƃ̔���
				::Ishikawa::Math::Float3 CollideVector(
					const ::Ishikawa::Math::Float3& _startPosition,
					const ::Ishikawa::Math::Float3& _vector,
					const ::Ishikawa::Math::Matrix& _matrixVectorToWorld,
					float _reflectionRate
				) const;

			private:
				// ����I�u�W�F�N�g�f�[�^�ǂݍ���
				void LoadItObjectData();
				// ����I�u�W�F�N�g�f�[�^�J��
				void ReleaseItObjectData();

				// ���[���h�ϊ��s��z��̏�����
				void InitializeWorldArray();
				// ���[���h�ϊ��s��z��̍X�V
				void UpdateWorldArray(float _elapsedTime);
				// ���[���h�ϊ��s��z��̏I��
				void FinalizeWorldArray();
			};
		}
	}
}

#endif
