//=============================================================================
//	ITCollision.cpp
//
//	自作衝突判定のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITCollision.h"
#include <float.h>
#include "Ishikawa/Common/DebugLog.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "ITFloatN.h"
#include "ITMatrix.h"
#include "ITCollisionShapes.h"
#include "ITMathFunctions.h"

using namespace ::Ishikawa::Math;


/// <summary>
/// 反射用変換行列(XY平面を反射平面に重なるように変換する行列)の作成
/// </summary>
/// <param name="_anchor">基点座標</param>
/// <param name="_normal">反射平面の法線ベクトル</param>
/// <returns>反射用変換行列</returns>
static ::Ishikawa::Math::Matrix GetRefrectionMatrix(
	const ::Ishikawa::Math::Float3& _anchor,
	const ::Ishikawa::Math::Float3& _normal
){
	if(_normal.LengthSquared() < FLT_EPSILON){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Reflection vector length is ZERO.");
	}

	// 各軸ベクトルの作成
	const ::Ishikawa::Math::Float3& axisZ = _normal;
	::Ishikawa::Math::Float3 axisX(axisZ.z-axisZ.y, axisZ.x-axisZ.z, axisZ.y-axisZ.x);
	::Ishikawa::Math::Float3 axisY = axisX.Cross(axisZ);

	return ::Ishikawa::Math::Matrix(
		axisX.x, axisX.y, axisX.z, 0.0f,
		axisY.x, axisY.y, axisY.z, 0.0f,
		axisZ.x, axisZ.y, axisZ.z, 0.0f,
		_anchor.x, _anchor.y, _anchor.z, 1.0f
	);
}


/// <summary>
/// ベクトルを平面で反射させる
/// </summary>
/// <param name="_startPosition">ベクトル始点</param>
/// <param name="_vector">ベクトル</param>
/// <param name="_matrixVectorToPlanar">ベクトル空間から平面空間への変換行列</param>
/// <param name="_planarAnchor">反射平面の基点座標</param>
/// <param name="_planarNormal">反射平面の法線ベクトル</param>
/// <param name="_reflectionRate">反射率</param>
/// <returns>反射後の位置へのベクトル</returns>
::Ishikawa::Math::Float3 Collision::ReflectVectorAtPlanar(
	const ::Ishikawa::Math::Float3& _startPosition,
	const ::Ishikawa::Math::Float3& _vector,
	const ::Ishikawa::Math::Matrix& _matrixVectorToPlanar,
	const ::Ishikawa::Math::Float3& _planarAnchor,
	const ::Ishikawa::Math::Float3& _planarNormal,
	float _reflectionRate
){
	if(_vector.LengthSquared() < FLT_EPSILON){
		// ベクトルがゼロベクトルならそのまま返す
		return _vector;
	}

	// ベクトル空間から反射空間への変換行列
	::Ishikawa::Math::Matrix matrixVectorToReflection = _matrixVectorToPlanar * GetRefrectionMatrix(_planarAnchor, _planarNormal).Invert();

	// 反射空間上でのベクトル始点
	::Ishikawa::Math::Float4 start = ::Ishikawa::Math::Float4(_startPosition, 1.0f) * matrixVectorToReflection;
	// 反射空間上でのベクトル終点
	::Ishikawa::Math::Float4 end = ::Ishikawa::Math::Float4(_startPosition+_vector, 1.0f) * matrixVectorToReflection;

	// 裏から通過する場合か反射平面を通過しない場合はそのまま返す
	if((start.z < 0.0f)||(end.z > 0.0f)) return _vector;

	// 反射空間上での反射後の座標
	::Ishikawa::Math::Float4 reflected = end;
	reflected.z *= -_reflectionRate;

	// ベクトル空間上での反射後の座標
	::Ishikawa::Math::Float4 reflectedEnd = reflected * matrixVectorToReflection.Invert();
	// ベクトル空間上での反射後の位置へのベクトル
	::Ishikawa::Math::Float3 reflectedVector(
		reflectedEnd.x - _startPosition.x,
		reflectedEnd.y - _startPosition.y,
		reflectedEnd.z - _startPosition.z
	);

	return reflectedVector;
}

