//=============================================================================
//	Game.h
//
//	ゲームのヘッダファイル
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
	/// ゲーム
	/// </summary>
	class Game final : public ::Ishikawa::Game::UseDirectXBase{
	private:
		/// <summary>
		/// ゲームステート
		/// </summary>
		enum class State : int{
			/// <summary>初期化</summary>
			Initalize,
			/// <summary>活性状態</summary>
			Active,
			/// <summary>非活性状態</summary>
			Deactive,
			/// <summary>終了</summary>
			Finalize
		};


	private:
		// 1回当たりの最大経過時間
		static const float MAX_ELAPSED_TIME;
		// ウィンドウタイトル名
		static const wchar_t* const WINDOW_TITLE_NAME;
		// acfファイルパス
		static const char* const SOUND_ACF_FILEPATH;
		// acbファイルパス
		static const char* const SOUND_ACB_FILEPATH;
		// awbファイルパス
		static const char* const SOUND_AWB_FILEPATH;

	private:
		/// <summary>ステート</summary>
		State state;
		/// <summary>シーンマネージャ</summary>
		Scene::Manager sceneManager;
		/// <summary>キーボード</summary>
		::Ishikawa::Input::Keyboard keyboard;


	public:
		// デフォルトコンストラクタ
		Game();
		// デストラクタ
		virtual ~Game() override final;

	public:
		/// <summary>キーボードの取得</summary>
		/// <returns>キーボード</returns>
		const ::Ishikawa::Input::Keyboard& GetKeyboard() const { return this->keyboard; }

	private:
		// 実行するか?
		bool IsExecute() const;

	public:
		// ウィンドウに読み込まれた
		virtual void OnWindowLoaded() override final;
		// ウィンドウが閉じられた
		virtual void OnWindowClosed() override final;
		// ウィンドウサイズが変更された
		virtual void OnWindowSizeChanged() override final;
		// ウィンドウが最大化された
		virtual void OnWindowMaximized() override final;
		// ウィンドウの最大化が解除された
		virtual void OnCancelOfWindowMaximized() override final;
		// ウィンドウが最小化された
		virtual void OnWindowMinimized() override final;
		// ウィンドウの最小化が解除された
		virtual void OnCancelOfWindowMinimized() override final;
		// ウィンドウが中断された
		virtual void OnWindowSuspended() override final;
		// ウィンドウが再開された
		virtual void OnWindowResumed() override final;
		// ウィンドウが活性化した
		virtual void OnWindowActivated() override final;
		// ウィンドウが非活性化した
		virtual void OnWindowDeactivated() override final;
		// キーボードステートが変更された
		virtual void OnChangedKeyboardState(unsigned int _message, unsigned int _wParam, long _lParam) override final;
		// マウスステートが変更された
		virtual void OnChangedMouseState(unsigned int _message, unsigned int _wParam, long _lParam) override final;

		// デバイスに読み込まれた
		virtual void OnDeviceLoaded() override final;
		// デバイスが復元された
		virtual void OnDeviceRestored() override final;
		// デバイスが破棄された
		virtual void OnDeviceLost() override final;

	public:
		// 初期化
		virtual void Initialize(
			HINSTANCE__* _hInstance,
			::Ishikawa::Window::WindowClass::ProcedureType _windowProcedure
		) override final;
		// 活性化する
		virtual void Activate(int _nCmdShow) override final;
		// 終了
		virtual void Finalize() override final;

	protected:
		// 実行
		virtual void Execute() override final;

	private:
		// 更新
		void Update();
		// 描画
		void Render();
	};
}

#endif
