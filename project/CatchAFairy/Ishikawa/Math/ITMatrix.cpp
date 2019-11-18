//=============================================================================
//	ITMatrix.cpp
//
//	����s��̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITMatrix.h"
#include <float.h>
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

using namespace Ishikawa::Math;


/// <summary>�P�ʍs��</summary>
const Matrix Matrix::IDENTITY(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);


/// <summary>
/// �R�s�[�R���X�g���N�^
/// </summary>
/// <param name="_original">�I���W�i���f�[�^</param>
Matrix::Matrix(const ::DirectX::SimpleMath::Matrix& _original) :
	row1(_original.m[0]),
	row2(_original.m[1]),
	row3(_original.m[2]),
	row4(_original.m[3])
{
	// �������Ȃ�
}

/// <summary>
/// ������Z�q
/// </summary>
/// <param name="_original">�I���W�i���f�[�^</param>
/// <returns>���g�̃f�[�^</returns>
Matrix& Matrix::operator=(const ::DirectX::SimpleMath::Matrix& _original){
	this->_11 = _original._11;
	this->_12 = _original._12;
	this->_13 = _original._13;
	this->_14 = _original._14;
	this->_21 = _original._21;
	this->_22 = _original._22;
	this->_23 = _original._23;
	this->_24 = _original._24;
	this->_31 = _original._31;
	this->_32 = _original._32;
	this->_33 = _original._33;
	this->_34 = _original._34;
	this->_41 = _original._41;
	this->_42 = _original._42;
	this->_43 = _original._43;
	this->_44 = _original._44;
	return (*this);
}


