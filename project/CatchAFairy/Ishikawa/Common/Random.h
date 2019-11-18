//=============================================================================
//	Random.h
//
//	乱数のヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#ifndef __RANDOM_H__
#define __RANDOM_H__

namespace Ishikawa{
	namespace Common{
		/// <summary>
		/// 乱数
		/// </summary>
		namespace Random{
			// 乱数生成
			unsigned long Random();
			// 乱数生成
			double RealRandom();

			// リセット
			void Reset(unsigned long _seed);
		};
	}
}

#endif
