//=============================================================================
//	ITWindowManager.h
//
//	自作ウィンドウマネージャのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_WINDOW_MANAGER_H__
#define __IT_WINDOW_MANAGER_H__

#include "Ishikawa/Common/PatternBase/SingletonBase.h"
#include "ITWindowListEdge.h"


namespace Ishikawa{
	namespace Window{
		/// <summary>
		/// 自作ウィンドウマネージャ
		/// </summary>
		class Manager final : public ::Ishikawa::Common::SingletonBase{
		public:
			// GetInstance<Manager>()でアクセスするためフレンド化
			friend class Ishikawa::Common::SingletonManager;


		private:
			// シングルトンインスタンス
			static Manager* instance;

		private:
			/// <summary>ウィンドウリストの端</summary>
			ListEdge windowListEdge;


		public:
			// デフォルトコンストラクタ
			Manager();
			// デストラクタ
			virtual ~Manager() override final;
		private:
			/// <summary>コピーコンストラクタ[削除]</summary>
			Manager(const Manager&) = delete;
			/// <summary>代入演算子[削除]</summary>
			Manager& operator=(const Manager&) = delete;

		public:
			/// <summary>ウィンドウリストの先頭の取得</summary>
			/// <returns>ウィンドウリストの先頭</returns>
			Interface* GetHeadWindow() const { return this->windowListEdge.GetNextITWindow(); }
			/// <summary>ウィンドウリストの終端の取得</summary>
			/// <returns>ウィンドウリストの終端</returns>
			Interface* GetTailWindow() const { return this->windowListEdge.GetBeforeITWindow(); }

		public:
			// ウィンドウの追加
			void AddWindow(Interface* const _window);

		private:
			// 初期化
			void Initialize();
		};
	}
}

#endif
