//=============================================================================
//	InterfaceRandom.h
//
//	乱数生成器インターフェースのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#ifndef __INTERFACE_RANDOM_H__
#define __INTERFACE_RANDOM_H__

namespace Ishikawa{
	namespace Common{
		/// <summary>
		/// 乱数生成器インターフェース
		/// </summary>
		class InterfaceRandom{
		public:
			/// <summary>デフォルトコンストラクタ</summary>
			InterfaceRandom(){}
			/// <summary>デストラクタ</summary>
			virtual ~InterfaceRandom(){}

		public:
			// 乱数生成
			virtual unsigned long Random() const = 0;
			// 乱数生成
			virtual double RealRandom() const = 0;

		public:
			// リセット
			virtual void Reset(unsigned long _seed) = 0;
		};
	}
}

#endif
