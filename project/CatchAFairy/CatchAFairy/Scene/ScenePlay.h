//=============================================================================
//	ScenePlay.h
//
//	�v���C�V�[���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SCENE_PLAY_H__
#define __SCENE_PLAY_H__

#include "SceneBase.h"
#include "Ishikawa/DirectX/Texture/ITTexture2D.h"
#include "CatchAFairy/Object/PlayScene/DirectionalLight.h"
#include "CatchAFairy/Object/PlayScene/PlayCamera.h"
#include "CatchAFairy/Object/PlayScene/Ground.h"
#include "CatchAFairy/Object/PlayScene/Player.h"
#include "CatchAFairy/Object/PlayScene/EnemyFairies.h"
#include "CatchAFairy/Object/PlayScene/Trees.h"
#include "CatchAFairy/Object/PlayScene/PointLight.h"
#include "CatchAFairy/Object/PlayScene/GUI/GUIManager.h"
#include "CatchAFairy/Object/PlayScene/GUI/FairyPositionView.h"
#include "CatchAFairy/Object/PlayScene/GUI/TimeView.h"
#include "CatchAFairy/Object/PlayScene/GUI/OperationMethodView.h"

namespace Ishikawa{
	namespace Input{
		class Keyboard;
	}
	namespace DirectX{
		namespace Device{
			class Device;
		}
	}
	namespace Math{
		struct Float4;
		struct Matrix;
	}
}

namespace CatchAFairy{
	class Game;

	namespace Scene{
		/// <summary>
		/// �v���C�V�[��
		/// </summary>
		class Play final : public Base{
		private:
			// �o�̓C���f�b�N�X��
			static const unsigned int OUTPUT_INDEX_COUNT;
			// �X�e�[�W�T�C�Y�̔���
			static const float STAGE_SIZE_HALF;
			// �f�B���N�V���i�����C�g�̐F
			static const ::Ishikawa::Math::Float4 DIRECTIONAL_LIGHT_COLOR;


		private:
			/// <summary>�o�̓����_�[�^�[�Q�b�g�e�N�X�`��</summary>
			::Ishikawa::DirectX::Texture::Texture2D outputRenderTargetTexture;

			/// <summary>���s���C�g</summary>
			::CatchAFairy::Object::PlayScene::DirectionalLight directionalLight;
			/// <summary>�v���C�J����</summary>
			::CatchAFairy::Object::PlayScene::PlayCamera playCamera;
			/// <summary>�n��</summary>
			::CatchAFairy::Object::PlayScene::Ground ground;
			/// <summary>�v���C��</summary>
			::CatchAFairy::Object::PlayScene::Player player;
			/// <summary>�G�d���B</summary>
			::CatchAFairy::Object::PlayScene::EnemyFairies enemyFairies;
			/// <summary>�؁X</summary>
			::CatchAFairy::Object::PlayScene::Trees trees;
			/// <summary>�|�C���g���C�g</summary>
			::CatchAFairy::Object::PlayScene::PointLight pointLight;

			/// <summary>GUI�}�l�[�W��</summary>
			::CatchAFairy::Object::PlayScene::GUI::GUIManager guiManager;

			/// <summary>�d���ʒu�\��</summary>
			::CatchAFairy::Object::PlayScene::GUI::FairyPositionView fairyPositionView;
			/// <summary>���ԕ\��</summary>
			::CatchAFairy::Object::PlayScene::GUI::TimeView timeView;
			/// <summary>������@�\��</summary>
			::CatchAFairy::Object::PlayScene::GUI::OperationMethodView operationMethodView;

			/// <summary>BGM��ID</summary>
			unsigned int bgmSoundId;
			/// <summary>�I���t���O</summary>
			int isEnd;


		public:
			// �f�t�H���g�R���X�g���N�^
			Play();
			// �f�X�g���N�^
			virtual ~Play() override final;

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
			/// <summary>�G�d���B�̎擾</summary>
			/// <returns>�G�d���B</returns>
			::CatchAFairy::Object::PlayScene::EnemyFairies& GetEnemyFairies(){
				return this->enemyFairies;
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
			// ���U�鉹�̍Đ�
			void OnPlaySoundOfKaraburi() const;
			// �ߊl���̍Đ�
			void OnPlaySoundOfCatched() const;

			/// <summary>�I������</summary>
			void OnEnd(){ this->isEnd = 1; }
			/// <summary>�I�����邩?</summary>
			/// <returns>true:���� false:���Ȃ�</returns>
			bool IsEnd() const { return (this->isEnd != 0); }

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

			// �X�e�[�W�ƈړ��x�N�g���̔���
			::Ishikawa::Math::Float3 CollideMoveVectorAtStage(
				const ::Ishikawa::Math::Float3& _startPosition,
				const ::Ishikawa::Math::Float3& _moveVector,
				const ::Ishikawa::Math::Matrix& _localToWorld,
				float _reflectionRate
			) const;

		private:
			// �f�o�C�X�ˑ����\�[�X�쐬
			void CreateDeviceDependentResources();
			// �f�o�C�X�ˑ����\�[�X�J��
			void ReleaseDeviceDependentResources();

			// �o�̓e�N�X�`���쐬
			void CreateOutputTextures();

			// �o�̓��\�[�X������
			void InitializeOutputResources();
			// �I�u�W�F�N�g���\�[�X������
			void InitializeObjectResources();

			// �J�������_1�Ԃ̕`��
			void RenderOfCameraEyeFirst();
			// ���s���C�g�̕`��
			void RenderOfDirectionalLight();
			// �J�������_���ˌ��̕`��
			void RenderOfCameraEyeReflectionLight();
			// �J�������_�|�C���g���C�g�̕`��
			void RenderOfCameraEyePointLight();
		};
	}
}

#endif
