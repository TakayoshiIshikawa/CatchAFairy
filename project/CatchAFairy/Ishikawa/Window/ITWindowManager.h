//=============================================================================
//	ITWindowManager.h
//
//	����E�B���h�E�}�l�[�W���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_WINDOW_MANAGER_H__
#define __IT_WINDOW_MANAGER_H__

#include "Ishikawa/Common/PatternBase/SingletonBase.h"
#include "ITWindowListEdge.h"


namespace Ishikawa{
	namespace Window{
		/// <summary>
		/// ����E�B���h�E�}�l�[�W��
		/// </summary>
		class Manager final : public ::Ishikawa::Common::SingletonBase{
		public:
			// GetInstance<Manager>()�ŃA�N�Z�X���邽�߃t�����h��
			friend class Ishikawa::Common::SingletonManager;


		private:
			// �V���O���g���C���X�^���X
			static Manager* instance;

		private:
			/// <summary>�E�B���h�E���X�g�̒[</summary>
			ListEdge windowListEdge;


		public:
			// �f�t�H���g�R���X�g���N�^
			Manager();
			// �f�X�g���N�^
			virtual ~Manager() override final;
		private:
			/// <summary>�R�s�[�R���X�g���N�^[�폜]</summary>
			Manager(const Manager&) = delete;
			/// <summary>������Z�q[�폜]</summary>
			Manager& operator=(const Manager&) = delete;

		public:
			/// <summary>�E�B���h�E���X�g�̐擪�̎擾</summary>
			/// <returns>�E�B���h�E���X�g�̐擪</returns>
			Interface* GetHeadWindow() const { return this->windowListEdge.GetNextITWindow(); }
			/// <summary>�E�B���h�E���X�g�̏I�[�̎擾</summary>
			/// <returns>�E�B���h�E���X�g�̏I�[</returns>
			Interface* GetTailWindow() const { return this->windowListEdge.GetBeforeITWindow(); }

		public:
			// �E�B���h�E�̒ǉ�
			void AddWindow(Interface* const _window);

		private:
			// ������
			void Initialize();
		};
	}
}

#endif
