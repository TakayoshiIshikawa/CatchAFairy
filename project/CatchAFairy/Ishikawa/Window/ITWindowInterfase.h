//=============================================================================
//	ITWindowInterface.h
//
//	����E�B���h�E�C���^�[�t�F�[�X�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_WINDOW_INTERFACE_H__
#define __IT_WINDOW_INTERFACE_H__

struct HINSTANCE__;
struct HWND__;
struct HMENU__;

namespace Ishikawa{
	namespace Window{
		namespace DependentResource{
			class Interface;
			class ListEdge;
		}
		class ListEdge;


		/// <summary>
		/// ����E�B���h�E�C���^�[�t�F�[�X
		/// </summary>
		class Interface{
		public:
			/// <summary>�f�t�H���g�R���X�g���N�^</summary>
			Interface(){}
			/// <summary>�f�X�g���N�^</summary>
			virtual ~Interface(){}

		public:
			// ����E�B���h�E���X�g�̒[��?
			virtual bool IsITWindowListEdge() const = 0;
			// ���̎���E�B���h�E�̎擾
			virtual Interface* GetNextITWindow() const = 0;
			// �O�̎���E�B���h�E�̎擾
			virtual Interface* GetBeforeITWindow() const = 0;
			// �n���h���̎擾
			virtual HWND__* GetHandle() const = 0;
			// �T�C�Y�̎擾
			virtual void GetSize(long* const _width, long* const _height) const = 0;

		public:
			// ���̎���E�B���h�E�̐ݒ�
			virtual void SetNextITWindow(Interface* const _next) = 0;
			// �O�̎���E�B���h�E�̐ݒ�
			virtual void SetBeforeITWindow(Interface* const _before) = 0;

		public:
			// ���X�g����O���
			virtual void RemoveITWindowList() = 0;

			// �쐬
			virtual void Create(
				const wchar_t* const _className,
				const wchar_t* const _title,
				const unsigned long _style,
				const unsigned long _exStyle,
				const long _x,
				const long _y,
				const long _width,
				const long _height,
				HWND__* const _parent,
				HMENU__* const _menu,
				HINSTANCE__* const _hInstance,
				void* const _param
			) = 0;
			// �\��
			virtual void Show(int nCmdShow) = 0;
			// �I������
			virtual void Exit() const = 0;

			// �ˑ����\�[�X�̒ǉ�(1����)
			virtual void AddDependentResource(DependentResource::Interface* const _dependentResource) = 0;
			// �ˑ����\�[�X�̒ǉ�(�z���X�g����)
			// �ǉ�������͌����X�g�͋�ɂȂ�܂�
			virtual void AddDependentResourceList(DependentResource::ListEdge* const _dependentResourceListEdge) = 0;

			// �v���V�[�W��
			virtual bool Procedure(unsigned int message, unsigned int wParam, long lParam) = 0;

		protected:
			// ����E�B���h�E�̏�����
			virtual void InitializeITWindow() = 0;
		};
	}
}

#endif
