//=============================================================================
//	ITWindowListEdge.h
//
//	自作ウィンドウのリスト端のヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_WINDOW_LIST_EDGE_H__
#define __IT_WINDOW_LIST_EDGE_H__

#include "ITWindowInterfase.h"


namespace Ishikawa{
	namespace Window{
		/// <summary>
		/// 自作ウィンドウのリスト端
		/// </summary>
		class ListEdge final : public Interface{
		private:
			/// <summary>次の自作ウィンドウ</summary>
			Interface* nextITWindow;
			/// <summary>前の自作ウィンドウ</summary>
			Interface* beforeITWindow;


		public:
			// デフォルトコンストラクタ
			ListEdge();
			// デストラクタ
			virtual ~ListEdge() override final;

		public:
			/// <summary>自作ウィンドウリストの端か?</summary>
			/// <returns>常にtrue</returns>
			virtual bool IsITWindowListEdge() const override final { return true; }
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
			/// <summary>ハンドルの取得[出来ない]</summary>
			/// <returns>常にNULL</returns>
			virtual HWND__* GetHandle() const override final { return nullptr; }
			/// <summary>サイズの取得[出来ない]</summary>
			virtual void GetSize(long* const, long* const) const override final{}

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

		public:
			/// <summary>リストから外れる[]</summary>
			virtual void RemoveITWindowList() override final{}

			/// <summary>作成[しない]</summary>
			virtual void Create(
				const wchar_t* const,
				const wchar_t* const,
				const unsigned long,
				const unsigned long,
				const long,
				const long,
				const long,
				const long,
				HWND__* const,
				HMENU__* const,
				HINSTANCE__* const,
				void* const
			) override final{}
			/// <summary>表示[しない]</summary>
			virtual void Show(int) override final{}
			/// <summary>終了する[しない]</summary>
			virtual void Exit() const override final{}

			/// <summary>依存リソースの追加[しない](1つだけ)</summary>
			virtual void AddDependentResource(DependentResource::Interface* const) override final{}
			/// <summary>依存リソースの追加[しない](循環リストごと)</summary>
			virtual void AddDependentResourceList(DependentResource::ListEdge* const) override final{}

			/// <summary>プロシージャ[何もしない]</summary>
			/// <returns>デフォルトウィンドウプロシージャに返すためfalseのみ</returns>
			virtual bool Procedure(unsigned int, unsigned int, long) override final { return false; }

			// 初期化
			void Initialize();

		protected:
			// 自作ウィンドウの初期化
			virtual void InitializeITWindow() override final;
		};
	}
}

#endif
