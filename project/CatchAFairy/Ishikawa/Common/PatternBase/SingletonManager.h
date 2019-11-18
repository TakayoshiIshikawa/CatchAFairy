//=============================================================================
//	SingletonManager.h
//
//	シングルトンマネージャのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SINGLETONMANAGER_H__
#define __SINGLETONMANAGER_H__

#include "WinMain.h"
#include "Ishikawa/Common/SafePointer.h"


namespace Ishikawa{
	namespace Common{
		class SingletonBase;


		/// <summary>
		/// シングルトンマネージャ
		/// </summary>
		class SingletonManager final{
		public:
			// このクラスのインスタンスを削除するためにフレンド化
			friend class SafePointer<SingletonManager>;
			// AddInstance()にアクセスするためにフレンド化
			friend class SingletonBase;
			// Finalize()にアクセスするためにフレンド化
			friend int Main::Execute(HINSTANCE__*, HINSTANCE__*, char*, int);


		private:
			// シングルトンインスタンス
			static SafePointer<SingletonManager> instance;

		private:
			/// <summary>最後のシングルトンインスタンス</summary>
			SingletonBase* lastSingletonInstance;


		public:
			// インスタンスの取得[他のシングルトンクラス]
			template<class SingletonClass>
			static SingletonClass& GetInstance();
		private:
			// インスタンスの取得[シングルトンマネージャ]
			static SingletonManager& GetInstance();

		private:
			// デフォルトコンストラクタ
			SingletonManager();
			// デストラクタ
			~SingletonManager();
		private:
			/// <summary>コピーコンストラクタ[削除]</summary>
			SingletonManager(const SingletonManager&) = delete;
			/// <summary>代入演算子[削除]</summary>
			SingletonManager& operator=(const SingletonManager&) = delete;

		private:
			// シングルトンインスタンスの追加
			void AddInstance(SingletonBase* const singleton);
			// 終了処理
			void Finalize();
		};
	}
}

#endif
