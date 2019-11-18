//=============================================================================
//	ITCriAdx2Le.h
//
//	����CRI_ADX2_Le���p�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#ifndef __IT_CRI_ADX2_LE_H__
#define __IT_CRI_ADX2_LE_H__

#include "Ishikawa/Common/PatternBase/SingletonBase.h"

namespace Ishikawa{
	namespace CriAdx2Le{
		/// <summary>
		/// ����CRI_ADX2_Le���p
		/// </summary>
		class ITCriAdx2Le final : public ::Ishikawa::Common::SingletonBase{
		public:
			// GetInstance<ITCriAdx2Le>()�ŃA�N�Z�X���邽�߃t�����h��
			friend class Ishikawa::Common::SingletonManager;


		private:
			// Pimpl�𗘗p���ĉB��
			class PimplOfITCriAdx2Le;


		private:
			// �V���O���g���C���X�^���X
			static ITCriAdx2Le* instance;

		private:
			/// <summary>�B���f�[�^</summary>
			PimplOfITCriAdx2Le* itCriAdx2Le;


		private:
			// �f�t�H���g�R���X�g���N�^
			ITCriAdx2Le();
			// �f�X�g���N�^
			virtual ~ITCriAdx2Le() override final;
		private:
			/// <summary>�R�s�[�R���X�g���N�^[�폜]</summary>
			ITCriAdx2Le(const ITCriAdx2Le&) = delete;
			/// <summary>������Z�q[�폜]</summary>
			ITCriAdx2Le& operator=(const ITCriAdx2Le&) = delete;

		public:
			// �{�����[���ݒ�
			void SetVolume(float _volume);

		public:
			// ������
			void Initialize(const char* const _acfFilepath, const char* const _acbFilepath, const char* const _awbFilepath);
			// �X�V
			void Update();
			// �I��
			void Finalize();

			// acb�t�@�C���ύX
			void ChangeAcbFile(const char* const _acbFilepath, const char* const _awbFilepath);

			// �Đ�
			unsigned int Play(int _number);
			// ��~
			void Stop(unsigned int _id);
		};
	}
}

#endif