/// <summary>
/// ベクトルをカプセルで反射させる
/// </summary>
/// <param name="_startPosition">ベクトル始点</param>
/// <param name="_vector">ベクトル</param>
/// <param name="_matrixVectorToCapsule">ベクトル空間からカプセル空間への変換行列</param>
/// <param name="_capsuleAxisStartPosition">カプセル軸始点</param>
/// <param name="_capsuleAxisVector">カプセル軸ベクトル</param>
/// <param name="_capsuleRadius">カプセル半径</param>
/// <param name="_reflectionRate">反射率</param>
/// <returns>反射後の位置へのベクトル</returns>
::Ishikawa::Math::Float3 Collision::ReflectVectorAtCapsule(
	const ::Ishikawa::Math::Float3& _startPosition,
	const ::Ishikawa::Math::Float3& _vector,
	const ::Ishikawa::Math::Matrix& _matrixVectorToCapsule,
	const ::Ishikawa::Math::Float3& _capsuleAxisStartPosition,
	const ::Ishikawa::Math::Float3& _capsuleAxisVector,
	float _capsuleRadius,
	float _reflectionRate
){
	if(_vector.LengthSquared() < FLT_EPSILON){
		// ベクトルがゼロベクトルならそのまま返す
		return _vector;
	}

	float squareLengthCapsuleAxisVector = _capsuleAxisVector.LengthSquared();
	if(squareLengthCapsuleAxisVector < FLT_EPSILON){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Capsule axis vector length is ZERO.");
	}

	// カプセル空間でのベクトル直線
	::Ishikawa::Math::Float3 vectorInCapsule = (::Ishikawa::Math::Float4(_vector, 0.0f)*_matrixVectorToCapsule).GetXyz();
	if(vectorInCapsule.LengthSquared() < FLT_EPSILON){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Vector length is ZERO.");
	}
	::Ishikawa::Math::Float3 vectorStartInCapsule = (::Ishikawa::Math::Float4(_startPosition, 1.0f)*_matrixVectorToCapsule).GetXyz();
	
	Line vectorLineInCapsule(vectorStartInCapsule, vectorInCapsule);
	Capsule capsule(_capsuleAxisStartPosition, _capsuleAxisVector, _capsuleRadius);
	float collPointRate[2] = {0.0f, 0.0f};
	if(IsCollideLineAndCapsule(vectorLineInCapsule, capsule, &collPointRate[0], &collPointRate[1])){
		if((collPointRate[0] > 0.0f) && (collPointRate[0] < 1.0f)){
			// 反射させる
			::Ishikawa::Math::Float3 collPoint = vectorLineInCapsule.GetPoint(collPointRate[0]);
			float rate = capsule.axis.vector.Dot(collPoint-capsule.axis.start) / capsule.axis.vector.LengthSquared();
			::Ishikawa::Math::Float3 reflectNormal = collPoint - capsule.axis.GetPoint(rate);
			::Ishikawa::Math::Float3 reflectionVector = vectorLineInCapsule.direction * (1.0f-collPointRate[0]);
			::Ishikawa::Math::Float3 reflect = (reflectNormal*(reflectionVector.Dot(reflectNormal)/reflectNormal.LengthSquared()));
			::Ishikawa::Math::Float3 reflectedVector = vectorLineInCapsule.direction - reflect*(1.0f+_reflectionRate);
			::Ishikawa::Math::Float3 reflected = (::Ishikawa::Math::Float4(reflectedVector, 0.0f) * _matrixVectorToCapsule.Invert()).GetXyz();
			return reflected;
		}
	}
	// 反射なし
	return _vector;
	
/*
	// カプセル空間でのベクトル
	::Ishikawa::Math::Float4 vectorInCapsule4 = ::Ishikawa::Math::Float4(_vector, 0.0f) * _matrixVectorToCapsule;
	::Ishikawa::Math::Float3 vectorInCapsule(vectorInCapsule4.x, vectorInCapsule4.y, vectorInCapsule4.z);
	float squareLengthVector = vectorInCapsule.LengthSquared();
	if(squareLengthVector < FLT_EPSILON){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Vector length is ZERO.");
	}
	// カプセル空間でのベクトル始点
	::Ishikawa::Math::Float4 vectorStartInCapsule4 = ::Ishikawa::Math::Float4(_startPosition, 1.0f) * _matrixVectorToCapsule;
	::Ishikawa::Math::Float3 vectorStartInCapsule(vectorStartInCapsule4.x, vectorStartInCapsule4.y, vectorStartInCapsule4.z);

	auto GetReflectVector = [=](
		const ::Ishikawa::Math::Float3& _direction,
		const ::Ishikawa::Math::Float3& _reflectPosition
		)->::Ishikawa::Math::Float3
	{
		// 反射空間への変換行列
		::Ishikawa::Math::Matrix reflectionMatrix = GetRefrectionMatrix(_reflectPosition, _direction);
		::Ishikawa::Math::Matrix invertReflectionMatrix = reflectionMatrix.Invert();
		// 反射空間上でのベクトル終点
		::Ishikawa::Math::Float4 end = ::Ishikawa::Math::Float4(vectorStartInCapsule+vectorInCapsule, 1.0f) * invertReflectionMatrix;
		// 反射空間上での反射後の座標
		::Ishikawa::Math::Float4 reflected = end;
		reflected.z *= -_reflectionRate;
		// ベクトル空間上での反射後の座標
		::Ishikawa::Math::Float4 reflectedEnd = reflected * reflectionMatrix * _matrixVectorToCapsule.Invert();
		// ベクトル空間上での反射後の位置へのベクトル
		::Ishikawa::Math::Float3 reflectedVector(
			reflectedEnd.x - _startPosition.x,
			reflectedEnd.y - _startPosition.y,
			reflectedEnd.z - _startPosition.z
		);
		return reflectedVector;
	};

	// ベクトルがカプセル軸に平行なら処理を変更
	if(_capsuleAxisVector.Cross(vectorInCapsule).LengthSquared() < FLT_EPSILON){
		// カプセル軸始点からベクトル始点への差分ベクトル
		::Ishikawa::Math::Float3 deltaStartToAxisStart = _capsuleAxisStartPosition - vectorStartInCapsule;
		// カプセル軸ベクトルと差分ベクトルの内積
		float axisVectorDotDelta = _capsuleAxisVector.Dot(deltaStartToAxisStart);
		// ベクトル上のカプセル軸始点に一番近い点までの距離とベクトル長さとの比率
		float lengthRateAtNearestAxisStartPointInVector = axisVectorDotDelta / squareLengthVector;
		// 2つのベクトルの距離の二乗
		float squareLengthTwoVector = (deltaStartToAxisStart - vectorInCapsule*lengthRateAtNearestAxisStartPointInVector).LengthSquared();
		// 距離が半径よりあれば反射しない(ベクトルをそのまま返す)
		float squareLengthDelta = _capsuleRadius*_capsuleRadius - squareLengthTwoVector;
		if(squareLengthDelta < 0.0f){
			return _vector;
		}

		// カプセル軸終点
		::Ishikawa::Math::Float3 capsuleAxisEndPosition = _capsuleAxisStartPosition + _capsuleAxisVector;
		// ベクトル上のカプセル軸終点に一番近い点までの距離とベクトル長さとの比率
		float lengthRateAtNearestAxisEndPointInVector = _capsuleAxisVector.Dot(capsuleAxisEndPosition - vectorStartInCapsule) / squareLengthVector;

		// カプセル表面と当たる位置を計算に必要な差分情報を計算
		float delta = Functions::SquareRoot(squareLengthDelta / vectorInCapsule.LengthSquared());
		// 方向判定
		if(_capsuleAxisVector.Dot(vectorInCapsule) > 0.0f){
			// 同方向ならカプセル軸始点から調べる
			float startRate = lengthRateAtNearestAxisStartPointInVector - delta;
			if((startRate >= 0.0f) && (startRate <= 1.0f)){
				// カプセル軸始点付近で反射する
				::Ishikawa::Math::Float3 reflectPosition = vectorStartInCapsule + vectorInCapsule*startRate;
				::Ishikawa::Math::Float3 direction = reflectPosition - _capsuleAxisStartPosition;
				return GetReflectVector(direction, reflectPosition);
			}
			float endRate = lengthRateAtNearestAxisEndPointInVector + delta;
			if((endRate >= 0.0f) && (endRate <= 1.0f)){
				// カプセル軸終点付近で反射する
				::Ishikawa::Math::Float3 reflectPosition = vectorStartInCapsule + vectorInCapsule*endRate;
				::Ishikawa::Math::Float3 direction = reflectPosition - capsuleAxisEndPosition;
				return GetReflectVector(direction, reflectPosition);
			}
		}
		else{
			// 逆方向ならカプセル軸終点から調べる
			float endRate = lengthRateAtNearestAxisEndPointInVector - delta;
			if((endRate >= 0.0f) && (endRate <= 1.0f)){
				// カプセル軸終点付近で反射する
				::Ishikawa::Math::Float3 reflectPosition = vectorStartInCapsule + vectorInCapsule*endRate;
				::Ishikawa::Math::Float3 direction = reflectPosition - capsuleAxisEndPosition;
				return GetReflectVector(direction, reflectPosition);
			}
			float startRate = lengthRateAtNearestAxisStartPointInVector + delta;
			if((startRate >= 0.0f) && (startRate <= 1.0f)){
				// カプセル軸始点付近で反射する
				::Ishikawa::Math::Float3 reflectPosition = vectorStartInCapsule + vectorInCapsule*startRate;
				::Ishikawa::Math::Float3 direction = reflectPosition - _capsuleAxisStartPosition;
				return GetReflectVector(direction, reflectPosition);
			}
		}
		// 反射しなかった
		return _vector;
	}

	// 最近点を探す
	{
		// ベクトルとカプセル軸ベクトルの内積
		float vectorDotAxisVector = vectorInCapsule.Dot(_capsuleAxisVector);
		float mulLengths = squareLengthVector*squareLengthCapsuleAxisVector;
		float div = 1.0f - vectorDotAxisVector*vectorDotAxisVector/mulLengths;
		if((div < FLT_EPSILON) && (div > -FLT_EPSILON)){
			// 異常終了
			throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't get nearest point.");
		}
		// カプセル軸始点からベクトル始点への差分ベクトル
		::Ishikawa::Math::Float3 deltaAxisStartToStart = vectorStartInCapsule - _capsuleAxisStartPosition;
		// カプセル軸ベクトルと差分ベクトルの内積
		float axisVectorDotDelta = _capsuleAxisVector.Dot(deltaAxisStartToStart);
		// ベクトルと差分ベクトルの内積
		float vectorDotDelta = vectorInCapsule.Dot(deltaAxisStartToStart);
		// ベクトルの始点から最近点までの距離比率
		float nearestVectorPointRate = (axisVectorDotDelta*vectorDotAxisVector/mulLengths - vectorDotDelta/squareLengthVector) / div;
		// カプセル軸始点から最近点までの距離比率
//		float nearestAxisVectorPointRate = (nearestVectorPointRate*vectorDotAxisVector + axisVectorDotDelta) / squareLengthCapsuleAxisVector;

		// ベクトルの中で最近点に一番近い位置
		::Ishikawa::Math::Float3 nearestVectorPoint = vectorStartInCapsule;
		if(nearestVectorPointRate > 0.0f){
			nearestVectorPoint = vectorStartInCapsule + ((nearestVectorPointRate > 1.0f) ? (vectorInCapsule) : (vectorInCapsule*nearestVectorPointRate));
		}
		// カプセル軸の中でベクトル内の最近点に一番近い位置
		float nearestAxisVectorPointRate = _capsuleAxisVector.Dot(nearestVectorPoint - _capsuleAxisStartPosition);
		::Ishikawa::Math::Float3 nearestAxisVectorPoint = _capsuleAxisStartPosition;
		if(nearestAxisVectorPointRate > 0.0f){
			nearestAxisVectorPoint = _capsuleAxisStartPosition + ((nearestAxisVectorPointRate > 1.0f) ? (_capsuleAxisVector) : (_capsuleAxisVector*nearestAxisVectorPointRate));
		}

		// TODO:カプセル表面で反射させる

		if((nearestAxisVectorPoint-nearestVectorPoint).LengthSquared() <= (_capsuleRadius*_capsuleRadius)){
			// カプセルと接触しているなら始点で反射 (表面での反射実装までの代用処理)
			::Ishikawa::Math::Float3 reflectPosition = vectorStartInCapsule;
			// カプセル軸の中で反射点に一番近い位置
			float nearestPointRateInAxisVector = _capsuleAxisVector.Dot(reflectPosition - _capsuleAxisStartPosition);
			::Ishikawa::Math::Float3 nearestPointInAxisVector = _capsuleAxisStartPosition;
			if(nearestPointRateInAxisVector > 0.0f){
				nearestAxisVectorPoint = _capsuleAxisStartPosition + ((nearestPointRateInAxisVector > 1.0f) ? (_capsuleAxisVector) : (_capsuleAxisVector*nearestPointRateInAxisVector));
			}
			// 反射方向
			::Ishikawa::Math::Float3 direction = reflectPosition - nearestPointInAxisVector;
			// 反射
			return GetReflectVector(direction, reflectPosition);
		}

		// 反射しない
		return _vector;
	}
*/
}

