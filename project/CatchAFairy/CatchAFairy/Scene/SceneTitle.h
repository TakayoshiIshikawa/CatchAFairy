//=============================================================================
//	SceneTitle.h
//
//	�^�C�g���V�[���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

#include "SceneBase.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "Ishikawa/DirectX/Texture/ITTexture2D.h"
#include "Ishikawa/DirectX/Texture/ITFileTexture.h"
#include "CatchAFairy/Shader/User/TitleOutputShaderUser.h"

namespace Ishikawa{
	namespace Input{
		class Keyboard;
	}
	namespace DirectX{
		namespace Device{
			class Device;
		}
	}
}

namespace CatchAFairy{
	class Game;


	namespace Scene{
		/// <summary>
		/// �^�C�g���V�[��
		/// </summary>
		class Title final : public Base{
		private:
			// �w�i�C���f�b�N�X��
			static const unsigned int BACKGROUND_INDEX_COUNT;
			// �{�^���C���f�b�N�X��
			static const unsigned int BUTTON_INDEX_COUNT;


		private:
			/// <summary>�o�̓����_�[�^�[�Q�b�g�e�N�X�`��</summary>
			::Ishikawa::DirectX::Texture::Texture2D outputRenderTargetTexture;
			/// <summary>�^�C�g���o�̓V�F�[�_���[�U</summary>
			::CatchAFairy::Shader::User::TitleOutput titleOutputShaderUser;
			/// <summary>�w�i�摜</summary>
			::Ishikawa::DirectX::Texture::FileTexture backgroundTexture;
			/// <summary>�w�i���_�o�b�t�@</summary>
			ID3D11Buffer* backgroundVertexBuffer;
			/// <summary>�w�i�C���f�b�N�X�o�b�t�@</summary>
			ID3D11Buffer* backgroundIndexBuffer;
			/// <summary>�{�^���摜</summary>
			::Ishikawa::DirectX::Texture::FileTexture buttonsTexture;
			/// <summary>�v���C�{�^���̋�`��� [(x,y):���� (z,w):�E��]</summary>
			::Ishikawa::Math::Float4 playButtonRect;
			/// <summary>���[���{�^���̋�`��� [(x,y):���� (z,w):�E��]</summary>
			::Ishikawa::Math::Float4 ruleButtonRect;
			/// <summary>�I���{�^���̋�`��� [(x,y):���� (z,w):�E��]</summary>
			::Ishikawa::Math::Float4 endButtonRect;
			/// <summary>�v���C�{�^�����_�o�b�t�@</summary>
			ID3D11Buffer* playButtonVertexBuffer;
			/// <summary>���[���{�^�����_�o�b�t�@</summary>
			ID3D11Buffer* ruleButtonVertexBuffer;
			/// <summary>�I���{�^�����_�o�b�t�@</summary>
			ID3D11Buffer* endButtonVertexBuffer;
			/// <summary>�{�^���C���f�b�N�X�o�b�t�@</summary>
			ID3D11Buffer* buttonIndexBuffer;

			/// <summary>BGM��ID</summary>
			unsigned int bgmSoundId;

		public:
			// �f�t�H���g�R���X�g���N�^
			Title();
			// �f�X�g���N�^
			virtual ~Title() override final;

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
			// �Q�[���̎擾
			::CatchAFairy::Game* GetGame() const;
			// �Q�[���̈ˑ����Ă��鎩��f�o�C�X�̎擾
			::Ishikawa::DirectX::Device::Device* GetGameDependentITDevice() const;
			// �L�[�{�[�h�̎擾
			const ::Ishikawa::Input::Keyboard& GetKeyboard() const;

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
			// �f�o�C�X�ˑ����\�[�X�쐬
			void CreateDeviceDependentResources();
			// �f�o�C�X�ˑ����\�[�X�J��
			void ReleaseDeviceDependentResources();

			// �o�̓e�N�X�`���쐬
			void CreateOutputTextures();

			// �w�i���_�o�b�t�@�쐬
			void CreateBackgroundVertexBuffer();
			// �w�i�C���f�b�N�X�o�b�t�@�쐬
			void CreateBackgroundIndexBuffer();
			// �w�i���_�o�b�t�@�J��
			void ReleaseBackgroundVertexBuffer();
			// �w�i�C���f�b�N�X�o�b�t�@�J��
			void ReleaseBackgroundIndexBuffer();

			// �v���C�{�^�����_�o�b�t�@�쐬
			void CreatePlayButtonVertexBuffer();
			// ���[���{�^�����_�o�b�t�@�쐬
			void CreateRuleButtonVertexBuffer();
			// �v���C�{�^�����_�o�b�t�@�쐬
			void CreateEndButtonVertexBuffer();
			// �{�^���C���f�b�N�X�o�b�t�@�쐬
			void CreateButtonIndexBuffer();
			// �v���C�{�^�����_�o�b�t�@�J��
			void ReleasePlayButtonVertexBuffer();
			// ���[���{�^�����_�o�b�t�@�J��
			void ReleaseRuleButtonVertexBuffer();
			// �v���C�{�^�����_�o�b�t�@�J��
			void ReleaseEndButtonVertexBuffer();
			// �{�^���C���f�b�N�X�o�b�t�@�J��
			void ReleaseButtonIndexBuffer();

			// �o�͂ɕ`��
			void RenderToOutput();
		};
	}
}

#endif
