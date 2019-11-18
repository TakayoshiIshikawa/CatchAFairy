//=============================================================================
//	ITFloatN.h
//
//	����n����float�^�����x�N�g���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_FLOAT_N_H__
#define __IT_FLOAT_N_H__

namespace DirectX{
	namespace SimpleMath{
		struct Vector2;
		struct Vector3;
		struct Vector4;
	}
}

namespace Ishikawa{
	namespace Math{
		struct Matrix;


		/// <summary>
		/// 2����float�^�����x�N�g��
		/// </summary>
		struct Float2 final{
		public:
			// �[���x�N�g��
			static const Float2 ZERO;
			// X�������x�N�g��
			static const Float2 UNIT_X;
			// Y�������x�N�g��
			static const Float2 UNIT_Y;
			// �S��1.0�̃x�N�g��
			static const Float2 ONE;


		public:
#pragma warning(disable:4201)
			union{
				struct{
					/// <summary>X����</summary>
					float x;
					/// <summary>Y����</summary>
					float y;
				};
				/// <summary>�z��</summary>
				float v[2];
			};
#pragma warning(default:4201)


		public:
			/// <summary>�f�t�H���g�R���X�g���N�^</summary>
			Float2():x(0.0f),y(0.0f){}
			/// <summary>�t���R���X�g���N�^</summary>
			/// <param name="_x">X����</param>
			/// <param name="_y">Y����</param>
			Float2(const float _x, const float _y):x(_x),y(_y){}
			/// <summary>�t���R���X�g���N�^</summary>
			/// <param name="_v">�z��</param>
			Float2(const float (&_v)[2]):v{_v[0],_v[1]}{}
			/// <summary>�R�s�[�R���X�g���N�^</summary>
			/// <param name="_original">�I���W�i���f�[�^</param>
			Float2(const Float2& _original):x(_original.x),y(_original.y){}
			// �R�s�[�R���X�g���N�^
			Float2(const ::DirectX::SimpleMath::Vector2& _original);
			/// <summary>�f�X�g���N�^</summary>
			~Float2(){}

		public:
			/// <summary>������Z�q</summary>
			/// <param name="_original">�I���W�i���f�[�^</param>
			/// <returns>���g�̃f�[�^</returns>
			Float2& operator=(const Float2& _original){
				this->x = _original.x;
				this->y = _original.y;
				return (*this);
			}
			// ������Z�q
			Float2& operator=(const ::DirectX::SimpleMath::Vector2& _original);
			/// <summary>���Z</summary>
			/// <param name="_left">���Z�l</param>
			/// <returns>���Z����</returns>
			Float2 operator+(const Float2& _left) const {
				return Float2(this->x+_left.x, this->y+_left.y);
			}
			/// <summary>���Z���</summary>
			/// <param name="_left">���Z�l</param>
			/// <returns>���g�̃f�[�^</returns>
			Float2& operator+=(const Float2& _left){
				this->x += _left.x;
				this->y += _left.y;
				return (*this);
			}
			/// <summary>���Z</summary>
			/// <param name="_left">���Z�l</param>
			/// <returns>���Z����</returns>
			Float2 operator-(const Float2& _left) const {
				return Float2(this->x-_left.x, this->y-_left.y);
			}
			/// <summary>���Z���</summary>
			/// <param name="_left">���Z�l</param>
			/// <returns>���g�̃f�[�^</returns>
			Float2& operator-=(const Float2& _left){
				this->x -= _left.x;
				this->y -= _left.y;
				return (*this);
			}
			/// <summary>�X�J���{</summary>
			/// <param name="_scalar">�{��</param>
			/// <returns>�X�J���{�����x�N�g��</returns>
			Float2 operator*(const float _scalar) const {
				return Float2(
					_scalar * this->x,
					_scalar * this->y
				);
			}
			/// <summary>�X�J���{</summary>
			/// <param name="_scalar">�{��</param>
			/// <returns>���g�̃f�[�^</returns>
			Float2& operator*=(const float _scalar){
				this->x *= _scalar;
				this->y *= _scalar;
				return (*this);
			}

		public:
			/// <summary>����</summary>
			/// <param name="_vector">�x�N�g��</param>
			/// <returns>����</returns>
			float Dot(const Float2& _vector) const {
				return (this->x*_vector.x + this->y*_vector.y);
			}
			/// <summary>�O��</summary>
			/// <param name="_vector">�x�N�g��</param>
			/// <returns>�O��</returns>
			float Cross(const Float2& _vector) const {
				return (this->x*_vector.y - this->y*_vector.x);
			}
			/// <summary>������2��</summary>
			/// <returns>������2��</returns>
			float LengthSquared() const {
				return (this->x*this->x + this->y*this->y);
			}
			// ����
			float Length() const;
			// ���K��
			void Normalize();
			// ���K��
			void Normalize(Float2* const _normalizedVector) const;
		};