/// <summary>
/// 点と直線の距離の2乗の取得
/// </summary>
/// <param name="_point">点</param>
/// <param name="_line">直線</param>
/// <param name="_nearestRatioOfLine">直線の最近点比率</param>
/// <returns>点と直線の距離の2乗</returns>
float Collision::GetDistanceSquaredOfPointAndLine(
	const Float3& _point,
	const Line& _line,
	float* const _nearestRatioOfLine
){
	float lineDirectionLengthSquared = _line.direction.LengthSquared();
	if(lineDirectionLengthSquared < FLT_EPSILON){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("Line direction length is 0.");
	}

	const Float3 delta = _point - _line.origin;
	if(_nearestRatioOfLine != nullptr){
		(*_nearestRatioOfLine) = _line.direction.Dot(delta) / lineDirectionLengthSquared;
	}

	return (_line.direction.Cross(delta).LengthSquared() / lineDirectionLengthSquared);
}

/// <summary>
/// 点と直線の距離の取得
/// </summary>
/// <param name="_point">点</param>
/// <param name="_line">直線</param>
/// <param name="_nearestRatioOfLine">直線の最近点比率</param>
/// <returns>点と直線の距離</returns>
float Collision::GetDistanceOfPointAndLine(
	const Float3& _point,
	const Line& _line,
	float* const _nearestRatioOfLine
){
	return Functions::SquareRoot(GetDistanceSquaredOfPointAndLine(_point, _line, _nearestRatioOfLine));
}

