//=============================================================================
//	SceneListEdge.h
//
//	�V�[�����X�g�̒[�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SCENE_LIST_EDGE_H__
#define __SCENE_LIST_EDGE_H__

#include "SceneInterface.h"


namespace CatchAFairy{
	namespace Scene{
		/// <summary>
		/// ���X�g�̒[
		/// </summary>
		class ListEdge final : public Interface{
		protected:
			/// <summary>���̃V�[��</summary>
			Interface* nextScene;
			/// <summary>�O�̃V�[��</summary>
			Interface* beforeScene;


		public:
			// �f�t�H���g�R���X�g���N�^
			ListEdge();
			// �f�X�g���N�^
			virtual ~ListEdge() override final;

		public:
			/// <summary>�V�[�����X�g�̒[��?</summary>
			/// <returns>���true</returns>
			virtual bool IsSceneListEdge() const override final { return true; }
			/// <summary>�V�[���}�l�[�W���̎擾[�o���Ȃ�]</summary>
			/// <returns>���NULL</returns>
			virtual Manager* GetSceneManager() const override final { return nullptr; }
			/// <summary>���̃V�[���̎擾</summary>
			/// <returns>���̃V�[��</returns>
			virtual Interface* GetNextScene() const override final { return this->nextScene; }
			/// <summary>�O�̃V�[���̎擾</summary>
			/// <returns>�O�̃V�[��</returns>
			virtual Interface* GetBeforeScene() const override final { return this->beforeScene; }
			/// <summary>�e�N�X�`���T�C�Y�̎擾[�o���Ȃ�]</summary>
			/// <param name="_width">���̊i�[��</param>
			/// <param name="_height">�����̊i�[��</param>
			virtual void GetTextureSize(unsigned int* const _width, unsigned int* const _height) const override final{
				NOT_USING(_width); NOT_USING(_height);
			}
			/// <summary>�����_�[�^�[�Q�b�g�r���[�̎擾[�o���Ȃ�]</summary>
			/// <returns>���NULL</returns>
			virtual ID3D11RenderTargetView* GetRenderTargetView() const override final { return nullptr; }
			/// <summary>�V�F�[�_���\�[�X�r���[�̎擾[�o���Ȃ�]</summary>
			/// <returns>���NULL</returns>
			virtual ID3D11ShaderResourceView* GetShaderResourceView() const override final { return nullptr; }
			/// <summary>�X�V���邩?</summary>
			/// <returns>���false</returns>
			virtual bool IsUpdateScene() const override final { return false; }
			/// <summary>�`�悷�邩?</summary>
			/// <returns>���false</returns>
			virtual bool IsRenderScene() const override final { return false; }

		public:
			/// <summary>�V�[���}�l�[�W���̐ݒ�[�o���Ȃ�]</summary>
			virtual void SetSceneManager(Manager* const) override final{}
			/// <summary>���̃V�[���̐ݒ�</summary>
			/// <param name="_next">���̃V�[��</param>
			virtual void SetNextScene(Interface* const _next) override final { this->nextScene = _next; }
			/// <summary>�O�̃V�[���̐ݒ�</summary>
			/// <param name="_before">�O�̃V�[��</param>
			virtual void SetBeforeScene(Interface* const _before) override final { this->beforeScene = _before; }
			/// <summary>�X�V�t���O�̐ݒ�[�o���Ȃ�]</summary>
			virtual void SetUpdateFlag(bool) override final{}
			/// <summary>�`��t���O�̐ݒ�[�o���Ȃ�]</summary>
			virtual void SetRenderFlag(bool) override final{}

		public:
			/// <summary>�Q�[���̃f�o�C�X���ǂݍ��܂ꂽ</summary>
			virtual void OnLoadedGameDevice() override final{}
			/// <summary>�Q�[���̃f�o�C�X���������ꂽ</summary>
			virtual void OnRestoredGameDevice() override final{}
			/// <summary>�Q�[���̃f�o�C�X���j�����ꂽ</summary>
			virtual void OnLostGameDevice() override final{}

		public:
			/// <summary>���X�g����O���[�O��Ȃ�]</summary>
			virtual void RemoveSceneList() override final{}
			/// <summary>�T�C�Y�ύX</summary>
			virtual void ChangeSceneSize(unsigned int, unsigned int) override final{}

			/// <summary>�J�n</summary>
			virtual void Start() override final{}
			/// <summary>�X�V</summary>
			virtual void Update(float) override final{}
			/// <summary>�`��</summary>
			virtual void Render() override final{}
			/// <summary>�I��</summary>
			virtual void End() override final{}

			// ������
			void Initialize();

		protected:
			// �V�[���̏�����
			virtual void InitializeScene() override final;
		};
	}
}

#endif
