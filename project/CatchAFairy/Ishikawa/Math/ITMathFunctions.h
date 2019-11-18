//=============================================================================
//	ITMathFunctions.h
//
//	自作数学関数のヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __MATH_FUNCTIONS_H__
#define __MATH_FUNCTIONS_H__

namespace Ishikawa{
	namespace Math{
		namespace Functions{
			/// <summary>
			/// 2値のうち小さいほうを返す。('<'を使う)
			/// </summary>
			/// <param name="_a">値1</param>
			/// <param name="_b">値2</param>
			/// <returns>小さいほうの値</returns>
			template<typename _TypeElement>
			inline _TypeElement Min(_TypeElement _a, _TypeElement _b){ return (_a < _b) ? (_a) : (_b); }
			/// <summary>
			/// 2値のうち大きいほうを返す。('<'を使う)
			/// </summary>
			/// <param name="_a">値1</param>
			/// <param name="_b">値2</param>
			/// <returns>大きいほうの値</returns>
			template<typename _TypeElement>
			inline _TypeElement Max(_TypeElement _a, _TypeElement _b){ return (_a < _b) ? (_b) : (_a); }
			/// <summary>
			/// 値をクリッピングして返す。('<'を使う)
			/// </summary>
			/// <param name="_value">値</param>
			/// <param name="_min">最小値</param>
			/// <param name="_max">最大値</param>
			/// <returns>クリッピングした値</returns>
			template<typename _TypeElement>
			inline _TypeElement Clip(_TypeElement _value, _TypeElement _min, _TypeElement _max){
				return (_value < _min) ? (_min) : ((_max < _value) ? (_max) : (_value));
			}

			// 平方根
			extern inline float SquareRoot(float _value);
			// 平方根
			extern inline double SquareRoot(double _value);
			// 平方根
			extern inline long double SquareRoot(long double _value);
		}
	}
}

#endif
