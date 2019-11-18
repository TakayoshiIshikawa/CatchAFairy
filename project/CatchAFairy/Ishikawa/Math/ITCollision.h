//=============================================================================
//	ITCollision.h
//
//	����Փ˔���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_COLLISION_H__
#define __IT_COLLISION_H__

namespace Ishikawa{
	namespace Math{
		struct Float3;
		struct Matrix;

		/// <summary>
		/// �Փ˔���
		/// </summary>
		namespace Collision{
			struct Line;
			struct LineSegment;
			struct Planar3x2;
			struct Planar4;
			struct Sphere;
			struct InfiniteCylinder;
			struct Capsule;

			// �x�N�g���𕽖ʂŔ��˂�����
			::Ishikawa::Math::Float3 ReflectVectorAtPlanar(
				const ::Ishikawa::Math::Float3& _startPosition,
				const ::Ishikawa::Math::Float3& _vector,
				const ::Ishikawa::Math::Matrix& _matrixVectorToPlanar,
				const ::Ishikawa::Math::Float3& _planarAnchor,
				const ::Ishikawa::Math::Float3& _planarNormal,
				float _reflectionRate
			);
			// �x�N�g�����J�v�Z���Ŕ��˂�����
			::Ishikawa::Math::Float3 ReflectVectorAtCapsule(
				const ::Ishikawa::Math::Float3& _startPosition,
				const ::Ishikawa::Math::Float3& _vector,
				const ::Ishikawa::Math::Matrix& _matrixVectorToCapsule,
				const ::Ishikawa::Math::Float3& _capsuleAxisStartPosition,
				const ::Ishikawa::Math::Float3& _capsuleAxisVector,
				float _capsuleRadius,
				float _reflectionRate
			);

			// �_�ƒ����̋�����2��̎擾
			float GetDistanceSquaredOfPointAndLine(
				const Float3& _point,
				const Line& _line,
				float* const _nearestRatioOfLine = nullptr
			);
			// �_�ƒ����̋����̎擾
			float GetDistanceOfPointAndLine(
				const Float3& _point,
				const Line& _line,
				float* const _nearestRatioOfLine = nullptr
			);
			// 2�����Ԃ̋�����2��̎擾
			float GetDistanceSquaredOf2Lines(
				const Line& _line1,
				const Line& _line2,
				float* const _nearestRatioOfLine1 = nullptr,
				float* const _nearestRatioOfLine2 = nullptr
			);
			// 2�����Ԃ̋����̎擾
			float GetDistanceOf2Lines(
				const Line& _line1,
				const Line& _line2,
				float* const _nearestRatioOfLine1 = nullptr,
				float* const _nearestRatioOfLine2 = nullptr
			);
			// �������m�̏Փ˔���
			bool IsCollide2Lines(
				const Line& _line1,
				const Line& _line2,
				::Ishikawa::Math::Float3* const _collisionPoint = nullptr
			);
			// �����Ƌ��̏Փ˔���
			bool IsCollideLineAndSphere(
				const Line& _line,
				const Sphere& _sphere,
				float* const _lineRatioAtCollidePoint1 = nullptr,
				float* const _lineRatioAtCollidePoint2 = nullptr
			);
			// �����Ɩ����~���̏Փ˔���
			bool IsCollideLineAndInfiniteCylinder(
				const Line& _line,
				const InfiniteCylinder& _infiniteCylinder,
				float* const _lineRatioAtCollidePoint1 = nullptr,
				float* const _lineRatioAtCollidePoint2 = nullptr
			);
			// �����ƃJ�v�Z���̏Փ˔���
			bool IsCollideLineAndCapsule(
				const Line& _line,
				const Capsule& _capsule,
				float* const _lineRatioAtCollidePoint1 = nullptr,
				float* const _lineRatioAtCollidePoint2 = nullptr
			);
			// �_�Ɛ����̋�����2��̎擾
			float GetDistanceSquaredOfPointAndLineSegment(
				const Float3& _point,
				const LineSegment& _lineSegment,
				float* const _nearestRatioOfLineSegment = nullptr
			);
			// �_�Ɛ����̋����̎擾
			float GetDistanceOfPointAndLineSegment(
				const Float3& _point,
				const LineSegment& _lineSegment,
				float* const _nearestRatioOfLineSegment = nullptr
			);
			// 2�����Ԃ̋�����2��̎擾
			float GetDistanceSquaredOf2LineSegments(
				const LineSegment& _lineSegment1,
				const LineSegment& _lineSegment2,
				float* const _nearestRatioOfLineSegment1 = nullptr,
				float* const _nearestRatioOfLineSegment2 = nullptr
			);
			// 2�����Ԃ̋����̎擾
			float GetDistanceOf2LineSegments(
				const LineSegment& _lineSegment1,
				const LineSegment& _lineSegment2,
				float* const _nearestRatioOfLineSegment1 = nullptr,
				float* const _nearestRatioOfLineSegment2 = nullptr
			);
			// �������m�̏Փ˔���
			bool IsCollide2LineSegments(
				::Ishikawa::Math::Float3* const _collisionPoint,
				float* const _collisionLengthRatioOfLineSegment1,
				float* const _collisionLengthRatioOfLineSegment2,
				const LineSegment& _lineSegment1,
				const LineSegment& _lineSegment2
			);
			// �����ƕ��ʂ̏Փ˔���
			bool IsCollideLineSegmentAndPlanar(
				::Ishikawa::Math::Float3* const _collisionPoint,
				float* const _collisionLengthRatioOfLineSegment,
				const LineSegment& _lineSegment,
				const Planar3x2& _planar,
				bool _isCheckCollisionOfReverseSideToo = false
			);
		}
	}
}

#endif
