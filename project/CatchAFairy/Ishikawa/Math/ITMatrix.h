//=============================================================================
//	ITMatrix.h
//
//	自作行列のヘッダファイル
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
		/// 自作行列(4x4)
		/// </summary>
		struct Matrix final{
		public:
			// 単位行列
			static const Matrix IDENTITY;


		public:
#pragma warning(disable:4201)
			union{
				struct{
					/// <summary>成分:行1,列1</summary>
					float _11;
					/// <summary>成分:行1,列2</summary>
					float _12;
					/// <summary>成分:行1,列3</summary>
					float _13;
					/// <summary>成分:行1,列4</summary>
					float _14;
					/// <summary>成分:行2,列1</summary>
					float _21;
					/// <summary>成分:行2,列2</summary>
					float _22;
					/// <summary>成分:行2,列3</summary>
					float _23;
					/// <summary>成分:行2,列4</summary>
					float _24;
					/// <summary>成分:行3,列1</summary>
					float _31;
					/// <summary>成分:行3,列2</summary>
					float _32;
					/// <summary>成分:行3,列3</summary>
					float _33;
					/// <summary>成分:行3,列4</summary>
					float _34;
					/// <summary>成分:行4,列1</summary>
					float _41;
					/// <summary>成分:行4,列2</summary>
					float _42;
					/// <summary>成分:行4,列3</summary>
					float _43;
					/// <summary>成分:行4,列4</summary>
					float _44;
				};
				struct{
					/// <summary>行1</summary>
					Float4 row1;
					/// <summary>行2</summary>
					Float4 row2;
					/// <summary>行3</summary>
					Float4 row3;
					/// <summary>行4</summary>
					Float4 row4;
				};
				/// <summary>配列</summary>
				float m[4][4];
			};
#pragma warning(default:4201)

		public:
			/// <summary>デフォルトコンストラクタ</summary>
			Matrix():row1(),row2(),row3(),row4(){}
			/// <summary>フルコンストラクタ</summary>
			/// <param name="__11">成分:行1,列1</param>
			/// <param name="__12">成分:行1,列2</param>
			/// <param name="__13">成分:行1,列3</param>
			/// <param name="__14">成分:行1,列4</param>
			/// <param name="__21">成分:行2,列1</param>
			/// <param name="__22">成分:行2,列2</param>
			/// <param name="__23">成分:行2,列3</param>
			/// <param name="__24">成分:行2,列4</param>
			/// <param name="__31">成分:行3,列1</param>
			/// <param name="__32">成分:行3,列2</param>
			/// <param name="__33">成分:行3,列3</param>
			/// <param name="__34">成分:行3,列4</param>
			/// <param name="__41">成分:行4,列1</param>
			/// <param name="__42">成分:行4,列2</param>
			/// <param name="__43">成分:行4,列3</param>
			/// <param name="__44">成分:行4,列4</param>
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
			/// <summary>フルコンストラクタ</summary>
			/// <param name="_row1">行1</param>
			/// <param name="_row2">行2</param>
			/// <param name="_row3">行3</param>
			/// <param name="_row4">行4</param>
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
			/// <summary>フルコンストラクタ</summary>
			/// <param name="_m">行列</param>
			Matrix(const float (&_m)[4][4]) :
				row1(_m[1]),row2(_m[2]),row3(_m[3]),row4(_m[4])
			{
			}
			/// <summary>コピーコンストラクタ</summary>
			/// <param name="_original">オリジナルデータ</param>
			Matrix(const Matrix& _original) :
				row1(_original.row1),
				row2(_original.row2),
				row3(_original.row3),
				row4(_original.row4)
			{
			}
			// コピーコンストラクタ
			Matrix(const ::DirectX::SimpleMath::Matrix& _original);
#pragma warning(disable:4588)
			/// <summary>デストラクタ</summary>
			~Matrix(){}
#pragma warning(default:4588)

		public:
			/// <summary>代入演算子</summary>
			/// <param name="_original">オリジナルデータ</param>
			/// <returns>自身のデータ</returns>
			Matrix& operator=(const Matrix& _original){
				this->row1 = _original.row1;
				this->row2 = _original.row2;
				this->row3 = _original.row3;
				this->row4 = _original.row4;
				return (*this);
			}
			// 代入演算子
			Matrix& operator=(const ::DirectX::SimpleMath::Matrix& _original);
			/// <summary>加算</summary>
			/// <param name="_left">加算値</param>
			/// <returns>加算結果</returns>
			Matrix operator+(const Matrix& _left) const {
				return Matrix(
					this->row1 + _left.row1,
					this->row2 + _left.row2,
					this->row3 + _left.row3,
					this->row4 + _left.row4
				);
			}
			/// <summary>加算代入</summary>
			/// <param name="_left">加算値</param>
			/// <returns>自身のデータ</returns>
			Matrix& operator+=(const Matrix& _left){
				this->row1 += _left.row1;
				this->row2 += _left.row2;
				this->row3 += _left.row3;
				this->row4 += _left.row4;
				return (*this);
			}
			/// <summary>減算</summary>
			/// <param name="_left">減算値</param>
			/// <returns>減算結果</returns>
			Matrix operator-(const Matrix& _left) const {
				return Matrix(
					this->row1 - _left.row1,
					this->row2 - _left.row2,
					this->row3 - _left.row3,
					this->row4 - _left.row4
				);
			}
			/// <summary>減算代入</summary>
			/// <param name="_left">減算値</param>
			/// <returns>自身のデータ</returns>
			Matrix& operator-=(const Matrix& _left){
				this->row1 -= _left.row1;
				this->row2 -= _left.row2;
				this->row3 -= _left.row3;
				this->row4 -= _left.row4;
				return (*this);
			}
			/// <summary>スカラ倍</summary>
			/// <param name="_scalar">倍数</param>
			/// <returns>スカラ倍したベクトル</returns>
			Matrix operator*(float _scalar) const {
				return Matrix(
					this->row1 * _scalar,
					this->row2 * _scalar,
					this->row3 * _scalar,
					this->row4 * _scalar
				);
			}
			/// <summary>スカラ倍</summary>
			/// <param name="_scalar">倍数</param>
			/// <returns>自身のデータ</returns>
			Matrix& operator*=(float _scalar){
				this->row1 *= _scalar;
				this->row2 *= _scalar;
				this->row3 *= _scalar;
				this->row4 *= _scalar;
				return (*this);
			}
			/// <summary>
			/// ベクトルとの掛け算
			/// </summary>
			/// <param name="_vector">列ベクトル</param>
			/// <returns>計算したベクトル</returns>
			Float4 operator*(const Float4& _vector) const {
				return Float4(
					this->_11*_vector.x + this->_12*_vector.y + this->_13*_vector.z + this->_14*_vector.w,
					this->_21*_vector.x + this->_22*_vector.y + this->_23*_vector.z + this->_24*_vector.w,
					this->_31*_vector.x + this->_32*_vector.y + this->_33*_vector.z + this->_34*_vector.w,
					this->_41*_vector.x + this->_42*_vector.y + this->_43*_vector.z + this->_44*_vector.w
				);
			}
			/// <summary>
			/// 行列との掛け算
			/// </summary>
			/// <param name="_matrix">行列</param>
			/// <returns>計算した行列</returns>
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
			/// 行列との掛け算
			/// </summary>
			/// <param name="_matrix">行列</param>
			/// <returns>自身のデータ</returns>
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
			/// <summary>転置</summary>
			/// <returns>転置行列</returns>
			Matrix Transpose() const {
				return Matrix(
					this->_11, this->_21, this->_31, this->_41,
					this->_12, this->_22, this->_32, this->_42,
					this->_13, this->_23, this->_33, this->_43,
					this->_14, this->_24, this->_34, this->_44
				);
			}
			// 逆行列
			Matrix Invert() const;
		};


		// パースペクティブ行列生成
		Matrix CreatePerspectiveMatrix(float _radian, float _aspect, float _nearClip, float _farClip);
		// 平行投影行列作成
		Matrix CreateOrthographicMatrix(float _width, float _height, float _nearClip, float _farClip);
		// 平行移動行列生成
		Matrix CreateTranslationMatrix(float _x, float _y, float _z);
		// X軸回転行列生成
		Matrix CreateRotationMatrixOfAxisX(float _radian);
		// Y軸回転行列生成
		Matrix CreateRotationMatrixOfAxisY(float _radian);
		// Z軸回転行列生成
		Matrix CreateRotationMatrixOfAxisZ(float _radian);
		// 拡縮行列生成
		Matrix CreateScaleMatrix(float _x, float _y, float _z);
		// ワールド変換行列をビュー変換行列に変換する
		void ConvertWorldMatrixToViewMatrix(Matrix* const _view, const Matrix& _world);
	}
}

#endif
