//=============================================================================
//	Xorshift.cpp
//
//	Xorshift乱数生成器のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "Xorshift.h"

using namespace ::Ishikawa::Common;

/// <summary>
/// デフォルトコンストラクタ
/// </summary>
Xorshift::Xorshift() :
	InterfaceRandom(),
	x(0UL),
	y(0UL),
	z(0UL),
	w(0UL)
{
	// シード値 0で初期化
	this->Reset(0UL);
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="_seed">シード値</param>
Xorshift::Xorshift(unsigned long int _seed) :
	InterfaceRandom(),
	x(0UL),
	y(0UL),
	z(0UL),
	w(0UL)
{
	// シード値で初期化
	this->Reset(_seed);
}

/// <summary>
/// コピーコンストラクタ
/// </summary>
/// <param name="_original">元データ</param>
Xorshift::Xorshift(const Xorshift& _original) :
	InterfaceRandom(),
	x(_original.x),
	y(_original.y),
	z(_original.z),
	w(_original.w)
{
	// 何もしない
}

/// <summary>
/// フルコンストラクタ
/// </summary>
/// <param name="_x">X</param>
/// <param name="_y">Y</param>
/// <param name="_z">Z</param>
/// <param name="_w">W</param>
Xorshift::Xorshift(
	unsigned long int _x,
	unsigned long int _y,
	unsigned long int _z,
	unsigned long int _w
) :
	InterfaceRandom(),
	x(_x),
	y(_y),
	z(_z),
	w(_w)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Xorshift::~Xorshift(){
	// 何もしない
}

/// <summary>
/// 代入演算子
/// </summary>
/// <param name="_original">元データ</param>
/// <returns>自身のデータ</returns>
Xorshift& Xorshift::operator=(const Xorshift& _original){
	this->x = _original.x;
	this->y = _original.y;
	this->z = _original.z;
	this->w = _original.w;
	return (*this);
}


/// <summary>
/// 乱数生成
/// </summary>
/// <returns>整数値 範囲[0, 2^32)</returns>
unsigned long int Xorshift::Random() const {
	unsigned long int tmp = this->x ^ (this->x << 11);
	this->x = this->y;
	this->y = this->z;
	this->z = this->w;
	this->w ^= tmp ^ (tmp>>8U) ^ (this->w>>19);
	return (this->w);
}

/// <summary>
/// 乱数生成
/// </summary>
/// <returns>実数値 範囲[0.0, 1.0)</returns>
double Xorshift::RealRandom() const {
	unsigned long int tmp = this->x ^ (this->x << 11);
	this->x = this->y;
	this->y = this->z;
	this->z = this->w;
	this->w ^= tmp ^ (tmp>>8U) ^ (this->w>>19);
	return (((this->x+0.5)/4294967296.0 + this->w)/4294967296.0);
}


/// <summary>
/// リセット
/// </summary>
/// <param name="_seed">シード値</param>
void Xorshift::Reset(unsigned long int _seed){
	do{
		_seed = 1812433253*_seed + 1;
		_seed ^= _seed << 13;
		_seed ^= _seed >> 17;
		this->x = 123464980 ^ _seed;
		_seed = 1812433253*_seed + 1;
		_seed ^= _seed << 13;
		_seed ^= _seed >> 17;
		this->y = 3447902351 ^ _seed;
		_seed = 1812433253*_seed + 1;
		_seed ^= _seed << 13;
		_seed ^= _seed >> 17;
		this->z = 2859490775 ^ _seed;
		_seed = 1812433253*_seed + 1;
		_seed ^= _seed << 13;
		_seed ^= _seed >> 17;
		this->w = 47621719 ^ _seed;
	}while((this->x == 0UL) && (this->y == 0UL) && (this->z == 0UL) && (this->w == 0UL));
}
