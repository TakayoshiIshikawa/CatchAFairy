//=============================================================================
//	ITMatrix.h
//
//	����s��̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_MATRIX_H__
#define __IT_MATRIX_H__

namespace DirectX{
	namespace SimpleMath{
		struct Matrix;
	}
}
#include "ITFloatN.h"


namespace Ishikawa{
	namespace Math{
		/// <summary>
		/// ����s��(4x4)
		/// </summary>
		struct Matrix final{
		public:
			// �P�ʍs��
			static const Matrix IDENTITY;


		public:
#pragma warning(disable:4201)
			union{
				struct{
					/// <summary>����:�s1,��1</summary>
					float _11;
					/// <summary>����:�s1,��2</summary>
					float _12;
					/// <summary>����:�s1,��3</summary>
					float _13;
					/// <summary>����:�s1,��4</summary>
					float _14;
					/// <summary>����:�s2,��1</summary>
					float _21;
					/// <summary>����:�s2,��2</summary>
					float _22;
					/// <summary>����:�s2,��3</summary>
					float _23;
					/// <summary>����:�s2,��4</summary>
					float _24;
					/// <summary>����:�s3,��1</summary>
					float _31;
					/// <summary>����:�s3,��2</summary>
					float _32;
					/// <summary>����:�s3,��3</summary>
					float _33;
					/// <summary>����:�s3,��4</summary>
					float _34;
					/// <summary>����:�s4,��1</summary>
					float _41;
					/// <summary>����:�s4,��2</summary>
					float _42;
					/// <summary>����:�s4,��3</summary>
					float _43;
					/// <summary>����:�s4,��4</summary>
					float _44;
				};
				struct{
					/// <summary>�s1</summary>
					Float4 row1;
					/// <summary>�s2</summary>
					Float4 row2;
					/// <summary>�s3</summary>
					Float4 row3;
					/// <summary>�s4</summary>
					Float4 row4;
				};
				/// <summary>�z��</summary>
				float m[4][4];
			};
#pragma warning(default:4201)

		public:
			/// <summary>�f�t�H���g�R���X�g���N�^</summary>
			Matrix():row1(),row2(),row3(),row4(){}
			/// <summary>�t���R���X�g���N�^</summary>
			/// <param name="__11">����:�s1,��1</param>
			/// <param name="__12">����:�s1,��2</param>
			/// <param name="__13">����:�s1,��3</param>
			/// <param name="__14">����:�s1,��4</param>
			/// <param name="__21">����:�s2,��1</param>
			/// <param name="__22">����:�s2,��2</param>
			/// <param name="__23">����:�s2,��3</param>
			/// <param name="__24">����:�s2,��4</param>
			/// <param name="__31">����:�s3,��1</param>
			/// <param name="__32">����:�s3,��2</param>
			/// <param name="__33">����:�s3,��3</param>
			/// <param name="__34">����:�s3,��4</param>
			/// <param name="__41">����:�s4,��1</param>
			/// <param name="__42">����:�s4,��2</param>
			/// <param name="__43">����:�s4,��3</param>
			/// <param name="__44">����:�s4,��4</param>
			Matrix(
				float __11, float __12, float __13, float __14,
				float __21, float __22, float __23, float __24,
				float __31, float __32, float __33, float __34,
				float __41, float __42, float __43, float __44
			) :
				row1(__11,__12,__13,__14),
				row2(__21,__22,__23,__24),
				row3(__31,__32,__33,__34),
				row4(__41,__42,__43,__44)
			{
			}
			/// <summary>�t���R���X�g���N�^</summary>
			/// <param name="_row1">�s1</param>
			/// <param name="_row2">�s2</param>
			/// <param name="_row3">�s3</param>
			/// <param name="_row4">�s4</param>
			Matrix(
				const Float4& _row1,
				const Float4& _row2,
				const Float4& _row3,
				const Float4& _row4
			) :
				row1(_row1),
				row2(_row2),
				row3(_row3),
				row4(_row4)
			{
			}
			/// <summary>�t���R���X�g���N�^</summary>
			/// <param name="_m">�s��</param>
			Matrix(const float (&_m)[4][4]) :
				row1(_m[1]),row2(_m[2]),row3(_m[3]),row4(_m[4])
			{
			}
			/// <summary>�R�s�[�R���X�g���N�^</summary>
			/// <param name="_original">�I���W�i���f�[�^</param>
			Matrix(const Matrix& _original) :
				row1(_original.row1),
				row2(_original.row2),
				row3(_original.row3),
				row4(_original.row4)
			{
			}
			// �R�s�[�R���X�g���N�^
			Matrix(const ::DirectX::SimpleMath::Matrix& _original);
#pragma warning(disable:4588)
			/// <summary>�f�X�g���N�^</summary>
			~Matrix(){}
#pragma warning(default:4588)

