//=============================================================================
//	ITWindowDependentResourceBase.h
//
//	����E�B���h�E�ˑ����\�[�X�̊��̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_WINDOW_DEPENDENT_RESOURCE_BASE_H__
#define __IT_WINDOW_DEPENDENT_RESOURCE_BASE_H__

#include "ITWindowDependentResourceInterface.h"


namespace Ishikawa{
	namespace Window{
		namespace DependentResource{
			/// <summary>
			/// ����E�B���h�E�ˑ����\�[�X�̊��
			/// </summary>
			class Base : public Interface{
			private:
				/// <summary>�ˑ����Ă���E�B���h�E</summary>
				Window* dependentITWindow;
				/// <summary>���̎���E�B���h�E�ˑ����\�[�X</summary>
				Interface* nextITWindowDependentResource;
				/// <summary>�O�̎���E�B���h�E�ˑ����\�[�X</summary>
				Interface* beforeITWindowDependentResource;


			public:
				// �f�t�H���g�R���X�g���N�^
				Base();
				// �f�X�g���N�^
				virtual ~Base() override;

			public:
				/// <summary>����E�B���h�E�ˑ����\�[�X���X�g�̒[��?</summary>
				/// <returns>���false</returns>
				virtual bool IsITWindowDependentResourceListEdge() const override final { return false; }
				/// <summary>�ˑ����Ă���E�B���h�E�̎擾[�o���Ȃ�]</summary>
				/// <returns>�ˑ����Ă���E�B���h�E</returns>
				virtual Window* GetDependentITWindow() const override final { return this->dependentITWindow; }
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
				/// <summary>�ˑ����鎩��E�B���h�E�̐ݒ�</summary>
				/// <param name="_window">�ˑ����鎩��E�B���h�E</param>
				virtual void SetDependentITWindow(Window* const _window) override final {
					this->dependentITWindow = _window;
				}
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
				// ���X�g����O���
				virtual void RemoveITWindowDependentResourceList() override final;

			protected:
				// ����E�B���h�E�ˑ����\�[�X�̏�����
				virtual void InitializeITWindowDependentResource() override final;
			};
		}
	}
}

#endif
