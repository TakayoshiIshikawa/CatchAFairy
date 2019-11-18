//=============================================================================
//	ITCollisionShapes.h
//
//	����Փˌ`��̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_COLLISION_SHAPES_H__
#define __IT_COLLISION_SHAPES_H__

#include "ITFloatN.h"

namespace Ishikawa{
	namespace Math{
		namespace Collision{
			/// <summary>
			/// ����
			/// </summary>
			struct Line{
			public:
				/// <summary>��_</summary>
				Float3 origin;
				/// <summary>�����x�N�g��</summary>
				Float3 direction;


			public:
				/// <summary>�f�t�H���g�R���X�g���N�^</summary>
				Line():origin(),direction(){}
				/// <summary>�t���R���X�g���N�^</summary>
				/// <param name="_origin">�n�_</param>
				/// <param name="_direction">�����x�N�g��</param>
				Line(const Float3& _origin, const Float3& _direction) :
					origin(_origin),
					direction(_direction)
				{}
				/// <summary>�R���X�g���N�^</summary>
				/// <param name="_originX">�n�_��x���W</param>
				/// <param name="_originY">�n�_��y���W</param>
				/// <param name="_originZ">�n�_��z���W</param>
				/// <param name="_directionX">�����x�N�g����x���W</param>
				/// <param name="_directionY">�����x�N�g����y���W</param>
				/// <param name="_directionZ">�����x�N�g����z���W</param>
				Line(
					const float _originX, const float _originY, const float _originZ,
					const float _directionX, const float _directionY, const float _directionZ
				) :
					origin(_originX, _originY, _originZ),
					direction(_directionX, _directionY, _directionZ)
				{}
				/// <summary>�R�s�[�R���X�g���N�^</summary>
				/// <param name="_original">�R�s�[��</param>
				Line(const Line& _original) :
					origin(_original.origin),
					direction(_original.direction)
				{}
				/// <summary>�f�X�g���N�^</summary>
				~Line(){}

			public:
				/// <summary>������Z�q</summary>
				/// <param name="_original">�R�s�[��</param>
				/// <returns>���g�̃f�[�^</returns>
				Line& operator=(const Line& _original){
					this->origin = _original.origin;
					this->direction = _original.direction;
					return (*this);
				}

			public:
				/// <summary>���̒�����ɂ�����W�̎擾</summary>
				/// <param name="_ratio">�䗦</param>
				/// <returns>�Ή�������W</returns>
				Float3 GetPoint(const float _ratio) const {
					return (this->origin + this->direction*_ratio);
				}
			};



			/// <summary>
			/// ����
			/// </summary>
			struct LineSegment{
			public:
				/// <summary>�n�_</summary>
				Float3 start;
				/// <summary>�x�N�g��</summary>
				Float3 vector;


			public:
				/// <summary>�f�t�H���g�R���X�g���N�^</summary>
				LineSegment():start(),vector(){}
				/// <summary>�t���R���X�g���N�^</summary>
				/// <param name="_start">�n�_</param>
				/// <param name="_vector">�x�N�g��</param>
				LineSegment(const Float3& _start, const Float3& _vector) :
					start(_start),
					vector(_vector)
				{}
				/// <summary>�R���X�g���N�^</summary>
				/// <param name="_startX">�n�_��x���W</param>
				/// <param name="_startY">�n�_��y���W</param>
				/// <param name="_startZ">�n�_��z���W</param>
				/// <param name="_vectorX">�x�N�g����x���W</param>
				/// <param name="_vectorY">�x�N�g����y���W</param>
				/// <param name="_vectorZ">�x�N�g����z���W</param>
				LineSegment(
					const float _startX, const float _startY, const float _startZ,
					const float _vectorX, const float _vectorY, const float _vectorZ
				) :
					start(_startX, _startY, _startZ),
					vector(_vectorX, _vectorY, _vectorZ)
				{}
				/// <summary>�R�s�[�R���X�g���N�^</summary>
				/// <param name="_original">�R�s�[��</param>
				LineSegment(const LineSegment& _original) :
					start(_original.start),
					vector(_original.vector)
				{}
				/// <summary>�f�X�g���N�^</summary>
				~LineSegment(){}

