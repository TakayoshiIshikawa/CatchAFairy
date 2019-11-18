//=============================================================================
//	SceneLogo.h
//
//	���S�V�[���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SCENE_LOGO_H__
#define __SCENE_LOGO_H__

struct ID3D11Buffer;

#include "SceneBase.h"
#include "Ishikawa/DirectX/Texture/ITTexture2D.h"
#include "Ishikawa/DirectX/Texture/ITFileTexture.h"
#include "CatchAFairy/Shader/User/LogoShaderUser.h"

namespace Ishikawa{
	namespace DirectX{
		namespace Device{
			class Device;
		}
	}
}


namespace CatchAFairy{
	namespace Scene{
		/// <summary>
		/// ���S�V�[��
		/// </summary>
		class Logo final : public Base{
		private:
			enum class UpdateFunctionType;


		private:
			// �o������
			static const float APPEARING_TIME;
			// �\������
			static const float PRINT_TIME;
			// �B������
			static const float HIDING_TIME;
			// �C���f�b�N�X��
			static const unsigned int INDEX_COUNT;


		private:
			/// <summary>�o�̓����_�[�^�[�Q�b�g�e�N�X�`��</summary>
			::Ishikawa::DirectX::Texture::Texture2D outputRenderTargetTexture;
			/// <summary>���S�V�F�[�_���[�U</summary>
			::CatchAFairy::Shader::User::Logo logoShaderUser;
			/// <summary>���S�摜</summary>
			::Ishikawa::DirectX::Texture::FileTexture logoTexture;
			/// <summary>�\���F[0.0:�� �` 1.0:��]</summary>
			float viewColor;
			/// <summary>���݂̍X�V�����̎��</summary>
			UpdateFunctionType currentUpdateType;
			/// <summary>�^�C�}�[</summary>
			float timer;
			/// <summary>���_�o�b�t�@</summary>
			ID3D11Buffer* vertexBuffer;
			/// <summary>�C���f�b�N�X�o�b�t�@</summary>
			ID3D11Buffer* indexBuffer;


		public:
			// �f�t�H���g�R���X�g���N�^
			Logo();
			// �f�X�g���N�^
			virtual ~Logo() override final;

		public:
			/// <summary>�e�N�X�`���T�C�Y�̎擾</summary>
			/// <param name="_width">���̊i�[��</param>
			/// <param name="_height">�����̊i�[��</param>
			virtual void GetTextureSize(unsigned int* const _width, unsigned int* const _height) const override final{
				this->outputRenderTargetTexture.GetTextureSize(_width, _height);
			}
			/// <summary>�����_�[�^�[�Q�b�g�r���[�̎擾</summary>
			/// <returns>�����_�[�^�[�Q�b�g�r���[</returns>
			virtual ID3D11RenderTargetView* GetRenderTargetView() const override final {
				return this->outputRenderTargetTexture.GetRenderTargetView();
			}
			/// <summary>�V�F�[�_���\�[�X�r���[�̎擾</summary>
			/// <returns>�V�F�[�_���\�[�X�r���[</returns>
			virtual ID3D11ShaderResourceView* GetShaderResourceView() const override final {
				return this->outputRenderTargetTexture.GetShaderResourceView();
			}

		private:
			// �Q�[���̈ˑ����Ă��鎩��f�o�C�X�̎擾
			::Ishikawa::DirectX::Device::Device* GetGameDependentITDevice() const;

		public:
			// �Q�[���̃f�o�C�X���ǂݍ��܂ꂽ
			virtual void OnLoadedGameDevice() override final;
			// �Q�[���̃f�o�C�X���������ꂽ
			virtual void OnRestoredGameDevice() override final;
			// �Q�[���̃f�o�C�X���j�����ꂽ
			virtual void OnLostGameDevice() override final;

		public:
			// �T�C�Y�ύX
			virtual void ChangeSceneSize(unsigned int _width, unsigned int _height) override final;

			// ������
			virtual void Initialize() override final;

			// �J�n
			virtual void Start() override final;
			// �X�V
			virtual void Update(float _elapsedTime) override final;
			// �`��
			virtual void Render() override final;
			// �I��
			virtual void End() override final;

		private:
			// �o�̓e�N�X�`���쐬
			void CreateOutputTextures();
			// ���_�o�b�t�@�쐬
			void CreateVertexBuffer();
			// �C���f�b�N�X�o�b�t�@�쐬
			void CreateIndexBuffer();
			// ���_�o�b�t�@�J��
			void ReleaseVertexBuffer();
			// �C���f�b�N�X�o�b�t�@�J��
			void ReleaseIndexBuffer();

			// ���S�o��
			void AppearLogo(float _elapsedTime);
			// ���S�\��
			void PrintLogo(float _elapsedTime);
			// ���S�B��
			void HideLogo(float _elapsedTime);
		};
	}
}

#endif
