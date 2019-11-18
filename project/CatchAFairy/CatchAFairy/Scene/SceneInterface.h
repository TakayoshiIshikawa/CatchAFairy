//=============================================================================
//	SceneInterface.h
//
//	�V�[���C���^�[�t�F�[�X�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SCENE_INTERFACE_H__
#define __SCENE_INTERFACE_H__

#include "Ishikawa/DirectX/Texture/ITTexture2DInterface.h"
#include "Ishikawa/DirectX/Texture/ITRenderTargetInterface.h"
#include "Ishikawa/DirectX/Texture/ITShaderResourceInterface.h"


namespace CatchAFairy{
	namespace Scene{
		class Manager;


		/// <summary>
		/// �V�[���C���^�[�t�F�[�X
		/// </summary>
		class Interface :
			public ::Ishikawa::DirectX::Texture::Texture2DInterface,
			public ::Ishikawa::DirectX::Texture::RenderTargetInterface,
			public ::Ishikawa::DirectX::Texture::ShaderResourceInterface
		{
		public:
			/// <summary>�f�t�H���g�R���X�g���N�^</summary>
			Interface(){}
			/// <summary>�f�X�g���N�^</summary>
			virtual ~Interface(){}

		public:
			// �V�[�����X�g�̒[��?
			virtual bool IsSceneListEdge() const = 0;
			// �V�[���}�l�[�W���̎擾
			virtual Manager* GetSceneManager() const = 0;
			// ���̃V�[���̎擾
			virtual Interface* GetNextScene() const = 0;
			// �O�̃V�[���̎擾
			virtual Interface* GetBeforeScene() const = 0;
			// �X�V���邩?
			virtual bool IsUpdateScene() const = 0;
			// �`�悷�邩?
			virtual bool IsRenderScene() const = 0;

		public:
			// �V�[���}�l�[�W���̐ݒ�
			virtual void SetSceneManager(Manager* const _manager) = 0;
			// ���̃V�[���̐ݒ�
			virtual void SetNextScene(Interface* const _next) = 0;
			// �O�̃V�[���̐ݒ�
			virtual void SetBeforeScene(Interface* const _before) = 0;
			// �X�V�t���O�̐ݒ�
			virtual void SetUpdateFlag(bool _flag) = 0;
			// �`��t���O�̐ݒ�
			virtual void SetRenderFlag(bool _flag) = 0;

		public:
			// �Q�[���̃f�o�C�X���ǂݍ��܂ꂽ
			virtual void OnLoadedGameDevice() = 0;
			// �Q�[���̃f�o�C�X���������ꂽ
			virtual void OnRestoredGameDevice() = 0;
			// �Q�[���̃f�o�C�X���j�����ꂽ
			virtual void OnLostGameDevice() = 0;

		public:
			// ���X�g����O���
			virtual void RemoveSceneList() = 0;
			// �T�C�Y�ύX
			virtual void ChangeSceneSize(unsigned int _width, unsigned int _height) = 0;

			// �J�n
			virtual void Start() = 0;
			// �X�V
			virtual void Update(float _elapsedTime) = 0;
			// �`��
			virtual void Render() = 0;
			// �I��
			virtual void End() = 0;

		protected:
			// �V�[���̏�����
			virtual void InitializeScene() = 0;
		};
	}
}

#endif
