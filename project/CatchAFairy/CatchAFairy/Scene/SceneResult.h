//=============================================================================
//	SceneResult.h
//
//	���U���g�V�[���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SCENE_RESULT_H__
#define __SCENE_RESULT_H__

#include "SceneBase.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "Ishikawa/DirectX/Texture/ITTexture2D.h"
#include "Ishikawa/DirectX/Texture/ITFileTexture.h"
#include "CatchAFairy/Shader/User/TitleOutputShaderUser.h"
#include "CatchAFairy/Data/EnemyFairyType.h"

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
		/// ���U���g�V�[��
		/// </summary>
		class Result final : public Base{
		private:
			// �w�i�C���f�b�N�X��
			static const unsigned int BACKGROUND_INDEX_COUNT;

			// �����̃e�N�X�`���R�[�f�B�l�[�g [(x,y):���� (z,w):�E��]
			static const ::Ishikawa::Math::Float4 NUMBER_TEXTURECOORDINATE[11];
			/// <summary>�e�d�����̕\������</summary>
			static const unsigned int FAIRY_COUNT_DIGIT = 2U;
			/// <summary>�e�d���X�R�A�̕\������</summary>
			static const unsigned int FAIRY_SCORE_DIGIT = 5U;
			/// <summary>���v�X�R�A�̕\������</summary>
			static const unsigned int TOTAL_SCORE_DIGIT = 5U;
			// �e�d���̃X�R�A
			static const unsigned int FAIRY_SCORE[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)];

			// �d���J�E���g�����T�C�Y
			static const ::Ishikawa::Math::Float2 FAIRY_COUNT_NUMBER_SIZE;
			// �X�R�A�����T�C�Y
			static const ::Ishikawa::Math::Float2 SCORE_NUMBER_SIZE;
			// �d���J�E���g���W
			static const ::Ishikawa::Math::Float2 FAIRY_COUNT_POSITION[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)];
			// �d���X�R�A���W
			static const ::Ishikawa::Math::Float2 FAIRY_SCORE_POSITION[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)];
			// ���v�X�R�A���W
			static const ::Ishikawa::Math::Float2 TOTAL_SCORE_POSITION;

			/// <summary>�e�d�����̐����̐擪�C���f�b�N�X</summary>
			static const unsigned int FAIRY_COUNT_NUMBER_FARST_INDEX = 0U;
			/// <summary>�e�d���X�R�A�̐����̐擪�C���f�b�N�X</summary>
			static const unsigned int FAIRY_SCORE_NUMBER_FARST_INDEX =
				FAIRY_COUNT_NUMBER_FARST_INDEX +
				FAIRY_COUNT_DIGIT*static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count);
			/// <summary>���v�X�R�A�̐����̐擪�C���f�b�N�X</summary>
			static const unsigned int TOTAL_SCORE_NUMBER_FARST_INDEX =
				FAIRY_SCORE_NUMBER_FARST_INDEX +
				FAIRY_SCORE_DIGIT*static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count);
			/// <summary>�\������鐔���̍��v</summary>
			static const unsigned int TOTAL_VIEW_NUMBER_COUNT =
				TOTAL_SCORE_NUMBER_FARST_INDEX + TOTAL_SCORE_DIGIT;
			// ������\�����邽�߂̒��_�̍��v
			static const unsigned int TOTAL_VIEW_NUMBER_VERTEX_COUNT;
			// ������\�����邽�߂̃C���f�b�N�X�̍��v
			static const unsigned int TOTAL_VIEW_NUMBER_INDEX_COUNT;


		private:
			/// <summary>�o�̓����_�[�^�[�Q�b�g�e�N�X�`��</summary>
			::Ishikawa::DirectX::Texture::Texture2D outputRenderTargetTexture;
			/// <summary>�^�C�g���o�̓V�F�[�_���[�U (�g���܂킵)</summary>
			::CatchAFairy::Shader::User::TitleOutput titleOutputShaderUser;
			/// <summary>�w�i�摜</summary>
			::Ishikawa::DirectX::Texture::FileTexture backgroundTexture;
			/// <summary>�w�i���_�o�b�t�@</summary>
			ID3D11Buffer* backgroundVertexBuffer;
			/// <summary>�w�i�C���f�b�N�X�o�b�t�@</summary>
			ID3D11Buffer* backgroundIndexBuffer;
			/// <summary>�����摜</summary>
			::Ishikawa::DirectX::Texture::FileTexture numbersTexture;
			/// <summary>�������_�o�b�t�@</summary>
			ID3D11Buffer* numberVertexBuffer;
			/// <summary>�����C���f�b�N�X�o�b�t�@</summary>
			ID3D11Buffer* numberIndexBuffer;
			/// <summary>��ނ��Ƃ̕ߊl�d����</summary>
			unsigned int catchedFairyCount[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)];

			/// <summary>BGM��ID</summary>
			unsigned int bgmSoundId;

		public:
			// �f�t�H���g�R���X�g���N�^
			Result();
			// �f�X�g���N�^
			virtual ~Result() override final;

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
			// �����̕�������e�N�X�`���R�[�f�B�l�[�g���擾����
			const ::Ishikawa::Math::Float4& GetNumberTexcoord(const char _numberCharacter);

		public:
			/// <summary>�ߊl�d�����̐ݒ�</summary>
			/// <param name="_type">�d���̎��</param>
			/// <param name="_count">�ߊl��</param>
			void SetCatchedFairyCount(CatchAFairy::Data::EnemyFairyType _type, unsigned int _count){
				if(_type == CatchAFairy::Data::EnemyFairyType::Count) return;
				this->catchedFairyCount[static_cast<unsigned int>(_type)] = _count;
			}

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

			// �������_�o�b�t�@�쐬
			void CreateNumberVertexBuffer();
			// �����C���f�b�N�X�o�b�t�@�쐬
			void CreateNumberIndexBuffer();
			// �������_�o�b�t�@�J��
			void ReleaseNumberVertexBuffer();
			// �����C���f�b�N�X�o�b�t�@�J��
			void ReleaseNumberIndexBuffer();

			// �o�͂ɕ`��
			void RenderToOutput();
		};
	}
}

#endif
