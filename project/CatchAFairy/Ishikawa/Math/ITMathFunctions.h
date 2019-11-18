//=============================================================================
//	ITMathFunctions.h
//
//	���쐔�w�֐��̃w�b�_�t�@�C��
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
			/// 2�l�̂����������ق���Ԃ��B('<'���g��)
			/// </summary>
			/// <param name="_a">�l1</param>
			/// <param name="_b">�l2</param>
			/// <returns>�������ق��̒l</returns>
			template<typename _TypeElement>
			inline _TypeElement Min(_TypeElement _a, _TypeElement _b){ return (_a < _b) ? (_a) : (_b); }
			/// <summary>
			/// 2�l�̂����傫���ق���Ԃ��B('<'���g��)
			/// </summary>
			/// <param name="_a">�l1</param>
			/// <param name="_b">�l2</param>
			/// <returns>�傫���ق��̒l</returns>
			template<typename _TypeElement>
			inline _TypeElement Max(_TypeElement _a, _TypeElement _b){ return (_a < _b) ? (_b) : (_a); }
			/// <summary>
			/// �l���N���b�s���O���ĕԂ��B('<'���g��)
			/// </summary>
			/// <param name="_value">�l</param>
			/// <param name="_min">�ŏ��l</param>
			/// <param name="_max">�ő�l</param>
			/// <returns>�N���b�s���O�����l</returns>
			template<typename _TypeElement>
			inline _TypeElement Clip(_TypeElement _value, _TypeElement _min, _TypeElement _max){
				return (_value < _min) ? (_min) : ((_max < _value) ? (_max) : (_value));
			}

			// ������
			extern inline float SquareRoot(float _value);
			// ������
			extern inline double SquareRoot(double _value);
			// ������
			extern inline long double SquareRoot(long double _value);
		}
	}
}

#endif
