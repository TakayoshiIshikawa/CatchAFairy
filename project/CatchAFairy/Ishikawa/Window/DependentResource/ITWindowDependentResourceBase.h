//=============================================================================
//	ITWindowDependentResourceBase.h
//
//	自作ウィンドウ依存リソースの基底のヘッダファイル
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
			/// 自作ウィンドウ依存リソースの基底
			/// </summary>
			class Base : public Interface{
			private:
				/// <summary>依存しているウィンドウ</summary>
				Window* dependentITWindow;
				/// <summary>次の自作ウィンドウ依存リソース</summary>
				Interface* nextITWindowDependentResource;
				/// <summary>前の自作ウィンドウ依存リソース</summary>
				Interface* beforeITWindowDependentResource;


			public:
				// デフォルトコンストラクタ
				Base();
				// デストラクタ
				virtual ~Base() override;

			public:
				/// <summary>自作ウィンドウ依存リソースリストの端か?</summary>
				/// <returns>常にfalse</returns>
				virtual bool IsITWindowDependentResourceListEdge() const override final { return false; }
				/// <summary>依存しているウィンドウの取得[出来ない]</summary>
				/// <returns>依存しているウィンドウ</returns>
				virtual Window* GetDependentITWindow() const override final { return this->dependentITWindow; }
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
				/// <summary>依存する自作ウィンドウの設定</summary>
				/// <param name="_window">依存する自作ウィンドウ</param>
				virtual void SetDependentITWindow(Window* const _window) override final {
					this->dependentITWindow = _window;
				}
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
				// リストから外れる
				virtual void RemoveITWindowDependentResourceList() override final;

			protected:
				// 自作ウィンドウ依存リソースの初期化
				virtual void InitializeITWindowDependentResource() override final;
			};
		}
	}
}

#endif
