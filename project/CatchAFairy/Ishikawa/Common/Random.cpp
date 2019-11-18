//=============================================================================
//	Random.cpp
//
//	�����̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "Random.h"
#include "Xorshift.h"

namespace Ishikawa{
	namespace Common{
		namespace Random{
			namespace {
				/// <summary>���p����</summary>
				static Xorshift random;
			}

			/// <summary>
			/// ��������
			/// </summary>
			/// <returns>�����l �͈�[0, 2^32)</returns>
			unsigned long Random(){
				return random.Random();
			}

			/// <summary>
			/// ��������
			/// </summary>
			/// <returns>�����l �͈�[0.0, 1.0)</returns>
			double RealRandom(){
				return random.RealRandom();
			}


			/// <summary>
			/// ���Z�b�g
			/// </summary>
			/// <param name="_seed">�V�[�h�l</param>
			void Reset(unsigned long _seed){
				return random.Reset(_seed);
			}
		}
	}
}
