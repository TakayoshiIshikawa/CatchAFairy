//=============================================================================
//	PlayCamera.h
//
//	�v���C�J�����̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __PLAY_CAMERA_H__
#define __PLAY_CAMERA_H__

#include "CatchAFairy/Object/UpdateObjectInterface.h"
#include "CatchAFairy/Object/CameraObjectInterface.h"
#include "Ishikawa/Math/ITMatrix.h"
#include "Ishikawa/DirectX/Texture/ITTexture2D.h"
#include "CatchAFairy/Shader/User/PlayCameraEyeFirstShaderUser.h"
#include "CatchAFairy/Shader/User/PlayCameraEyeLastShaderUser.h"
#include "CatchAFairy/Shader/User/CameraEyeShadowShaderUser.h"
#include "CatchAFairy/Shader/User/CameraEyeReflectionLightShaderUser.h"
#include "CatchAFairy/Shader/User/CameraEyePointLightShaderUser.h"


namespace CatchAFairy{
	namespace Object{
		class RenderInterface;
		class RenderSameInterface;
		class PointLightInterface;

		namespace PlayScene{
			class DirectionalLight;


			/// <summary>
			/// �v���C�J����
			/// </summary>
			class PlayCamera final :
				public ::CatchAFairy::Object::UpdateInterface,
				public ::CatchAFairy::Object::CameraInterface
			{
			private:
				// �ŏI�C���f�b�N�X��
				static const unsigned int LAST_INDEX_COUNT;
				// �Ǐ]�䗦
				static const float FOLLOW_RATE;
				// ����p
				static const float VIEW_ANGLE;
				// �A�X�y�N�g��
				static const float ASPECT_RATE;
				// �j�A�N���b�v
				static const float NEAR_CLIP;
				// �t�@�[�N���b�v
				static const float FAR_CLIP;
				// �N���b�v����
				static const float CLIP_CENTER;

			private:
				/// <summary>�g�U���˃����_�[�^�[�Q�b�g�e�N�X�`��</summary>
				::Ishikawa::DirectX::Texture::Texture2D diffuseRenderTargetTexture;
				/// <summary>�@�������_�[�^�[�Q�b�g�e�N�X�`��</summary>
				::Ishikawa::DirectX::Texture::Texture2D normalRenderTargetTexture;
				/// <summary>���W�����_�[�^�[�Q�b�g�e�N�X�`��</summary>
				::Ishikawa::DirectX::Texture::Texture2D positionRenderTargetTexture;
				/// <summary>1�ԃf�v�X�X�e���V���e�N�X�`��</summary>
				::Ishikawa::DirectX::Texture::Texture2D firstDepthStencilTexture;
				/// <summary>1�ԃV�F�[�_���[�U</summary>
				::CatchAFairy::Shader::User::PlayCameraEyeFirst firstShaderUser;

				/// <summary>�o�̓^�[�Q�b�g�e�N�X�`��</summary>
				::Ishikawa::DirectX::Texture::Texture2D* outputTargetTexture;
				/// <summary>�ŏI�V�F�[�_���[�U</summary>
				::CatchAFairy::Shader::User::PlayCameraEyeLast lastShaderUser;
				/// <summary>�ŏI���_�o�b�t�@</summary>
				ID3D11Buffer* lastVertexBuffer;
				/// <summary>�ŏI�C���f�b�N�X�o�b�t�@</summary>
				ID3D11Buffer* lastIndexBuffer;

				/// <summary>�e�����_�[�^�[�Q�b�g�e�N�X�`��</summary>
				::Ishikawa::DirectX::Texture::Texture2D shadowRenderTargetTexture;
				/// <summary>�e�V�F�[�_���[�U</summary>
				::CatchAFairy::Shader::User::CameraEyeShadow shadowShaderUser;

				/// <summary>���ˌ������_�[�^�[�Q�b�g�e�N�X�`��</summary>
				::Ishikawa::DirectX::Texture::Texture2D reflectionLightRenderTargetTexture;
				/// <summary>���ˌ��f�v�X�X�e���V���e�N�X�`��</summary>
				::Ishikawa::DirectX::Texture::Texture2D reflectionLightDepthStencilTexture;
				/// <summary>���ˌ��V�F�[�_���[�U</summary>
				::CatchAFairy::Shader::User::CameraEyeReflectionLight reflectionLightShaderUser;

				/// <summary>�|�C���g���C�g�����_�[�^�[�Q�b�g�e�N�X�`��</summary>
				::Ishikawa::DirectX::Texture::Texture2D pointLightRenderTargetTexture;
				/// <summary>�|�C���g���C�g�V�F�[�_���[�U</summary>
				::CatchAFairy::Shader::User::CameraEyePointLight pointLightShaderUser;

				/// <summary>���[�J���s��</summary>
				::Ishikawa::Math::Matrix local;
				/// <summary>���[���h�s��</summary>
				::Ishikawa::Math::Matrix world;
				/// <summary>�r���[�s��</summary>
				::Ishikawa::Math::Matrix view;
				/// <summary>�ڕW�r���[�s��</summary>
				::Ishikawa::Math::Matrix targetView;
				/// <summary>�v���W�F�N�V�����s��</summary>
				::Ishikawa::Math::Matrix projection;
				/// <summary>���s���C�g</summary>
				DirectionalLight* directionalLight;


			public:
				// �f�t�H���g�R���X�g���N�^
				PlayCamera();
				// �f�X�g���N�^
				virtual ~PlayCamera() override final;

			public:
				/// <summary>���[���h�ϊ��s��̎擾</summary>
				/// <returns>���[���h�ϊ��s��</returns>
				virtual const ::Ishikawa::Math::Matrix& GetWorld() const override final{
					return this->world;
				}
				/// <summary>�r���[�ϊ��s��̎擾</summary>
				/// <returns>�r���[�ϊ��s��</returns>
				virtual const ::Ishikawa::Math::Matrix& GetView() const override final{
					return this->view;
				}
				/// <summary>�v���W�F�N�V�����ϊ��s��̎擾</summary>
				/// <returns>�v���W�F�N�V�����ϊ��s��</returns>
				virtual const ::Ishikawa::Math::Matrix& GetProjection() const override final{
					return this->projection;
				}
				/// <summary>�ŏI�V�F�[�_���\�[�X�r���[�̎擾</summary>
				/// <returns>�ŏI�V�F�[�_���\�[�X�r���[</returns>
				virtual ID3D11ShaderResourceView* GetLastShaderResourceView() const override final{
					return (this->outputTargetTexture == nullptr) ? (nullptr) :
						this->outputTargetTexture->GetShaderResourceView();
				}
				/// <summary>1�ԃV�F�[�_���[�U�̎擾</summary>
				/// <returns>1�ԃV�F�[�_���[�U</returns>
				::CatchAFairy::Shader::User::PlayCameraEyeFirst* GetFirstShaderUser(){
					return (&(this->firstShaderUser));
				}
				/// <summary>���ˌ��V�F�[�_���[�U�̎擾</summary>
				/// <returns>���ˌ��V�F�[�_���[�U</returns>
				::CatchAFairy::Shader::User::CameraEyeReflectionLight* GetReflectionLightShaderUser(){
					return (&(this->reflectionLightShaderUser));
				}

			public:
				// ���s���C�g�̐ݒ�
				void SetDirectionalLight(DirectionalLight* const _directionalLight);
				/// <summary>�o�̓^�[�Q�b�g�e�N�X�`���̐ݒ�</summary>
				/// <param name="_outputTargetTexture">�o�̓^�[�Q�b�g�e�N�X�`��</param>
				void SetOutputTargetTexture(::Ishikawa::DirectX::Texture::Texture2D* const _outputTargetTexture){
					this->outputTargetTexture = _outputTargetTexture;
				}

			public:
				// �Ǐ]�I�u�W�F�N�g�̈ړ�
				void OnMoveFollowingTheObject(const ::Ishikawa::Math::Matrix& _followMatrix);

			public:
				// �Q�[���̃f�o�C�X�Ɉˑ����郊�\�[�X�̒ǉ�
				virtual void AddITDeviceDependentResources(::Ishikawa::DirectX::Device::Device* const _itDevice) override final;
				// �f�o�C�X�ˑ����\�[�X�쐬
				virtual void CreateDeviceDependentResources() override final;
				// �f�o�C�X�ˑ����\�[�X�J��
				virtual void ReleaseDeviceDependentResources() override final;

				// ������
				void Initialize();
				// �X�V
				virtual void Update(float _elapsedTime) override final;

				// 1�ԃV�F�[�_�̕`��O����
				void DrawBeforeOfFirstShader();
				// 1�ԃV�F�[�_�ɃI�u�W�F�N�g��`��
				void RenderObjectToFirstShader(
					const ::CatchAFairy::Object::RenderInterface& _renderObject
				);
				// 1�ԃV�F�[�_�ɕ����I�u�W�F�N�g��`��
				void RenderSameObjectToFirstShader(
					const ::CatchAFairy::Object::RenderSameInterface& _renderObjects
				);
				// 1�ԃV�F�[�_�̕`��㏈��
				void DrawAfterOfFirstShader();
				// �e�V�F�[�_�`��
				void RenderOfShadowShader();
				// ���ˌ��V�F�[�_�̕`��O����
				void DrawBeforeOfReflectionLightShader();
				// ���ˌ��V�F�[�_�`��
				void RenderOfReflectionLightShader(const ::CatchAFairy::Object::PointLightInterface& _pointLightObject);
				// ���ˌ��V�F�[�_�̕`��㏈��
				void DrawAfterOfReflectionLightShader();
				// �|�C���g���C�g�V�F�[�_�̕`��O����
				void DrawBeforeOfPointLightShader();
				// �|�C���g���C�g�V�F�[�_�`��
				void RenderOfPointLightShader(const ::CatchAFairy::Object::PointLightInterface& _pointLightObject);
				// �|�C���g���C�g�V�F�[�_�̕`��㏈��
				void DrawAfterOfPointLightShader();
				// �ŏI�V�F�[�_�`��
				void RenderOfLastShader();

			private:
				// 1�ԃe�N�X�`���쐬
				void CreateFirstTextures();
				// �e�e�N�X�`���쐬
				void CreateShadowTextures();
				// ���ˌ��e�N�X�`���쐬
				void CreateReflectionLightTextures();
				// �|�C���g���C�g�e�N�X�`���쐬
				void CreatePointLightTextures();

				// �ŏI���_�o�b�t�@�쐬
				void CreateLastVertexBuffer();
				// �ŏI�C���f�b�N�X�o�b�t�@�쐬
				void CreateLastIndexBuffer();
				// �ŏI���_�o�b�t�@�J��
				void ReleaseLastVertexBuffer();
				// �ŏI�C���f�b�N�X�o�b�t�@�J��
				void ReleaseLastIndexBuffer();

				// 1�ԃ��\�[�X������
				void InitializeFirstResources();
				// �ŏI���\�[�X������
				void InitializeLastResources();
				// �e���\�[�X������
				void InitializeShadowResources();
				// ���ˌ����\�[�X������
				void InitializeReflectionLightResources();
				// �|�C���g���C�g���\�[�X������
				void InitializePointLightResources();
			};
		}
	}
}

#endif
