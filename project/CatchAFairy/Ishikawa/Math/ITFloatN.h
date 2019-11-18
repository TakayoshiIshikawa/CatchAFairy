//=============================================================================
//	ITFloatN.h
//
//	自作n次元float型実数ベクトルのヘッダファイル
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
		/// 2次元float型実数ベクトル
		/// </summary>
		struct Float2 final{
		public:
			// ゼロベクトル
			static const Float2 ZERO;
			// X軸方向ベクトル
			static const Float2 UNIT_X;
			// Y軸方向ベクトル
			static const Float2 UNIT_Y;
			// 全て1.0のベクトル
			static const Float2 ONE;


		public:
#pragma warning(disable:4201)
			union{
				struct{
					/// <summary>X成分</summary>
					float x;
					/// <summary>Y成分</summary>
					float y;
				};
				/// <summary>配列</summary>
				float v[2];
			};
#pragma warning(default:4201)


		public:
			/// <summary>デフォルトコンストラクタ</summary>
			Float2():x(0.0f),y(0.0f){}
			/// <summary>フルコンストラクタ</summary>
			/// <param name="_x">X成分</param>
			/// <param name="_y">Y成分</param>
			Float2(const float _x, const float _y):x(_x),y(_y){}
			/// <summary>フルコンストラクタ</summary>
			/// <param name="_v">配列</param>
			Float2(const float (&_v)[2]):v{_v[0],_v[1]}{}
			/// <summary>コピーコンストラクタ</summary>
			/// <param name="_original">オリジナルデータ</param>
			Float2(const Float2& _original):x(_original.x),y(_original.y){}
			// コピーコンストラクタ
			Float2(const ::DirectX::SimpleMath::Vector2& _original);
			/// <summary>デストラクタ</summary>
			~Float2(){}

		public:
			/// <summary>代入演算子</summary>
			/// <param name="_original">オリジナルデータ</param>
			/// <returns>自身のデータ</returns>
			Float2& operator=(const Float2& _original){
				this->x = _original.x;
				this->y = _original.y;
				return (*this);
			}
			// 代入演算子
			Float2& operator=(const ::DirectX::SimpleMath::Vector2& _original);
			/// <summary>加算</summary>
			/// <param name="_left">加算値</param>
			/// <returns>加算結果</returns>
			Float2 operator+(const Float2& _left) const {
				return Float2(this->x+_left.x, this->y+_left.y);
			}
			/// <summary>加算代入</summary>
			/// <param name="_left">加算値</param>
			/// <returns>自身のデータ</returns>
			Float2& operator+=(const Float2& _left){
				this->x += _left.x;
				this->y += _left.y;
				return (*this);
			}
			/// <summary>減算</summary>
			/// <param name="_left">減算値</param>
			/// <returns>減算結果</returns>
			Float2 operator-(const Float2& _left) const {
				return Float2(this->x-_left.x, this->y-_left.y);
			}
			/// <summary>減算代入</summary>
			/// <param name="_left">減算値</param>
			/// <returns>自身のデータ</returns>
			Float2& operator-=(const Float2& _left){
				this->x -= _left.x;
				this->y -= _left.y;
				return (*this);
			}
			/// <summary>スカラ倍</summary>
			/// <param name="_scalar">倍数</param>
			/// <returns>スカラ倍したベクトル</returns>
			Float2 operator*(const float _scalar) const {
				return Float2(
					_scalar * this->x,
					_scalar * this->y
				);
			}
			/// <summary>スカラ倍</summary>
			/// <param name="_scalar">倍数</param>
			/// <returns>自身のデータ</returns>
			Float2& operator*=(const float _scalar){
				this->x *= _scalar;
				this->y *= _scalar;
				return (*this);
			}

		public:
			/// <summary>内積</summary>
			/// <param name="_vector">ベクトル</param>
			/// <returns>内積</returns>
			float Dot(const Float2& _vector) const {
				return (this->x*_vector.x + this->y*_vector.y);
			}
			/// <summary>外積</summary>
			/// <param name="_vector">ベクトル</param>
			/// <returns>外積</returns>
			float Cross(const Float2& _vector) const {
				return (this->x*_vector.y - this->y*_vector.x);
			}
			/// <summary>長さの2乗</summary>
			/// <returns>長さの2乗</returns>
			float LengthSquared() const {
				return (this->x*this->x + this->y*this->y);
			}
			// 長さ
			float Length() const;
			// 正規化
			void Normalize();
			// 正規化
			void Normalize(Float2* const _normalizedVector) const;
		};

		/// <summary>
		/// 3次元float型実数ベクトル
		/// </summary>
		struct Float3 final{
		public:
			// ゼロベクトル
			static const Float3 ZERO;
			// X軸方向ベクトル
			static const Float3 UNIT_X;
			// Y軸方向ベクトル
			static const Float3 UNIT_Y;
			// Z軸方向ベクトル
			static const Float3 UNIT_Z;
			// 全て1.0のベクトル
			static const Float3 ONE;


		public:
#pragma warning(disable:4201)
			union{
				struct{
					/// <summary>X成分</summary>
					float x;
					/// <summary>Y成分</summary>
					float y;
					/// <summary>Z成分</summary>
					float z;
				};
				/// <summary>配列</summary>
				float v[3];
			};
#pragma warning(default:4201)


		public:
			/// <summary>デフォルトコンストラクタ</summary>
			Float3():x(0.0f),y(0.0f),z(0.0f){}
			/// <summary>フルコンストラクタ</summary>
			/// <param name="_x">X成分</param>
			/// <param name="_y">Y成分</param>
			/// <param name="_z">Z成分</param>
			Float3(const float _x, const float _y, const float _z):x(_x),y(_y),z(_z){}
			/// <summary>フルコンストラクタ</summary>
			/// <param name="_v">配列</param>
			Float3(const float (&_v)[3]):v{_v[0],_v[1],_v[2]}{}
			/// <summary>コンストラクタ</summary>
			/// <param name="_xy">XY成分</param>
			/// <param name="_z">Z成分</param>
			Float3(const Float2& _xy, const float _z):x(_xy.x),y(_xy.y),z(_z){}
			/// <summary>コンストラクタ</summary>
			/// <param name="_x">X成分</param>
			/// <param name="_yz">YZ成分</param>
			Float3(const float _x, const Float2& _yz):x(_x),y(_yz.x),z(_yz.y){}
			/// <summary>コピーコンストラクタ</summary>
			/// <param name="_original">オリジナルデータ</param>
			Float3(const Float3& _original):x(_original.x),y(_original.y),z(_original.z){}
			// コピーコンストラクタ
			Float3(const ::DirectX::SimpleMath::Vector3& _original);
			/// <summary>デストラクタ</summary>
			~Float3(){}

		public:
			/// <summary>代入演算子</summary>
			/// <param name="_original">オリジナルデータ</param>
			/// <returns>自身のデータ</returns>
			Float3& operator=(const Float3& _original){
				this->x = _original.x;
				this->y = _original.y;
				this->z = _original.z;
				return (*this);
			}
			// 代入演算子
			Float3& operator=(const ::DirectX::SimpleMath::Vector3& _original);
			/// <summary>加算</summary>
			/// <param name="_left">加算値</param>
			/// <returns>加算結果</returns>
			Float3 operator+(const Float3& _left) const {
				return Float3(this->x+_left.x, this->y+_left.y, this->z+_left.z);
			}
			/// <summary>加算代入</summary>
			/// <param name="_left">加算値</param>
			/// <returns>自身のデータ</returns>
			Float3& operator+=(const Float3& _left){
				this->x += _left.x;
				this->y += _left.y;
				this->z += _left.z;
				return (*this);
			}
			/// <summary>減算</summary>
			/// <param name="_left">減算値</param>
			/// <returns>減算結果</returns>
			Float3 operator-(const Float3& _left) const {
				return Float3(this->x-_left.x, this->y-_left.y, this->z-_left.z);
			}
			/// <summary>減算代入</summary>
			/// <param name="_left">減算値</param>
			/// <returns>自身のデータ</returns>
			Float3& operator-=(const Float3& _left){
				this->x -= _left.x;
				this->y -= _left.y;
				this->z -= _left.z;
				return (*this);
			}
			/// <summary>スカラ倍</summary>
			/// <param name="_scalar">倍数</param>
			/// <returns>スカラ倍したベクトル</returns>
			Float3 operator*(const float _scalar) const {
				return Float3(
					_scalar * this->x,
					_scalar * this->y,
					_scalar * this->z
				);
			}
			/// <summary>スカラ倍</summary>
			/// <param name="_scalar">倍数</param>
			/// <returns>自身のデータ</returns>
			Float3& operator*=(const float _scalar){
				this->x *= _scalar;
				this->y *= _scalar;
				this->z *= _scalar;
				return (*this);
			}

		public:
			/// <summary>ベクトル(x,y)の取得</summary>
			/// <returns>ベクトル(x,y)</returns>
			Float2 GetXy() const { return Float2(this->x, this->y); }
			/// <summary>ベクトル(x,z)の取得</summary>
			/// <returns>ベクトル(x,z)</returns>
			Float2 GetXz() const { return Float2(this->x, this->z); }
			/// <summary>ベクトル(y,x)の取得</summary>
			/// <returns>ベクトル(y,x)</returns>
			Float2 GetYx() const { return Float2(this->y, this->x); }
			/// <summary>ベクトル(y,z)の取得</summary>
			/// <returns>ベクトル(y,z)</returns>
			Float2 GetYz() const { return Float2(this->y, this->z); }
			/// <summary>ベクトル(z,x)の取得</summary>
			/// <returns>ベクトル(z,x)</returns>
			Float2 GetZx() const { return Float2(this->z, this->x); }
			/// <summary>ベクトル(z,y)の取得</summary>
			/// <returns>ベクトル(z,y)</returns>
			Float2 GetZy() const { return Float2(this->z, this->y); }

		public:
			/// <summary>ベクトル(x,y)の設定</summary>
			/// <param name="_v">ベクトル(x,y)</param>
			void SetXy(const Float2& _v){ this->x = _v.x; this->y = _v.y; }
			/// <summary>ベクトル(x,z)の設定</summary>
			/// <param name="_v">ベクトル(x,z)</param>
			void SetXz(const Float2& _v){ this->x = _v.x; this->z = _v.y; }
			/// <summary>ベクトル(y,x)の設定</summary>
			/// <param name="_v">ベクトル(y,x)</param>
			void SetYx(const Float2& _v){ this->y = _v.x; this->x = _v.y; }
			/// <summary>ベクトル(y,z)の設定</summary>
			/// <param name="_v">ベクトル(y,z)</param>
			void SetYz(const Float2& _v){ this->y = _v.x; this->z = _v.y; }
			/// <summary>ベクトル(z,x)の設定</summary>
			/// <param name="_v">ベクトル(z,x)</param>
			void SetZx(const Float2& _v){ this->z = _v.x; this->x = _v.y; }
			/// <summary>ベクトル(z,y)の設定</summary>
			/// <param name="_v">ベクトル(z,y)</param>
			void SetZy(const Float2& _v){ this->z = _v.x; this->y = _v.y; }

		public:
			/// <summary>内積</summary>
			/// <param name="_vector">ベクトル</param>
			/// <returns>内積</returns>
			float Dot(const Float3& _vector) const {
				return (this->x*_vector.x + this->y*_vector.y + this->z*_vector.z);
			}
			/// <summary>外積</summary>
			/// <param name="_vector">ベクトル</param>
			/// <returns>外積</returns>
			Float3 Cross(const Float3& _vector) const {
				return Float3(
					this->y*_vector.z - this->z*_vector.y,
					this->z*_vector.x - this->x*_vector.z,
					this->x*_vector.y - this->y*_vector.x
				);
			}
			/// <summary>長さの2乗</summary>
			/// <returns>長さの2乗</returns>
			float LengthSquared() const {
				return (this->x*this->x + this->y*this->y + this->z*this->z);
			}
			// 長さ
			float Length() const;
			// 正規化
			void Normalize();
			// 正規化
			void Normalize(Float3* const _normalizedVector) const;
		};

		/// <summary>
		/// 4次元float型実数ベクトル
		/// </summary>
		struct Float4 final{
		public:
			// ゼロベクトル
			static const Float4 ZERO;
			// X軸方向ベクトル
			static const Float4 UNIT_X;
			// Y軸方向ベクトル
			static const Float4 UNIT_Y;
			// Z軸方向ベクトル
			static const Float4 UNIT_Z;
			// W軸方向ベクトル
			static const Float4 UNIT_W;
			// 全て1.0のベクトル
			static const Float4 ONE;


		public:
#pragma warning(disable:4201)
			union{
				struct{
					/// <summary>X成分</summary>
					float x;
					/// <summary>Y成分</summary>
					float y;
					/// <summary>Z成分</summary>
					float z;
					/// <summary>W成分</summary>
					float w;
				};
				/// <summary>配列</summary>
				float v[4];
			};
#pragma warning(default:4201)


		public:
			/// <summary>デフォルトコンストラクタ</summary>
			Float4():x(0.0f),y(0.0f),z(0.0f),w(0.0f){}
			/// <summary>フルコンストラクタ</summary>
			/// <param name="_x">X成分</param>
			/// <param name="_y">Y成分</param>
			/// <param name="_z">Z成分</param>
			/// <param name="_w">W成分</param>
			Float4(const float _x, const float _y, const float _z, const float _w):x(_x),y(_y),z(_z),w(_w){}
			/// <summary>フルコンストラクタ</summary>
			/// <param name="_v">配列</param>
			Float4(const float (&_v)[4]):v{_v[0],_v[1],_v[2],_v[3]}{}
			/// <summary>コンストラクタ</summary>
			/// <param name="_xy">XY成分</param>
			/// <param name="_z">Z成分</param>
			/// <param name="_w">W成分</param>
			Float4(const Float2& _xy, const float _z, const float _w):x(_xy.x),y(_xy.y),z(_z),w(_w){}
			/// <summary>コンストラクタ</summary>
			/// <param name="_x">X成分</param>
			/// <param name="_yz">YZ成分</param>
			/// <param name="_w">W成分</param>
			Float4(const float _x, const Float2& _yz, const float _w):x(_x),y(_yz.x),z(_yz.y),w(_w){}
			/// <summary>コンストラクタ</summary>
			/// <param name="_x">X成分</param>
			/// <param name="_y">Y成分</param>
			/// <param name="_zw">ZW成分</param>
			Float4(const float _x, const float _y, const Float2& _zw):x(_x),y(_y),z(_zw.x),w(_zw.y){}
			/// <summary>コンストラクタ</summary>
			/// <param name="_xy">XY成分</param>
			/// <param name="_zw">ZW成分</param>
			Float4(const Float2& _xy, const Float2& _zw):x(_xy.x),y(_xy.y),z(_zw.x),w(_zw.y){}
			/// <summary>コンストラクタ</summary>
			/// <param name="_xyz">XYZ成分</param>
			/// <param name="_w">W成分</param>
			Float4(const Float3& _xyz, const float _w):x(_xyz.x),y(_xyz.y),z(_xyz.z),w(_w){}
			/// <summary>コンストラクタ</summary>
			/// <param name="_x">X成分</param>
			/// <param name="_yzw">YZW成分</param>
			Float4(const float _x, const Float3& _yzw):x(_x),y(_yzw.x),z(_yzw.y),w(_yzw.z){}
			/// <summary>コピーコンストラクタ</summary>
			/// <param name="_original">オリジナルデータ</param>
			Float4(const Float4& _original):x(_original.x),y(_original.y),z(_original.z),w(_original.w){}
			// コピーコンストラクタ
			Float4(const ::DirectX::SimpleMath::Vector4& _original);
			/// <summary>デストラクタ</summary>
			~Float4(){}

		public:
			/// <summary>代入演算子</summary>
			/// <param name="_original">オリジナルデータ</param>
			/// <returns>自身のデータ</returns>
			Float4& operator=(const Float4& _original){
				this->x = _original.x;
				this->y = _original.y;
				this->z = _original.z;
				this->w = _original.w;
				return (*this);
			}
			// 代入演算子
			Float4& operator=(const ::DirectX::SimpleMath::Vector4& _original);
			/// <summary>加算</summary>
			/// <param name="_left">加算値</param>
			/// <returns>加算結果</returns>
			Float4 operator+(const Float4& _left) const {
				return Float4(this->x+_left.x, this->y+_left.y, this->z+_left.z, this->w+_left.w);
			}
			/// <summary>加算代入</summary>
			/// <param name="_left">加算値</param>
			/// <returns>自身のデータ</returns>
			Float4& operator+=(const Float4& _left){
				this->x += _left.x;
				this->y += _left.y;
				this->z += _left.z;
				this->w += _left.w;
				return (*this);
			}
			/// <summary>減算</summary>
			/// <param name="_left">減算値</param>
			/// <returns>減算結果</returns>
			Float4 operator-(const Float4& _left) const {
				return Float4(this->x-_left.x, this->y-_left.y, this->z-_left.z, this->w-_left.w);
			}
			/// <summary>減算代入</summary>
			/// <param name="_left">減算値</param>
			/// <returns>自身のデータ</returns>
			Float4& operator-=(const Float4& _left){
				this->x -= _left.x;
				this->y -= _left.y;
				this->z -= _left.z;
				this->w -= _left.w;
				return (*this);
			}
			/// <summary>スカラ倍</summary>
			/// <param name="_scalar">倍数</param>
			/// <returns>スカラ倍したベクトル</returns>
			Float4 operator*(const float _scalar) const {
				return Float4(
					_scalar * this->x,
					_scalar * this->y,
					_scalar * this->z,
					_scalar * this->w
				);
			}
			/// <summary>スカラ倍</summary>
			/// <param name="_scalar">倍数</param>
			/// <returns>自身のデータ</returns>
			Float4& operator*=(const float _scalar){
				this->x *= _scalar;
				this->y *= _scalar;
				this->z *= _scalar;
				this->w *= _scalar;
				return (*this);
			}
			// 行列との掛け算
			Float4 operator*(const Matrix& _matrix) const;
			// 行列との掛け算
			Float4& operator*=(const Matrix& _matrix);

		public:
			/// <summary>ベクトル(x,y)の取得</summary>
			/// <returns>ベクトル(x,y)</returns>
			Float2 GetXy() const { return Float2(this->x, this->y); }
			/// <summary>ベクトル(x,z)の取得</summary>
			/// <returns>ベクトル(x,z)</returns>
			Float2 GetXz() const { return Float2(this->x, this->z); }
			/// <summary>ベクトル(x,w)の取得</summary>
			/// <returns>ベクトル(x,w)</returns>
			Float2 GetXw() const { return Float2(this->x, this->w); }
			/// <summary>ベクトル(y,x)の取得</summary>
			/// <returns>ベクトル(y,x)</returns>
			Float2 GetYx() const { return Float2(this->y, this->x); }
			/// <summary>ベクトル(y,z)の取得</summary>
			/// <returns>ベクトル(y,z)</returns>
			Float2 GetYz() const { return Float2(this->y, this->z); }
			/// <summary>ベクトル(y,w)の取得</summary>
			/// <returns>ベクトル(y,w)</returns>
			Float2 GetYw() const { return Float2(this->y, this->w); }
			/// <summary>ベクトル(z,x)の取得</summary>
			/// <returns>ベクトル(z,x)</returns>
			Float2 GetZx() const { return Float2(this->z, this->x); }
			/// <summary>ベクトル(z,y)の取得</summary>
			/// <returns>ベクトル(z,y)</returns>
			Float2 GetZy() const { return Float2(this->z, this->y); }
			/// <summary>ベクトル(z,w)の取得</summary>
			/// <returns>ベクトル(z,w)</returns>
			Float2 GetZw() const { return Float2(this->z, this->w); }
			/// <summary>ベクトル(w,x)の取得</summary>
			/// <returns>ベクトル(w,x)</returns>
			Float2 GetWx() const { return Float2(this->w, this->x); }
			/// <summary>ベクトル(w,y)の取得</summary>
			/// <returns>ベクトル(w,y)</returns>
			Float2 GetWy() const { return Float2(this->w, this->y); }
			/// <summary>ベクトル(w,z)の取得</summary>
			/// <returns>ベクトル(w,z)</returns>
			Float2 GetWz() const { return Float2(this->w, this->z); }
			/// <summary>ベクトル(x,y,z)の取得</summary>
			/// <returns>ベクトル(x,y,z)</returns>
			Float3 GetXyz() const { return Float3(this->x, this->y, this->z); }
			/// <summary>ベクトル(x,y,w)の取得</summary>
			/// <returns>ベクトル(x,y,w)</returns>
			Float3 GetXyw() const { return Float3(this->x, this->y, this->w); }
			/// <summary>ベクトル(x,z,y)の取得</summary>
			/// <returns>ベクトル(x,z,y)</returns>
			Float3 GetXzy() const { return Float3(this->x, this->z, this->y); }
			/// <summary>ベクトル(x,z,w)の取得</summary>
			/// <returns>ベクトル(x,z,w)</returns>
			Float3 GetXzw() const { return Float3(this->x, this->z, this->w); }
			/// <summary>ベクトル(x,w,y)の取得</summary>
			/// <returns>ベクトル(x,w,y)</returns>
			Float3 GetXwy() const { return Float3(this->x, this->w, this->y); }
			/// <summary>ベクトル(x,w,z)の取得</summary>
			/// <returns>ベクトル(x,w,z)</returns>
			Float3 GetXwz() const { return Float3(this->x, this->w, this->z); }
			/// <summary>ベクトル(y,x,z)の取得</summary>
			/// <returns>ベクトル(y,x,z)</returns>
			Float3 GetYxz() const { return Float3(this->y, this->x, this->z); }
			/// <summary>ベクトル(y,x,w)の取得</summary>
			/// <returns>ベクトル(y,x,w)</returns>
			Float3 GetYxw() const { return Float3(this->y, this->x, this->w); }
			/// <summary>ベクトル(y,z,x)の取得</summary>
			/// <returns>ベクトル(y,z,x)</returns>
			Float3 GetYzx() const { return Float3(this->y, this->z, this->x); }
			/// <summary>ベクトル(y,z,w)の取得</summary>
			/// <returns>ベクトル(y,z,w)</returns>
			Float3 GetYzw() const { return Float3(this->y, this->z, this->w); }
			/// <summary>ベクトル(y,w,x)の取得</summary>
			/// <returns>ベクトル(y,w,x)</returns>
			Float3 GetYwx() const { return Float3(this->y, this->w, this->x); }
			/// <summary>ベクトル(y,w,z)の取得</summary>
			/// <returns>ベクトル(y,w,z)</returns>
			Float3 GetYwz() const { return Float3(this->y, this->w, this->z); }
			/// <summary>ベクトル(z,x,y)の取得</summary>
			/// <returns>ベクトル(z,x,y)</returns>
			Float3 GetZxy() const { return Float3(this->z, this->x, this->y); }
			/// <summary>ベクトル(z,x,w)の取得</summary>
			/// <returns>ベクトル(z,x,w)</returns>
			Float3 GetZxw() const { return Float3(this->z, this->x, this->w); }
			/// <summary>ベクトル(z,y,x)の取得</summary>
			/// <returns>ベクトル(z,y,x)</returns>
			Float3 GetZyx() const { return Float3(this->z, this->y, this->x); }
			/// <summary>ベクトル(z,y,w)の取得</summary>
			/// <returns>ベクトル(z,y,w)</returns>
			Float3 GetZyw() const { return Float3(this->z, this->y, this->w); }
			/// <summary>ベクトル(z,w,x)の取得</summary>
			/// <returns>ベクトル(z,w,x)</returns>
			Float3 GetZwx() const { return Float3(this->z, this->w, this->x); }
			/// <summary>ベクトル(z,w,y)の取得</summary>
			/// <returns>ベクトル(z,w,y)</returns>
			Float3 GetZwy() const { return Float3(this->z, this->w, this->y); }
			/// <summary>ベクトル(w,x,y)の取得</summary>
			/// <returns>ベクトル(w,x,y)</returns>
			Float3 GetWxy() const { return Float3(this->w, this->x, this->y); }
			/// <summary>ベクトル(w,x,z)の取得</summary>
			/// <returns>ベクトル(w,x,z)</returns>
			Float3 GetWxz() const { return Float3(this->w, this->x, this->z); }
			/// <summary>ベクトル(w,y,x)の取得</summary>
			/// <returns>ベクトル(w,y,x)</returns>
			Float3 GetWyx() const { return Float3(this->w, this->y, this->x); }
			/// <summary>ベクトル(w,y,z)の取得</summary>
			/// <returns>ベクトル(w,y,z)</returns>
			Float3 GetWyz() const { return Float3(this->w, this->y, this->z); }
			/// <summary>ベクトル(w,z,x)の取得</summary>
			/// <returns>ベクトル(w,z,x)</returns>
			Float3 GetWzx() const { return Float3(this->w, this->z, this->x); }
			/// <summary>ベクトル(w,z,y)の取得</summary>
			/// <returns>ベクトル(w,z,y)</returns>
			Float3 GetWzy() const { return Float3(this->w, this->z, this->y); }

		public:
			/// <summary>ベクトル(x,y)の設定</summary>
			/// <param name="_v">ベクトル(x,y)</param>
			void SetXy(const Float2& _v){ this->x = _v.x; this->y = _v.y; }
			/// <summary>ベクトル(x,z)の設定</summary>
			/// <param name="_v">ベクトル(x,z)</param>
			void SetXz(const Float2& _v){ this->x = _v.x; this->z = _v.y; }
			/// <summary>ベクトル(x,w)の設定</summary>
			/// <param name="_v">ベクトル(x,w)</param>
			void SetXw(const Float2& _v){ this->x = _v.x; this->w = _v.y; }
			/// <summary>ベクトル(y,x)の設定</summary>
			/// <param name="_v">ベクトル(y,x)</param>
			void SetYx(const Float2& _v){ this->y = _v.x; this->x = _v.y; }
			/// <summary>ベクトル(y,z)の設定</summary>
			/// <param name="_v">ベクトル(y,z)</param>
			void SetYz(const Float2& _v){ this->y = _v.x; this->z = _v.y; }
			/// <summary>ベクトル(y,w)の設定</summary>
			/// <param name="_v">ベクトル(y,w)</param>
			void SetYw(const Float2& _v){ this->y = _v.x; this->w = _v.y; }
			/// <summary>ベクトル(z,x)の設定</summary>
			/// <param name="_v">ベクトル(z,x)</param>
			void SetZx(const Float2& _v){ this->z = _v.x; this->x = _v.y; }
			/// <summary>ベクトル(z,y)の設定</summary>
			/// <param name="_v">ベクトル(z,y)</param>
			void SetZy(const Float2& _v){ this->z = _v.x; this->y = _v.y; }
			/// <summary>ベクトル(z,w)の設定</summary>
			/// <param name="_v">ベクトル(z,w)</param>
			void SetZw(const Float2& _v){ this->z = _v.x; this->w = _v.y; }
			/// <summary>ベクトル(w,x)の設定</summary>
			/// <param name="_v">ベクトル(w,x)</param>
			void SetWx(const Float2& _v){ this->w = _v.x; this->x = _v.y; }
			/// <summary>ベクトル(w,y)の設定</summary>
			/// <param name="_v">ベクトル(w,y)</param>
			void SetWy(const Float2& _v){ this->w = _v.x; this->y = _v.y; }
			/// <summary>ベクトル(w,z)の設定</summary>
			/// <param name="_v">ベクトル(w,z)</param>
			void SetWz(const Float2& _v){ this->w = _v.x; this->z = _v.y; }
			/// <summary>ベクトル(x,y,z)の取得</summary>
			/// <param name="_v">ベクトル(x,y,z)</param>
			void SetXyz(const Float3& _v){ this->x = _v.x; this->y = _v.y; this->z = _v.z; }
			/// <summary>ベクトル(x,y,w)の取得</summary>
			/// <param name="_v">ベクトル(x,y,w)</param>
			void SetXyw(const Float3& _v){ this->x = _v.x; this->y = _v.y; this->w = _v.z; }
			/// <summary>ベクトル(x,z,y)の取得</summary>
			/// <param name="_v">ベクトル(x,z,y)</param>
			void SetXzy(const Float3& _v){ this->x = _v.x; this->z = _v.y; this->y = _v.z; }
			/// <summary>ベクトル(x,z,w)の取得</summary>
			/// <param name="_v">ベクトル(x,z,w)</param>
			void SetXzw(const Float3& _v){ this->x = _v.x; this->z = _v.y; this->w = _v.z; }
			/// <summary>ベクトル(x,w,y)の取得</summary>
			/// <param name="_v">ベクトル(x,w,y)</param>
			void SetXwy(const Float3& _v){ this->x = _v.x; this->w = _v.y; this->y = _v.z; }
			/// <summary>ベクトル(x,w,z)の取得</summary>
			/// <param name="_v">ベクトル(x,w,z)</param>
			void SetXwz(const Float3& _v){ this->x = _v.x; this->w = _v.y; this->z = _v.z; }
			/// <summary>ベクトル(y,x,z)の取得</summary>
			/// <param name="_v">ベクトル(y,x,z)</param>
			void SetYxz(const Float3& _v){ this->y = _v.x; this->x = _v.y; this->z = _v.z; }
			/// <summary>ベクトル(y,x,w)の取得</summary>
			/// <param name="_v">ベクトル(y,x,w)</param>
			void SetYxw(const Float3& _v){ this->y = _v.x; this->x = _v.y; this->w = _v.z; }
			/// <summary>ベクトル(y,z,x)の取得</summary>
			/// <param name="_v">ベクトル(y,z,x)</param>
			void SetYzx(const Float3& _v){ this->y = _v.x; this->z = _v.y; this->x = _v.z; }
			/// <summary>ベクトル(y,z,w)の取得</summary>
			/// <param name="_v">ベクトル(y,z,w)</param>
			void SetYzw(const Float3& _v){ this->y = _v.x; this->z = _v.y; this->w = _v.z; }
			/// <summary>ベクトル(y,w,x)の取得</summary>
			/// <param name="_v">ベクトル(y,w,x)</param>
			void SetYwx(const Float3& _v){ this->y = _v.x; this->w = _v.y; this->x = _v.z; }
			/// <summary>ベクトル(y,w,z)の取得</summary>
			/// <param name="_v">ベクトル(y,w,z)</param>
			void SetYwz(const Float3& _v){ this->y = _v.x; this->w = _v.y; this->z = _v.z; }
			/// <summary>ベクトル(z,x,y)の取得</summary>
			/// <param name="_v">ベクトル(z,x,y)</param>
			void SetZxy(const Float3& _v){ this->z = _v.x; this->x = _v.y; this->y = _v.z; }
			/// <summary>ベクトル(z,x,w)の取得</summary>
			/// <param name="_v">ベクトル(z,x,w)</param>
			void SetZxw(const Float3& _v){ this->z = _v.x; this->x = _v.y; this->w = _v.z; }
			/// <summary>ベクトル(z,y,x)の取得</summary>
			/// <param name="_v">ベクトル(z,y,x)</param>
			void SetZyx(const Float3& _v){ this->z = _v.x; this->y = _v.y; this->x = _v.z; }
			/// <summary>ベクトル(z,y,w)の取得</summary>
			/// <param name="_v">ベクトル(z,y,w)</param>
			void SetZyw(const Float3& _v){ this->z = _v.x; this->y = _v.y; this->w = _v.z; }
			/// <summary>ベクトル(z,w,x)の取得</summary>
			/// <param name="_v">ベクトル(z,w,x)</param>
			void SetZwx(const Float3& _v){ this->z = _v.x; this->w = _v.y; this->x = _v.z; }
			/// <summary>ベクトル(z,w,y)の取得</summary>
			/// <param name="_v">ベクトル(z,w,y)</param>
			void SetZwy(const Float3& _v){ this->z = _v.x; this->w = _v.y; this->y = _v.z; }
			/// <summary>ベクトル(w,x,y)の取得</summary>
			/// <param name="_v">ベクトル(w,x,y)</param>
			void SetWxy(const Float3& _v){ this->w = _v.x; this->x = _v.y; this->y = _v.z; }
			/// <summary>ベクトル(w,x,z)の取得</summary>
			/// <param name="_v">ベクトル(w,x,z)</param>
			void SetWxz(const Float3& _v){ this->w = _v.x; this->x = _v.y; this->z = _v.z; }
			/// <summary>ベクトル(w,y,x)の取得</summary>
			/// <param name="_v">ベクトル(w,y,x)</param>
			void SetWyx(const Float3& _v){ this->w = _v.x; this->y = _v.y; this->x = _v.z; }
			/// <summary>ベクトル(w,y,z)の取得</summary>
			/// <param name="_v">ベクトル(w,y,z)</param>
			void SetWyz(const Float3& _v){ this->w = _v.x; this->y = _v.y; this->z = _v.z; }
			/// <summary>ベクトル(w,z,x)の取得</summary>
			/// <param name="_v">ベクトル(w,z,x)</param>
			void SetWzx(const Float3& _v){ this->w = _v.x; this->z = _v.y; this->x = _v.z; }
			/// <summary>ベクトル(w,z,y)の取得</summary>
			/// <param name="_v">ベクトル(w,z,y)</param>
			void SetWzy(const Float3& _v){ this->w = _v.x; this->z = _v.y; this->y = _v.z; }

		public:
			/// <summary>内積</summary>
			/// <param name="_vector">ベクトル</param>
			/// <returns>内積</returns>
			float Dot(const Float4& _vector) const {
				return (this->x*_vector.x + this->y*_vector.y + this->z*_vector.z + this->w*_vector.w);
			}
			/// <summary>長さの2乗</summary>
			/// <returns>長さの2乗</returns>
			float LengthSquared() const {
				return (this->x*this->x + this->y*this->y + this->z*this->z + this->w*this->w);
			}
			// 長さ
			float Length() const;
			// 正規化
			void Normalize();
			// 正規化
			void Normalize(Float4* const _normalizedVector) const;
		};
	}
}

#endif
