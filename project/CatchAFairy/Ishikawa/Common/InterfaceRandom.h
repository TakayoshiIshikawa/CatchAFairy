//=============================================================================
//	InterfaceRandom.h
//
//	����������C���^�[�t�F�[�X�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#ifndef __INTERFACE_RANDOM_H__
#define __INTERFACE_RANDOM_H__

namespace Ishikawa{
	namespace Common{
		/// <summary>
		/// ����������C���^�[�t�F�[�X
		/// </summary>
		class InterfaceRandom{
		public:
			/// <summary>�f�t�H���g�R���X�g���N�^</summary>
			InterfaceRandom(){}
			/// <summary>�f�X�g���N�^</summary>
			virtual ~InterfaceRandom(){}

		public:
			// ��������
			virtual unsigned long Random() const = 0;
			// ��������
			virtual double RealRandom() const = 0;

		public:
			// ���Z�b�g
			virtual void Reset(unsigned long _seed) = 0;
		};
	}
}

#endif