/// <summary>
/// 2直線間の距離の2乗の取得
/// </summary>
/// <param name="_line1">直線1</param>
/// <param name="_line2">直線2</param>
/// <param name="_nearestRatioOfLine1">直線1の最近点比率</param>
/// <param name="_nearestRatioOfLine2">直線2の最近点比率</param>
/// <returns>2直線間の距離の2乗</returns>
float Collision::GetDistanceSquaredOf2Lines(
	const Line& _line1,
	const Line& _line2,
	float* const _nearestRatioOfLine1,
	float* const _nearestRatioOfLine2
){
	float line1DirectionLengthSquared = _line1.direction.LengthSquared();
	if(line1DirectionLengthSquared < FLT_EPSILON){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("Line1 direction length is 0.");
	}
	float line2DirectionLengthSquared = _line2.direction.LengthSquared();
	if(line2DirectionLengthSquared < FLT_EPSILON){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("Line2 direction length is 0.");
	}

	const Float3 normal = _line1.direction.Cross(_line2.direction);
	const Float3 vector = _line2.origin - _line1.origin;

	if(normal.LengthSquared() < FLT_EPSILON){
		// 2直線が平行

		if(_nearestRatioOfLine1 != nullptr){
			(*_nearestRatioOfLine1) = 0.0f;
		}
		if(_nearestRatioOfLine2 != nullptr){
			(*_nearestRatioOfLine2) = _line1.direction.Dot(vector) / line1DirectionLengthSquared;
		}

		return (_line1.direction.Cross(vector).LengthSquared() / line1DirectionLengthSquared);
	}

	if((_nearestRatioOfLine1 != nullptr) || (_nearestRatioOfLine2 != nullptr)){
		const float dotOfLine1AndLine2 = _line1.direction.Dot(_line2.direction);

		const float nearestRatioOfLine1 =
			(line2DirectionLengthSquared*_line1.direction.Dot(vector) - dotOfLine1AndLine2*_line2.direction.Dot(vector)) /
			(line1DirectionLengthSquared*line2DirectionLengthSquared - dotOfLine1AndLine2*dotOfLine1AndLine2);

		if(_nearestRatioOfLine1 != nullptr){
			(*_nearestRatioOfLine1) = nearestRatioOfLine1;
		}
		if(_nearestRatioOfLine2 != nullptr){
			const Float3 nearestPointOfLine1 = _line1.origin + _line1.direction*nearestRatioOfLine1;

			(*_nearestRatioOfLine2) = _line2.direction.Dot(nearestPointOfLine1 - _line2.origin) / line2DirectionLengthSquared;
		}
	}

	const float dot = normal.Dot(vector);
	return ((dot*dot) / normal.LengthSquared());
}

/// <summary>
/// 2直線間の距離の取得
/// </summary>
/// <param name="_line1">直線1</param>
/// <param name="_line2">直線2</param>
/// <param name="_nearestRatioOfLine1">直線1の最近点比率</param>
/// <param name="_nearestRatioOfLine2">直線2の最近点比率</param>
/// <returns>2直線間の距離</returns>
float Collision::GetDistanceOf2Lines(
	const Line& _line1,
	const Line& _line2,
	float* const _nearestRatioOfLine1,
	float* const _nearestRatioOfLine2
){
	return Functions::SquareRoot(GetDistanceSquaredOf2Lines(_line1, _line2, _nearestRatioOfLine1, _nearestRatioOfLine2));
}

/// <summary>
/// 直線同士の衝突判定
/// </summary>
/// <param name="_line1">直線1</param>
/// <param name="_line2">直線2</param>
/// <param name="_collisionPoint">衝突座標格納先</param>
/// <returns>true:衝突している false:衝突していない</returns>
bool Collision::IsCollide2Lines(
	const Line& _line1,
	const Line& _line2,
	::Ishikawa::Math::Float3* const _collisionPoint
){
	if(_line1.direction.LengthSquared() < FLT_EPSILON){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("Line1 direction length is 0.");
	}
	if(_line2.direction.LengthSquared() < FLT_EPSILON){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("Line2 direction length is 0.");
	}

	// 直線1と直線2の法線
	const Float3 normalOfLine1AndLine2 = _line1.direction.Cross(_line2.direction);
	// 平行なら交差しない
	if(normalOfLine1AndLine2.LengthSquared() < FLT_EPSILON) return false;

	// 直線1上の点から直線2上の点へのベクトル
	const Float3 vectorLine1PointToLine2Point = _line2.origin - _line1.origin;
	// 直線1と直線1上の点から直線2上の点へのベクトルの法線
	const Float3 normalOfLine1AndVector = vectorLine1PointToLine2Point.Cross(_line1.direction);
	if(normalOfLine1AndVector.LengthSquared() < FLT_EPSILON){
		// 直線2の基点で交差する
		if(_collisionPoint != nullptr) (*_collisionPoint) = _line2.origin;
		return true;
	}

	const Float3 checkVector = normalOfLine1AndLine2.Cross(normalOfLine1AndVector);
	// 交差しない
	if(checkVector.LengthSquared() > FLT_EPSILON) return false;

	// 交差する

	// 衝突座標計算
	if(_collisionPoint != nullptr){
		float s = 0.0f;
		if(normalOfLine1AndLine2.x > FLT_EPSILON){
			s = normalOfLine1AndVector.x / normalOfLine1AndLine2.x;
		}
		else if(normalOfLine1AndLine2.y > FLT_EPSILON){
			s = normalOfLine1AndVector.y / normalOfLine1AndLine2.y;
		}
		else{
			s = normalOfLine1AndVector.z / normalOfLine1AndLine2.z;
		}
		(*_collisionPoint) = _line1.GetPoint(s);
	}

	return true;
}

