//=============================================================================
//	ITWindow.h
//
//	����E�B���h�E�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_WINDOW_H__
#define __IT_WINDOW_H__

#include "ITWindowInterfase.h"
#include "DependentResource/ITWindowDependentResourceListEdge.h"


namespace Ishikawa{
	namespace Window{
		/// <summary>
		/// ����E�B���h�E
		/// </summary>
		class Window final : public Interface{
		private:
			// ���f���t���O
			static const unsigned long WINDOW_SUSPENDING_FLAG;
			// ��ʃT�C�Y�ύX���t���O
			static const unsigned long WINDOW_SIZE_MOVING_FLAG;
			// �ŏ����t���O
			static const unsigned long WINDOW_MINIMIZING_FLAG;
			// �ő剻�t���O
			static const unsigned long WINDOW_MAXIMIZING_FLAG;
			// �t���X�N���[���t���O
			static const unsigned long WINDOW_FULLSCREEN_FLAG;

		private:
			/// <summary>���̎���E�B���h�E</summary>
			Interface* nextITWindow;
			/// <summary>�O�̎���E�B���h�E</summary>
			Interface* beforeITWindow;
			/// <summary>�n���h��</summary>
			HWND__* handle;
			/// <summary>�X�^�C��</summary>
			unsigned long style;
			/// <summary>EX�X�^�C��</summary>
			unsigned long exStyle;
			/// <summary>��</summary>
			long width;
			/// <summary>����</summary>
			long height;
			/// <summary>�ő剻�O�̕�</summary>
			long widthOfBeforeMaximize;
			/// <summary>�ő剻�O�̍���</summary>
			long heightOfBeforeMaximize;
			/// <summary>�E�B���h�E�t���O</summary>
			unsigned long windowFlag;
			/// <summary>���̃E�B���h�E�Ɉˑ����郊�\�[�X�̃��X�g�̒[</summary>
			DependentResource::ListEdge windowDependentResourceListEdge;


		public:
			// �f�t�H���g�R���X�g���N�^
			Window();
			// �f�X�g���N�^
			virtual ~Window();

		public:
			/// <summary>����E�B���h�E���X�g�̒[��?</summary>
			/// <returns>���false</returns>
			virtual bool IsITWindowListEdge() const override final { return false; }
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
			/// <summary>�n���h���̎擾</summary>
			/// <returns>�n���h��</returns>
			virtual HWND__* GetHandle() const override final { return this->handle; }
			/// <summary>�T�C�Y�̎擾</summary>
			/// <param name="_width">���̊i�[��</param>
			/// <param name="_height">�����̊i�[��</param>
			virtual void GetSize(long* const _width, long* const _height) const override final {
				if(_width != nullptr) (*_width) = this->width;
				if(_height != nullptr) (*_height) = this->height;
			}
			// ���f����?
			bool IsSuspending() const;
			// ��ʃT�C�Y�ύX����?
			bool IsSizeMoving() const;
			// �ŏ�������?
			bool IsMinimizing() const;
			// �ő剻����?
			bool IsMaximizing() const;
			// �t���X�N���[������?
			bool IsFullscreen() const;

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
			// ���f���t���O�̐ݒ�
			void SetSuspendFlag(bool _flag);
			// ��ʃT�C�Y�ύX���t���O�̐ݒ�
			void SetSizeMoveFlag(bool _flag);
			// �ŏ������t���O�̐ݒ�
			void SetMinimizeFlag(bool _flag);
			// �ő剻���t���O�̐ݒ�
			void SetMaximizeFlag(bool _flag);
			// �t���X�N���[�����t���O�̐ݒ�
			void SetFullscreenFlag(bool _flag);

		public:
			// ���X�g����O���
			virtual void RemoveITWindowList() override final;

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
			) override final;
			// �\��
			virtual void Show(int nCmdShow) override final;
			// �I������
			virtual void Exit() const override final;

			// �ˑ����\�[�X�̒ǉ�(1����)
			virtual void AddDependentResource(DependentResource::Interface* const _dependentResource) override final;
			// �ˑ����\�[�X�̒ǉ�(�z���X�g����)
			// �ǉ�������͌����X�g�͋�ɂȂ�܂�
			virtual void AddDependentResourceList(DependentResource::ListEdge* const _dependentResourceListEdge) override final;

			// �v���V�[�W��
			virtual bool Procedure(unsigned int _message, unsigned int _wParam, long _lParam) override final;

			// ������
			void Initialize();

		protected:
			// ����E�B���h�E�̏�����
			virtual void InitializeITWindow() override final;

		private:
			// ����
			void Close();
			// ���f
			void Suspend();
			// �ĊJ
			void Resume();
			// �T�C�Y�ύX
			void ChangeSize(long _width, long _height);
			// ������
			void Activate();
			// �񊈐���
			void Deactivate();
			// �ŏ���
			void Minimize();
			// �ŏ�������
			void CancelOfMinimize();
			// �ő剻
			void Maximize();
			// �ő剻����
			void CancelOfMaximize();
		};
	}
}

#endif
