//=============================================================================
//	ITWindowDependentResourceListEdge.h
//
//	����E�B���h�E�ˑ����\�[�X�̃��X�g�[�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_WINDOW_DEPENDENT_RESOURCE_LIST_EDGE_H__
#define __IT_WINDOW_DEPENDENT_RESOURCE_LIST_EDGE_H__

#include "ITWindowDependentResourceInterface.h"


namespace Ishikawa{
	namespace Window{
		namespace DependentResource{
			/// <summary>
			/// ����E�B���h�E�ˑ����\�[�X�̃��X�g�[
			/// </summary>
			class ListEdge final : public Interface{
			private:
				/// <summary>���̎���E�B���h�E�ˑ����\�[�X</summary>
				Interface* nextITWindowDependentResource;
				/// <summary>�O�̎���E�B���h�E�ˑ����\�[�X</summary>
				Interface* beforeITWindowDependentResource;


			public:
				// �f�t�H���g�R���X�g���N�^
				ListEdge();
				// �f�X�g���N�^
				virtual ~ListEdge() override final;

			public:
				/// <summary>����E�B���h�E�ˑ����\�[�X���X�g�̒[��?</summary>
				/// <returns>���true</returns>
				virtual bool IsITWindowDependentResourceListEdge() const override final { return true; }
				/// <summary>�ˑ����Ă���E�B���h�E�̎擾[�o���Ȃ�]</summary>
				/// <returns>���NULL</returns>
				virtual Window* GetDependentITWindow() const override final { return nullptr; }
				/// <summary>���̎���E�B���h�E�ˑ����\�[�X�̎擾</summary>
				/// <returns>���̎���E�B���h�E�ˑ����\�[�X</returns>
				virtual Interface* GetNextITWindowDependentResource() const override final {
					return this->nextITWindowDependentResource;
				}
				/// <summary>�O�̎���E�B���h�E�ˑ����\�[�X�̎擾</summary>
				/// <returns>�O�̎���E�B���h�E�ˑ����\�[�X</returns>
				virtual Interface* GetBeforeITWindowDependentResource() const override final {
					return this->beforeITWindowDependentResource;
				}

			public:
				/// <summary>�ˑ����鎩��E�B���h�E�̐ݒ�[�o���Ȃ�]</summary>
				virtual void SetDependentITWindow(Window* const) override final{}
				/// <summary>���̎���E�B���h�E�ˑ����\�[�X�̐ݒ�</summary>
				/// <param name="_next">���̎���E�B���h�E�ˑ����\�[�X</param>
				virtual void SetNextITWindowDependentResource(Interface* const _next) override final {
					this->nextITWindowDependentResource = _next;
				}
				/// <summary>�O�̎���E�B���h�E�ˑ����\�[�X�̐ݒ�</summary>
				/// <param name="_before">�O�̎���E�B���h�E�ˑ����\�[�X</param>
				virtual void SetBeforeITWindowDependentResource(Interface* const _before) override final{
					this->beforeITWindowDependentResource = _before;
				}

			public:
				/// <summary>�E�B���h�E�ɓǂݍ��܂ꂽ</summary>
				virtual void OnWindowLoaded() override final{}
				/// <summary>�E�B���h�E������ꂽ</summary>
				virtual void OnWindowClosed() override final{}
				/// <summary>�E�B���h�E�T�C�Y���ύX���ꂽ</summary>
				virtual void OnWindowSizeChanged() override final{}
				/// <summary>�E�B���h�E���ő剻���ꂽ</summary>
				virtual void OnWindowMaximized() override final{}
				/// <summary>�E�B���h�E�̍ő剻���������ꂽ</summary>
				virtual void OnCancelOfWindowMaximized() override final{}
				/// <summary>�E�B���h�E���ŏ������ꂽ</summary>
				virtual void OnWindowMinimized() override final{}
				/// <summary>�E�B���h�E�̍ŏ������������ꂽ</summary>
				virtual void OnCancelOfWindowMinimized() override final{}
				/// <summary>�E�B���h�E�����f���ꂽ</summary>
				virtual void OnWindowSuspended() override final{}
				/// <summary>�E�B���h�E���ĊJ���ꂽ</summary>
				virtual void OnWindowResumed() override final{}
				/// <summary>�E�B���h�E������������</summary>
				virtual void OnWindowActivated() override final{}
				/// <summary>�E�B���h�E���񊈐�������</summary>
				virtual void OnWindowDeactivated() override final{}
				/// <summary>�L�[�{�[�h�X�e�[�g���ύX���ꂽ</summary>
				virtual void OnChangedKeyboardState(unsigned int, unsigned int, long) override final{}
				/// <summary>�}�E�X�X�e�[�g���ύX���ꂽ</summary>
				virtual void OnChangedMouseState(unsigned int, unsigned int, long) override final{}

			public:
				/// <summary>���X�g����O���[�O��Ȃ�]</summary>
				virtual void RemoveITWindowDependentResourceList() override final{}

				// ������
				void Initialize();

			protected:
				// ����E�B���h�E�ˑ����\�[�X�̏�����
				virtual void InitializeITWindowDependentResource() override final;
			};
		}
	}
}

#endif
