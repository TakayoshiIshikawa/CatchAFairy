//=============================================================================
//	ITWindowDependentResourceInterface.h
//
//	����E�B���h�E�ˑ����\�[�X�C���^�[�t�F�[�X�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_WINDOW_DEPENDENT_RESOURCE_INTERFACE_H__
#define __IT_WINDOW_DEPENDENT_RESOURCE_INTERFACE_H__


namespace Ishikawa{
	namespace Window{
		class Window;


		namespace DependentResource{
			/// <summary>
			/// ����E�B���h�E�ˑ����\�[�X�C���^�[�t�F�[�X
			/// </summary>
			class Interface{
			public:
				/// <summary>�f�t�H���g�R���X�g���N�^</summary>
				Interface(){}
				/// <summary>�f�X�g���N�^</summary>
				virtual ~Interface(){}

			public:
				// ����E�B���h�E�ˑ����\�[�X���X�g�̒[��?
				virtual bool IsITWindowDependentResourceListEdge() const = 0;
				// �ˑ����Ă���E�B���h�E�̎擾
				virtual Window* GetDependentITWindow() const = 0;
				// ���̎���E�B���h�E�ˑ����\�[�X�̎擾
				virtual Interface* GetNextITWindowDependentResource() const = 0;
				// �O�̎���E�B���h�E�ˑ����\�[�X�̎擾
				virtual Interface* GetBeforeITWindowDependentResource() const = 0;

			public:
				// �ˑ����鎩��E�B���h�E�̐ݒ�
				virtual void SetDependentITWindow(Window* const _window) = 0;
				// ���̎���E�B���h�E�ˑ����\�[�X�̐ݒ�
				virtual void SetNextITWindowDependentResource(Interface* const _next) = 0;
				// �O�̎���E�B���h�E�ˑ����\�[�X�̐ݒ�
				virtual void SetBeforeITWindowDependentResource(Interface* const _before) = 0;

			public:
				// �E�B���h�E�ɓǂݍ��܂ꂽ
				virtual void OnWindowLoaded() = 0;
				// �E�B���h�E������ꂽ
				virtual void OnWindowClosed() = 0;
				// �E�B���h�E�T�C�Y���ύX���ꂽ
				virtual void OnWindowSizeChanged() = 0;
				// �E�B���h�E���ő剻���ꂽ
				virtual void OnWindowMaximized() = 0;
				// �E�B���h�E�̍ő剻���������ꂽ
				virtual void OnCancelOfWindowMaximized() = 0;
				// �E�B���h�E���ŏ������ꂽ
				virtual void OnWindowMinimized() = 0;
				// �E�B���h�E�̍ŏ������������ꂽ
				virtual void OnCancelOfWindowMinimized() = 0;
				// �E�B���h�E�����f���ꂽ
				virtual void OnWindowSuspended() = 0;
				// �E�B���h�E���ĊJ���ꂽ
				virtual void OnWindowResumed() = 0;
				// �E�B���h�E������������
				virtual void OnWindowActivated() = 0;
				// �E�B���h�E���񊈐�������
				virtual void OnWindowDeactivated() = 0;
				// �L�[�{�[�h�X�e�[�g���ύX���ꂽ
				virtual void OnChangedKeyboardState(unsigned int _message, unsigned int _wParam, long _lParam) = 0;
				// �}�E�X�X�e�[�g���ύX���ꂽ
				virtual void OnChangedMouseState(unsigned int _message, unsigned int _wParam, long _lParam) = 0;

			public:
				// ���X�g����O���
				virtual void RemoveITWindowDependentResourceList() = 0;

			protected:
				// ����E�B���h�E�ˑ����\�[�X�̏�����
				virtual void InitializeITWindowDependentResource() = 0;
			};
		}
	}
}

#endif