		/// <summary>
		/// 3����float�^�����x�N�g��
		/// </summary>
		struct Float3 final{
		public:
			// �[���x�N�g��
			static const Float3 ZERO;
			// X�������x�N�g��
			static const Float3 UNIT_X;
			// Y�������x�N�g��
			static const Float3 UNIT_Y;
			// Z�������x�N�g��
			static const Float3 UNIT_Z;
			// �S��1.0�̃x�N�g��
			static const Float3 ONE;


		public:
#pragma warning(disable:4201)
			union{
				struct{
					/// <summary>X����</summary>
					float x;
					/// <summary>Y����</summary>
					float y;
					/// <summary>Z����</summary>
					float z;
				};
				/// <summary>�z��</summary>
				float v[3];
			};
#pragma warning(default:4201)


		public:
			/// <summary>�f�t�H���g�R���X�g���N�^</summary>
			Float3():x(0.0f),y(0.0f),z(0.0f){}
			/// <summary>�t���R���X�g���N�^</summary>
			/// <param name="_x">X����</param>
			/// <param name="_y">Y����</param>
			/// <param name="_z">Z����</param>
			Float3(const float _x, const float _y, const float _z):x(_x),y(_y),z(_z){}
			/// <summary>�t���R���X�g���N�^</summary>
			/// <param name="_v">�z��</param>
			Float3(const float (&_v)[3]):v{_v[0],_v[1],_v[2]}{}
			/// <summary>�R���X�g���N�^</summary>
			/// <param name="_xy">XY����</param>
			/// <param name="_z">Z����</param>
			Float3(const Float2& _xy, const float _z):x(_xy.x),y(_xy.y),z(_z){}
			/// <summary>�R���X�g���N�^</summary>
			/// <param name="_x">X����</param>
			/// <param name="_yz">YZ����</param>
			Float3(const float _x, const Float2& _yz):x(_x),y(_yz.x),z(_yz.y){}
			/// <summary>�R�s�[�R���X�g���N�^</summary>
			/// <param name="_original">�I���W�i���f�[�^</param>
			Float3(const Float3& _original):x(_original.x),y(_original.y),z(_original.z){}
			// �R�s�[�R���X�g���N�^
			Float3(const ::DirectX::SimpleMath::Vector3& _original);
			/// <summary>�f�X�g���N�^</summary>
			~Float3(){}

		public:
			/// <summary>������Z�q</summary>
			/// <param name="_original">�I���W�i���f�[�^</param>
			/// <returns>���g�̃f�[�^</returns>
			Float3& operator=(const Float3& _original){
				this->x = _original.x;
				this->y = _original.y;
				this->z = _original.z;
				return (*this);
			}
			// ������Z�q
			Float3& operator=(const ::DirectX::SimpleMath::Vector3& _original);
			/// <summary>���Z</summary>
			/// <param name="_left">���Z�l</param>
			/// <returns>���Z����</returns>
			Float3 operator+(const Float3& _left) const {
				return Float3(this->x+_left.x, this->y+_left.y, this->z+_left.z);
			}
			/// <summary>���Z���</summary>
			/// <param name="_left">���Z�l</param>
			/// <returns>���g�̃f�[�^</returns>
			Float3& operator+=(const Float3& _left){
				this->x += _left.x;
				this->y += _left.y;
				this->z += _left.z;
				return (*this);
			}
			/// <summary>���Z</summary>
			/// <param name="_left">���Z�l</param>
			/// <returns>���Z����</returns>
			Float3 operator-(const Float3& _left) const {
				return Float3(this->x-_left.x, this->y-_left.y, this->z-_left.z);
			}
			/// <summary>���Z���</summary>
			/// <param name="_left">���Z�l</param>
			/// <returns>���g�̃f�[�^</returns>
			Float3& operator-=(const Float3& _left){
				this->x -= _left.x;
				this->y -= _left.y;
				this->z -= _left.z;
				return (*this);
			}
			/// <summary>�X�J���{</summary>
			/// <param name="_scalar">�{��</param>
			/// <returns>�X�J���{�����x�N�g��</returns>
			Float3 operator*(const float _scalar) const {
				return Float3(
					_scalar * this->x,
					_scalar * this->y,
					_scalar * this->z
				);
			}
			/// <summary>�X�J���{</summary>
			/// <param name="_scalar">�{��</param>
			/// <returns>���g�̃f�[�^</returns>
			Float3& operator*=(const float _scalar){
				this->x *= _scalar;
				this->y *= _scalar;
				this->z *= _scalar;
				return (*this);
			}

