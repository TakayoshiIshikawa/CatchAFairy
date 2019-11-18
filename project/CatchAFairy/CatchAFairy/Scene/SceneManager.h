//=============================================================================
//	SceneManager.h
//
//	�V�[���}�l�[�W���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "SceneListEdge.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "CatchAFairy/Shader/User/OutputScreenShaderUser.h"

namespace CatchAFairy{
	class Game;

	namespace Scene{
		class Interface;
		class Base;


		/// <summary>
		/// �V�[���}�l�[�W��
		/// </summary>
		class Manager final{
		public:
			// �V�[���o�̓e�N�X�`���T�C�Y
			static const unsigned int SCENE_OUTPUT_TEXTURE_SIZE;
			// �V�[���o�̓e�N�X�`���̕`�敝
			static const unsigned int SCENE_OUTPUT_TEXTURE_WIDTH;
			// �V�[���o�̓e�N�X�`���̕`�捂��
			static const unsigned int SCENE_OUTPUT_TEXTURE_HEIGHT;
			// �V�[���o�̓e�N�X�`���̃A�X�y�N�g��
			static const float SCENE_OUTPUT_TEXTURE_ASPECT;
			// �]���̐F
			static const ::Ishikawa::Math::Float4 MARGIN_COLOR;

		private:
			/// <summary>�Q�[��</summary>
			Game* game;
			/// <summary>�V�[�����X�g�̒[</summary>
			ListEdge sceneListEdge;
			/// <summary>��ʏo�̓V�F�[�_���[�U</summary>
			::CatchAFairy::Shader::User::OutputScreen outputScreenShaderUser;
			/// <summary>�V�[���ύX�����t���[����?</summary>
			int isChangedSceneFrame;

		public:
			// �f�t�H���g�R���X�g���N�^
			Manager();
			// �f�X�g���N�^
			~Manager();

		public:
			/// <summary>�Q�[���̎擾</summary>
			/// <returns>�Q�[��</returns>
			Game* GetGame() const { return this->game; }

		private:
			// �Q�[�����ˑ����Ă��鎩��f�o�C�X�̎擾
			::Ishikawa::DirectX::Device::Device* GetGameDependentITDevice() const;
			// ���݂̃V�[�����擾
			Interface* GetCurrentScene() const;


		public:
			/// <summary>�Q�[���̐ݒ�</summary>
			/// <param name="_game">�Q�[��</param>
			void SetGame(Game* const _game){ this->game = _game; }

		public:
			// �Q�[���̃f�o�C�X���ǂݍ��܂ꂽ
			void OnLoadedGameDevice();
			// �Q�[���̃f�o�C�X���������ꂽ
			void OnRestoredGameDevice();
			// �Q�[���̃f�o�C�X���j�����ꂽ
			void OnLostGameDevice();

		public:
			// ������
			void Initialize();
			// �X�V
			void Update(float _elapsedTime);
			// �`��
			void Render();
			// ��ʂ֕`��
			void RenderToScreen(
				ID3D11RenderTargetView* const _renderTargetView,
				ID3D11DepthStencilView* const _depthStencilView,
				unsigned int _width,
				unsigned int _height
			);
			// �I��
			void Finalize();

			// �v�b�V��
			void PushScene(Base* const _scene);
			// �|�b�v
			void PopScene();
			// �`�F���W
			void ChangeScene(Base* const _scene);
			// �S�폜
			void ClearSceneList();
		};
	}
}

#endif
