//=============================================================================
//	ITCollision.cpp
//
//	����Փ˔���̃\�[�X�t�@�C��
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
/// ���˗p�ϊ��s��(XY���ʂ𔽎˕��ʂɏd�Ȃ�悤�ɕϊ�����s��)�̍쐬
/// </summary>
/// <param name="_anchor">��_���W</param>
/// <param name="_normal">���˕��ʂ̖@���x�N�g��</param>
/// <returns>���˗p�ϊ��s��</returns>
static ::Ishikawa::Math::Matrix GetRefrectionMatrix(
	const ::Ishikawa::Math::Float3& _anchor,
	const ::Ishikawa::Math::Float3& _normal
){
	if(_normal.LengthSquared() < FLT_EPSILON){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Reflection vector length is ZERO.");
	}

	// �e���x�N�g���̍쐬
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
/// �x�N�g���𕽖ʂŔ��˂�����
/// </summary>
/// <param name="_startPosition">�x�N�g���n�_</param>
/// <param name="_vector">�x�N�g��</param>
/// <param name="_matrixVectorToPlanar">�x�N�g����Ԃ��畽�ʋ�Ԃւ̕ϊ��s��</param>
/// <param name="_planarAnchor">���˕��ʂ̊�_���W</param>
/// <param name="_planarNormal">���˕��ʂ̖@���x�N�g��</param>
/// <param name="_reflectionRate">���˗�</param>
/// <returns>���ˌ�̈ʒu�ւ̃x�N�g��</returns>
::Ishikawa::Math::Float3 Collision::ReflectVectorAtPlanar(
	const ::Ishikawa::Math::Float3& _startPosition,
	const ::Ishikawa::Math::Float3& _vector,
	const ::Ishikawa::Math::Matrix& _matrixVectorToPlanar,
	const ::Ishikawa::Math::Float3& _planarAnchor,
	const ::Ishikawa::Math::Float3& _planarNormal,
	float _reflectionRate
){
	if(_vector.LengthSquared() < FLT_EPSILON){
		// �x�N�g�����[���x�N�g���Ȃ炻�̂܂ܕԂ�
		return _vector;
	}

	// �x�N�g����Ԃ��甽�ˋ�Ԃւ̕ϊ��s��
	::Ishikawa::Math::Matrix matrixVectorToReflection = _matrixVectorToPlanar * GetRefrectionMatrix(_planarAnchor, _planarNormal).Invert();

	// ���ˋ�ԏ�ł̃x�N�g���n�_
	::Ishikawa::Math::Float4 start = ::Ishikawa::Math::Float4(_startPosition, 1.0f) * matrixVectorToReflection;
	// ���ˋ�ԏ�ł̃x�N�g���I�_
	::Ishikawa::Math::Float4 end = ::Ishikawa::Math::Float4(_startPosition+_vector, 1.0f) * matrixVectorToReflection;

	// ������ʉ߂���ꍇ�����˕��ʂ�ʉ߂��Ȃ��ꍇ�͂��̂܂ܕԂ�
	if((start.z < 0.0f)||(end.z > 0.0f)) return _vector;

	// ���ˋ�ԏ�ł̔��ˌ�̍��W
	::Ishikawa::Math::Float4 reflected = end;
	reflected.z *= -_reflectionRate;

	// �x�N�g����ԏ�ł̔��ˌ�̍��W
	::Ishikawa::Math::Float4 reflectedEnd = reflected * matrixVectorToReflection.Invert();
	// �x�N�g����ԏ�ł̔��ˌ�̈ʒu�ւ̃x�N�g��
	::Ishikawa::Math::Float3 reflectedVector(
		reflectedEnd.x - _startPosition.x,
		reflectedEnd.y - _startPosition.y,
		reflectedEnd.z - _startPosition.z
	);

	return reflectedVector;
}

/// <summary>
/// �x�N�g�����J�v�Z���Ŕ��˂�����
/// </summary>
/// <param name="_startPosition">�x�N�g���n�_</param>
/// <param name="_vector">�x�N�g��</param>
/// <param name="_matrixVectorToCapsule">�x�N�g����Ԃ���J�v�Z����Ԃւ̕ϊ��s��</param>
/// <param name="_capsuleAxisStartPosition">�J�v�Z�����n�_</param>
/// <param name="_capsuleAxisVector">�J�v�Z�����x�N�g��</param>
/// <param name="_capsuleRadius">�J�v�Z�����a</param>
/// <param name="_reflectionRate">���˗�</param>
/// <returns>���ˌ�̈ʒu�ւ̃x�N�g��</returns>
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
		// �x�N�g�����[���x�N�g���Ȃ炻�̂܂ܕԂ�
		return _vector;
	}

	float squareLengthCapsuleAxisVector = _capsuleAxisVector.LengthSquared();
	if(squareLengthCapsuleAxisVector < FLT_EPSILON){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Capsule axis vector length is ZERO.");
	}

	// �J�v�Z����Ԃł̃x�N�g������
	::Ishikawa::Math::Float3 vectorInCapsule = (::Ishikawa::Math::Float4(_vector, 0.0f)*_matrixVectorToCapsule).GetXyz();
	if(vectorInCapsule.LengthSquared() < FLT_EPSILON){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Vector length is ZERO.");
	}
	::Ishikawa::Math::Float3 vectorStartInCapsule = (::Ishikawa::Math::Float4(_startPosition, 1.0f)*_matrixVectorToCapsule).GetXyz();
	
	Line vectorLineInCapsule(vectorStartInCapsule, vectorInCapsule);
	Capsule capsule(_capsuleAxisStartPosition, _capsuleAxisVector, _capsuleRadius);
	float collPointRate[2] = {0.0f, 0.0f};
	if(IsCollideLineAndCapsule(vectorLineInCapsule, capsule, &collPointRate[0], &collPointRate[1])){
		if((collPointRate[0] > 0.0f) && (collPointRate[0] < 1.0f)){
			// ���˂�����
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
	// ���˂Ȃ�
	return _vector;
	
/*
	// �J�v�Z����Ԃł̃x�N�g��
	::Ishikawa::Math::Float4 vectorInCapsule4 = ::Ishikawa::Math::Float4(_vector, 0.0f) * _matrixVectorToCapsule;
	::Ishikawa::Math::Float3 vectorInCapsule(vectorInCapsule4.x, vectorInCapsule4.y, vectorInCapsule4.z);
	float squareLengthVector = vectorInCapsule.LengthSquared();
	if(squareLengthVector < FLT_EPSILON){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Vector length is ZERO.");
	}
	// �J�v�Z����Ԃł̃x�N�g���n�_
	::Ishikawa::Math::Float4 vectorStartInCapsule4 = ::Ishikawa::Math::Float4(_startPosition, 1.0f) * _matrixVectorToCapsule;
	::Ishikawa::Math::Float3 vectorStartInCapsule(vectorStartInCapsule4.x, vectorStartInCapsule4.y, vectorStartInCapsule4.z);

	auto GetReflectVector = [=](
		const ::Ishikawa::Math::Float3& _direction,
		const ::Ishikawa::Math::Float3& _reflectPosition
		)->::Ishikawa::Math::Float3
	{
		// ���ˋ�Ԃւ̕ϊ��s��
		::Ishikawa::Math::Matrix reflectionMatrix = GetRefrectionMatrix(_reflectPosition, _direction);
		::Ishikawa::Math::Matrix invertReflectionMatrix = reflectionMatrix.Invert();
		// ���ˋ�ԏ�ł̃x�N�g���I�_
		::Ishikawa::Math::Float4 end = ::Ishikawa::Math::Float4(vectorStartInCapsule+vectorInCapsule, 1.0f) * invertReflectionMatrix;
		// ���ˋ�ԏ�ł̔��ˌ�̍��W
		::Ishikawa::Math::Float4 reflected = end;
		reflected.z *= -_reflectionRate;
		// �x�N�g����ԏ�ł̔��ˌ�̍��W
		::Ishikawa::Math::Float4 reflectedEnd = reflected * reflectionMatrix * _matrixVectorToCapsule.Invert();
		// �x�N�g����ԏ�ł̔��ˌ�̈ʒu�ւ̃x�N�g��
		::Ishikawa::Math::Float3 reflectedVector(
			reflectedEnd.x - _startPosition.x,
			reflectedEnd.y - _startPosition.y,
			reflectedEnd.z - _startPosition.z
		);
		return reflectedVector;
	};

	// �x�N�g�����J�v�Z�����ɕ��s�Ȃ珈����ύX
	if(_capsuleAxisVector.Cross(vectorInCapsule).LengthSquared() < FLT_EPSILON){
		// �J�v�Z�����n�_����x�N�g���n�_�ւ̍����x�N�g��
		::Ishikawa::Math::Float3 deltaStartToAxisStart = _capsuleAxisStartPosition - vectorStartInCapsule;
		// �J�v�Z�����x�N�g���ƍ����x�N�g���̓���
		float axisVectorDotDelta = _capsuleAxisVector.Dot(deltaStartToAxisStart);
		// �x�N�g����̃J�v�Z�����n�_�Ɉ�ԋ߂��_�܂ł̋����ƃx�N�g�������Ƃ̔䗦
		float lengthRateAtNearestAxisStartPointInVector = axisVectorDotDelta / squareLengthVector;
		// 2�̃x�N�g���̋����̓��
		float squareLengthTwoVector = (deltaStartToAxisStart - vectorInCapsule*lengthRateAtNearestAxisStartPointInVector).LengthSquared();
		// ���������a��肠��Δ��˂��Ȃ�(�x�N�g�������̂܂ܕԂ�)
		float squareLengthDelta = _capsuleRadius*_capsuleRadius - squareLengthTwoVector;
		if(squareLengthDelta < 0.0f){
			return _vector;
		}

		// �J�v�Z�����I�_
		::Ishikawa::Math::Float3 capsuleAxisEndPosition = _capsuleAxisStartPosition + _capsuleAxisVector;
		// �x�N�g����̃J�v�Z�����I�_�Ɉ�ԋ߂��_�܂ł̋����ƃx�N�g�������Ƃ̔䗦
		float lengthRateAtNearestAxisEndPointInVector = _capsuleAxisVector.Dot(capsuleAxisEndPosition - vectorStartInCapsule) / squareLengthVector;

		// �J�v�Z���\�ʂƓ�����ʒu���v�Z�ɕK�v�ȍ��������v�Z
		float delta = Functions::SquareRoot(squareLengthDelta / vectorInCapsule.LengthSquared());
		// ��������
		if(_capsuleAxisVector.Dot(vectorInCapsule) > 0.0f){
			// �������Ȃ�J�v�Z�����n�_���璲�ׂ�
			float startRate = lengthRateAtNearestAxisStartPointInVector - delta;
			if((startRate >= 0.0f) && (startRate <= 1.0f)){
				// �J�v�Z�����n�_�t�߂Ŕ��˂���
				::Ishikawa::Math::Float3 reflectPosition = vectorStartInCapsule + vectorInCapsule*startRate;
				::Ishikawa::Math::Float3 direction = reflectPosition - _capsuleAxisStartPosition;
				return GetReflectVector(direction, reflectPosition);
			}
			float endRate = lengthRateAtNearestAxisEndPointInVector + delta;
			if((endRate >= 0.0f) && (endRate <= 1.0f)){
				// �J�v�Z�����I�_�t�߂Ŕ��˂���
				::Ishikawa::Math::Float3 reflectPosition = vectorStartInCapsule + vectorInCapsule*endRate;
				::Ishikawa::Math::Float3 direction = reflectPosition - capsuleAxisEndPosition;
				return GetReflectVector(direction, reflectPosition);
			}
		}
		else{
			// �t�����Ȃ�J�v�Z�����I�_���璲�ׂ�
			float endRate = lengthRateAtNearestAxisEndPointInVector - delta;
			if((endRate >= 0.0f) && (endRate <= 1.0f)){
				// �J�v�Z�����I�_�t�߂Ŕ��˂���
				::Ishikawa::Math::Float3 reflectPosition = vectorStartInCapsule + vectorInCapsule*endRate;
				::Ishikawa::Math::Float3 direction = reflectPosition - capsuleAxisEndPosition;
				return GetReflectVector(direction, reflectPosition);
			}
			float startRate = lengthRateAtNearestAxisStartPointInVector + delta;
			if((startRate >= 0.0f) && (startRate <= 1.0f)){
				// �J�v�Z�����n�_�t�߂Ŕ��˂���
				::Ishikawa::Math::Float3 reflectPosition = vectorStartInCapsule + vectorInCapsule*startRate;
				::Ishikawa::Math::Float3 direction = reflectPosition - _capsuleAxisStartPosition;
				return GetReflectVector(direction, reflectPosition);
			}
		}
		// ���˂��Ȃ�����
		return _vector;
	}

	// �ŋߓ_��T��
	{
		// �x�N�g���ƃJ�v�Z�����x�N�g���̓���
		float vectorDotAxisVector = vectorInCapsule.Dot(_capsuleAxisVector);
		float mulLengths = squareLengthVector*squareLengthCapsuleAxisVector;
		float div = 1.0f - vectorDotAxisVector*vectorDotAxisVector/mulLengths;
		if((div < FLT_EPSILON) && (div > -FLT_EPSILON)){
			// �ُ�I��
			throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't get nearest point.");
		}
		// �J�v�Z�����n�_����x�N�g���n�_�ւ̍����x�N�g��
		::Ishikawa::Math::Float3 deltaAxisStartToStart = vectorStartInCapsule - _capsuleAxisStartPosition;
		// �J�v�Z�����x�N�g���ƍ����x�N�g���̓���
		float axisVectorDotDelta = _capsuleAxisVector.Dot(deltaAxisStartToStart);
		// �x�N�g���ƍ����x�N�g���̓���
		float vectorDotDelta = vectorInCapsule.Dot(deltaAxisStartToStart);
		// �x�N�g���̎n�_����ŋߓ_�܂ł̋����䗦
		float nearestVectorPointRate = (axisVectorDotDelta*vectorDotAxisVector/mulLengths - vectorDotDelta/squareLengthVector) / div;
		// �J�v�Z�����n�_����ŋߓ_�܂ł̋����䗦
//		float nearestAxisVectorPointRate = (nearestVectorPointRate*vectorDotAxisVector + axisVectorDotDelta) / squareLengthCapsuleAxisVector;

		// �x�N�g���̒��ōŋߓ_�Ɉ�ԋ߂��ʒu
		::Ishikawa::Math::Float3 nearestVectorPoint = vectorStartInCapsule;
		if(nearestVectorPointRate > 0.0f){
			nearestVectorPoint = vectorStartInCapsule + ((nearestVectorPointRate > 1.0f) ? (vectorInCapsule) : (vectorInCapsule*nearestVectorPointRate));
		}
		// �J�v�Z�����̒��Ńx�N�g�����̍ŋߓ_�Ɉ�ԋ߂��ʒu
		float nearestAxisVectorPointRate = _capsuleAxisVector.Dot(nearestVectorPoint - _capsuleAxisStartPosition);
		::Ishikawa::Math::Float3 nearestAxisVectorPoint = _capsuleAxisStartPosition;
		if(nearestAxisVectorPointRate > 0.0f){
			nearestAxisVectorPoint = _capsuleAxisStartPosition + ((nearestAxisVectorPointRate > 1.0f) ? (_capsuleAxisVector) : (_capsuleAxisVector*nearestAxisVectorPointRate));
		}

		// TODO:�J�v�Z���\�ʂŔ��˂�����

		if((nearestAxisVectorPoint-nearestVectorPoint).LengthSquared() <= (_capsuleRadius*_capsuleRadius)){
			// �J�v�Z���ƐڐG���Ă���Ȃ�n�_�Ŕ��� (�\�ʂł̔��ˎ����܂ł̑�p����)
			::Ishikawa::Math::Float3 reflectPosition = vectorStartInCapsule;
			// �J�v�Z�����̒��Ŕ��˓_�Ɉ�ԋ߂��ʒu
			float nearestPointRateInAxisVector = _capsuleAxisVector.Dot(reflectPosition - _capsuleAxisStartPosition);
			::Ishikawa::Math::Float3 nearestPointInAxisVector = _capsuleAxisStartPosition;
			if(nearestPointRateInAxisVector > 0.0f){
				nearestAxisVectorPoint = _capsuleAxisStartPosition + ((nearestPointRateInAxisVector > 1.0f) ? (_capsuleAxisVector) : (_capsuleAxisVector*nearestPointRateInAxisVector));
			}
			// ���˕���
			::Ishikawa::Math::Float3 direction = reflectPosition - nearestPointInAxisVector;
			// ����
			return GetReflectVector(direction, reflectPosition);
		}

		// ���˂��Ȃ�
		return _vector;
	}
*/
}

/// <summary>
/// �_�ƒ����̋�����2��̎擾
/// </summary>
/// <param name="_point">�_</param>
/// <param name="_line">����</param>
/// <param name="_nearestRatioOfLine">�����̍ŋߓ_�䗦</param>
/// <returns>�_�ƒ����̋�����2��</returns>
float Collision::GetDistanceSquaredOfPointAndLine(
	const Float3& _point,
	const Line& _line,
	float* const _nearestRatioOfLine
){
	float lineDirectionLengthSquared = _line.direction.LengthSquared();
	if(lineDirectionLengthSquared < FLT_EPSILON){
		// �ُ�I��
		::Ishikawa::Common::Exception::FunctionFailed("Line direction length is 0.");
	}

	const Float3 delta = _point - _line.origin;
	if(_nearestRatioOfLine != nullptr){
		(*_nearestRatioOfLine) = _line.direction.Dot(delta) / lineDirectionLengthSquared;
	}

	return (_line.direction.Cross(delta).LengthSquared() / lineDirectionLengthSquared);
}

/// <summary>
/// �_�ƒ����̋����̎擾
/// </summary>
/// <param name="_point">�_</param>
/// <param name="_line">����</param>
/// <param name="_nearestRatioOfLine">�����̍ŋߓ_�䗦</param>
/// <returns>�_�ƒ����̋���</returns>
float Collision::GetDistanceOfPointAndLine(
	const Float3& _point,
	const Line& _line,
	float* const _nearestRatioOfLine
){
	return Functions::SquareRoot(GetDistanceSquaredOfPointAndLine(_point, _line, _nearestRatioOfLine));
}

/// <summary>
/// 2�����Ԃ̋�����2��̎擾
/// </summary>
/// <param name="_line1">����1</param>
/// <param name="_line2">����2</param>
/// <param name="_nearestRatioOfLine1">����1�̍ŋߓ_�䗦</param>
/// <param name="_nearestRatioOfLine2">����2�̍ŋߓ_�䗦</param>
/// <returns>2�����Ԃ̋�����2��</returns>
float Collision::GetDistanceSquaredOf2Lines(
	const Line& _line1,
	const Line& _line2,
	float* const _nearestRatioOfLine1,
	float* const _nearestRatioOfLine2
){
	float line1DirectionLengthSquared = _line1.direction.LengthSquared();
	if(line1DirectionLengthSquared < FLT_EPSILON){
		// �ُ�I��
		::Ishikawa::Common::Exception::FunctionFailed("Line1 direction length is 0.");
	}
	float line2DirectionLengthSquared = _line2.direction.LengthSquared();
	if(line2DirectionLengthSquared < FLT_EPSILON){
		// �ُ�I��
		::Ishikawa::Common::Exception::FunctionFailed("Line2 direction length is 0.");
	}

	const Float3 normal = _line1.direction.Cross(_line2.direction);
	const Float3 vector = _line2.origin - _line1.origin;

	if(normal.LengthSquared() < FLT_EPSILON){
		// 2���������s

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
/// 2�����Ԃ̋����̎擾
/// </summary>
/// <param name="_line1">����1</param>
/// <param name="_line2">����2</param>
/// <param name="_nearestRatioOfLine1">����1�̍ŋߓ_�䗦</param>
/// <param name="_nearestRatioOfLine2">����2�̍ŋߓ_�䗦</param>
/// <returns>2�����Ԃ̋���</returns>
float Collision::GetDistanceOf2Lines(
	const Line& _line1,
	const Line& _line2,
	float* const _nearestRatioOfLine1,
	float* const _nearestRatioOfLine2
){
	return Functions::SquareRoot(GetDistanceSquaredOf2Lines(_line1, _line2, _nearestRatioOfLine1, _nearestRatioOfLine2));
}

/// <summary>
/// �������m�̏Փ˔���
/// </summary>
/// <param name="_line1">����1</param>
/// <param name="_line2">����2</param>
/// <param name="_collisionPoint">�Փˍ��W�i�[��</param>
/// <returns>true:�Փ˂��Ă��� false:�Փ˂��Ă��Ȃ�</returns>
bool Collision::IsCollide2Lines(
	const Line& _line1,
	const Line& _line2,
	::Ishikawa::Math::Float3* const _collisionPoint
){
	if(_line1.direction.LengthSquared() < FLT_EPSILON){
		// �ُ�I��
		::Ishikawa::Common::Exception::FunctionFailed("Line1 direction length is 0.");
	}
	if(_line2.direction.LengthSquared() < FLT_EPSILON){
		// �ُ�I��
		::Ishikawa::Common::Exception::FunctionFailed("Line2 direction length is 0.");
	}

	// ����1�ƒ���2�̖@��
	const Float3 normalOfLine1AndLine2 = _line1.direction.Cross(_line2.direction);
	// ���s�Ȃ�������Ȃ�
	if(normalOfLine1AndLine2.LengthSquared() < FLT_EPSILON) return false;

	// ����1��̓_���璼��2��̓_�ւ̃x�N�g��
	const Float3 vectorLine1PointToLine2Point = _line2.origin - _line1.origin;
	// ����1�ƒ���1��̓_���璼��2��̓_�ւ̃x�N�g���̖@��
	const Float3 normalOfLine1AndVector = vectorLine1PointToLine2Point.Cross(_line1.direction);
	if(normalOfLine1AndVector.LengthSquared() < FLT_EPSILON){
		// ����2�̊�_�Ō�������
		if(_collisionPoint != nullptr) (*_collisionPoint) = _line2.origin;
		return true;
	}

	const Float3 checkVector = normalOfLine1AndLine2.Cross(normalOfLine1AndVector);
	// �������Ȃ�
	if(checkVector.LengthSquared() > FLT_EPSILON) return false;

	// ��������

	// �Փˍ��W�v�Z
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
/// �����Ƌ��̏Փ˔���
/// </summary>
/// <param name="_line">����</param>
/// <param name="_sphere">��</param>
/// <param name="_lineRatioAtCollidePoint1">�Փ˓_1�̒����̔䗦 (�l�̏������ق�)</param>
/// <param name="_lineRatioAtCollidePoint2">�Փ˓_2�̒����̔䗦 (�l�̑傫���ق�)</param>
/// <returns>true:�������Ă��� false:�������Ă��Ȃ�</returns>
bool Collision::IsCollideLineAndSphere(
	const Line& _line,
	const Sphere& _sphere,
	float* const _lineRatioAtCollidePoint1,
	float* const _lineRatioAtCollidePoint2
){
	if(_line.direction.LengthSquared() < FLT_EPSILON){
		// �ُ�I��
		::Ishikawa::Common::Exception::FunctionFailed("Line direction length is 0.");
	}

	const Float3 deltaSphereOrigin = _sphere.origin - _line.origin;

	const float lineLengthSquared = _line.direction.LengthSquared();
	const float dotLineAndDelta = _line.direction.Dot(deltaSphereOrigin);
	const float rate = deltaSphereOrigin.LengthSquared() - (_sphere.radius*_sphere.radius);

	const float rootValue = dotLineAndDelta*dotLineAndDelta - lineLengthSquared*rate;
	if(rootValue < FLT_EPSILON){
		// ������Ȃ����ڂ��Ă���ꍇ
		const float lineRatioAtCollidePoint = dotLineAndDelta / lineLengthSquared;
		if(_lineRatioAtCollidePoint1 != nullptr) (*_lineRatioAtCollidePoint1) = lineRatioAtCollidePoint;
		if(_lineRatioAtCollidePoint2 != nullptr) (*_lineRatioAtCollidePoint2) = lineRatioAtCollidePoint;
	}
	else{
		// �ђʂ��Ă���ꍇ
		const float delta = Math::Functions::SquareRoot(rootValue);
		if(_lineRatioAtCollidePoint1 != nullptr) (*_lineRatioAtCollidePoint1) = (dotLineAndDelta - delta) / lineLengthSquared;
		if(_lineRatioAtCollidePoint2 != nullptr) (*_lineRatioAtCollidePoint2) = (dotLineAndDelta + delta) / lineLengthSquared;
	}

	return (rootValue >= 0.0f);
}

/// <summary>
/// �����Ɩ����~���̏Փ˔���
/// </summary>
/// <param name="_line">����</param>
/// <param name="_infiniteCylinder">�����~��</param>
/// <param name="_lineRatioAtCollidePoint1">�Փ˓_1�̒����̔䗦 (�l�̏������ق�)</param>
/// <param name="_lineRatioAtCollidePoint2">�Փ˓_2�̒����̔䗦 (�l�̑傫���ق�)</param>
/// <returns>true:�������Ă��� false:�������Ă��Ȃ�</returns>
bool Collision::IsCollideLineAndInfiniteCylinder(
	const Line& _line,
	const InfiniteCylinder& _infiniteCylinder,
	float* const _lineRatioAtCollidePoint1,
	float* const _lineRatioAtCollidePoint2
){
	const float lineLengthSquared = _line.direction.LengthSquared();
	if(lineLengthSquared < FLT_EPSILON){
		// �ُ�I��
		::Ishikawa::Common::Exception::FunctionFailed("Line direction length is 0.");
	}
	const float axisLengthSquared = _infiniteCylinder.axis.direction.LengthSquared();
	if(axisLengthSquared < FLT_EPSILON){
		// �ُ�I��
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
		// ������Ȃ����ڂ��Ă���ꍇ
		const float lineRatioAtCollidePoint = b / a;
		if(_lineRatioAtCollidePoint1 != nullptr) (*_lineRatioAtCollidePoint1) = lineRatioAtCollidePoint;
		if(_lineRatioAtCollidePoint2 != nullptr) (*_lineRatioAtCollidePoint2) = lineRatioAtCollidePoint;
	}
	else{
		// �ђʂ��Ă���ꍇ
		const float delta = Math::Functions::SquareRoot(rootValue);
		if(_lineRatioAtCollidePoint1 != nullptr) (*_lineRatioAtCollidePoint1) = (b - delta) / a;
		if(_lineRatioAtCollidePoint2 != nullptr) (*_lineRatioAtCollidePoint2) = (b + delta) / a;
	}

	return (rootValue >= 0.0f);
}

/// <summary>
/// �����ƃJ�v�Z���̏Փ˔���
/// </summary>
/// <param name="_line">����</param>
/// <param name="_capsule">�J�v�Z��</param>
/// <param name="_lineRatioAtCollidePoint1">�Փ˓_1�̒����̔䗦 (�l�̏������ق�)</param>
/// <param name="_lineRatioAtCollidePoint2">�Փ˓_2�̒����̔䗦 (�l�̑傫���ق�)</param>
/// <returns>true:�������Ă��� false:�������Ă��Ȃ�</returns>
bool Collision::IsCollideLineAndCapsule(
	const Line& _line,
	const Capsule& _capsule,
	float* const _lineRatioAtCollidePoint1,
	float* const _lineRatioAtCollidePoint2
){
	const float lineLengthSquared = _line.direction.LengthSquared();
	if(lineLengthSquared < FLT_EPSILON){
		// �ُ�I��
		::Ishikawa::Common::Exception::FunctionFailed("Line direction length is 0.");
	}
	const float axisLengthSquared = _capsule.axis.vector.LengthSquared();
	if(axisLengthSquared < FLT_EPSILON){
		// �ُ�I��
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
		// �������Ă��Ȃ�
		return false;
	}

	// �ǂ��ɓ������������ׂ�
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
		// �������Ă��Ȃ��\��������̂Œ��ׂ�
		float collidePointLineAndSphere[2] = {0.0f, 0.0f};
		if(collPos[0] == CollPosID::Before){
			if(!IsCollideLineAndSphere(
				_line,
				Sphere(_capsule.axis.start, _capsule.radius),
				&collidePointLineAndInfiniteCylinder[0],
				&collidePointLineAndInfiniteCylinder[1]
			)){
				// �������Ă��Ȃ�
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
				// �������Ă��Ȃ�
				return false;
			}
		}

		// �������Ă���
		if(_lineRatioAtCollidePoint1 != nullptr) (*_lineRatioAtCollidePoint1) = collidePointLineAndInfiniteCylinder[0];
		if(_lineRatioAtCollidePoint2 != nullptr) (*_lineRatioAtCollidePoint2) = collidePointLineAndInfiniteCylinder[1];

		return true;
	}

	// �ȍ~�͊m���ɓ������Ă���
	if((_lineRatioAtCollidePoint1 == nullptr) && (_lineRatioAtCollidePoint2 == nullptr)) return true;

	// �Փ˓_�𒲂ׂ�
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
/// �_�Ɛ����̋�����2��̎擾
/// </summary>
/// <param name="_point">�_</param>
/// <param name="_lineSegment">����</param>
/// <param name="_nearestRatioOfLineSegment">�����̍ŋߓ_�䗦</param>
/// <returns>�_�Ɛ����̋�����2��</returns>
float Collision::GetDistanceSquaredOfPointAndLineSegment(
	const Float3& _point,
	const LineSegment& _lineSegment,
	float* const _nearestRatioOfLineSegment
){
	float lineSegmentLengthSquared = _lineSegment.vector.LengthSquared();
	if(lineSegmentLengthSquared < FLT_EPSILON){
		// �ُ�I��
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
/// �_�Ɛ����̋����̎擾
/// </summary>
/// <param name="_point">�_</param>
/// <param name="_lineSegment">����</param>
/// <param name="_nearestRatioOfLineSegment">�����̍ŋߓ_�䗦</param>
/// <returns>�_�Ɛ����̋���</returns>
float Collision::GetDistanceOfPointAndLineSegment(
	const Float3& _point,
	const LineSegment& _lineSegment,
	float* const _nearestRatioOfLineSegment
){
	return Functions::SquareRoot(GetDistanceSquaredOfPointAndLineSegment(_point, _lineSegment, _nearestRatioOfLineSegment));
}

/// <summary>
/// 2�����Ԃ̋�����2��̎擾
/// </summary>
/// <param name="_lineSegment1">����1</param>
/// <param name="_lineSegment2">����2</param>
/// <param name="_nearestRatioOfLineSegment1">����1�̍ŋߓ_�䗦</param>
/// <param name="_nearestRatioOfLineSegment2">����2�̍ŋߓ_�䗦</param>
/// <returns>2�����Ԃ̋�����2��</returns>
float Collision::GetDistanceSquaredOf2LineSegments(
	const LineSegment& _lineSegment1,
	const LineSegment& _lineSegment2,
	float* const _nearestRatioOfLineSegment1,
	float* const _nearestRatioOfLineSegment2
){
	float lineSegment1LengthSquared = _lineSegment1.vector.LengthSquared();
	if(lineSegment1LengthSquared < FLT_EPSILON){
		// �ُ�I��
		::Ishikawa::Common::Exception::FunctionFailed("Line segment 1 length is 0.");
	}
	float lineSegment2LengthSquared = _lineSegment2.vector.LengthSquared();
	if(lineSegment2LengthSquared < FLT_EPSILON){
		// �ُ�I��
		::Ishikawa::Common::Exception::FunctionFailed("Line segment 2 length is 0.");
	}

	// ���������̔���
	const auto IsInLineSegment = [](const float _ratio)->bool{ return ((_ratio>0.0f)&&(_ratio<1.0f)); };
	// �������ɂȂ�悤�ɃN�����v
	const auto Clamp = [](const float _ratio)->float{ return Functions::Clip(_ratio, 0.0f, 1.0f); };

	const Line line1(_lineSegment1.start, _lineSegment1.vector);
	const Line line2(_lineSegment2.start, _lineSegment2.vector);

	float distanceSquared = FLT_MAX;
	float nearestRatioOfLineSegment1 = 0.0f;
	float nearestRatioOfLineSegment2 = 0.0f;

	// ���s�����������1�̎n�_���ŒZ�����Ɖ��肵�Ă͂��߂̏������Ƃ΂�
	if(_lineSegment1.vector.Cross(_lineSegment2.vector).LengthSquared() > FLT_EPSILON){
		// �e�������܂ޒ����̍ŋߓ_���Z�o
		distanceSquared = GetDistanceSquaredOf2Lines(
			line1, line2, &nearestRatioOfLineSegment1, &nearestRatioOfLineSegment2
		);
		// �����Ƃ��������Ȃ�I��
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
	// ����1�̒���1�Ԑ���2�ɋ߂��_�Ƃ̋������Z�o
	distanceSquared = GetDistanceSquaredOfPointAndLine(
		_lineSegment1.start + _lineSegment1.vector*nearestRatioOfLineSegment1,
		line2,
		&nearestRatioOfLineSegment2
	);
	// �������Ȃ�I��
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
	// ����2�̒���1�Ԑ���1�ɋ߂��_�Ƃ̋������Z�o
	distanceSquared = GetDistanceSquaredOfPointAndLine(
		_lineSegment2.start + _lineSegment2.vector*nearestRatioOfLineSegment2,
		line1,
		&nearestRatioOfLineSegment1
	);
	// �������Ȃ�I��
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
	// ���ꂼ��Ɉ�ԋ߂��ꏊ�̋������Z�o
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
/// 2�����Ԃ̋����̎擾
/// </summary>
/// <param name="_lineSegment1">����1</param>
/// <param name="_lineSegment2">����2</param>
/// <param name="_nearestRatioOfLineSegment1">����1�̍ŋߓ_�䗦</param>
/// <param name="_nearestRatioOfLineSegment2">����2�̍ŋߓ_�䗦</param>
/// <returns>2�����Ԃ̋���</returns>
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
/// �������m�̏Փ˔���
/// </summary>
/// <param name="_collisionPoint">�Փˍ��W�i�[��</param>
/// <param name="_collisionLengthRatioOfLineSegment1">�Փˍ��W�̐������䗦�i�[��</param>
/// <param name="_collisionLengthRatioOfLineSegment2">�Փˍ��W�̐������䗦�i�[��</param>
/// <param name="_lineSegment1">����1</param>
/// <param name="_lineSegment2">����2</param>
/// <returns>true:�Փ˂��Ă��� false:�Փ˂��Ă��Ȃ�</returns>
bool Collision::IsCollide2LineSegments(
	::Ishikawa::Math::Float3* const _collisionPoint,
	float* const _collisionLengthRatioOfLineSegment1,
	float* const _collisionLengthRatioOfLineSegment2,
	const LineSegment& _lineSegment1,
	const LineSegment& _lineSegment2
){
	if(_lineSegment1.vector.LengthSquared() < FLT_EPSILON){
		// �ُ�I��
		::Ishikawa::Common::Exception::FunctionFailed("LineSegment1 vector length is 0.");
	}
	if(_lineSegment2.vector.LengthSquared() < FLT_EPSILON){
		// �ُ�I��
		::Ishikawa::Common::Exception::FunctionFailed("LineSegment2 vector length is 0.");
	}

	// ����1�Ɛ���2�̖@��
	const Float3 normalOfLS1AndLS2 = _lineSegment1.vector.Cross(_lineSegment2.vector);
	// ���s�Ȃ�������Ȃ�
	if(normalOfLS1AndLS2.LengthSquared() < FLT_EPSILON) return false;

	// ����1�̎n�_�������2�̎n�_�ւ̃x�N�g��
	const Float3 vectorLS1StartToLS2Start = _lineSegment2.start - _lineSegment1.start;
	// ����1�Ɛ���1�̎n�_�������2�̎n�_�ւ̃x�N�g���̖@��
	const Float3 normalOfLS1AndVector = vectorLS1StartToLS2Start.Cross(_lineSegment1.vector);
	if(normalOfLS1AndVector.LengthSquared() < FLT_EPSILON){
		// ����2�̎n�_�Ō�������
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
	// �������Ȃ�
	if(checkVector.LengthSquared() > FLT_EPSILON) return false;

	// ��������

	// �Փˍ��W�v�Z
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
/// �����ƕ��ʂ̏Փ˔���
/// </summary>
/// <param name="_collisionPoint">�Փˍ��W�i�[��</param>
/// <param name="_collisionLengthRatioOfLineSegment">�Փˍ��W�̐������䗦�i�[��</param>
/// <param name="_lineSegment">����</param>
/// <param name="_planar">����</param>
/// <param name="_isCheckCollisionOfReverseSideToo">��������̏Փ˂����m���邩�̃t���O</param>
/// <returns>true:�Փ˂��Ă��� false:�Փ˂��Ă��Ȃ�</returns>
bool Collision::IsCollideLineSegmentAndPlanar(
	::Ishikawa::Math::Float3* const _collisionPoint,
	float* const _collisionLengthRatioOfLineSegment,
	const LineSegment& _lineSegment,
	const Planar3x2& _planar,
	bool _isCheckCollisionOfReverseSideToo
){
	if(_lineSegment.vector.LengthSquared() < FLT_EPSILON){
		// �ُ�I��
		::Ishikawa::Common::Exception::FunctionFailed("LineSegment vector length is 0.");
	}
	if(_planar.normal.LengthSquared() < FLT_EPSILON){
		// �ُ�I��
		::Ishikawa::Common::Exception::FunctionFailed("Planar normal length is 0.");
	}

	// ���ʂ̊�_��������̎n�_�ւ̃x�N�g��
	const Float3 vectorToStart = _lineSegment.start - _planar.origin;
	// ���ʂ̖@���ƕ��ʂ̊�_��������̎n�_�ւ̃x�N�g���̓���
	const float dotNormalAndVectorS = _planar.normal.Dot(vectorToStart);

	// ��������̃x�N�g�������m���Ȃ��ꍇ�ɐ����̎��_�������ɂ���ΏՓ˂��Ȃ�
	if((!_isCheckCollisionOfReverseSideToo) && (dotNormalAndVectorS < 0.0f)) return false;

	// ���ʂ̊�_��������̏I�_�ւ̃x�N�g��
	const Float3 vectorToEnd = vectorToStart + _lineSegment.vector;
	// ���ʂ̖@���ƕ��ʂ̊�_��������̏I�_�ւ̃x�N�g���̓���
	const float dotNormalAndVectorE = _planar.normal.Dot(vectorToEnd);

	// �����̎n�_�ƏI�_���������ɂ���ꍇ�͏Փ˂��Ȃ�
	if((dotNormalAndVectorS*dotNormalAndVectorE) > 0.0f) return false;

	// �Փ˂���

	// �䗦���o�����߂̒l
	const float div = dotNormalAndVectorS - dotNormalAndVectorE;
	// �Փˍ��W�̐������䗦
	const float collisionLengthRatioOfLineSegment = ((div*div)<FLT_EPSILON) ? (0.0f) : (dotNormalAndVectorS / div);
	if(_collisionLengthRatioOfLineSegment != nullptr){
		(*_collisionLengthRatioOfLineSegment) = collisionLengthRatioOfLineSegment;
	}
	if(_collisionPoint != nullptr){
		(*_collisionPoint) = _lineSegment.start + _lineSegment.vector*collisionLengthRatioOfLineSegment;
	}

	return true;
}
