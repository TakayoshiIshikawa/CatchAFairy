//=============================================================================
//	Random.h
//
//	�����̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#ifndef __RANDOM_H__
#define __RANDOM_H__

namespace Ishikawa{
	namespace Common{
		/// <summary>
		/// ����
		/// </summary>
		namespace Random{
			// ��������
			unsigned long Random();
			// ��������
			double RealRandom();

			// ���Z�b�g
			void Reset(unsigned long _seed);
		};
	}
}

#endif