		public:
			/// <summary>�x�N�g��(x,y)�̎擾</summary>
			/// <returns>�x�N�g��(x,y)</returns>
			Float2 GetXy() const { return Float2(this->x, this->y); }
			/// <summary>�x�N�g��(x,z)�̎擾</summary>
			/// <returns>�x�N�g��(x,z)</returns>
			Float2 GetXz() const { return Float2(this->x, this->z); }
			/// <summary>�x�N�g��(y,x)�̎擾</summary>
			/// <returns>�x�N�g��(y,x)</returns>
			Float2 GetYx() const { return Float2(this->y, this->x); }
			/// <summary>�x�N�g��(y,z)�̎擾</summary>
			/// <returns>�x�N�g��(y,z)</returns>
			Float2 GetYz() const { return Float2(this->y, this->z); }
			/// <summary>�x�N�g��(z,x)�̎擾</summary>
			/// <returns>�x�N�g��(z,x)</returns>
			Float2 GetZx() const { return Float2(this->z, this->x); }
			/// <summary>�x�N�g��(z,y)�̎擾</summary>
			/// <returns>�x�N�g��(z,y)</returns>
			Float2 GetZy() const { return Float2(this->z, this->y); }

		public:
			/// <summary>�x�N�g��(x,y)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(x,y)</param>
			void SetXy(const Float2& _v){ this->x = _v.x; this->y = _v.y; }
			/// <summary>�x�N�g��(x,z)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(x,z)</param>
			void SetXz(const Float2& _v){ this->x = _v.x; this->z = _v.y; }
			/// <summary>�x�N�g��(y,x)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(y,x)</param>
			void SetYx(const Float2& _v){ this->y = _v.x; this->x = _v.y; }
			/// <summary>�x�N�g��(y,z)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(y,z)</param>
			void SetYz(const Float2& _v){ this->y = _v.x; this->z = _v.y; }
			/// <summary>�x�N�g��(z,x)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(z,x)</param>
			void SetZx(const Float2& _v){ this->z = _v.x; this->x = _v.y; }
			/// <summary>�x�N�g��(z,y)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(z,y)</param>
			void SetZy(const Float2& _v){ this->z = _v.x; this->y = _v.y; }

		public:
			/// <summary>����</summary>
			/// <param name="_vector">�x�N�g��</param>
			/// <returns>����</returns>
			float Dot(const Float3& _vector) const {
				return (this->x*_vector.x + this->y*_vector.y + this->z*_vector.z);
			}
			/// <summary>�O��</summary>
			/// <param name="_vector">�x�N�g��</param>
			/// <returns>�O��</returns>
			Float3 Cross(const Float3& _vector) const {
				return Float3(
					this->y*_vector.z - this->z*_vector.y,
					this->z*_vector.x - this->x*_vector.z,
					this->x*_vector.y - this->y*_vector.x
				);
			}
			/// <summary>������2��</summary>
			/// <returns>������2��</returns>
			float LengthSquared() const {
				return (this->x*this->x + this->y*this->y + this->z*this->z);
			}
			// ����
			float Length() const;
			// ���K��
			void Normalize();
			// ���K��
			void Normalize(Float3* const _normalizedVector) const;
		};

		/// <summary>
		/// 4����float�^�����x�N�g��
		/// </summary>
		struct Float4 final{
		public:
			// �[���x�N�g��
			static const Float4 ZERO;
			// X�������x�N�g��
			static const Float4 UNIT_X;
			// Y�������x�N�g��
			static const Float4 UNIT_Y;
			// Z�������x�N�g��
			static const Float4 UNIT_Z;
			// W�������x�N�g��
			static const Float4 UNIT_W;
			// �S��1.0�̃x�N�g��
			static const Float4 ONE;


		public:
#pragma warning(disable:4201)
			union{
				struct{
					/// <summary>X����</summary>
					float x;
					/// <summary>Y����</summary>
					float y;
					/// <summary>Z����</summary>
					float z;
					/// <summary>W����</summary>
					float w;
				};
				/// <summary>�z��</summary>
				float v[4];
			};
#pragma warning(default:4201)


