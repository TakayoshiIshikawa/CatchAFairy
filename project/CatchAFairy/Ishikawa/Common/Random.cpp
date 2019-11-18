//=============================================================================
//	Random.cpp
//
//	乱数のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "Random.h"
#include "Xorshift.h"

namespace Ishikawa{
	namespace Common{
		namespace Random{
			namespace {
				/// <summary>利用乱数</summary>
				static Xorshift random;
			}

			/// <summary>
			/// 乱数生成
			/// </summary>
			/// <returns>整数値 範囲[0, 2^32)</returns>
			unsigned long Random(){
				return random.Random();
			}

			/// <summary>
			/// 乱数生成
			/// </summary>
			/// <returns>実数値 範囲[0.0, 1.0)</returns>
			double RealRandom(){
				return random.RealRandom();
			}


			/// <summary>
			/// リセット
			/// </summary>
			/// <param name="_seed">シード値</param>
			void Reset(unsigned long _seed){
				return random.Reset(_seed);
			}
		}
	}
}
