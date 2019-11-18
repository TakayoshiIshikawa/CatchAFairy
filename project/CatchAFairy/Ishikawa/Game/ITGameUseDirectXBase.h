//=============================================================================
//	ITGameUseDirectXBase.h
//
//	DirectXを利用する自作ゲーム基底のヘッダファイル
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
		/// DirectXを利用する自作ゲーム基底
		/// </summary>
		class UseDirectXBase :
			public ::Ishikawa::Window::DependentResource::Base,
			public ::Ishikawa::DirectX::Device::DependentResource::Base
		{
		private:
			/// <summary>ウィンドウクラス</summary>
			::Ishikawa::Window::WindowClass windowClass;
			/// <summary>ウィンドウ</summary>
			::Ishikawa::Window::Window window;
			/// <summary>DirectXデバイス</summary>
			::Ishikawa::DirectX::Device::Device directXDevice;
			/// <summary>スワップチェーン</summary>
			::Ishikawa::DirectX::SwapChain swapChain;
			/// <summary>ウィンドウクラス名</summary>
			const wchar_t* windowClassName;
			/// <summary>ウィンドウクラススタイル</summary>
			unsigned long windowClassStyle;
			/// <summary>ウィンドウクラスクラスエクストラ</summary>
			long windowClassClassExtra;
			/// <summary>ウィンドウクラスウィンドウエクストラ</summary>
			long windowClassWindowExtra;
			/// <summary>ウィンドウタイトル</summary>
			const wchar_t* windowTitle;
			/// <summary>ウィンドウスタイル</summary>
			unsigned long windowStyle;
			/// <summary>ウィンドウEXスタイル</summary>
			unsigned long windowExStyle;
			/// <summary>ウィンドウ幅</summary>
			long windowWidth;
			/// <summary>ウィンドウ高さ</summary>
			long windowHeight;



		public:
			// デフォルトコンストラクタ
			UseDirectXBase();
			// コンストラクタ
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
			// デストラクタ
			virtual ~UseDirectXBase();

		protected:
			/// <summary>スワップチェーンの取得</summary>
			/// <returns>スワップチェーン</returns>
			::Ishikawa::DirectX::SwapChain& GetSwapChain(){ return this->swapChain; }

		public:
			// 初期化
			virtual void Initialize(
				HINSTANCE__* _hInstance,
				::Ishikawa::Window::WindowClass::ProcedureType _windowProcedure
			) = 0;
			// 活性化
			virtual void Activate(int _nCmdShow) = 0;
			// 実行
			virtual int Run() final;
			// 終了
			virtual void Finalize() = 0;

			// 終了させる
			virtual void Exit() const final;

		protected:
			// 初期化
			virtual void InitializeUseDirectXBase(
				HINSTANCE__* _hInstance,
				::Ishikawa::Window::WindowClass::ProcedureType _windowProcedure
			) final;
			// ウィンドウ表示
			virtual void ShowWindow(int _nCmdShow) final;
			// 実行
			virtual void Execute() = 0;
			// 自身の依存先を設定する
			virtual void SettingDependentThis() final;
		};
	}
}

#endif
