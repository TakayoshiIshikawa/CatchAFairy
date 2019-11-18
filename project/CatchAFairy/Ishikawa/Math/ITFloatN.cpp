//=============================================================================
//	ITFloatN.cpp
//
//	自作n次元float型実数ベクトルのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITFloatN.h"
#include <math.h>
#pragma warning(disable:4061)
#pragma warning(disable:4365)
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#pragma warning(disable:4917)
#include <d3d11_1.h>
#pragma warning(default:4917)
#pragma warning(default:4820)
#pragma warning(default:4668)
#pragma warning(default:4365)
#pragma warning(default:4061)
#pragma warning(disable:4548)
#pragma warning(disable:4668)
#pragma warning(disable:5029)
#include "DirectXTK/Inc/SimpleMath.h"
#pragma warning(default:5029)
#pragma warning(default:4668)
#pragma warning(default:4548)
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "ITMatrix.h"

using namespace Ishikawa::Math;


/// <summary>ゼロベクトル</summary>
const Float2 Float2::ZERO(0.0f, 0.0f);
/// <summary>X軸方向ベクトル</summary>
const Float2 Float2::UNIT_X(1.0f, 0.0f);
/// <summary>Y軸方向ベクトル</summary>
const Float2 Float2::UNIT_Y(0.0f, 1.0f);
/// <summary>全て1.0のベクトル</summary>
const Float2 Float2::ONE(1.0f, 1.0f);

/// <summary>
/// コピーコンストラクタ
/// </summary>
/// <param name="_original">オリジナルデータ</param>
Float2::Float2(const ::DirectX::SimpleMath::Vector2& _original) :
	x(_original.x),
	y(_original.y)
{
	// 何もしない
}

/// <summary>
/// 代入演算子
/// </summary>
/// <param name="_original">オリジナルデータ</param>
/// <returns>自身のデータ</returns>
Float2& Float2::operator=(const ::DirectX::SimpleMath::Vector2& _original){
	this->x = _original.x;
	this->y = _original.y;
	return (*this);
}

/// <summary>
/// 長さ
/// </summary>
float Float2::Length() const {
	return sqrtf(this->x*this->x + this->y*this->y);
}


/// <summary>
/// 正規化
/// </summary>
void Float2::Normalize(){
	float length = this->Length();
	if(length < FLT_EPSILON){
		(*this) = ZERO;
		return;
	}
	(*this) *= (1.0f / length);
}
/// <summary>
/// 正規化
/// </summary>
/// <param name="_normalizedVector">正規化ベクトルの格納先</param>
void Float2::Normalize(Float2* const _normalizedVector) const {
	if(_normalizedVector == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_normalizedVector is NULL.");
	}

	float length = this->Length();
	if(length < FLT_EPSILON){
		(*_normalizedVector) = ZERO;
		return;
	}
	(*_normalizedVector) = (*this) * (1.0f / length);
}


/// <summary>ゼロベクトル</summary>
const Float3 Float3::ZERO(0.0f, 0.0f, 0.0f);
/// <summary>X軸方向ベクトル</summary>
const Float3 Float3::UNIT_X(1.0f, 0.0f, 0.0f);
/// <summary>Y軸方向ベクトル</summary>
const Float3 Float3::UNIT_Y(0.0f, 1.0f, 0.0f);
/// <summary>Z軸方向ベクトル</summary>
const Float3 Float3::UNIT_Z(0.0f, 0.0f, 1.0f);
/// <summary>全て1.0のベクトル</summary>
const Float3 Float3::ONE(1.0f, 1.0f, 1.0f);

/// <summary>
/// コピーコンストラクタ
/// </summary>
/// <param name="_original">オリジナルデータ</param>
Float3::Float3(const ::DirectX::SimpleMath::Vector3& _original) :
	x(_original.x),
	y(_original.y),
	z(_original.z)
{
	// 何もしない
}

/// <summary>
/// 代入演算子
/// </summary>
/// <param name="_original">オリジナルデータ</param>
/// <returns>自身のデータ</returns>
Float3& Float3::operator=(const ::DirectX::SimpleMath::Vector3& _original){
	this->x = _original.x;
	this->y = _original.y;
	this->z = _original.z;
	return (*this);
}

/// <summary>
/// 長さ
/// </summary>
float Float3::Length() const {
	return sqrtf(this->x*this->x + this->y*this->y + this->z*this->z);
}


