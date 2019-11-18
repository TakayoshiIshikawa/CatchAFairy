//=============================================================================
//	SceneBase.h
//
//	�V�[�����̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SCENE_BASE_H__
#define __SCENE_BASE_H__

#include "SceneInterface.h"


namespace CatchAFairy{
	namespace Scene{
		/// <summary>
		/// �V�[�����
		/// </summary>
		class Base : public Interface{
		private:
			// �X�V�t���O
			static const unsigned long SCENE_UPDATE_FLAG;
			// �`��t���O
			static const unsigned long SCENE_RENDER_FLAG;

		private:
			/// <summary>�V�[���}�l�[�W��</summary>
			Manager* sceneManager;
			/// <summary>���̃V�[��</summary>
			Interface* nextScene;
			/// <summary>�O�̃V�[��</summary>
			Interface* beforeScene;
			/// <summary>�V�[���t���O</summary>
			unsigned long sceneFlag;


		public:
			// �f�t�H���g�R���X�g���N�^
			Base();
			// �f�X�g���N�^
			virtual ~Base() override;

		public:
			/// <summary>�V�[�����X�g�̒[��?</summary>
			/// <returns>���false</returns>
			virtual bool IsSceneListEdge() const override final { return false; }
			/// <summary>�V�[���}�l�[�W���̎擾</summary>
			/// <returns>�V�[���}�l�[�W��</returns>
			virtual Manager* GetSceneManager() const override final { return this->sceneManager; }
			/// <summary>���̃V�[���̎擾</summary>
			/// <returns>���̃V�[��</returns>
			virtual Interface* GetNextScene() const override final { return this->nextScene; }
			/// <summary>�O�̃V�[���̎擾</summary>
			/// <returns>�O�̃V�[��</returns>
			virtual Interface* GetBeforeScene() const override final { return this->beforeScene; }
			// �X�V���邩?
			virtual bool IsUpdateScene() const override final;
			// �`�悷�邩?
			virtual bool IsRenderScene() const override final;

		public:
			/// <summary>�V�[���}�l�[�W���̐ݒ�</summary>
			/// <param name="_manager">�V�[���}�l�[�W��</param>
			virtual void SetSceneManager(Manager* const _manager) override final {
				this->sceneManager = _manager;
			}
			/// <summary>���̃V�[���̐ݒ�</summary>
			/// <param name="_next">���̃V�[��</param>
			virtual void SetNextScene(Interface* const _next) override final { this->nextScene = _next; }
			/// <summary>�O�̃V�[���̐ݒ�</summary>
			/// <param name="_before">�O�̃V�[��</param>
			virtual void SetBeforeScene(Interface* const _before) override final { this->beforeScene = _before; }
			// �X�V�t���O�̐ݒ�
			virtual void SetUpdateFlag(bool _flag) override final;
			// �`��t���O�̐ݒ�
			virtual void SetRenderFlag(bool _flag) override final;

		public:
			// ���X�g����O���
			virtual void RemoveSceneList() override final;
			// ������
			virtual void Initialize() = 0;

		protected:
			// �V�[���̏�����
			virtual void InitializeScene() override final;
		};
	}
}

#endif
