//=============================================================================
//	ITMathFunctions.cpp
//
//	自作数学関数のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITMathFunctions.h"
#include <math.h>
#include "Ishikawa/Common/Exception/ITExceptions.h"

using namespace ::Ishikawa::Math;

/// <summary>
/// 平方根
/// </summary>
/// <param name="_value">入力</param>
/// <returns>入力の平方根</returns>
inline float Functions::SquareRoot(float _value){
	if(_value < 0.0f){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't get the square root of a negative value.");
	}
	return sqrtf(_value);
}
/// <summary>
/// 平方根
/// </summary>
/// <param name="_value">入力</param>
/// <returns>入力の平方根</returns>
inline double Functions::SquareRoot(double _value){
	if(_value < 0.0f){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't get the square root of a negative value.");
	}
	return sqrt(_value);
}
/// <summary>
/// 平方根
/// </summary>
/// <param name="_value">入力</param>
/// <returns>入力の平方根</returns>
inline long double Functions::SquareRoot(long double _value){
	if(_value < 0.0f){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't get the square root of a negative value.");
	}
	return sqrtl(_value);
}
