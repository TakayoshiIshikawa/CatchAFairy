//=============================================================================
//	Xorshift.h
//
//	Xorshift����������̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#ifndef __XORSHIFT_H__
#define __XORSHIFT_H__

#include "InterfaceRandom.h"


namespace Ishikawa{
	namespace Common{
		/// <summary>
		/// Xorshift����������
		/// </summary>
		class Xorshift final : public InterfaceRandom{
		private:
			/// <summary>X</summary>
			mutable unsigned long int x;
			/// <summary>Y</summary>
			mutable unsigned long int y;
			/// <summary>Z</summary>
			mutable unsigned long int z;
			/// <summary>W</summary>
			mutable unsigned long int w;


		public:
			// �f�t�H���g�R���X�g���N�^
			Xorshift();
			// �R���X�g���N�^
			Xorshift(unsigned long int _seed);
			// �R�s�[�R���X�g���N�^
			Xorshift(const Xorshift& _original);
			// �t���R���X�g���N�^
			Xorshift(
				unsigned long int _x,
				unsigned long int _y,
				unsigned long int _z,
				unsigned long int _w
			);
			// �f�X�g���N�^
			virtual ~Xorshift() override final;
		public:
			// ������Z�q
			Xorshift& operator=(const Xorshift& _original);

		public:
			// ��������
			virtual unsigned long Random() const override final;
			// ��������
			virtual double RealRandom() const override final;

		public:
			// ���Z�b�g
			virtual void Reset(unsigned long _seed) override final;
		};
	}
}

#endif