/// <summary>
/// �t�s��
/// </summary>
/// <returns>�t�s��</returns>
Matrix Matrix::Invert() const {
	float mat11 =
		+(this->_22*this->_33*this->_44 + this->_23*this->_34*this->_42 + this->_24*this->_32*this->_43)
		-(this->_24*this->_33*this->_42 + this->_23*this->_32*this->_44 + this->_22*this->_34*this->_43);
	float mat12 =
		-(this->_12*this->_33*this->_44 + this->_13*this->_34*this->_42 + this->_14*this->_32*this->_43)
		+(this->_14*this->_33*this->_42 + this->_13*this->_32*this->_44 + this->_12*this->_34*this->_43);
	float mat13 =
		+(this->_12*this->_23*this->_44 + this->_13*this->_24*this->_42 + this->_14*this->_22*this->_43)
		-(this->_14*this->_23*this->_42 + this->_13*this->_22*this->_44 + this->_12*this->_24*this->_43);
	float mat14 =
		-(this->_12*this->_23*this->_34 + this->_13*this->_24*this->_32 + this->_14*this->_22*this->_33)
		+(this->_14*this->_23*this->_32 + this->_13*this->_22*this->_34 + this->_12*this->_24*this->_33);

	float matValue = this->_11*mat11 + this->_21*mat12 + this->_31*mat13 + this->_41*mat14;
	// �s��̒l��0�ɂȂ�ꍇ�͋t�s�񂪑��݂��Ȃ�
	if((matValue > -FLT_EPSILON) && (matValue < FLT_EPSILON)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Inverse matrix does not exist.");
	}

	float mat21 =
		-(this->_21*this->_33*this->_44 + this->_23*this->_34*this->_41 + this->_24*this->_31*this->_43)
		+(this->_24*this->_33*this->_41 + this->_23*this->_31*this->_44 + this->_21*this->_34*this->_43);
	float mat22 =
		+(this->_11*this->_33*this->_44 + this->_13*this->_34*this->_41 + this->_14*this->_31*this->_43)
		-(this->_14*this->_33*this->_41 + this->_13*this->_31*this->_44 + this->_11*this->_34*this->_43);
	float mat23 =
		-(this->_11*this->_23*this->_44 + this->_13*this->_24*this->_41 + this->_14*this->_21*this->_43)
		+(this->_14*this->_23*this->_41 + this->_13*this->_21*this->_44 + this->_11*this->_24*this->_43);
	float mat24 =
		+(this->_11*this->_23*this->_34 + this->_13*this->_24*this->_31 + this->_14*this->_21*this->_33)
		-(this->_14*this->_23*this->_31 + this->_13*this->_21*this->_34 + this->_11*this->_24*this->_33);
	float mat31 =
		+(this->_21*this->_32*this->_44 + this->_22*this->_34*this->_41 + this->_24*this->_31*this->_42)
		-(this->_24*this->_32*this->_41 + this->_22*this->_31*this->_44 + this->_21*this->_34*this->_42);
	float mat32 =
		-(this->_11*this->_32*this->_44 + this->_12*this->_34*this->_41 + this->_14*this->_31*this->_42)
		+(this->_14*this->_32*this->_41 + this->_12*this->_31*this->_44 + this->_11*this->_34*this->_42);
	float mat33 =
		+(this->_11*this->_22*this->_44 + this->_12*this->_24*this->_41 + this->_14*this->_21*this->_42)
		-(this->_14*this->_22*this->_41 + this->_12*this->_21*this->_44 + this->_11*this->_24*this->_42);
	float mat34 =
		-(this->_11*this->_22*this->_34 + this->_12*this->_24*this->_31 + this->_14*this->_21*this->_32)
		+(this->_14*this->_22*this->_31 + this->_12*this->_21*this->_34 + this->_11*this->_24*this->_32);
	float mat41 =
		-(this->_21*this->_32*this->_43 + this->_22*this->_33*this->_41 + this->_23*this->_31*this->_42)
		+(this->_23*this->_32*this->_41 + this->_22*this->_31*this->_43 + this->_21*this->_33*this->_42);
	float mat42 =
		+(this->_11*this->_32*this->_43 + this->_12*this->_33*this->_41 + this->_13*this->_31*this->_42)
		-(this->_13*this->_32*this->_41 + this->_12*this->_31*this->_43 + this->_11*this->_33*this->_42);
	float mat43 =
		-(this->_11*this->_22*this->_43 + this->_12*this->_23*this->_41 + this->_13*this->_21*this->_42)
		+(this->_13*this->_22*this->_41 + this->_12*this->_21*this->_43 + this->_11*this->_23*this->_42);
	float mat44 =
		+(this->_11*this->_22*this->_33 + this->_12*this->_23*this->_31 + this->_13*this->_21*this->_32)
		-(this->_13*this->_22*this->_31 + this->_12*this->_21*this->_33 + this->_11*this->_23*this->_32);

	float invMatVal = 1.0f / matValue;

	return Matrix(
		invMatVal*mat11, invMatVal*mat12, invMatVal*mat13, invMatVal*mat14,
		invMatVal*mat21, invMatVal*mat22, invMatVal*mat23, invMatVal*mat24, 
		invMatVal*mat31, invMatVal*mat32, invMatVal*mat33, invMatVal*mat34, 
		invMatVal*mat41, invMatVal*mat42, invMatVal*mat43, invMatVal*mat44 
	);
}



/// <summary>
/// �p�[�X�y�N�e�B�u�s�񐶐�
/// </summary>
/// <param name="_radian">�ˉe�p�x</param>
/// <param name="_aspect">�A�X�y�N�g��</param>
/// <param name="_nearClip">�j�A�N���b�v</param>
/// <param name="_farClip">�t�@�[�N���b�v</param>
/// <returns>�p�[�X�y�N�e�B�u�s��</returns>
Matrix Ishikawa::Math::CreatePerspectiveMatrix(float _radian, float _aspect, float _nearClip, float _farClip){
	float m22 = 1.0f / tanf(_radian / 2.0f);
	float m11 = _aspect * m22;
	float m33 = _farClip / (_farClip - _nearClip);
	float m43 = -_nearClip * m33;

	return Matrix(
		m11, 0.0f, 0.0f, 0.0f,
		0.0f,  m22, 0.0f, 0.0f,
		0.0f, 0.0f,  m33, 1.0f,
		0.0f, 0.0f,  m43, 0.0f
	);
}