		public:
			/// <summary>�f�t�H���g�R���X�g���N�^</summary>
			Float4():x(0.0f),y(0.0f),z(0.0f),w(0.0f){}
			/// <summary>�t���R���X�g���N�^</summary>
			/// <param name="_x">X����</param>
			/// <param name="_y">Y����</param>
			/// <param name="_z">Z����</param>
			/// <param name="_w">W����</param>
			Float4(const float _x, const float _y, const float _z, const float _w):x(_x),y(_y),z(_z),w(_w){}
			/// <summary>�t���R���X�g���N�^</summary>
			/// <param name="_v">�z��</param>
			Float4(const float (&_v)[4]):v{_v[0],_v[1],_v[2],_v[3]}{}
			/// <summary>�R���X�g���N�^</summary>
			/// <param name="_xy">XY����</param>
			/// <param name="_z">Z����</param>
			/// <param name="_w">W����</param>
			Float4(const Float2& _xy, const float _z, const float _w):x(_xy.x),y(_xy.y),z(_z),w(_w){}
			/// <summary>�R���X�g���N�^</summary>
			/// <param name="_x">X����</param>
			/// <param name="_yz">YZ����</param>
			/// <param name="_w">W����</param>
			Float4(const float _x, const Float2& _yz, const float _w):x(_x),y(_yz.x),z(_yz.y),w(_w){}
			/// <summary>�R���X�g���N�^</summary>
			/// <param name="_x">X����</param>
			/// <param name="_y">Y����</param>
			/// <param name="_zw">ZW����</param>
			Float4(const float _x, const float _y, const Float2& _zw):x(_x),y(_y),z(_zw.x),w(_zw.y){}
			/// <summary>�R���X�g���N�^</summary>
			/// <param name="_xy">XY����</param>
			/// <param name="_zw">ZW����</param>
			Float4(const Float2& _xy, const Float2& _zw):x(_xy.x),y(_xy.y),z(_zw.x),w(_zw.y){}
			/// <summary>�R���X�g���N�^</summary>
			/// <param name="_xyz">XYZ����</param>
			/// <param name="_w">W����</param>
			Float4(const Float3& _xyz, const float _w):x(_xyz.x),y(_xyz.y),z(_xyz.z),w(_w){}
			/// <summary>�R���X�g���N�^</summary>
			/// <param name="_x">X����</param>
			/// <param name="_yzw">YZW����</param>
			Float4(const float _x, const Float3& _yzw):x(_x),y(_yzw.x),z(_yzw.y),w(_yzw.z){}
			/// <summary>�R�s�[�R���X�g���N�^</summary>
			/// <param name="_original">�I���W�i���f�[�^</param>
			Float4(const Float4& _original):x(_original.x),y(_original.y),z(_original.z),w(_original.w){}
			// �R�s�[�R���X�g���N�^
			Float4(const ::DirectX::SimpleMath::Vector4& _original);
			/// <summary>�f�X�g���N�^</summary>
			~Float4(){}

		public:
			/// <summary>������Z�q</summary>
			/// <param name="_original">�I���W�i���f�[�^</param>
			/// <returns>���g�̃f�[�^</returns>
			Float4& operator=(const Float4& _original){
				this->x = _original.x;
				this->y = _original.y;
				this->z = _original.z;
				this->w = _original.w;
				return (*this);
			}
			// ������Z�q
			Float4& operator=(const ::DirectX::SimpleMath::Vector4& _original);
			/// <summary>���Z</summary>
			/// <param name="_left">���Z�l</param>
			/// <returns>���Z����</returns>
			Float4 operator+(const Float4& _left) const {
				return Float4(this->x+_left.x, this->y+_left.y, this->z+_left.z, this->w+_left.w);
			}
			/// <summary>���Z���</summary>
			/// <param name="_left">���Z�l</param>
			/// <returns>���g�̃f�[�^</returns>
			Float4& operator+=(const Float4& _left){
				this->x += _left.x;
				this->y += _left.y;
				this->z += _left.z;
				this->w += _left.w;
				return (*this);
			}
			/// <summary>���Z</summary>
			/// <param name="_left">���Z�l</param>
			/// <returns>���Z����</returns>
			Float4 operator-(const Float4& _left) const {
				return Float4(this->x-_left.x, this->y-_left.y, this->z-_left.z, this->w-_left.w);
			}
			/// <summary>���Z���</summary>
			/// <param name="_left">���Z�l</param>
			/// <returns>���g�̃f�[�^</returns>
			Float4& operator-=(const Float4& _left){
				this->x -= _left.x;
				this->y -= _left.y;
				this->z -= _left.z;
				this->w -= _left.w;
				return (*this);
			}
			/// <summary>�X�J���{</summary>
			/// <param name="_scalar">�{��</param>
			/// <returns>�X�J���{�����x�N�g��</returns>
			Float4 operator*(const float _scalar) const {
				return Float4(
					_scalar * this->x,
					_scalar * this->y,
					_scalar * this->z,
					_scalar * this->w
				);
			}
			/// <summary>�X�J���{</summary>
			/// <param name="_scalar">�{��</param>
			/// <returns>���g�̃f�[�^</returns>
			Float4& operator*=(const float _scalar){
				this->x *= _scalar;
				this->y *= _scalar;
				this->z *= _scalar;
				this->w *= _scalar;
				return (*this);
			}
			// �s��Ƃ̊|���Z
			Float4 operator*(const Matrix& _matrix) const;
			// �s��Ƃ̊|���Z
			Float4& operator*=(const Matrix& _matrix);

