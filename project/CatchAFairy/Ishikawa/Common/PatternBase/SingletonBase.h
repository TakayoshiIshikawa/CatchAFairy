//=============================================================================
//	SingletonBase.h
//
//	�V���O���g���̊��̃w�b�_�t�@�C��
//
//	Copyright(c) 2018 - 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SINGLETONBASE_H__
#define __SINGLETONBASE_H__

namespace Ishikawa{
	namespace Common{
		/// <summary>
		/// �V���O���g���̊��
		/// </summary>
		class SingletonBase{
		public:
			// �}�l�[�W���ŃA�N�Z�X���邽�߂Ƀt�����h��
			friend class SingletonManager;


		private:
			/// <summary>�O�̃V���O���g���C���X�^���X</summary>
			SingletonBase* beforeSingleton;


		protected:
			// �f�t�H���g�R���X�g���N�^
			SingletonBase();
			// �f�X�g���N�^
			virtual ~SingletonBase();
		private:
			/// <summary>�R�s�[�R���X�g���N�^[�폜]</summary>
			SingletonBase(const SingletonBase&) = delete;
			/// <summary>������Z�q[�폜]</summary>
			SingletonBase& operator=(const SingletonBase&) = delete;

		private:
			/// <summary>�O�̃V���O���g���C���X�^���X�̎擾</summary>
			/// <returns>�O�̃V���O���g���C���X�^���X</returns>
			SingletonBase* GetBeforeSingleton() const { return this->beforeSingleton; }
	
		private:
			/// <summary>�O�̃V���O���g���C���X�^���X�̐ݒ�</summary>
			/// <param name="before">�O�̃V���O���g���C���X�^���X</param>
			void SetBeforeSingleton(SingletonBase* const before){ this->beforeSingleton = before; }
		};
	}
}

#endif