			public:
				/// <summary>������Z�q</summary>
				/// <param name="_original">�R�s�[��</param>
				/// <returns>���g�̃f�[�^</returns>
				LineSegment& operator=(const LineSegment& _original){
					this->start = _original.start;
					this->vector = _original.vector;
					return (*this);
				}

			public:
				/// <summary>�I�_�̎擾</summary>
				/// <returns>�I�_</returns>
				Float3 GetEndPoint() const {
					return (this->start + this->vector);
				}

				/// <summary>���̐������܂ޒ����̎擾</summary>
				/// <returns>�Ή����钼��</returns>
				Line GetLine() const {
					return Line(this->start, this->vector);
				}

				/// <summary>���̐�����ɂ�����W�̎擾</summary>
				/// <param name="_ratio">�䗦</param>
				/// <returns>�Ή�������W [�䗦�̒l��0.0�`1.0�ɃN���b�s���O����܂�]</returns>
				Float3 GetPoint(const float _ratio) const {
					float ratio = (_ratio<0.0f)?(0.0f):((_ratio>1.0f)?(1.0f):(_ratio));
					return (this->start + this->vector*ratio);
				}
			};



			/// <summary>
			/// ����[��_�Ɩ@��]
			/// </summary>
			struct Planar3x2{
				/// <summary>��_</summary>
				Float3 origin;
				/// <summary>�@��</summary>
				Float3 normal;


			public:
				/// <summary>�f�t�H���g�R���X�g���N�^</summary>
				Planar3x2():origin(),normal(){}
				/// <summary>�t���R���X�g���N�^</summary>
				/// <param name="_origin">��_</param>
				/// <param name="_normal">�@��</param>
				Planar3x2(const Float3& _origin, const Float3& _normal) :
					origin(_origin),
					normal(_normal)
				{}
				/// <summary>�R���X�g���N�^</summary>
				/// <param name="_originX">��_��x���W</param>
				/// <param name="_originY">��_��y���W</param>
				/// <param name="_originZ">��_��z���W</param>
				/// <param name="_normalX">�@����x���W</param>
				/// <param name="_normalY">�@����y���W</param>
				/// <param name="_normalZ">�@����z���W</param>
				Planar3x2(
					const float _originX, const float _originY, const float _originZ,
					const float _normalX, const float _normalY, const float _normalZ
				) :
					origin(_originX, _originY, _originZ),
					normal(_normalX, _normalY, _normalZ)
				{}
				/// <summary>�R�s�[�R���X�g���N�^</summary>
				/// <param name="_original">�R�s�[��</param>
				Planar3x2(const Planar3x2& _original) :
					origin(_original.origin),
					normal(_original.normal)
				{}
				/// <summary>�f�X�g���N�^</summary>
				~Planar3x2(){}

			public:
				/// <summary>������Z�q</summary>
				/// <param name="_original">�R�s�[��</param>
				/// <returns>���g�̃f�[�^</returns>
				Planar3x2& operator=(const Planar3x2& _original){
					this->origin = _original.origin;
					this->normal = _original.normal;
					return (*this);
				}
			};

			/// <summary>
			/// ����[0=ax+by+cz+d]
			/// </summary>
			struct Planar4{
				/// <summary>x�̌W��</summary>
				float a;
				/// <summary>y�̌W��</summary>
				float b;
				/// <summary>z�̌W��</summary>
				float c;
				/// <summary>�o�C�A�X</summary>
				float d;