		public:
			/// <summary>�x�N�g��(x,y)�̎擾</summary>
			/// <returns>�x�N�g��(x,y)</returns>
			Float2 GetXy() const { return Float2(this->x, this->y); }
			/// <summary>�x�N�g��(x,z)�̎擾</summary>
			/// <returns>�x�N�g��(x,z)</returns>
			Float2 GetXz() const { return Float2(this->x, this->z); }
			/// <summary>�x�N�g��(x,w)�̎擾</summary>
			/// <returns>�x�N�g��(x,w)</returns>
			Float2 GetXw() const { return Float2(this->x, this->w); }
			/// <summary>�x�N�g��(y,x)�̎擾</summary>
			/// <returns>�x�N�g��(y,x)</returns>
			Float2 GetYx() const { return Float2(this->y, this->x); }
			/// <summary>�x�N�g��(y,z)�̎擾</summary>
			/// <returns>�x�N�g��(y,z)</returns>
			Float2 GetYz() const { return Float2(this->y, this->z); }
			/// <summary>�x�N�g��(y,w)�̎擾</summary>
			/// <returns>�x�N�g��(y,w)</returns>
			Float2 GetYw() const { return Float2(this->y, this->w); }
			/// <summary>�x�N�g��(z,x)�̎擾</summary>
			/// <returns>�x�N�g��(z,x)</returns>
			Float2 GetZx() const { return Float2(this->z, this->x); }
			/// <summary>�x�N�g��(z,y)�̎擾</summary>
			/// <returns>�x�N�g��(z,y)</returns>
			Float2 GetZy() const { return Float2(this->z, this->y); }
			/// <summary>�x�N�g��(z,w)�̎擾</summary>
			/// <returns>�x�N�g��(z,w)</returns>
			Float2 GetZw() const { return Float2(this->z, this->w); }
			/// <summary>�x�N�g��(w,x)�̎擾</summary>
			/// <returns>�x�N�g��(w,x)</returns>
			Float2 GetWx() const { return Float2(this->w, this->x); }
			/// <summary>�x�N�g��(w,y)�̎擾</summary>
			/// <returns>�x�N�g��(w,y)</returns>
			Float2 GetWy() const { return Float2(this->w, this->y); }
			/// <summary>�x�N�g��(w,z)�̎擾</summary>
			/// <returns>�x�N�g��(w,z)</returns>
			Float2 GetWz() const { return Float2(this->w, this->z); }
			/// <summary>�x�N�g��(x,y,z)�̎擾</summary>
			/// <returns>�x�N�g��(x,y,z)</returns>
			Float3 GetXyz() const { return Float3(this->x, this->y, this->z); }
			/// <summary>�x�N�g��(x,y,w)�̎擾</summary>
			/// <returns>�x�N�g��(x,y,w)</returns>
			Float3 GetXyw() const { return Float3(this->x, this->y, this->w); }
			/// <summary>�x�N�g��(x,z,y)�̎擾</summary>
			/// <returns>�x�N�g��(x,z,y)</returns>
			Float3 GetXzy() const { return Float3(this->x, this->z, this->y); }
			/// <summary>�x�N�g��(x,z,w)�̎擾</summary>
			/// <returns>�x�N�g��(x,z,w)</returns>
			Float3 GetXzw() const { return Float3(this->x, this->z, this->w); }
			/// <summary>�x�N�g��(x,w,y)�̎擾</summary>
			/// <returns>�x�N�g��(x,w,y)</returns>
			Float3 GetXwy() const { return Float3(this->x, this->w, this->y); }
			/// <summary>�x�N�g��(x,w,z)�̎擾</summary>
			/// <returns>�x�N�g��(x,w,z)</returns>
			Float3 GetXwz() const { return Float3(this->x, this->w, this->z); }
			/// <summary>�x�N�g��(y,x,z)�̎擾</summary>
			/// <returns>�x�N�g��(y,x,z)</returns>
			Float3 GetYxz() const { return Float3(this->y, this->x, this->z); }
			/// <summary>�x�N�g��(y,x,w)�̎擾</summary>
			/// <returns>�x�N�g��(y,x,w)</returns>
			Float3 GetYxw() const { return Float3(this->y, this->x, this->w); }
			/// <summary>�x�N�g��(y,z,x)�̎擾</summary>
			/// <returns>�x�N�g��(y,z,x)</returns>
			Float3 GetYzx() const { return Float3(this->y, this->z, this->x); }
			/// <summary>�x�N�g��(y,z,w)�̎擾</summary>
			/// <returns>�x�N�g��(y,z,w)</returns>
			Float3 GetYzw() const { return Float3(this->y, this->z, this->w); }
			/// <summary>�x�N�g��(y,w,x)�̎擾</summary>
			/// <returns>�x�N�g��(y,w,x)</returns>
			Float3 GetYwx() const { return Float3(this->y, this->w, this->x); }
			/// <summary>�x�N�g��(y,w,z)�̎擾</summary>
			/// <returns>�x�N�g��(y,w,z)</returns>
			Float3 GetYwz() const { return Float3(this->y, this->w, this->z); }
			/// <summary>�x�N�g��(z,x,y)�̎擾</summary>
			/// <returns>�x�N�g��(z,x,y)</returns>
			Float3 GetZxy() const { return Float3(this->z, this->x, this->y); }
			/// <summary>�x�N�g��(z,x,w)�̎擾</summary>
			/// <returns>�x�N�g��(z,x,w)</returns>
			Float3 GetZxw() const { return Float3(this->z, this->x, this->w); }
			/// <summary>�x�N�g��(z,y,x)�̎擾</summary>
			/// <returns>�x�N�g��(z,y,x)</returns>
			Float3 GetZyx() const { return Float3(this->z, this->y, this->x); }
			/// <summary>�x�N�g��(z,y,w)�̎擾</summary>
			/// <returns>�x�N�g��(z,y,w)</returns>
			Float3 GetZyw() const { return Float3(this->z, this->y, this->w); }
			/// <summary>�x�N�g��(z,w,x)�̎擾</summary>
			/// <returns>�x�N�g��(z,w,x)</returns>
			Float3 GetZwx() const { return Float3(this->z, this->w, this->x); }
			/// <summary>�x�N�g��(z,w,y)�̎擾</summary>
			/// <returns>�x�N�g��(z,w,y)</returns>
			Float3 GetZwy() const { return Float3(this->z, this->w, this->y); }
			/// <summary>�x�N�g��(w,x,y)�̎擾</summary>
			/// <returns>�x�N�g��(w,x,y)</returns>
			Float3 GetWxy() const { return Float3(this->w, this->x, this->y); }
			/// <summary>�x�N�g��(w,x,z)�̎擾</summary>
			/// <returns>�x�N�g��(w,x,z)</returns>
			Float3 GetWxz() const { return Float3(this->w, this->x, this->z); }
			/// <summary>�x�N�g��(w,y,x)�̎擾</summary>
			/// <returns>�x�N�g��(w,y,x)</returns>
			Float3 GetWyx() const { return Float3(this->w, this->y, this->x); }
			/// <summary>�x�N�g��(w,y,z)�̎擾</summary>
			/// <returns>�x�N�g��(w,y,z)</returns>
			Float3 GetWyz() const { return Float3(this->w, this->y, this->z); }
			/// <summary>�x�N�g��(w,z,x)�̎擾</summary>
			/// <returns>�x�N�g��(w,z,x)</returns>
			Float3 GetWzx() const { return Float3(this->w, this->z, this->x); }
			/// <summary>�x�N�g��(w,z,y)�̎擾</summary>
			/// <returns>�x�N�g��(w,z,y)</returns>
			Float3 GetWzy() const { return Float3(this->w, this->z, this->y); }