/// <summary>
/// 直線と球の衝突判定
/// </summary>
/// <param name="_line">直線</param>
/// <param name="_sphere">球</param>
/// <param name="_lineRatioAtCollidePoint1">衝突点1の直線の比率 (値の小さいほう)</param>
/// <param name="_lineRatioAtCollidePoint2">衝突点2の直線の比率 (値の大きいほう)</param>
/// <returns>true:当たっている false:当たっていない</returns>
bool Collision::IsCollideLineAndSphere(
	const Line& _line,
	const Sphere& _sphere,
	float* const _lineRatioAtCollidePoint1,
	float* const _lineRatioAtCollidePoint2
){
	if(_line.direction.LengthSquared() < FLT_EPSILON){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("Line direction length is 0.");
	}

	const Float3 deltaSphereOrigin = _sphere.origin - _line.origin;

	const float lineLengthSquared = _line.direction.LengthSquared();
	const float dotLineAndDelta = _line.direction.Dot(deltaSphereOrigin);
	const float rate = deltaSphereOrigin.LengthSquared() - (_sphere.radius*_sphere.radius);

	const float rootValue = dotLineAndDelta*dotLineAndDelta - lineLengthSquared*rate;
	if(rootValue < FLT_EPSILON){
		// 当たらないか接している場合
		const float lineRatioAtCollidePoint = dotLineAndDelta / lineLengthSquared;
		if(_lineRatioAtCollidePoint1 != nullptr) (*_lineRatioAtCollidePoint1) = lineRatioAtCollidePoint;
		if(_lineRatioAtCollidePoint2 != nullptr) (*_lineRatioAtCollidePoint2) = lineRatioAtCollidePoint;
	}
	else{
		// 貫通している場合
		const float delta = Math::Functions::SquareRoot(rootValue);
		if(_lineRatioAtCollidePoint1 != nullptr) (*_lineRatioAtCollidePoint1) = (dotLineAndDelta - delta) / lineLengthSquared;
		if(_lineRatioAtCollidePoint2 != nullptr) (*_lineRatioAtCollidePoint2) = (dotLineAndDelta + delta) / lineLengthSquared;
	}

	return (rootValue >= 0.0f);
}

/// <summary>
/// 直線と無限円柱の衝突判定
/// </summary>
/// <param name="_line">直線</param>
/// <param name="_infiniteCylinder">無限円柱</param>
/// <param name="_lineRatioAtCollidePoint1">衝突点1の直線の比率 (値の小さいほう)</param>
/// <param name="_lineRatioAtCollidePoint2">衝突点2の直線の比率 (値の大きいほう)</param>
/// <returns>true:当たっている false:当たっていない</returns>
bool Collision::IsCollideLineAndInfiniteCylinder(
	const Line& _line,
	const InfiniteCylinder& _infiniteCylinder,
	float* const _lineRatioAtCollidePoint1,
	float* const _lineRatioAtCollidePoint2
){
	const float lineLengthSquared = _line.direction.LengthSquared();
	if(lineLengthSquared < FLT_EPSILON){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("Line direction length is 0.");
	}
	const float axisLengthSquared = _infiniteCylinder.axis.direction.LengthSquared();
	if(axisLengthSquared < FLT_EPSILON){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("Infinite cylinder axis direction length is 0.");
	}

	const Float3 deltaInfiniteCylinderOrigin = _infiniteCylinder.axis.origin - _line.origin;

	const float deltaLengthSquared = deltaInfiniteCylinderOrigin.LengthSquared();
	const float dotLineAndAxis = _line.direction.Dot(_infiniteCylinder.axis.direction);
	const float dotLineAndDelta = _line.direction.Dot(deltaInfiniteCylinderOrigin);
	const float dotDeltaAndAxis = deltaInfiniteCylinderOrigin.Dot(_infiniteCylinder.axis.direction);
	
	const float a = lineLengthSquared - (dotLineAndAxis*dotLineAndAxis)/axisLengthSquared;
	const float b = dotLineAndDelta - (dotLineAndAxis*dotDeltaAndAxis)/axisLengthSquared;
	const float c = deltaLengthSquared - (dotDeltaAndAxis*dotDeltaAndAxis)/axisLengthSquared - _infiniteCylinder.radius*_infiniteCylinder.radius;

	const float rootValue = b*b - a*c;
	if(rootValue < FLT_EPSILON){
		// 当たらないか接している場合
		const float lineRatioAtCollidePoint = b / a;
		if(_lineRatioAtCollidePoint1 != nullptr) (*_lineRatioAtCollidePoint1) = lineRatioAtCollidePoint;
		if(_lineRatioAtCollidePoint2 != nullptr) (*_lineRatioAtCollidePoint2) = lineRatioAtCollidePoint;
	}
	else{
		// 貫通している場合
		const float delta = Math::Functions::SquareRoot(rootValue);
		if(_lineRatioAtCollidePoint1 != nullptr) (*_lineRatioAtCollidePoint1) = (b - delta) / a;
		if(_lineRatioAtCollidePoint2 != nullptr) (*_lineRatioAtCollidePoint2) = (b + delta) / a;
	}

	return (rootValue >= 0.0f);
}

