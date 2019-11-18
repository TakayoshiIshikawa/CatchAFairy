//=============================================================================
//	ITWindowDependentResourceListEdge.h
//
//	自作ウィンドウ依存リソースのリスト端のヘッダファイル
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
			/// 自作ウィンドウ依存リソースのリスト端
			/// </summary>
			class ListEdge final : public Interface{
			private:
				/// <summary>次の自作ウィンドウ依存リソース</summary>
				Interface* nextITWindowDependentResource;
				/// <summary>前の自作ウィンドウ依存リソース</summary>
				Interface* beforeITWindowDependentResource;


			public:
				// デフォルトコンストラクタ
				ListEdge();
				// デストラクタ
				virtual ~ListEdge() override final;

			public:
				/// <summary>自作ウィンドウ依存リソースリストの端か?</summary>
				/// <returns>常にtrue</returns>
				virtual bool IsITWindowDependentResourceListEdge() const override final { return true; }
				/// <summary>依存しているウィンドウの取得[出来ない]</summary>
				/// <returns>常にNULL</returns>
				virtual Window* GetDependentITWindow() const override final { return nullptr; }
				/// <summary>次の自作ウィンドウ依存リソースの取得</summary>
				/// <returns>次の自作ウィンドウ依存リソース</returns>
				virtual Interface* GetNextITWindowDependentResource() const override final {
					return this->nextITWindowDependentResource;
				}
				/// <summary>前の自作ウィンドウ依存リソースの取得</summary>
				/// <returns>前の自作ウィンドウ依存リソース</returns>
				virtual Interface* GetBeforeITWindowDependentResource() const override final {
					return this->beforeITWindowDependentResource;
				}

			public:
				/// <summary>依存する自作ウィンドウの設定[出来ない]</summary>
				virtual void SetDependentITWindow(Window* const) override final{}
				/// <summary>次の自作ウィンドウ依存リソースの設定</summary>
				/// <param name="_next">次の自作ウィンドウ依存リソース</param>
				virtual void SetNextITWindowDependentResource(Interface* const _next) override final {
					this->nextITWindowDependentResource = _next;
				}
				/// <summary>前の自作ウィンドウ依存リソースの設定</summary>
				/// <param name="_before">前の自作ウィンドウ依存リソース</param>
				virtual void SetBeforeITWindowDependentResource(Interface* const _before) override final{
					this->beforeITWindowDependentResource = _before;
				}

			public:
				/// <summary>ウィンドウに読み込まれた</summary>
				virtual void OnWindowLoaded() override final{}
				/// <summary>ウィンドウが閉じられた</summary>
				virtual void OnWindowClosed() override final{}
				/// <summary>ウィンドウサイズが変更された</summary>
				virtual void OnWindowSizeChanged() override final{}
				/// <summary>ウィンドウが最大化された</summary>
				virtual void OnWindowMaximized() override final{}
				/// <summary>ウィンドウの最大化が解除された</summary>
				virtual void OnCancelOfWindowMaximized() override final{}
				/// <summary>ウィンドウが最小化された</summary>
				virtual void OnWindowMinimized() override final{}
				/// <summary>ウィンドウの最小化が解除された</summary>
				virtual void OnCancelOfWindowMinimized() override final{}
				/// <summary>ウィンドウが中断された</summary>
				virtual void OnWindowSuspended() override final{}
				/// <summary>ウィンドウが再開された</summary>
				virtual void OnWindowResumed() override final{}
				/// <summary>ウィンドウが活性化した</summary>
				virtual void OnWindowActivated() override final{}
				/// <summary>ウィンドウが非活性化した</summary>
				virtual void OnWindowDeactivated() override final{}
				/// <summary>キーボードステートが変更された</summary>
				virtual void OnChangedKeyboardState(unsigned int, unsigned int, long) override final{}
				/// <summary>マウスステートが変更された</summary>
				virtual void OnChangedMouseState(unsigned int, unsigned int, long) override final{}

			public:
				/// <summary>リストから外れる[外れない]</summary>
				virtual void RemoveITWindowDependentResourceList() override final{}

				// 初期化
				void Initialize();

			protected:
				// 自作ウィンドウ依存リソースの初期化
				virtual void InitializeITWindowDependentResource() override final;
			};
		}
	}
}

#endif