		public:
			/// <summary>������Z�q</summary>
			/// <param name="_original">�I���W�i���f�[�^</param>
			/// <returns>���g�̃f�[�^</returns>
			Matrix& operator=(const Matrix& _original){
				this->row1 = _original.row1;
				this->row2 = _original.row2;
				this->row3 = _original.row3;
				this->row4 = _original.row4;
				return (*this);
			}
			// ������Z�q
			Matrix& operator=(const ::DirectX::SimpleMath::Matrix& _original);
			/// <summary>���Z</summary>
			/// <param name="_left">���Z�l</param>
			/// <returns>���Z����</returns>
			Matrix operator+(const Matrix& _left) const {
				return Matrix(
					this->row1 + _left.row1,
					this->row2 + _left.row2,
					this->row3 + _left.row3,
					this->row4 + _left.row4
				);
			}
			/// <summary>���Z���</summary>
			/// <param name="_left">���Z�l</param>
			/// <returns>���g�̃f�[�^</returns>
			Matrix& operator+=(const Matrix& _left){
				this->row1 += _left.row1;
				this->row2 += _left.row2;
				this->row3 += _left.row3;
				this->row4 += _left.row4;
				return (*this);
			}
			/// <summary>���Z</summary>
			/// <param name="_left">���Z�l</param>
			/// <returns>���Z����</returns>
			Matrix operator-(const Matrix& _left) const {
				return Matrix(
					this->row1 - _left.row1,
					this->row2 - _left.row2,
					this->row3 - _left.row3,
					this->row4 - _left.row4
				);
			}
			/// <summary>���Z���</summary>
			/// <param name="_left">���Z�l</param>
			/// <returns>���g�̃f�[�^</returns>
			Matrix& operator-=(const Matrix& _left){
				this->row1 -= _left.row1;
				this->row2 -= _left.row2;
				this->row3 -= _left.row3;
				this->row4 -= _left.row4;
				return (*this);
			}
			/// <summary>�X�J���{</summary>
			/// <param name="_scalar">�{��</param>
			/// <returns>�X�J���{�����x�N�g��</returns>
			Matrix operator*(float _scalar) const {
				return Matrix(
					this->row1 * _scalar,
					this->row2 * _scalar,
					this->row3 * _scalar,
					this->row4 * _scalar
				);
			}
			/// <summary>�X�J���{</summary>
			/// <param name="_scalar">�{��</param>
			/// <returns>���g�̃f�[�^</returns>
			Matrix& operator*=(float _scalar){
				this->row1 *= _scalar;
				this->row2 *= _scalar;
				this->row3 *= _scalar;
				this->row4 *= _scalar;
				return (*this);
			}
			/// <summary>
			/// �x�N�g���Ƃ̊|���Z
			/// </summary>
			/// <param name="_vector">��x�N�g��</param>
			/// <returns>�v�Z�����x�N�g��</returns>
			Float4 operator*(const Float4& _vector) const {
				return Float4(
					this->_11*_vector.x + this->_12*_vector.y + this->_13*_vector.z + this->_14*_vector.w,
					this->_21*_vector.x + this->_22*_vector.y + this->_23*_vector.z + this->_24*_vector.w,
					this->_31*_vector.x + this->_32*_vector.y + this->_33*_vector.z + this->_34*_vector.w,
					this->_41*_vector.x + this->_42*_vector.y + this->_43*_vector.z + this->_44*_vector.w
				);
			}
			/// <summary>
			/// �s��Ƃ̊|���Z
			/// </summary>
			/// <param name="_matrix">�s��</param>
			/// <returns>�v�Z�����s��</returns>
			Matrix operator*(const Matrix& _matrix) const {
				return Matrix(
					this->_11*_matrix._11 + this->_12*_matrix._21 + this->_13*_matrix._31 + this->_14*_matrix._41,
					this->_11*_matrix._12 + this->_12*_matrix._22 + this->_13*_matrix._32 + this->_14*_matrix._42,
					this->_11*_matrix._13 + this->_12*_matrix._23 + this->_13*_matrix._33 + this->_14*_matrix._43,
					this->_11*_matrix._14 + this->_12*_matrix._24 + this->_13*_matrix._34 + this->_14*_matrix._44,
					this->_21*_matrix._11 + this->_22*_matrix._21 + this->_23*_matrix._31 + this->_24*_matrix._41,
					this->_21*_matrix._12 + this->_22*_matrix._22 + this->_23*_matrix._32 + this->_24*_matrix._42,
					this->_21*_matrix._13 + this->_22*_matrix._23 + this->_23*_matrix._33 + this->_24*_matrix._43,
					this->_21*_matrix._14 + this->_22*_matrix._24 + this->_23*_matrix._34 + this->_24*_matrix._44,
					this->_31*_matrix._11 + this->_32*_matrix._21 + this->_33*_matrix._31 + this->_34*_matrix._41,
					this->_31*_matrix._12 + this->_32*_matrix._22 + this->_33*_matrix._32 + this->_34*_matrix._42,
					this->_31*_matrix._13 + this->_32*_matrix._23 + this->_33*_matrix._33 + this->_34*_matrix._43,
					this->_31*_matrix._14 + this->_32*_matrix._24 + this->_33*_matrix._34 + this->_34*_matrix._44,
					this->_41*_matrix._11 + this->_42*_matrix._21 + this->_43*_matrix._31 + this->_44*_matrix._41,
					this->_41*_matrix._12 + this->_42*_matrix._22 + this->_43*_matrix._32 + this->_44*_matrix._42,
					this->_41*_matrix._13 + this->_42*_matrix._23 + this->_43*_matrix._33 + this->_44*_matrix._43,
					this->_41*_matrix._14 + this->_42*_matrix._24 + this->_43*_matrix._34 + this->_44*_matrix._44
				);
			}
			/// <summary>
			/// �s��Ƃ̊|���Z
			/// </summary>
			/// <param name="_matrix">�s��</param>
			/// <returns>���g�̃f�[�^</returns>
			Matrix& operator*=(const Matrix& _matrix){
				float __11 = this->_11*_matrix._11 + this->_12*_matrix._21 + this->_13*_matrix._31 + this->_14*_matrix._41;
				float __12 = this->_11*_matrix._12 + this->_12*_matrix._22 + this->_13*_matrix._32 + this->_14*_matrix._42;
				float __13 = this->_11*_matrix._13 + this->_12*_matrix._23 + this->_13*_matrix._33 + this->_14*_matrix._43;
				float __14 = this->_11*_matrix._14 + this->_12*_matrix._24 + this->_13*_matrix._34 + this->_14*_matrix._44;
				float __21 = this->_21*_matrix._11 + this->_22*_matrix._21 + this->_23*_matrix._31 + this->_24*_matrix._41;
				float __22 = this->_21*_matrix._12 + this->_22*_matrix._22 + this->_23*_matrix._32 + this->_24*_matrix._42;
				float __23 = this->_21*_matrix._13 + this->_22*_matrix._23 + this->_23*_matrix._33 + this->_24*_matrix._43;
				float __24 = this->_21*_matrix._14 + this->_22*_matrix._24 + this->_23*_matrix._34 + this->_24*_matrix._44;
				float __31 = this->_31*_matrix._11 + this->_32*_matrix._21 + this->_33*_matrix._31 + this->_34*_matrix._41;
				float __32 = this->_31*_matrix._12 + this->_32*_matrix._22 + this->_33*_matrix._32 + this->_34*_matrix._42;
				float __33 = this->_31*_matrix._13 + this->_32*_matrix._23 + this->_33*_matrix._33 + this->_34*_matrix._43;
				float __34 = this->_31*_matrix._14 + this->_32*_matrix._24 + this->_33*_matrix._34 + this->_34*_matrix._44;
				float __41 = this->_41*_matrix._11 + this->_42*_matrix._21 + this->_43*_matrix._31 + this->_44*_matrix._41;
				float __42 = this->_41*_matrix._12 + this->_42*_matrix._22 + this->_43*_matrix._32 + this->_44*_matrix._42;
				float __43 = this->_41*_matrix._13 + this->_42*_matrix._23 + this->_43*_matrix._33 + this->_44*_matrix._43;
				float __44 = this->_41*_matrix._14 + this->_42*_matrix._24 + this->_43*_matrix._34 + this->_44*_matrix._44;
				
				this->_11=__11; this->_12=__12; this->_13=__13; this->_14=__14;
				this->_21=__21; this->_22=__22; this->_23=__23; this->_24=__24;
				this->_31=__31; this->_32=__32; this->_33=__33; this->_34=__34;
				this->_41=__41; this->_42=__42; this->_43=__43; this->_44=__44;

				return (*this);
			}

