//=============================================================================
//	ITWindow.h
//
//	自作ウィンドウのヘッダファイル
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
		/// 自作ウィンドウ
		/// </summary>
		class Window final : public Interface{
		private:
			// 中断中フラグ
			static const unsigned long WINDOW_SUSPENDING_FLAG;
			// 画面サイズ変更中フラグ
			static const unsigned long WINDOW_SIZE_MOVING_FLAG;
			// 最小化フラグ
			static const unsigned long WINDOW_MINIMIZING_FLAG;
			// 最大化フラグ
			static const unsigned long WINDOW_MAXIMIZING_FLAG;
			// フルスクリーンフラグ
			static const unsigned long WINDOW_FULLSCREEN_FLAG;

		private:
			/// <summary>次の自作ウィンドウ</summary>
			Interface* nextITWindow;
			/// <summary>前の自作ウィンドウ</summary>
			Interface* beforeITWindow;
			/// <summary>ハンドル</summary>
			HWND__* handle;
			/// <summary>スタイル</summary>
			unsigned long style;
			/// <summary>EXスタイル</summary>
			unsigned long exStyle;
			/// <summary>幅</summary>
			long width;
			/// <summary>高さ</summary>
			long height;
			/// <summary>最大化前の幅</summary>
			long widthOfBeforeMaximize;
			/// <summary>最大化前の高さ</summary>
			long heightOfBeforeMaximize;
			/// <summary>ウィンドウフラグ</summary>
			unsigned long windowFlag;
			/// <summary>このウィンドウに依存するリソースのリストの端</summary>
			DependentResource::ListEdge windowDependentResourceListEdge;


		public:
			// デフォルトコンストラクタ
			Window();
			// デストラクタ
			virtual ~Window();

		public:
			/// <summary>自作ウィンドウリストの端か?</summary>
			/// <returns>常にfalse</returns>
			virtual bool IsITWindowListEdge() const override final { return false; }
			/// <summary>次の自作ウィンドウ依存リソースの取得</summary>
			/// <returns>次の自作ウィンドウ依存リソース</returns>
			virtual Interface* GetNextITWindow() const override final {
				return this->nextITWindow;
			}
			/// <summary>前の自作ウィンドウ依存リソースの取得</summary>
			/// <returns>前の自作ウィンドウ依存リソース</returns>
			virtual Interface* GetBeforeITWindow() const override final {
				return this->beforeITWindow;
			}
			/// <summary>ハンドルの取得</summary>
			/// <returns>ハンドル</returns>
			virtual HWND__* GetHandle() const override final { return this->handle; }
			/// <summary>サイズの取得</summary>
			/// <param name="_width">幅の格納先</param>
			/// <param name="_height">高さの格納先</param>
			virtual void GetSize(long* const _width, long* const _height) const override final {
				if(_width != nullptr) (*_width) = this->width;
				if(_height != nullptr) (*_height) = this->height;
			}
			// 中断中か?
			bool IsSuspending() const;
			// 画面サイズ変更中か?
			bool IsSizeMoving() const;
			// 最小化中か?
			bool IsMinimizing() const;
			// 最大化中か?
			bool IsMaximizing() const;
			// フルスクリーン中か?
			bool IsFullscreen() const;

		public:
			/// <summary>次の自作ウィンドウの設定</summary>
			/// <param name="_next">次の自作ウィンドウ</param>
			virtual void SetNextITWindow(Interface* const _next) override final {
				this->nextITWindow = _next;
			}
			/// <summary>前の自作ウィンドウの設定</summary>
			/// <param name="_before">前の自作ウィンドウ</param>
			virtual void SetBeforeITWindow(Interface* const _before) override final {
				this->beforeITWindow = _before;
			}
			// 中断中フラグの設定
			void SetSuspendFlag(bool _flag);
			// 画面サイズ変更中フラグの設定
			void SetSizeMoveFlag(bool _flag);
			// 最小化中フラグの設定
			void SetMinimizeFlag(bool _flag);
			// 最大化中フラグの設定
			void SetMaximizeFlag(bool _flag);
			// フルスクリーン中フラグの設定
			void SetFullscreenFlag(bool _flag);

		public:
			// リストから外れる
			virtual void RemoveITWindowList() override final;

			// 作成
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
			// 表示
			virtual void Show(int nCmdShow) override final;
			// 終了する
			virtual void Exit() const override final;

			// 依存リソースの追加(1つだけ)
			virtual void AddDependentResource(DependentResource::Interface* const _dependentResource) override final;
			// 依存リソースの追加(循環リストごと)
			// 追加した後は元リストは空になります
			virtual void AddDependentResourceList(DependentResource::ListEdge* const _dependentResourceListEdge) override final;

			// プロシージャ
			virtual bool Procedure(unsigned int _message, unsigned int _wParam, long _lParam) override final;

			// 初期化
			void Initialize();

		protected:
			// 自作ウィンドウの初期化
			virtual void InitializeITWindow() override final;

		private:
			// 閉じる
			void Close();
			// 中断
			void Suspend();
			// 再開
			void Resume();
			// サイズ変更
			void ChangeSize(long _width, long _height);
			// 活性化
			void Activate();
			// 非活性化
			void Deactivate();
			// 最小化
			void Minimize();
			// 最小化解除
			void CancelOfMinimize();
			// 最大化
			void Maximize();
			// 最大化解除
			void CancelOfMaximize();
		};
	}
}

#endif
