//=============================================================================
//	ITWindowListEdge.h
//
//	����E�B���h�E�̃��X�g�[�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_WINDOW_LIST_EDGE_H__
#define __IT_WINDOW_LIST_EDGE_H__

#include "ITWindowInterfase.h"


namespace Ishikawa{
	namespace Window{
		/// <summary>
		/// ����E�B���h�E�̃��X�g�[
		/// </summary>
		class ListEdge final : public Interface{
		private:
			/// <summary>���̎���E�B���h�E</summary>
			Interface* nextITWindow;
			/// <summary>�O�̎���E�B���h�E</summary>
			Interface* beforeITWindow;


		public:
			// �f�t�H���g�R���X�g���N�^
			ListEdge();
			// �f�X�g���N�^
			virtual ~ListEdge() override final;

		public:
			/// <summary>����E�B���h�E���X�g�̒[��?</summary>
			/// <returns>���true</returns>
			virtual bool IsITWindowListEdge() const override final { return true; }
			/// <summary>���̎���E�B���h�E�ˑ����\�[�X�̎擾</summary>
			/// <returns>���̎���E�B���h�E�ˑ����\�[�X</returns>
			virtual Interface* GetNextITWindow() const override final {
				return this->nextITWindow;
			}
			/// <summary>�O�̎���E�B���h�E�ˑ����\�[�X�̎擾</summary>
			/// <returns>�O�̎���E�B���h�E�ˑ����\�[�X</returns>
			virtual Interface* GetBeforeITWindow() const override final {
				return this->beforeITWindow;
			}
			/// <summary>�n���h���̎擾[�o���Ȃ�]</summary>
			/// <returns>���NULL</returns>
			virtual HWND__* GetHandle() const override final { return nullptr; }
			/// <summary>�T�C�Y�̎擾[�o���Ȃ�]</summary>
			virtual void GetSize(long* const, long* const) const override final{}

		public:
			/// <summary>���̎���E�B���h�E�̐ݒ�</summary>
			/// <param name="_next">���̎���E�B���h�E</param>
			virtual void SetNextITWindow(Interface* const _next) override final {
				this->nextITWindow = _next;
			}
			/// <summary>�O�̎���E�B���h�E�̐ݒ�</summary>
			/// <param name="_before">�O�̎���E�B���h�E</param>
			virtual void SetBeforeITWindow(Interface* const _before) override final {
				this->beforeITWindow = _before;
			}

		public:
			/// <summary>���X�g����O���[]</summary>
			virtual void RemoveITWindowList() override final{}

			/// <summary>�쐬[���Ȃ�]</summary>
			virtual void Create(
				const wchar_t* const,
				const wchar_t* const,
				const unsigned long,
				const unsigned long,
				const long,
				const long,
				const long,
				const long,
				HWND__* const,
				HMENU__* const,
				HINSTANCE__* const,
				void* const
			) override final{}
			/// <summary>�\��[���Ȃ�]</summary>
			virtual void Show(int) override final{}
			/// <summary>�I������[���Ȃ�]</summary>
			virtual void Exit() const override final{}

			/// <summary>�ˑ����\�[�X�̒ǉ�[���Ȃ�](1����)</summary>
			virtual void AddDependentResource(DependentResource::Interface* const) override final{}
			/// <summary>�ˑ����\�[�X�̒ǉ�[���Ȃ�](�z���X�g����)</summary>
			virtual void AddDependentResourceList(DependentResource::ListEdge* const) override final{}

			/// <summary>�v���V�[�W��[�������Ȃ�]</summary>
			/// <returns>�f�t�H���g�E�B���h�E�v���V�[�W���ɕԂ�����false�̂�</returns>
			virtual bool Procedure(unsigned int, unsigned int, long) override final { return false; }

			// ������
			void Initialize();

		protected:
			// ����E�B���h�E�̏�����
			virtual void InitializeITWindow() override final;
		};
	}
}

#endif