		public:
			/// <summary>�]�u</summary>
			/// <returns>�]�u�s��</returns>
			Matrix Transpose() const {
				return Matrix(
					this->_11, this->_21, this->_31, this->_41,
					this->_12, this->_22, this->_32, this->_42,
					this->_13, this->_23, this->_33, this->_43,
					this->_14, this->_24, this->_34, this->_44
				);
			}
			// �t�s��
			Matrix Invert() const;
		};


		// �p�[�X�y�N�e�B�u�s�񐶐�
		Matrix CreatePerspectiveMatrix(float _radian, float _aspect, float _nearClip, float _farClip);
		// ���s���e�s��쐬
		Matrix CreateOrthographicMatrix(float _width, float _height, float _nearClip, float _farClip);
		// ���s�ړ��s�񐶐�
		Matrix CreateTranslationMatrix(float _x, float _y, float _z);
		// X����]�s�񐶐�
		Matrix CreateRotationMatrixOfAxisX(float _radian);
		// Y����]�s�񐶐�
		Matrix CreateRotationMatrixOfAxisY(float _radian);
		// Z����]�s�񐶐�
		Matrix CreateRotationMatrixOfAxisZ(float _radian);
		// �g�k�s�񐶐�
		Matrix CreateScaleMatrix(float _x, float _y, float _z);
		// ���[���h�ϊ��s����r���[�ϊ��s��ɕϊ�����
		void ConvertWorldMatrixToViewMatrix(Matrix* const _view, const Matrix& _world);
	}
}

#endif
