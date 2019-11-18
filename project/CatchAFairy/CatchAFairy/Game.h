//=============================================================================
//	Game.h
//
//	�Q�[���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __GAME_H__
#define __GAME_H__

#include "Ishikawa/Game/ITGameUseDirectXBase.h"
#include "Ishikawa/Input/ITKeyboard.h"
#include "Ishikawa/DirectX/ITSwapChain.h"
#include "Scene/SceneManager.h"


namespace CatchAFairy{
	/// <summary>
	/// �Q�[��
	/// </summary>
	class Game final : public ::Ishikawa::Game::UseDirectXBase{
	private:
		/// <summary>
		/// �Q�[���X�e�[�g
		/// </summary>
		enum class State : int{
			/// <summary>������</summary>
			Initalize,
			/// <summary>�������</summary>
			Active,
			/// <summary>�񊈐����</summary>
			Deactive,
			/// <summary>�I��</summary>
			Finalize
		};


	private:
		// 1�񓖂���̍ő�o�ߎ���
		static const float MAX_ELAPSED_TIME;
		// �E�B���h�E�^�C�g����
		static const wchar_t* const WINDOW_TITLE_NAME;
		// acf�t�@�C���p�X
		static const char* const SOUND_ACF_FILEPATH;
		// acb�t�@�C���p�X
		static const char* const SOUND_ACB_FILEPATH;
		// awb�t�@�C���p�X
		static const char* const SOUND_AWB_FILEPATH;

	private:
		/// <summary>�X�e�[�g</summary>
		State state;
		/// <summary>�V�[���}�l�[�W��</summary>
		Scene::Manager sceneManager;
		/// <summary>�L�[�{�[�h</summary>
		::Ishikawa::Input::Keyboard keyboard;


	public:
		// �f�t�H���g�R���X�g���N�^
		Game();
		// �f�X�g���N�^
		virtual ~Game() override final;

	public:
		/// <summary>�L�[�{�[�h�̎擾</summary>
		/// <returns>�L�[�{�[�h</returns>
		const ::Ishikawa::Input::Keyboard& GetKeyboard() const { return this->keyboard; }

	private:
		// ���s���邩?
		bool IsExecute() const;

	public:
		// �E�B���h�E�ɓǂݍ��܂ꂽ
		virtual void OnWindowLoaded() override final;
		// �E�B���h�E������ꂽ
		virtual void OnWindowClosed() override final;
		// �E�B���h�E�T�C�Y���ύX���ꂽ
		virtual void OnWindowSizeChanged() override final;
		// �E�B���h�E���ő剻���ꂽ
		virtual void OnWindowMaximized() override final;
		// �E�B���h�E�̍ő剻���������ꂽ
		virtual void OnCancelOfWindowMaximized() override final;
		// �E�B���h�E���ŏ������ꂽ
		virtual void OnWindowMinimized() override final;
		// �E�B���h�E�̍ŏ������������ꂽ
		virtual void OnCancelOfWindowMinimized() override final;
		// �E�B���h�E�����f���ꂽ
		virtual void OnWindowSuspended() override final;
		// �E�B���h�E���ĊJ���ꂽ
		virtual void OnWindowResumed() override final;
		// �E�B���h�E������������
		virtual void OnWindowActivated() override final;
		// �E�B���h�E���񊈐�������
		virtual void OnWindowDeactivated() override final;
		// �L�[�{�[�h�X�e�[�g���ύX���ꂽ
		virtual void OnChangedKeyboardState(unsigned int _message, unsigned int _wParam, long _lParam) override final;
		// �}�E�X�X�e�[�g���ύX���ꂽ
		virtual void OnChangedMouseState(unsigned int _message, unsigned int _wParam, long _lParam) override final;

		// �f�o�C�X�ɓǂݍ��܂ꂽ
		virtual void OnDeviceLoaded() override final;
		// �f�o�C�X���������ꂽ
		virtual void OnDeviceRestored() override final;
		// �f�o�C�X���j�����ꂽ
		virtual void OnDeviceLost() override final;

	public:
		// ������
		virtual void Initialize(
			HINSTANCE__* _hInstance,
			::Ishikawa::Window::WindowClass::ProcedureType _windowProcedure
		) override final;
		// ����������
		virtual void Activate(int _nCmdShow) override final;
		// �I��
		virtual void Finalize() override final;

	protected:
		// ���s
		virtual void Execute() override final;

	private:
		// �X�V
		void Update();
		// �`��
		void Render();
	};
}

#endif
