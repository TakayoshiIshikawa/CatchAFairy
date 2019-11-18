//=============================================================================
//	SingletonBase.h
//
//	シングルトンの基底のヘッダファイル
//
//	Copyright(c) 2018 - 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SINGLETONBASE_H__
#define __SINGLETONBASE_H__

namespace Ishikawa{
	namespace Common{
		/// <summary>
		/// シングルトンの基底
		/// </summary>
		class SingletonBase{
		public:
			// マネージャでアクセスするためにフレンド化
			friend class SingletonManager;


		private:
			/// <summary>前のシングルトンインスタンス</summary>
			SingletonBase* beforeSingleton;


		protected:
			// デフォルトコンストラクタ
			SingletonBase();
			// デストラクタ
			virtual ~SingletonBase();
		private:
			/// <summary>コピーコンストラクタ[削除]</summary>
			SingletonBase(const SingletonBase&) = delete;
			/// <summary>代入演算子[削除]</summary>
			SingletonBase& operator=(const SingletonBase&) = delete;

		private:
			/// <summary>前のシングルトンインスタンスの取得</summary>
			/// <returns>前のシングルトンインスタンス</returns>
			SingletonBase* GetBeforeSingleton() const { return this->beforeSingleton; }
	
		private:
			/// <summary>前のシングルトンインスタンスの設定</summary>
			/// <param name="before">前のシングルトンインスタンス</param>
			void SetBeforeSingleton(SingletonBase* const before){ this->beforeSingleton = before; }
		};
	}
}

#endif