		public:
			/// <summary>�x�N�g��(x,y)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(x,y)</param>
			void SetXy(const Float2& _v){ this->x = _v.x; this->y = _v.y; }
			/// <summary>�x�N�g��(x,z)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(x,z)</param>
			void SetXz(const Float2& _v){ this->x = _v.x; this->z = _v.y; }
			/// <summary>�x�N�g��(x,w)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(x,w)</param>
			void SetXw(const Float2& _v){ this->x = _v.x; this->w = _v.y; }
			/// <summary>�x�N�g��(y,x)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(y,x)</param>
			void SetYx(const Float2& _v){ this->y = _v.x; this->x = _v.y; }
			/// <summary>�x�N�g��(y,z)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(y,z)</param>
			void SetYz(const Float2& _v){ this->y = _v.x; this->z = _v.y; }
			/// <summary>�x�N�g��(y,w)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(y,w)</param>
			void SetYw(const Float2& _v){ this->y = _v.x; this->w = _v.y; }
			/// <summary>�x�N�g��(z,x)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(z,x)</param>
			void SetZx(const Float2& _v){ this->z = _v.x; this->x = _v.y; }
			/// <summary>�x�N�g��(z,y)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(z,y)</param>
			void SetZy(const Float2& _v){ this->z = _v.x; this->y = _v.y; }
			/// <summary>�x�N�g��(z,w)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(z,w)</param>
			void SetZw(const Float2& _v){ this->z = _v.x; this->w = _v.y; }
			/// <summary>�x�N�g��(w,x)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(w,x)</param>
			void SetWx(const Float2& _v){ this->w = _v.x; this->x = _v.y; }
			/// <summary>�x�N�g��(w,y)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(w,y)</param>
			void SetWy(const Float2& _v){ this->w = _v.x; this->y = _v.y; }
			/// <summary>�x�N�g��(w,z)�̐ݒ�</summary>
			/// <param name="_v">�x�N�g��(w,z)</param>
			void SetWz(const Float2& _v){ this->w = _v.x; this->z = _v.y; }
			/// <summary>�x�N�g��(x,y,z)�̎擾</summary>
			/// <param name="_v">�x�N�g��(x,y,z)</param>
			void SetXyz(const Float3& _v){ this->x = _v.x; this->y = _v.y; this->z = _v.z; }
			/// <summary>�x�N�g��(x,y,w)�̎擾</summary>
			/// <param name="_v">�x�N�g��(x,y,w)</param>
			void SetXyw(const Float3& _v){ this->x = _v.x; this->y = _v.y; this->w = _v.z; }
			/// <summary>�x�N�g��(x,z,y)�̎擾</summary>
			/// <param name="_v">�x�N�g��(x,z,y)</param>
			void SetXzy(const Float3& _v){ this->x = _v.x; this->z = _v.y; this->y = _v.z; }
			/// <summary>�x�N�g��(x,z,w)�̎擾</summary>
			/// <param name="_v">�x�N�g��(x,z,w)</param>
			void SetXzw(const Float3& _v){ this->x = _v.x; this->z = _v.y; this->w = _v.z; }
			/// <summary>�x�N�g��(x,w,y)�̎擾</summary>
			/// <param name="_v">�x�N�g��(x,w,y)</param>
			void SetXwy(const Float3& _v){ this->x = _v.x; this->w = _v.y; this->y = _v.z; }
			/// <summary>�x�N�g��(x,w,z)�̎擾</summary>
			/// <param name="_v">�x�N�g��(x,w,z)</param>
			void SetXwz(const Float3& _v){ this->x = _v.x; this->w = _v.y; this->z = _v.z; }
			/// <summary>�x�N�g��(y,x,z)�̎擾</summary>
			/// <param name="_v">�x�N�g��(y,x,z)</param>
			void SetYxz(const Float3& _v){ this->y = _v.x; this->x = _v.y; this->z = _v.z; }
			/// <summary>�x�N�g��(y,x,w)�̎擾</summary>
			/// <param name="_v">�x�N�g��(y,x,w)</param>
			void SetYxw(const Float3& _v){ this->y = _v.x; this->x = _v.y; this->w = _v.z; }
			/// <summary>�x�N�g��(y,z,x)�̎擾</summary>
			/// <param name="_v">�x�N�g��(y,z,x)</param>
			void SetYzx(const Float3& _v){ this->y = _v.x; this->z = _v.y; this->x = _v.z; }
			/// <summary>�x�N�g��(y,z,w)�̎擾</summary>
			/// <param name="_v">�x�N�g��(y,z,w)</param>
			void SetYzw(const Float3& _v){ this->y = _v.x; this->z = _v.y; this->w = _v.z; }
			/// <summary>�x�N�g��(y,w,x)�̎擾</summary>
			/// <param name="_v">�x�N�g��(y,w,x)</param>
			void SetYwx(const Float3& _v){ this->y = _v.x; this->w = _v.y; this->x = _v.z; }
			/// <summary>�x�N�g��(y,w,z)�̎擾</summary>
			/// <param name="_v">�x�N�g��(y,w,z)</param>
			void SetYwz(const Float3& _v){ this->y = _v.x; this->w = _v.y; this->z = _v.z; }
			/// <summary>�x�N�g��(z,x,y)�̎擾</summary>
			/// <param name="_v">�x�N�g��(z,x,y)</param>
			void SetZxy(const Float3& _v){ this->z = _v.x; this->x = _v.y; this->y = _v.z; }
			/// <summary>�x�N�g��(z,x,w)�̎擾</summary>
			/// <param name="_v">�x�N�g��(z,x,w)</param>
			void SetZxw(const Float3& _v){ this->z = _v.x; this->x = _v.y; this->w = _v.z; }
			/// <summary>�x�N�g��(z,y,x)�̎擾</summary>
			/// <param name="_v">�x�N�g��(z,y,x)</param>
			void SetZyx(const Float3& _v){ this->z = _v.x; this->y = _v.y; this->x = _v.z; }
			/// <summary>�x�N�g��(z,y,w)�̎擾</summary>
			/// <param name="_v">�x�N�g��(z,y,w)</param>
			void SetZyw(const Float3& _v){ this->z = _v.x; this->y = _v.y; this->w = _v.z; }
			/// <summary>�x�N�g��(z,w,x)�̎擾</summary>
			/// <param name="_v">�x�N�g��(z,w,x)</param>
			void SetZwx(const Float3& _v){ this->z = _v.x; this->w = _v.y; this->x = _v.z; }
			/// <summary>�x�N�g��(z,w,y)�̎擾</summary>
			/// <param name="_v">�x�N�g��(z,w,y)</param>
			void SetZwy(const Float3& _v){ this->z = _v.x; this->w = _v.y; this->y = _v.z; }
			/// <summary>�x�N�g��(w,x,y)�̎擾</summary>
			/// <param name="_v">�x�N�g��(w,x,y)</param>
			void SetWxy(const Float3& _v){ this->w = _v.x; this->x = _v.y; this->y = _v.z; }
			/// <summary>�x�N�g��(w,x,z)�̎擾</summary>
			/// <param name="_v">�x�N�g��(w,x,z)</param>
			void SetWxz(const Float3& _v){ this->w = _v.x; this->x = _v.y; this->z = _v.z; }
			/// <summary>�x�N�g��(w,y,x)�̎擾</summary>
			/// <param name="_v">�x�N�g��(w,y,x)</param>
			void SetWyx(const Float3& _v){ this->w = _v.x; this->y = _v.y; this->x = _v.z; }
			/// <summary>�x�N�g��(w,y,z)�̎擾</summary>
			/// <param name="_v">�x�N�g��(w,y,z)</param>
			void SetWyz(const Float3& _v){ this->w = _v.x; this->y = _v.y; this->z = _v.z; }
			/// <summary>�x�N�g��(w,z,x)�̎擾</summary>
			/// <param name="_v">�x�N�g��(w,z,x)</param>
			void SetWzx(const Float3& _v){ this->w = _v.x; this->z = _v.y; this->x = _v.z; }
			/// <summary>�x�N�g��(w,z,y)�̎擾</summary>
			/// <param name="_v">�x�N�g��(w,z,y)</param>
			void SetWzy(const Float3& _v){ this->w = _v.x; this->z = _v.y; this->y = _v.z; }

		public:
			/// <summary>����</summary>
			/// <param name="_vector">�x�N�g��</param>
			/// <returns>����</returns>
			float Dot(const Float4& _vector) const {
				return (this->x*_vector.x + this->y*_vector.y + this->z*_vector.z + this->w*_vector.w);
			}
			/// <summary>������2��</summary>
			/// <returns>������2��</returns>
			float LengthSquared() const {
				return (this->x*this->x + this->y*this->y + this->z*this->z + this->w*this->w);
			}
			// ����
			float Length() const;
			// ���K��
			void Normalize();
			// ���K��
			void Normalize(Float4* const _normalizedVector) const;
		};
	}
}

#endif
