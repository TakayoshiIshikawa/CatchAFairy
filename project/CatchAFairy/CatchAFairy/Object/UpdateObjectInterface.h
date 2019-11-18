//=============================================================================
//	UpdateObjectInterface.h
//
//	�X�V�I�u�W�F�N�g�C���^�[�t�F�[�X�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __UPDATE_OBJECT_INTERFACE_H__
#define __UPDATE_OBJECT_INTERFACE_H__

namespace CatchAFairy{
	namespace Object{
		/// <summary>
		/// �X�V�I�u�W�F�N�g�C���^�[�t�F�[�X
		/// </summary>
		class UpdateInterface{
		public:
			/// <summary>�f�t�H���g�R���X�g���N�^</summary>
			UpdateInterface(){}
			/// <summary>�f�X�g���N�^</summary>
			virtual ~UpdateInterface(){}

		public:
			// �X�V
			virtual void Update(float _elapsedTime) = 0;
		};
	}
}

#endif
