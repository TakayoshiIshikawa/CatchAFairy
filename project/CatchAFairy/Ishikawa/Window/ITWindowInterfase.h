//=============================================================================
//	ITWindowInterface.h
//
//	自作ウィンドウインターフェースのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_WINDOW_INTERFACE_H__
#define __IT_WINDOW_INTERFACE_H__

struct HINSTANCE__;
struct HWND__;
struct HMENU__;

namespace Ishikawa{
	namespace Window{
		namespace DependentResource{
			class Interface;
			class ListEdge;
		}
		class ListEdge;


		/// <summary>
		/// 自作ウィンドウインターフェース
		/// </summary>
		class Interface{
		public:
			/// <summary>デフォルトコンストラクタ</summary>
			Interface(){}
			/// <summary>デストラクタ</summary>
			virtual ~Interface(){}

		public:
			// 自作ウィンドウリストの端か?
			virtual bool IsITWindowListEdge() const = 0;
			// 次の自作ウィンドウの取得
			virtual Interface* GetNextITWindow() const = 0;
			// 前の自作ウィンドウの取得
			virtual Interface* GetBeforeITWindow() const = 0;
			// ハンドルの取得
			virtual HWND__* GetHandle() const = 0;
			// サイズの取得
			virtual void GetSize(long* const _width, long* const _height) const = 0;

		public:
			// 次の自作ウィンドウの設定
			virtual void SetNextITWindow(Interface* const _next) = 0;
			// 前の自作ウィンドウの設定
			virtual void SetBeforeITWindow(Interface* const _before) = 0;

		public:
			// リストから外れる
			virtual void RemoveITWindowList() = 0;

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
			) = 0;
			// 表示
			virtual void Show(int nCmdShow) = 0;
			// 終了する
			virtual void Exit() const = 0;

			// 依存リソースの追加(1つだけ)
			virtual void AddDependentResource(DependentResource::Interface* const _dependentResource) = 0;
			// 依存リソースの追加(循環リストごと)
			// 追加した後は元リストは空になります
			virtual void AddDependentResourceList(DependentResource::ListEdge* const _dependentResourceListEdge) = 0;

			// プロシージャ
			virtual bool Procedure(unsigned int message, unsigned int wParam, long lParam) = 0;

		protected:
			// 自作ウィンドウの初期化
			virtual void InitializeITWindow() = 0;
		};
	}
}

#endif
