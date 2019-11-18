//=============================================================================
//	SingletonManager.h
//
//	�V���O���g���}�l�[�W���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SINGLETONMANAGER_H__
#define __SINGLETONMANAGER_H__

#include "WinMain.h"
#include "Ishikawa/Common/SafePointer.h"


namespace Ishikawa{
	namespace Common{
		class SingletonBase;


		/// <summary>
		/// �V���O���g���}�l�[�W��
		/// </summary>
		class SingletonManager final{
		public:
			// ���̃N���X�̃C���X�^���X���폜���邽�߂Ƀt�����h��
			friend class SafePointer<SingletonManager>;
			// AddInstance()�ɃA�N�Z�X���邽�߂Ƀt�����h��
			friend class SingletonBase;
			// Finalize()�ɃA�N�Z�X���邽�߂Ƀt�����h��
			friend int Main::Execute(HINSTANCE__*, HINSTANCE__*, char*, int);


		private:
			// �V���O���g���C���X�^���X
			static SafePointer<SingletonManager> instance;

		private:
			/// <summary>�Ō�̃V���O���g���C���X�^���X</summary>
			SingletonBase* lastSingletonInstance;


		public:
			// �C���X�^���X�̎擾[���̃V���O���g���N���X]
			template<class SingletonClass>
			static SingletonClass& GetInstance();
		private:
			// �C���X�^���X�̎擾[�V���O���g���}�l�[�W��]
			static SingletonManager& GetInstance();

		private:
			// �f�t�H���g�R���X�g���N�^
			SingletonManager();
			// �f�X�g���N�^
			~SingletonManager();
		private:
			/// <summary>�R�s�[�R���X�g���N�^[�폜]</summary>
			SingletonManager(const SingletonManager&) = delete;
			/// <summary>������Z�q[�폜]</summary>
			SingletonManager& operator=(const SingletonManager&) = delete;

		private:
			// �V���O���g���C���X�^���X�̒ǉ�
			void AddInstance(SingletonBase* const singleton);
			// �I������
			void Finalize();
		};
	}
}

#endif