/// <summary>
/// 直線とカプセルの衝突判定
/// </summary>
/// <param name="_line">直線</param>
/// <param name="_capsule">カプセル</param>
/// <param name="_lineRatioAtCollidePoint1">衝突点1の直線の比率 (値の小さいほう)</param>
/// <param name="_lineRatioAtCollidePoint2">衝突点2の直線の比率 (値の大きいほう)</param>
/// <returns>true:当たっている false:当たっていない</returns>
bool Collision::IsCollideLineAndCapsule(
	const Line& _line,
	const Capsule& _capsule,
	float* const _lineRatioAtCollidePoint1,
	float* const _lineRatioAtCollidePoint2
){
	const float lineLengthSquared = _line.direction.LengthSquared();
	if(lineLengthSquared < FLT_EPSILON){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("Line direction length is 0.");
	}
	const float axisLengthSquared = _capsule.axis.vector.LengthSquared();
	if(axisLengthSquared < FLT_EPSILON){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("Capsule axis vector length is 0.");
	}

	const InfiniteCylinder infiniteCylinder = _capsule.GetInfiniteCylinder();
	float collidePointLineAndInfiniteCylinder[2] = {0.0f, 0.0f};

	if(!IsCollideLineAndInfiniteCylinder(
		_line,
		infiniteCylinder,
		&collidePointLineAndInfiniteCylinder[0],
		&collidePointLineAndInfiniteCylinder[1]
	)){
		// 当たっていない
		return false;
	}

	// どこに当たったか調べる
	enum CollPosID : int{
		Before = -1,
		Center = 0,
		After = 1
	};
	const auto CheckCollPosID = [&_capsule](const Float3 _point)->CollPosID{
		const LineSegment& axis = _capsule.axis;
		if(axis.vector.Dot(_point - axis.start) < 0.0f){
			return CollPosID::Before;
		}
		if(axis.vector.Dot(_point - axis.GetEndPoint()) > 0.0f){
			return CollPosID::After;
		}
		return CollPosID::Center;
	};

	CollPosID collPos[2] = {
		CheckCollPosID(_line.GetPoint(collidePointLineAndInfiniteCylinder[0])),
		CheckCollPosID(_line.GetPoint(collidePointLineAndInfiniteCylinder[1]))
	};

	if(collPos[0]*collPos[1] == 1){
		// 当たっていない可能性があるので調べる
		float collidePointLineAndSphere[2] = {0.0f, 0.0f};
		if(collPos[0] == CollPosID::Before){
			if(!IsCollideLineAndSphere(
				_line,
				Sphere(_capsule.axis.start, _capsule.radius),
				&collidePointLineAndInfiniteCylinder[0],
				&collidePointLineAndInfiniteCylinder[1]
			)){
				// 当たっていない
				return false;
			}
		}
		else if(collPos[0] == CollPosID::After){
			if(!IsCollideLineAndSphere(
				_line,
				Sphere(_capsule.axis.GetEndPoint(), _capsule.radius),
				&collidePointLineAndInfiniteCylinder[0],
				&collidePointLineAndInfiniteCylinder[1]
			)){
				// 当たっていない
				return false;
			}
		}

		// 当たっていた
		if(_lineRatioAtCollidePoint1 != nullptr) (*_lineRatioAtCollidePoint1) = collidePointLineAndInfiniteCylinder[0];
		if(_lineRatioAtCollidePoint2 != nullptr) (*_lineRatioAtCollidePoint2) = collidePointLineAndInfiniteCylinder[1];

		return true;
	}

	// 以降は確実に当たっている
	if((_lineRatioAtCollidePoint1 == nullptr) && (_lineRatioAtCollidePoint2 == nullptr)) return true;

	// 衝突点を調べる
	float collidePointArray[3][2] = {
		{0.0f, 0.0f},
		{collidePointLineAndInfiniteCylinder[0], collidePointLineAndInfiniteCylinder[1]},
		{0.0f, 0.0f}
	};

	IsCollideLineAndSphere(
		_line,
		Sphere(_capsule.axis.start, _capsule.radius),
		&collidePointArray[0][0],
		&collidePointArray[0][1]
	);
	IsCollideLineAndSphere(
		_line,
		Sphere(_capsule.axis.GetEndPoint(), _capsule.radius),
		&collidePointArray[2][0],
		&collidePointArray[2][1]
	);

	if(_lineRatioAtCollidePoint1 != nullptr) (*_lineRatioAtCollidePoint1) = collidePointArray[collPos[0]+1][0];
	if(_lineRatioAtCollidePoint2 != nullptr) (*_lineRatioAtCollidePoint2) = collidePointArray[collPos[1]+1][1];

	return true;
}

/// <summary>
/// 点と線分の距離の2乗の取得
/// </summary>
/// <param name="_point">点</param>
/// <param name="_lineSegment">線分</param>
/// <param name="_nearestRatioOfLineSegment">線分の最近点比率</param>
/// <returns>点と線分の距離の2乗</returns>
float Collision::GetDistanceSquaredOfPointAndLineSegment(
	const Float3& _point,
	const LineSegment& _lineSegment,
	float* const _nearestRatioOfLineSegment
){
	float lineSegmentLengthSquared = _lineSegment.vector.LengthSquared();
	if(lineSegmentLengthSquared < FLT_EPSILON){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("Line segment length is 0.");
	}

	const float ratio = _lineSegment.vector.Dot(_point - _lineSegment.start) / lineSegmentLengthSquared;
	const float nearest = Functions::Clip(ratio, 0.0f, 1.0f);
	const Float3 nearestLineSegmentPoint = _lineSegment.GetPoint(nearest);

	if(_nearestRatioOfLineSegment != nullptr){
		(*_nearestRatioOfLineSegment) = nearest;
	}

	return ((_point - nearestLineSegmentPoint).LengthSquared());
}

/// <summary>
/// 点と線分の距離の取得
/// </summary>
/// <param name="_point">点</param>
/// <param name="_lineSegment">線分</param>
/// <param name="_nearestRatioOfLineSegment">線分の最近点比率</param>
/// <returns>点と線分の距離</returns>
float Collision::GetDistanceOfPointAndLineSegment(
	const Float3& _point,
	const LineSegment& _lineSegment,
	float* const _nearestRatioOfLineSegment
){
	return Functions::SquareRoot(GetDistanceSquaredOfPointAndLineSegment(_point, _lineSegment, _nearestRatioOfLineSegment));
}

