//=============================================================================
//	ITGameUseDirectXBase.h
//
//	DirectX�𗘗p���鎩��Q�[�����̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_GAME_USE_DIRECT_X_BASE_H__
#define __IT_GAME_USE_DIRECT_X_BASE_H__

struct HINSTANCE__;
#include "Ishikawa/Window/DependentResource/ITWindowDependentResourceBase.h"
#include "Ishikawa/DirectX/Device/DependentResource/ITDeviceDependentResourceBase.h"
#include "Ishikawa/Window/ITWindowClass.h"
#include "Ishikawa/Window/ITWindow.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "Ishikawa/DirectX/ITSwapChain.h"

namespace Ishikawa{
	namespace Game{
		/// <summary>
		/// DirectX�𗘗p���鎩��Q�[�����
		/// </summary>
		class UseDirectXBase :
			public ::Ishikawa::Window::DependentResource::Base,
			public ::Ishikawa::DirectX::Device::DependentResource::Base
		{
		private:
			/// <summary>�E�B���h�E�N���X</summary>
			::Ishikawa::Window::WindowClass windowClass;
			/// <summary>�E�B���h�E</summary>
			::Ishikawa::Window::Window window;
			/// <summary>DirectX�f�o�C�X</summary>
			::Ishikawa::DirectX::Device::Device directXDevice;
			/// <summary>�X���b�v�`�F�[��</summary>
			::Ishikawa::DirectX::SwapChain swapChain;
			/// <summary>�E�B���h�E�N���X��</summary>
			const wchar_t* windowClassName;
			/// <summary>�E�B���h�E�N���X�X�^�C��</summary>
			unsigned long windowClassStyle;
			/// <summary>�E�B���h�E�N���X�N���X�G�N�X�g��</summary>
			long windowClassClassExtra;
			/// <summary>�E�B���h�E�N���X�E�B���h�E�G�N�X�g��</summary>
			long windowClassWindowExtra;
			/// <summary>�E�B���h�E�^�C�g��</summary>
			const wchar_t* windowTitle;
			/// <summary>�E�B���h�E�X�^�C��</summary>
			unsigned long windowStyle;
			/// <summary>�E�B���h�EEX�X�^�C��</summary>
			unsigned long windowExStyle;
			/// <summary>�E�B���h�E��</summary>
			long windowWidth;
			/// <summary>�E�B���h�E����</summary>
			long windowHeight;



		public:
			// �f�t�H���g�R���X�g���N�^
			UseDirectXBase();
			// �R���X�g���N�^
			UseDirectXBase(
				const wchar_t* _windowClassName,
				unsigned long _widowClassStyle,
				long _widowClassClassExtra,
				long _widowClassWindowExtra,
				const wchar_t* _windowTitle,
				unsigned long _widowStyle,
				unsigned long _widowExStyle,
				long _widowWidth,
				long _widowHeight
			);
			// �f�X�g���N�^
			virtual ~UseDirectXBase();

		protected:
			/// <summary>�X���b�v�`�F�[���̎擾</summary>
			/// <returns>�X���b�v�`�F�[��</returns>
			::Ishikawa::DirectX::SwapChain& GetSwapChain(){ return this->swapChain; }

		public:
			// ������
			virtual void Initialize(
				HINSTANCE__* _hInstance,
				::Ishikawa::Window::WindowClass::ProcedureType _windowProcedure
			) = 0;
			// ������
			virtual void Activate(int _nCmdShow) = 0;
			// ���s
			virtual int Run() final;
			// �I��
			virtual void Finalize() = 0;

			// �I��������
			virtual void Exit() const final;

		protected:
			// ������
			virtual void InitializeUseDirectXBase(
				HINSTANCE__* _hInstance,
				::Ishikawa::Window::WindowClass::ProcedureType _windowProcedure
			) final;
			// �E�B���h�E�\��
			virtual void ShowWindow(int _nCmdShow) final;
			// ���s
			virtual void Execute() = 0;
			// ���g�̈ˑ����ݒ肷��
			virtual void SettingDependentThis() final;
		};
	}
}

#endif