			public:
				/// <summary>�f�t�H���g�R���X�g���N�^</summary>
				Planar4():a(0.0f),b(0.0f),c(0.0f),d(0.0f){}
				/// <summary>�t���R���X�g���N�^</summary>
				/// <param name="_a">x�̌W��</param>
				/// <param name="_b">y�̌W��</param>
				/// <param name="_c">z�̌W��</param>
				/// <param name="_d">�o�C�A�X</param>
				Planar4(const float _a, const float _b, const float _c, const float _d) :
					a(_a),b(_b),c(_c),d(_d)
				{}
				/// <summary>�R�s�[�R���X�g���N�^</summary>
				/// <param name="_original">�R�s�[��</param>
				Planar4(const Planar4& _original) :
					a(_original.a),
					b(_original.b),
					c(_original.c),
					d(_original.d)
				{}
				/// <summary>�f�X�g���N�^</summary>
				~Planar4(){}

			public:
				/// <summary>������Z�q</summary>
				/// <param name="_original">�R�s�[��</param>
				/// <returns>���g�̃f�[�^</returns>
				Planar4& operator=(const Planar4& _original){
					this->a = _original.a;
					this->b = _original.b;
					this->c = _original.c;
					this->d = _original.d;
					return (*this);
				}
			};



			/// <summary>
			/// ��
			/// </summary>
			struct Sphere{
				/// <summary>��_</summary>
				Float3 origin;
				/// <summary>���a</summary>
				float radius;


			public:
				/// <summary>�f�t�H���g�R���X�g���N�^</summary>
				Sphere():origin(),radius(0.0f){}
				/// <summary>�t���R���X�g���N�^</summary>
				/// <param name="_origin">��_</param>
				/// <param name="_radius">���a</param>
				Sphere(const Float3& _origin, const float _radius) :
					origin(_origin),
					radius(_radius)
				{}
				/// <summary>�R���X�g���N�^</summary>
				/// <param name="_originX">��_��x���W</param>
				/// <param name="_originY">��_��y���W</param>
				/// <param name="_originZ">��_��z���W</param>
				/// <param name="_radius">���a</param>
				Sphere(
					const float _originX, const float _originY, const float _originZ,
					const float _radius
				) :
					origin(_originX, _originY, _originZ),
					radius(_radius)
				{}
				/// <summary>�R�s�[�R���X�g���N�^</summary>
				/// <param name="_original">�R�s�[��</param>
				Sphere(const Sphere& _original) :
					origin(_original.origin),
					radius(_original.radius)
				{}
				/// <summary>�f�X�g���N�^</summary>
				~Sphere(){}

			public:
				/// <summary>������Z�q</summary>
				/// <param name="_original">�R�s�[��</param>
				/// <returns>���g�̃f�[�^</returns>
				Sphere& operator=(const Sphere& _original){
					this->origin = _original.origin;
					this->radius = _original.radius;
					return (*this);
				}
			};



			/// <summary>
			/// �������~��
			/// </summary>
			struct InfiniteCylinder{
				/// <summary>��</summary>
				Line axis;
				/// <summary>���a</summary>
				float radius;


			public:
				/// <summary>�f�t�H���g�R���X�g���N�^</summary>
				InfiniteCylinder():axis(),radius(0.0f){}
				/// <summary>�t���R���X�g���N�^</summary>
				/// <param name="_axis">��</param>
				/// <param name="_radius">���a</param>
				InfiniteCylinder(const Line& _axis, const float _radius) :
					axis(_axis),
					radius(_radius)
				{}
				/// <summary>�t���R���X�g���N�^</summary>
				/// <param name="_origin">��_</param>
				/// <param name="_direction">�����x�N�g��</param>
				/// <param name="_radius">���a</param>
				InfiniteCylinder(const Float3& _origin, const Float3& _direction, const float _radius) :
					axis(_origin, _direction),
					radius(_radius)
				{}
				/// <summary>�R���X�g���N�^</summary>
				/// <param name="_originX">��_��x���W</param>
				/// <param name="_originY">��_��y���W</param>
				/// <param name="_originZ">��_��z���W</param>
				/// <param name="_directionX">�����x�N�g����x���W</param>
				/// <param name="_directionY">�����x�N�g����y���W</param>
				/// <param name="_directionZ">�����x�N�g����z���W</param>
				/// <param name="_radius">���a</param>
				InfiniteCylinder(
					const float _originX, const float _originY, const float _originZ,
					const float _directionX, const float _directionY, const float _directionZ,
					const float _radius
				) :
					axis(_originX, _originY, _originZ, _directionX, _directionY, _directionZ),
					radius(_radius)
				{}
				/// <summary>�R�s�[�R���X�g���N�^</summary>
				/// <param name="_original">�R�s�[��</param>
				InfiniteCylinder(const InfiniteCylinder& _original) :
					axis(_original.axis),
					radius(_original.radius)
				{}
				/// <summary>�f�X�g���N�^</summary>
				~InfiniteCylinder(){}

