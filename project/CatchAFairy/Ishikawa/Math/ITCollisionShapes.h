//=============================================================================
//	ITCollisionShapes.h
//
//	自作衝突形状のヘッダファイル
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
			/// 直線
			/// </summary>
			struct Line{
			public:
				/// <summary>基点</summary>
				Float3 origin;
				/// <summary>方向ベクトル</summary>
				Float3 direction;


			public:
				/// <summary>デフォルトコンストラクタ</summary>
				Line():origin(),direction(){}
				/// <summary>フルコンストラクタ</summary>
				/// <param name="_origin">始点</param>
				/// <param name="_direction">方向ベクトル</param>
				Line(const Float3& _origin, const Float3& _direction) :
					origin(_origin),
					direction(_direction)
				{}
				/// <summary>コンストラクタ</summary>
				/// <param name="_originX">始点のx座標</param>
				/// <param name="_originY">始点のy座標</param>
				/// <param name="_originZ">始点のz座標</param>
				/// <param name="_directionX">方向ベクトルのx座標</param>
				/// <param name="_directionY">方向ベクトルのy座標</param>
				/// <param name="_directionZ">方向ベクトルのz座標</param>
				Line(
					const float _originX, const float _originY, const float _originZ,
					const float _directionX, const float _directionY, const float _directionZ
				) :
					origin(_originX, _originY, _originZ),
					direction(_directionX, _directionY, _directionZ)
				{}
				/// <summary>コピーコンストラクタ</summary>
				/// <param name="_original">コピー元</param>
				Line(const Line& _original) :
					origin(_original.origin),
					direction(_original.direction)
				{}
				/// <summary>デストラクタ</summary>
				~Line(){}

			public:
				/// <summary>代入演算子</summary>
				/// <param name="_original">コピー元</param>
				/// <returns>自身のデータ</returns>
				Line& operator=(const Line& _original){
					this->origin = _original.origin;
					this->direction = _original.direction;
					return (*this);
				}

			public:
				/// <summary>この直線上にある座標の取得</summary>
				/// <param name="_ratio">比率</param>
				/// <returns>対応する座標</returns>
				Float3 GetPoint(const float _ratio) const {
					return (this->origin + this->direction*_ratio);
				}
			};



			/// <summary>
			/// 線分
			/// </summary>
			struct LineSegment{
			public:
				/// <summary>始点</summary>
				Float3 start;
				/// <summary>ベクトル</summary>
				Float3 vector;


			public:
				/// <summary>デフォルトコンストラクタ</summary>
				LineSegment():start(),vector(){}
				/// <summary>フルコンストラクタ</summary>
				/// <param name="_start">始点</param>
				/// <param name="_vector">ベクトル</param>
				LineSegment(const Float3& _start, const Float3& _vector) :
					start(_start),
					vector(_vector)
				{}
				/// <summary>コンストラクタ</summary>
				/// <param name="_startX">始点のx座標</param>
				/// <param name="_startY">始点のy座標</param>
				/// <param name="_startZ">始点のz座標</param>
				/// <param name="_vectorX">ベクトルのx座標</param>
				/// <param name="_vectorY">ベクトルのy座標</param>
				/// <param name="_vectorZ">ベクトルのz座標</param>
				LineSegment(
					const float _startX, const float _startY, const float _startZ,
					const float _vectorX, const float _vectorY, const float _vectorZ
				) :
					start(_startX, _startY, _startZ),
					vector(_vectorX, _vectorY, _vectorZ)
				{}
				/// <summary>コピーコンストラクタ</summary>
				/// <param name="_original">コピー元</param>
				LineSegment(const LineSegment& _original) :
					start(_original.start),
					vector(_original.vector)
				{}
				/// <summary>デストラクタ</summary>
				~LineSegment(){}

			public:
				/// <summary>代入演算子</summary>
				/// <param name="_original">コピー元</param>
				/// <returns>自身のデータ</returns>
				LineSegment& operator=(const LineSegment& _original){
					this->start = _original.start;
					this->vector = _original.vector;
					return (*this);
				}

			public:
				/// <summary>終点の取得</summary>
				/// <returns>終点</returns>
				Float3 GetEndPoint() const {
					return (this->start + this->vector);
				}

				/// <summary>この線分を含む直線の取得</summary>
				/// <returns>対応する直線</returns>
				Line GetLine() const {
					return Line(this->start, this->vector);
				}

				/// <summary>この線分上にある座標の取得</summary>
				/// <param name="_ratio">比率</param>
				/// <returns>対応する座標 [比率の値が0.0～1.0にクリッピングされます]</returns>
				Float3 GetPoint(const float _ratio) const {
					float ratio = (_ratio<0.0f)?(0.0f):((_ratio>1.0f)?(1.0f):(_ratio));
					return (this->start + this->vector*ratio);
				}
			};



			/// <summary>
			/// 平面[基点と法線]
			/// </summary>
			struct Planar3x2{
				/// <summary>基点</summary>
				Float3 origin;
				/// <summary>法線</summary>
				Float3 normal;


			public:
				/// <summary>デフォルトコンストラクタ</summary>
				Planar3x2():origin(),normal(){}
				/// <summary>フルコンストラクタ</summary>
				/// <param name="_origin">基点</param>
				/// <param name="_normal">法線</param>
				Planar3x2(const Float3& _origin, const Float3& _normal) :
					origin(_origin),
					normal(_normal)
				{}
				/// <summary>コンストラクタ</summary>
				/// <param name="_originX">基点のx座標</param>
				/// <param name="_originY">基点のy座標</param>
				/// <param name="_originZ">基点のz座標</param>
				/// <param name="_normalX">法線のx座標</param>
				/// <param name="_normalY">法線のy座標</param>
				/// <param name="_normalZ">法線のz座標</param>
				Planar3x2(
					const float _originX, const float _originY, const float _originZ,
					const float _normalX, const float _normalY, const float _normalZ
				) :
					origin(_originX, _originY, _originZ),
					normal(_normalX, _normalY, _normalZ)
				{}
				/// <summary>コピーコンストラクタ</summary>
				/// <param name="_original">コピー元</param>
				Planar3x2(const Planar3x2& _original) :
					origin(_original.origin),
					normal(_original.normal)
				{}
				/// <summary>デストラクタ</summary>
				~Planar3x2(){}

			public:
				/// <summary>代入演算子</summary>
				/// <param name="_original">コピー元</param>
				/// <returns>自身のデータ</returns>
				Planar3x2& operator=(const Planar3x2& _original){
					this->origin = _original.origin;
					this->normal = _original.normal;
					return (*this);
				}
			};

			/// <summary>
			/// 平面[0=ax+by+cz+d]
			/// </summary>
			struct Planar4{
				/// <summary>xの係数</summary>
				float a;
				/// <summary>yの係数</summary>
				float b;
				/// <summary>zの係数</summary>
				float c;
				/// <summary>バイアス</summary>
				float d;


			public:
				/// <summary>デフォルトコンストラクタ</summary>
				Planar4():a(0.0f),b(0.0f),c(0.0f),d(0.0f){}
				/// <summary>フルコンストラクタ</summary>
				/// <param name="_a">xの係数</param>
				/// <param name="_b">yの係数</param>
				/// <param name="_c">zの係数</param>
				/// <param name="_d">バイアス</param>
				Planar4(const float _a, const float _b, const float _c, const float _d) :
					a(_a),b(_b),c(_c),d(_d)
				{}
				/// <summary>コピーコンストラクタ</summary>
				/// <param name="_original">コピー元</param>
				Planar4(const Planar4& _original) :
					a(_original.a),
					b(_original.b),
					c(_original.c),
					d(_original.d)
				{}
				/// <summary>デストラクタ</summary>
				~Planar4(){}

			public:
				/// <summary>代入演算子</summary>
				/// <param name="_original">コピー元</param>
				/// <returns>自身のデータ</returns>
				Planar4& operator=(const Planar4& _original){
					this->a = _original.a;
					this->b = _original.b;
					this->c = _original.c;
					this->d = _original.d;
					return (*this);
				}
			};



			/// <summary>
			/// 球
			/// </summary>
			struct Sphere{
				/// <summary>基点</summary>
				Float3 origin;
				/// <summary>半径</summary>
				float radius;


			public:
				/// <summary>デフォルトコンストラクタ</summary>
				Sphere():origin(),radius(0.0f){}
				/// <summary>フルコンストラクタ</summary>
				/// <param name="_origin">基点</param>
				/// <param name="_radius">半径</param>
				Sphere(const Float3& _origin, const float _radius) :
					origin(_origin),
					radius(_radius)
				{}
				/// <summary>コンストラクタ</summary>
				/// <param name="_originX">基点のx座標</param>
				/// <param name="_originY">基点のy座標</param>
				/// <param name="_originZ">基点のz座標</param>
				/// <param name="_radius">半径</param>
				Sphere(
					const float _originX, const float _originY, const float _originZ,
					const float _radius
				) :
					origin(_originX, _originY, _originZ),
					radius(_radius)
				{}
				/// <summary>コピーコンストラクタ</summary>
				/// <param name="_original">コピー元</param>
				Sphere(const Sphere& _original) :
					origin(_original.origin),
					radius(_original.radius)
				{}
				/// <summary>デストラクタ</summary>
				~Sphere(){}

			public:
				/// <summary>代入演算子</summary>
				/// <param name="_original">コピー元</param>
				/// <returns>自身のデータ</returns>
				Sphere& operator=(const Sphere& _original){
					this->origin = _original.origin;
					this->radius = _original.radius;
					return (*this);
				}
			};



			/// <summary>
			/// 無限長円柱
			/// </summary>
			struct InfiniteCylinder{
				/// <summary>軸</summary>
				Line axis;
				/// <summary>半径</summary>
				float radius;


			public:
				/// <summary>デフォルトコンストラクタ</summary>
				InfiniteCylinder():axis(),radius(0.0f){}
				/// <summary>フルコンストラクタ</summary>
				/// <param name="_axis">軸</param>
				/// <param name="_radius">半径</param>
				InfiniteCylinder(const Line& _axis, const float _radius) :
					axis(_axis),
					radius(_radius)
				{}
				/// <summary>フルコンストラクタ</summary>
				/// <param name="_origin">基点</param>
				/// <param name="_direction">方向ベクトル</param>
				/// <param name="_radius">半径</param>
				InfiniteCylinder(const Float3& _origin, const Float3& _direction, const float _radius) :
					axis(_origin, _direction),
					radius(_radius)
				{}
				/// <summary>コンストラクタ</summary>
				/// <param name="_originX">基点のx座標</param>
				/// <param name="_originY">基点のy座標</param>
				/// <param name="_originZ">基点のz座標</param>
				/// <param name="_directionX">方向ベクトルのx座標</param>
				/// <param name="_directionY">方向ベクトルのy座標</param>
				/// <param name="_directionZ">方向ベクトルのz座標</param>
				/// <param name="_radius">半径</param>
				InfiniteCylinder(
					const float _originX, const float _originY, const float _originZ,
					const float _directionX, const float _directionY, const float _directionZ,
					const float _radius
				) :
					axis(_originX, _originY, _originZ, _directionX, _directionY, _directionZ),
					radius(_radius)
				{}
				/// <summary>コピーコンストラクタ</summary>
				/// <param name="_original">コピー元</param>
				InfiniteCylinder(const InfiniteCylinder& _original) :
					axis(_original.axis),
					radius(_original.radius)
				{}
				/// <summary>デストラクタ</summary>
				~InfiniteCylinder(){}

			public:
				/// <summary>代入演算子</summary>
				/// <param name="_original">コピー元</param>
				/// <returns>自身のデータ</returns>
				InfiniteCylinder& operator=(const InfiniteCylinder& _original){
					this->axis = _original.axis;
					this->radius = _original.radius;
					return (*this);
				}
			};



			/// <summary>
			/// カプセル
			/// </summary>
			struct Capsule{
				/// <summary>軸線分</summary>
				LineSegment axis;
				/// <summary>半径</summary>
				float radius;


			public:
				/// <summary>デフォルトコンストラクタ</summary>
				Capsule():axis(),radius(0.0f){}
				/// <summary>フルコンストラクタ</summary>
				/// <param name="_axis">軸線分</param>
				/// <param name="_radius">半径</param>
				Capsule(const LineSegment& _axis, const float _radius) :
					axis(_axis),
					radius(_radius)
				{}
				/// <summary>フルコンストラクタ</summary>
				/// <param name="_start">始点</param>
				/// <param name="_vector">ベクトル</param>
				/// <param name="_radius">半径</param>
				Capsule(const Float3& _start, const Float3& _vector, const float _radius) :
					axis(_start, _vector),
					radius(_radius)
				{}
				/// <summary>コンストラクタ</summary>
				/// <param name="_startX">始点のx座標</param>
				/// <param name="_startY">始点のy座標</param>
				/// <param name="_startZ">始点のz座標</param>
				/// <param name="_vectorX">ベクトルのx座標</param>
				/// <param name="_vectorY">ベクトルのy座標</param>
				/// <param name="_vectorZ">ベクトルのz座標</param>
				/// <param name="_radius">半径</param>
				Capsule(
					const float _startX, const float _startY, const float _startZ,
					const float _vectorX, const float _vectorY, const float _vectorZ,
					const float _radius
				) :
					axis(_startX, _startY, _startZ, _vectorX, _vectorY, _vectorZ),
					radius(_radius)
				{}
				/// <summary>コピーコンストラクタ</summary>
				/// <param name="_original">コピー元</param>
				Capsule(const Capsule& _original) :
					axis(_original.axis),
					radius(_original.radius)
				{}
				/// <summary>デストラクタ</summary>
				~Capsule(){}

			public:
				/// <summary>代入演算子</summary>
				/// <param name="_original">コピー元</param>
				/// <returns>自身のデータ</returns>
				Capsule& operator=(const Capsule& _original){
					this->axis = _original.axis;
					this->radius = _original.radius;
					return (*this);
				}

			public:
				/// <summary>このカプセルを内側に含む同半径の無限長円柱を取得する</summary>
				/// <returns>このカプセルを内側に含む同半径の無限長円柱</returns>
				InfiniteCylinder GetInfiniteCylinder() const {
					return InfiniteCylinder(this->axis.start, this->axis.vector, this->radius);
				}
			};
		}
	}
}

#endif