/// <summary>
/// 2線分間の距離の2乗の取得
/// </summary>
/// <param name="_lineSegment1">線分1</param>
/// <param name="_lineSegment2">線分2</param>
/// <param name="_nearestRatioOfLineSegment1">線分1の最近点比率</param>
/// <param name="_nearestRatioOfLineSegment2">線分2の最近点比率</param>
/// <returns>2線分間の距離の2乗</returns>
float Collision::GetDistanceSquaredOf2LineSegments(
	const LineSegment& _lineSegment1,
	const LineSegment& _lineSegment2,
	float* const _nearestRatioOfLineSegment1,
	float* const _nearestRatioOfLineSegment2
){
	float lineSegment1LengthSquared = _lineSegment1.vector.LengthSquared();
	if(lineSegment1LengthSquared < FLT_EPSILON){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("Line segment 1 length is 0.");
	}
	float lineSegment2LengthSquared = _lineSegment2.vector.LengthSquared();
	if(lineSegment2LengthSquared < FLT_EPSILON){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("Line segment 2 length is 0.");
	}

	// 線分内かの判定
	const auto IsInLineSegment = [](const float _ratio)->bool{ return ((_ratio>0.0f)&&(_ratio<1.0f)); };
	// 線分内になるようにクランプ
	const auto Clamp = [](const float _ratio)->float{ return Functions::Clip(_ratio, 0.0f, 1.0f); };

	const Line line1(_lineSegment1.start, _lineSegment1.vector);
	const Line line2(_lineSegment2.start, _lineSegment2.vector);

	float distanceSquared = FLT_MAX;
	float nearestRatioOfLineSegment1 = 0.0f;
	float nearestRatioOfLineSegment2 = 0.0f;

	// 平行だったら線分1の始点を最短距離と仮定してはじめの処理をとばす
	if(_lineSegment1.vector.Cross(_lineSegment2.vector).LengthSquared() > FLT_EPSILON){
		// 各線分を含む直線の最近点を算出
		distanceSquared = GetDistanceSquaredOf2Lines(
			line1, line2, &nearestRatioOfLineSegment1, &nearestRatioOfLineSegment2
		);
		// 両方とも線分内なら終了
		if(IsInLineSegment(nearestRatioOfLineSegment1) && IsInLineSegment(nearestRatioOfLineSegment2)){
			if(_nearestRatioOfLineSegment1 != nullptr){
				(*_nearestRatioOfLineSegment1) = nearestRatioOfLineSegment1;
			}
			if(_nearestRatioOfLineSegment2 != nullptr){
				(*_nearestRatioOfLineSegment2) = nearestRatioOfLineSegment2;
			}
			return distanceSquared;
		}
	}

	nearestRatioOfLineSegment1 = Clamp(nearestRatioOfLineSegment1);
	// 線分1の中で1番線分2に近い点との距離を算出
	distanceSquared = GetDistanceSquaredOfPointAndLine(
		_lineSegment1.start + _lineSegment1.vector*nearestRatioOfLineSegment1,
		line2,
		&nearestRatioOfLineSegment2
	);
	// 線分内なら終了
	if(IsInLineSegment(nearestRatioOfLineSegment2)){
		if(_nearestRatioOfLineSegment1 != nullptr){
			(*_nearestRatioOfLineSegment1) = nearestRatioOfLineSegment1;
		}
		if(_nearestRatioOfLineSegment2 != nullptr){
			(*_nearestRatioOfLineSegment2) = nearestRatioOfLineSegment2;
		}
		return distanceSquared;
	}

	nearestRatioOfLineSegment2 = Clamp(nearestRatioOfLineSegment2);
	// 線分2の中で1番線分1に近い点との距離を算出
	distanceSquared = GetDistanceSquaredOfPointAndLine(
		_lineSegment2.start + _lineSegment2.vector*nearestRatioOfLineSegment2,
		line1,
		&nearestRatioOfLineSegment1
	);
	// 線分内なら終了
	if(IsInLineSegment(nearestRatioOfLineSegment1)){
		if(_nearestRatioOfLineSegment1 != nullptr){
			(*_nearestRatioOfLineSegment1) = nearestRatioOfLineSegment1;
		}
		if(_nearestRatioOfLineSegment2 != nullptr){
			(*_nearestRatioOfLineSegment2) = nearestRatioOfLineSegment2;
		}
		return distanceSquared;
	}

	nearestRatioOfLineSegment1 = Clamp(nearestRatioOfLineSegment1);
	// それぞれに一番近い場所の距離を算出
	const Float3 nearestPointOfLineSegment1 = _lineSegment1.start + _lineSegment1.vector*nearestRatioOfLineSegment1;
	const Float3 nearestPointOfLineSegment2 = _lineSegment2.start + _lineSegment2.vector*nearestRatioOfLineSegment2;
	distanceSquared = (nearestPointOfLineSegment1-nearestPointOfLineSegment2).LengthSquared();
	if(_nearestRatioOfLineSegment1 != nullptr){
		(*_nearestRatioOfLineSegment1) = nearestRatioOfLineSegment1;
	}
	if(_nearestRatioOfLineSegment2 != nullptr){
		(*_nearestRatioOfLineSegment2) = nearestRatioOfLineSegment2;
	}
	return distanceSquared;
}

/// <summary>
/// 2線分間の距離の取得
/// </summary>
/// <param name="_lineSegment1">線分1</param>
/// <param name="_lineSegment2">線分2</param>
/// <param name="_nearestRatioOfLineSegment1">線分1の最近点比率</param>
/// <param name="_nearestRatioOfLineSegment2">線分2の最近点比率</param>
/// <returns>2線分間の距離</returns>
float Collision::GetDistanceOf2LineSegments(
	const LineSegment& _lineSegment1,
	const LineSegment& _lineSegment2,
	float* const _nearestRatioOfLineSegment1,
	float* const _nearestRatioOfLineSegment2
){
	return Functions::SquareRoot(GetDistanceSquaredOf2LineSegments(
		_lineSegment1, _lineSegment2, _nearestRatioOfLineSegment1, _nearestRatioOfLineSegment2
	));
}