			public:
				/// <summary>������Z�q</summary>
				/// <param name="_original">�R�s�[��</param>
				/// <returns>���g�̃f�[�^</returns>
				InfiniteCylinder& operator=(const InfiniteCylinder& _original){
					this->axis = _original.axis;
					this->radius = _original.radius;
					return (*this);
				}
			};



			/// <summary>
			/// �J�v�Z��
			/// </summary>
			struct Capsule{
				/// <summary>������</summary>
				LineSegment axis;
				/// <summary>���a</summary>
				float radius;


			public:
				/// <summary>�f�t�H���g�R���X�g���N�^</summary>
				Capsule():axis(),radius(0.0f){}
				/// <summary>�t���R���X�g���N�^</summary>
				/// <param name="_axis">������</param>
				/// <param name="_radius">���a</param>
				Capsule(const LineSegment& _axis, const float _radius) :
					axis(_axis),
					radius(_radius)
				{}
				/// <summary>�t���R���X�g���N�^</summary>
				/// <param name="_start">�n�_</param>
				/// <param name="_vector">�x�N�g��</param>
				/// <param name="_radius">���a</param>
				Capsule(const Float3& _start, const Float3& _vector, const float _radius) :
					axis(_start, _vector),
					radius(_radius)
				{}
				/// <summary>�R���X�g���N�^</summary>
				/// <param name="_startX">�n�_��x���W</param>
				/// <param name="_startY">�n�_��y���W</param>
				/// <param name="_startZ">�n�_��z���W</param>
				/// <param name="_vectorX">�x�N�g����x���W</param>
				/// <param name="_vectorY">�x�N�g����y���W</param>
				/// <param name="_vectorZ">�x�N�g����z���W</param>
				/// <param name="_radius">���a</param>
				Capsule(
					const float _startX, const float _startY, const float _startZ,
					const float _vectorX, const float _vectorY, const float _vectorZ,
					const float _radius
				) :
					axis(_startX, _startY, _startZ, _vectorX, _vectorY, _vectorZ),
					radius(_radius)
				{}
				/// <summary>�R�s�[�R���X�g���N�^</summary>
				/// <param name="_original">�R�s�[��</param>
				Capsule(const Capsule& _original) :
					axis(_original.axis),
					radius(_original.radius)
				{}
				/// <summary>�f�X�g���N�^</summary>
				~Capsule(){}

			public:
				/// <summary>������Z�q</summary>
				/// <param name="_original">�R�s�[��</param>
				/// <returns>���g�̃f�[�^</returns>
				Capsule& operator=(const Capsule& _original){
					this->axis = _original.axis;
					this->radius = _original.radius;
					return (*this);
				}

			public:
				/// <summary>���̃J�v�Z��������Ɋ܂ޓ����a�̖������~�����擾����</summary>
				/// <returns>���̃J�v�Z��������Ɋ܂ޓ����a�̖������~��</returns>
				InfiniteCylinder GetInfiniteCylinder() const {
					return InfiniteCylinder(this->axis.start, this->axis.vector, this->radius);
				}
			};
		}
	}
}

#endif