/// <summary>
/// ���s���e�s��쐬
/// </summary>
/// <param name="_width">��</param>
/// <param name="_height">����</param>
/// <param name="_nearClip">�j�A�N���b�v</param>
/// <param name="_farClip">�t�@�[�N���b�v</param>
/// <returns>���s���e�s��</returns>
Matrix Ishikawa::Math::CreateOrthographicMatrix(float _width, float _height, float _nearClip, float _farClip){
	float m11 = 2.0f / _width;
	float m22 = 2.0f / _height;
	float m33 = 2.0f / (_farClip - _nearClip);
	float m43 = -_nearClip * m33;

	return Matrix(
		 m11, 0.0f, 0.0f, 0.0f,
		0.0f,  m22, 0.0f, 0.0f,
		0.0f, 0.0f,  m33, 0.0f,
		0.0f, 0.0f,  m43, 1.0f
	);
}

/// <summary>
/// ���s�ړ��s�񐶐�
/// </summary>
/// <param name="_x">X���ړ���</param>
/// <param name="_y">Y���ړ���</param>
/// <param name="_z">Z���ړ���</param>
/// <returns>���s�ړ��s��</returns>
Matrix Ishikawa::Math::CreateTranslationMatrix(float _x, float _y, float _z){
	Matrix scale(Matrix::IDENTITY);

	scale._41 = _x;
	scale._42 = _y;
	scale._43 = _z;

	return scale;
}

/// <summary>
/// X����]�s�񐶐�
/// </summary>
/// <param name="_radian">��]�p�x</param>
/// <returns>X����]�s��</returns>
Matrix Ishikawa::Math::CreateRotationMatrixOfAxisX(float _radian){
	Matrix rotation(Matrix::IDENTITY);

	float cosValue = cosf(_radian);
	float sinValue = sinf(_radian);
	rotation._22 = cosValue;
	rotation._23 = -sinValue;
	rotation._32 = sinValue;
	rotation._33 = cosValue;

	return rotation;
}

/// <summary>
/// Y����]�s�񐶐�
/// </summary>
/// <param name="_radian">��]�p�x</param>
/// <returns>Y����]�s��</returns>
Matrix Ishikawa::Math::CreateRotationMatrixOfAxisY(float _radian){
	Matrix rotation(Matrix::IDENTITY);

	float cosValue = cosf(_radian);
	float sinValue = sinf(_radian);
	rotation._11 = cosValue;
	rotation._13 = sinValue;
	rotation._31 = -sinValue;
	rotation._33 = cosValue;

	return rotation;
}

/// <summary>
/// Z����]�s�񐶐�
/// </summary>
/// <param name="_radian">��]�p�x</param>
/// <returns>Z����]�s��</returns>
Matrix Ishikawa::Math::CreateRotationMatrixOfAxisZ(float _radian){
	Matrix rotation(Matrix::IDENTITY);

	float cosValue = cosf(_radian);
	float sinValue = sinf(_radian);
	rotation._11 = cosValue;
	rotation._12 = -sinValue;
	rotation._21 = sinValue;
	rotation._22 = cosValue;

	return rotation;
}

/// <summary>
/// �g�k�s�񐶐�
/// </summary>
/// <param name="_x">X���g�k��</param>
/// <param name="_y">Y���g�k��</param>
/// <param name="_z">Z���g�k��</param>
/// <returns>�g�k�s��</returns>
Matrix Ishikawa::Math::CreateScaleMatrix(float _x, float _y, float _z){
	Matrix scale(Matrix::IDENTITY);

	scale._11 = _x;
	scale._22 = _y;
	scale._33 = _z;

	return scale;
}

/// <summary>
/// ���[���h�ϊ��s����r���[�ϊ��s��ɕϊ�����
/// </summary>
/// <param name="_view">�r���[�ϊ��s��ۑ���</param>
/// <param name="_world">���[���h�ϊ��s��</param>
void Ishikawa::Math::ConvertWorldMatrixToViewMatrix(Matrix* const _view, const Matrix& _world){
	if(_view == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_view is NULL.");
	}

	(*_view) = _world.Invert();
}