/// <summary>
/// 線分同士の衝突判定
/// </summary>
/// <param name="_collisionPoint">衝突座標格納先</param>
/// <param name="_collisionLengthRatioOfLineSegment1">衝突座標の線分長比率格納先</param>
/// <param name="_collisionLengthRatioOfLineSegment2">衝突座標の線分長比率格納先</param>
/// <param name="_lineSegment1">線分1</param>
/// <param name="_lineSegment2">線分2</param>
/// <returns>true:衝突している false:衝突していない</returns>
bool Collision::IsCollide2LineSegments(
	::Ishikawa::Math::Float3* const _collisionPoint,
	float* const _collisionLengthRatioOfLineSegment1,
	float* const _collisionLengthRatioOfLineSegment2,
	const LineSegment& _lineSegment1,
	const LineSegment& _lineSegment2
){
	if(_lineSegment1.vector.LengthSquared() < FLT_EPSILON){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("LineSegment1 vector length is 0.");
	}
	if(_lineSegment2.vector.LengthSquared() < FLT_EPSILON){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("LineSegment2 vector length is 0.");
	}

	// 線分1と線分2の法線
	const Float3 normalOfLS1AndLS2 = _lineSegment1.vector.Cross(_lineSegment2.vector);
	// 平行なら交差しない
	if(normalOfLS1AndLS2.LengthSquared() < FLT_EPSILON) return false;

	// 線分1の始点から線分2の始点へのベクトル
	const Float3 vectorLS1StartToLS2Start = _lineSegment2.start - _lineSegment1.start;
	// 線分1と線分1の始点から線分2の始点へのベクトルの法線
	const Float3 normalOfLS1AndVector = vectorLS1StartToLS2Start.Cross(_lineSegment1.vector);
	if(normalOfLS1AndVector.LengthSquared() < FLT_EPSILON){
		// 線分2の始点で交差する
		if(_collisionPoint != nullptr) (*_collisionPoint) = _lineSegment2.start;
		if(_collisionLengthRatioOfLineSegment1 != nullptr){
			(*_collisionLengthRatioOfLineSegment1) = 
				vectorLS1StartToLS2Start.Dot(_lineSegment1.vector) / _lineSegment1.vector.LengthSquared();
		}
		if(_collisionLengthRatioOfLineSegment2 != nullptr){
			(*_collisionLengthRatioOfLineSegment2) = 0.0f;
		}
		return true;
	}

	const Float3 checkVector = normalOfLS1AndLS2.Cross(normalOfLS1AndVector);
	// 交差しない
	if(checkVector.LengthSquared() > FLT_EPSILON) return false;

	// 交差する

	// 衝突座標計算
	float s = 0.0f;
	if(normalOfLS1AndLS2.x > FLT_EPSILON){
		s = normalOfLS1AndVector.x / normalOfLS1AndLS2.x;
	}
	else if(normalOfLS1AndLS2.y > FLT_EPSILON){
		s = normalOfLS1AndVector.y / normalOfLS1AndLS2.y;
	}
	else{
		s = normalOfLS1AndVector.z / normalOfLS1AndLS2.z;
	}

	const Float3 vectorOfLS1StartToCollisionPoint = _lineSegment1.vector*s;
	const Float3 collisionPoint = _lineSegment1.start + vectorOfLS1StartToCollisionPoint;
	if(_collisionPoint != nullptr) (*_collisionPoint) = collisionPoint;
	if(_collisionLengthRatioOfLineSegment1 != nullptr){
		(*_collisionLengthRatioOfLineSegment1) = 
			vectorOfLS1StartToCollisionPoint.Dot(_lineSegment1.vector) / _lineSegment1.vector.LengthSquared();
	}
	if(_collisionLengthRatioOfLineSegment2 != nullptr){
		(*_collisionLengthRatioOfLineSegment2) = 
			(collisionPoint - _lineSegment2.start).Dot(_lineSegment2.vector) / _lineSegment2.vector.LengthSquared();
	}

	return true;
}

/// <summary>
/// 線分と平面の衝突判定
/// </summary>
/// <param name="_collisionPoint">衝突座標格納先</param>
/// <param name="_collisionLengthRatioOfLineSegment">衝突座標の線分長比率格納先</param>
/// <param name="_lineSegment">線分</param>
/// <param name="_planar">平面</param>
/// <param name="_isCheckCollisionOfReverseSideToo">裏側からの衝突も検知するかのフラグ</param>
/// <returns>true:衝突している false:衝突していない</returns>
bool Collision::IsCollideLineSegmentAndPlanar(
	::Ishikawa::Math::Float3* const _collisionPoint,
	float* const _collisionLengthRatioOfLineSegment,
	const LineSegment& _lineSegment,
	const Planar3x2& _planar,
	bool _isCheckCollisionOfReverseSideToo
){
	if(_lineSegment.vector.LengthSquared() < FLT_EPSILON){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("LineSegment vector length is 0.");
	}
	if(_planar.normal.LengthSquared() < FLT_EPSILON){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("Planar normal length is 0.");
	}

	// 平面の基点から線分の始点へのベクトル
	const Float3 vectorToStart = _lineSegment.start - _planar.origin;
	// 平面の法線と平面の基点から線分の始点へのベクトルの内積
	const float dotNormalAndVectorS = _planar.normal.Dot(vectorToStart);

	// 裏側からのベクトルを検知しない場合に線分の視点が裏側にあれば衝突しない
	if((!_isCheckCollisionOfReverseSideToo) && (dotNormalAndVectorS < 0.0f)) return false;

	// 平面の基点から線分の終点へのベクトル
	const Float3 vectorToEnd = vectorToStart + _lineSegment.vector;
	// 平面の法線と平面の基点から線分の終点へのベクトルの内積
	const float dotNormalAndVectorE = _planar.normal.Dot(vectorToEnd);

	// 線分の始点と終点が同じ側にある場合は衝突しない
	if((dotNormalAndVectorS*dotNormalAndVectorE) > 0.0f) return false;

	// 衝突する

	// 比率を出すための値
	const float div = dotNormalAndVectorS - dotNormalAndVectorE;
	// 衝突座標の線分長比率
	const float collisionLengthRatioOfLineSegment = ((div*div)<FLT_EPSILON) ? (0.0f) : (dotNormalAndVectorS / div);
	if(_collisionLengthRatioOfLineSegment != nullptr){
		(*_collisionLengthRatioOfLineSegment) = collisionLengthRatioOfLineSegment;
	}
	if(_collisionPoint != nullptr){
		(*_collisionPoint) = _lineSegment.start + _lineSegment.vector*collisionLengthRatioOfLineSegment;
	}

	return true;
}