/// <summary>
/// 正規化
/// </summary>
void Float3::Normalize(){
	float length = this->Length();
	if(length < FLT_EPSILON){
		(*this) = ZERO;
		return;
	}
	(*this) *= (1.0f / length);
}
/// <summary>
/// 正規化
/// </summary>
/// <param name="_normalizedVector">正規化ベクトルの格納先</param>
void Float3::Normalize(Float3* const _normalizedVector) const {
	if(_normalizedVector == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_normalizedVector is NULL.");
	}

	float length = this->Length();
	if(length < FLT_EPSILON){
		(*_normalizedVector) = ZERO;
		return;
	}
	(*_normalizedVector) = (*this) * (1.0f / length);
}


/// <summary>ゼロベクトル</summary>
const Float4 Float4::ZERO(0.0f, 0.0f, 0.0f, 0.0f);
/// <summary>X軸方向ベクトル</summary>
const Float4 Float4::UNIT_X(1.0f, 0.0f, 0.0f, 0.0f);
/// <summary>Y軸方向ベクトル</summary>
const Float4 Float4::UNIT_Y(0.0f, 1.0f, 0.0f, 0.0f);
/// <summary>Z軸方向ベクトル</summary>
const Float4 Float4::UNIT_Z(0.0f, 0.0f, 1.0f, 0.0f);
/// <summary>W軸方向ベクトル</summary>
const Float4 Float4::UNIT_W(0.0f, 0.0f, 0.0f, 1.0f);
/// <summary>全て1.0のベクトル</summary>
const Float4 Float4::ONE(1.0f, 1.0f, 1.0f, 1.0f);

/// <summary>
/// コピーコンストラクタ
/// </summary>
/// <param name="_original">オリジナルデータ</param>
Float4::Float4(const ::DirectX::SimpleMath::Vector4& _original) :
	x(_original.x),
	y(_original.y),
	z(_original.z),
	w(_original.w)
{
	// 何もしない
}

/// <summary>
/// 代入演算子
/// </summary>
/// <param name="_original">オリジナルデータ</param>
/// <returns>自身のデータ</returns>
Float4& Float4::operator=(const ::DirectX::SimpleMath::Vector4& _original){
	this->x = _original.x;
	this->y = _original.y;
	this->z = _original.z;
	this->w = _original.w;
	return (*this);
}

/// <summary>
/// 行列との掛け算
/// </summary>
/// <param name="_matrix">行列</param>
/// <returns>計算されたベクトル</returns>
Float4 Float4::operator*(const Matrix& _matrix) const {
	return Float4(
		this->x*_matrix._11 + this->y*_matrix._21 + this->z*_matrix._31 + this->w*_matrix._41,
		this->x*_matrix._12 + this->y*_matrix._22 + this->z*_matrix._32 + this->w*_matrix._42,
		this->x*_matrix._13 + this->y*_matrix._23 + this->z*_matrix._33 + this->w*_matrix._43,
		this->x*_matrix._14 + this->y*_matrix._24 + this->z*_matrix._34 + this->w*_matrix._44
	);
}

/// <summary>
/// 行列との掛け算
/// </summary>
/// <param name="_matrix">行列</param>
/// <returns>自身のデータ</returns>
Float4& Float4::operator*=(const Matrix& _matrix){
	float x_ = this->x*_matrix._11 + this->y*_matrix._21 + this->z*_matrix._31 + this->w*_matrix._41;
	float y_ = this->x*_matrix._12 + this->y*_matrix._22 + this->z*_matrix._32 + this->w*_matrix._42;
	float z_ = this->x*_matrix._13 + this->y*_matrix._23 + this->z*_matrix._33 + this->w*_matrix._43;
	float w_ = this->x*_matrix._14 + this->y*_matrix._24 + this->z*_matrix._34 + this->w*_matrix._44;

	this->x = x_;
	this->y = y_;
	this->z = z_;
	this->w = w_;

	return (*this);
}

/// <summary>
/// 長さ
/// </summary>
float Float4::Length() const {
	return sqrtf(this->x*this->x + this->y*this->y + this->z*this->z + this->w*this->w);
}


/// <summary>
/// 正規化
/// </summary>
void Float4::Normalize(){
	float length = this->Length();
	if(length < FLT_EPSILON){
		(*this) = ZERO;
		return;
	}
	(*this) *= (1.0f / length);
}
/// <summary>
/// 正規化
/// </summary>
/// <param name="_normalizedVector">正規化ベクトルの格納先</param>
void Float4::Normalize(Float4* const _normalizedVector) const {
	if(_normalizedVector == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_normalizedVector is NULL.");
	}

	float length = this->Length();
	if(length < FLT_EPSILON){
		(*_normalizedVector) = ZERO;
		return;
	}
	(*_normalizedVector) = (*this) * (1.0f / length);
}
