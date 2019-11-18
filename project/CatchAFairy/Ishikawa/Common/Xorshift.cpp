//=============================================================================
//	Xorshift.cpp
//
//	Xorshift����������̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "Xorshift.h"

using namespace ::Ishikawa::Common;

/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Xorshift::Xorshift() :
	InterfaceRandom(),
	x(0UL),
	y(0UL),
	z(0UL),
	w(0UL)
{
	// �V�[�h�l 0�ŏ�����
	this->Reset(0UL);
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="_seed">�V�[�h�l</param>
Xorshift::Xorshift(unsigned long int _seed) :
	InterfaceRandom(),
	x(0UL),
	y(0UL),
	z(0UL),
	w(0UL)
{
	// �V�[�h�l�ŏ�����
	this->Reset(_seed);
}

/// <summary>
/// �R�s�[�R���X�g���N�^
/// </summary>
/// <param name="_original">���f�[�^</param>
Xorshift::Xorshift(const Xorshift& _original) :
	InterfaceRandom(),
	x(_original.x),
	y(_original.y),
	z(_original.z),
	w(_original.w)
{
	// �������Ȃ�
}

/// <summary>
/// �t���R���X�g���N�^
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
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Xorshift::~Xorshift(){
	// �������Ȃ�
}

/// <summary>
/// ������Z�q
/// </summary>
/// <param name="_original">���f�[�^</param>
/// <returns>���g�̃f�[�^</returns>
Xorshift& Xorshift::operator=(const Xorshift& _original){
	this->x = _original.x;
	this->y = _original.y;
	this->z = _original.z;
	this->w = _original.w;
	return (*this);
}


/// <summary>
/// ��������
/// </summary>
/// <returns>�����l �͈�[0, 2^32)</returns>
unsigned long int Xorshift::Random() const {
	unsigned long int tmp = this->x ^ (this->x << 11);
	this->x = this->y;
	this->y = this->z;
	this->z = this->w;
	this->w ^= tmp ^ (tmp>>8U) ^ (this->w>>19);
	return (this->w);
}

/// <summary>
/// ��������
/// </summary>
/// <returns>�����l �͈�[0.0, 1.0)</returns>
double Xorshift::RealRandom() const {
	unsigned long int tmp = this->x ^ (this->x << 11);
	this->x = this->y;
	this->y = this->z;
	this->z = this->w;
	this->w ^= tmp ^ (tmp>>8U) ^ (this->w>>19);
	return (((this->x+0.5)/4294967296.0 + this->w)/4294967296.0);
}


/// <summary>
/// ���Z�b�g
/// </summary>
/// <param name="_seed">�V�[�h�l</param>
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
