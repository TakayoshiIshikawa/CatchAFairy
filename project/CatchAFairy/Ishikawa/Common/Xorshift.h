//=============================================================================
//	Xorshift.h
//
//	Xorshift乱数生成器のヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#ifndef __XORSHIFT_H__
#define __XORSHIFT_H__

#include "InterfaceRandom.h"


namespace Ishikawa{
	namespace Common{
		/// <summary>
		/// Xorshift乱数生成器
		/// </summary>
		class Xorshift final : public InterfaceRandom{
		private:
			/// <summary>X</summary>
			mutable unsigned long int x;
			/// <summary>Y</summary>
			mutable unsigned long int y;
			/// <summary>Z</summary>
			mutable unsigned long int z;
			/// <summary>W</summary>
			mutable unsigned long int w;


		public:
			// デフォルトコンストラクタ
			Xorshift();
			// コンストラクタ
			Xorshift(unsigned long int _seed);
			// コピーコンストラクタ
			Xorshift(const Xorshift& _original);
			// フルコンストラクタ
			Xorshift(
				unsigned long int _x,
				unsigned long int _y,
				unsigned long int _z,
				unsigned long int _w
			);
			// デストラクタ
			virtual ~Xorshift() override final;
		public:
			// 代入演算子
			Xorshift& operator=(const Xorshift& _original);

		public:
			// 乱数生成
			virtual unsigned long Random() const override final;
			// 乱数生成
			virtual double RealRandom() const override final;

		public:
			// リセット
			virtual void Reset(unsigned long _seed) override final;
		};
	}
}

#endif
