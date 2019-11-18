//=============================================================================
//	ITWindowDependentResourceInterface.h
//
//	自作ウィンドウ依存リソースインターフェースのヘッダファイル
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
			/// 自作ウィンドウ依存リソースインターフェース
			/// </summary>
			class Interface{
			public:
				/// <summary>デフォルトコンストラクタ</summary>
				Interface(){}
				/// <summary>デストラクタ</summary>
				virtual ~Interface(){}

			public:
				// 自作ウィンドウ依存リソースリストの端か?
				virtual bool IsITWindowDependentResourceListEdge() const = 0;
				// 依存しているウィンドウの取得
				virtual Window* GetDependentITWindow() const = 0;
				// 次の自作ウィンドウ依存リソースの取得
				virtual Interface* GetNextITWindowDependentResource() const = 0;
				// 前の自作ウィンドウ依存リソースの取得
				virtual Interface* GetBeforeITWindowDependentResource() const = 0;

			public:
				// 依存する自作ウィンドウの設定
				virtual void SetDependentITWindow(Window* const _window) = 0;
				// 次の自作ウィンドウ依存リソースの設定
				virtual void SetNextITWindowDependentResource(Interface* const _next) = 0;
				// 前の自作ウィンドウ依存リソースの設定
				virtual void SetBeforeITWindowDependentResource(Interface* const _before) = 0;

			public:
				// ウィンドウに読み込まれた
				virtual void OnWindowLoaded() = 0;
				// ウィンドウが閉じられた
				virtual void OnWindowClosed() = 0;
				// ウィンドウサイズが変更された
				virtual void OnWindowSizeChanged() = 0;
				// ウィンドウが最大化された
				virtual void OnWindowMaximized() = 0;
				// ウィンドウの最大化が解除された
				virtual void OnCancelOfWindowMaximized() = 0;
				// ウィンドウが最小化された
				virtual void OnWindowMinimized() = 0;
				// ウィンドウの最小化が解除された
				virtual void OnCancelOfWindowMinimized() = 0;
				// ウィンドウが中断された
				virtual void OnWindowSuspended() = 0;
				// ウィンドウが再開された
				virtual void OnWindowResumed() = 0;
				// ウィンドウが活性化した
				virtual void OnWindowActivated() = 0;
				// ウィンドウが非活性化した
				virtual void OnWindowDeactivated() = 0;
				// キーボードステートが変更された
				virtual void OnChangedKeyboardState(unsigned int _message, unsigned int _wParam, long _lParam) = 0;
				// マウスステートが変更された
				virtual void OnChangedMouseState(unsigned int _message, unsigned int _wParam, long _lParam) = 0;

			public:
				// リストから外れる
				virtual void RemoveITWindowDependentResourceList() = 0;

			protected:
				// 自作ウィンドウ依存リソースの初期化
				virtual void InitializeITWindowDependentResource() = 0;
			};
		}
	}
}

#endif
