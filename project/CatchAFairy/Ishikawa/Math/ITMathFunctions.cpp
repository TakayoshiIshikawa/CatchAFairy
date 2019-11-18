//=============================================================================
//	ITMathFunctions.cpp
//
//	���쐔�w�֐��̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITMathFunctions.h"
#include <math.h>
#include "Ishikawa/Common/Exception/ITExceptions.h"

using namespace ::Ishikawa::Math;

/// <summary>
/// ������
/// </summary>
/// <param name="_value">����</param>
/// <returns>���͂̕�����</returns>
inline float Functions::SquareRoot(float _value){
	if(_value < 0.0f){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't get the square root of a negative value.");
	}
	return sqrtf(_value);
}
/// <summary>
/// ������
/// </summary>
/// <param name="_value">����</param>
/// <returns>���͂̕�����</returns>
inline double Functions::SquareRoot(double _value){
	if(_value < 0.0f){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't get the square root of a negative value.");
	}
	return sqrt(_value);
}
/// <summary>
/// ������
/// </summary>
/// <param name="_value">����</param>
/// <returns>���͂̕�����</returns>
inline long double Functions::SquareRoot(long double _value){
	if(_value < 0.0f){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't get the square root of a negative value.");
	}
	return sqrtl(_value);
}
