//=============================================================================
//	TreesOffset.h
//
//	�؁X�����f�[�^�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __TREES_OFFSET_H__
#define __TREES_OFFSET_H__

namespace Ishikawa{
	namespace Math{
		struct Matrix;
	}
}

namespace CatchAFairy{
	namespace Data{
		/// <summary>
		/// �؁X�����f�[�^
		/// </summary>
		class TreesOffset final{
		private:
			// ���[���h�ϊ��s�񏉊����p�f�[�^
			struct WorldOffset;

		private:
			/// <summary>�v�f��</summary>
			unsigned int count;
			/// <summary>���[���h�ϊ��s�񏉊����p�f�[�^�z��</summary>
			WorldOffset* worldOffsetArray;
			/// <summary>���[���h�ϊ��s��z��</summary>
			::Ishikawa::Math::Matrix* worldArray;

		public:
			// �f�t�H���g�R���X�g���N�^
			TreesOffset();
			// �f�X�g���N�^
			~TreesOffset();

		public:
			/// <summary>�v�f���̎擾</summary>
			/// <returns>�v�f��</returns>
			unsigned int GetCount() const { return this->count; }
			/// <summary>���[���h�ϊ��s��z��̎擾</summary>
			/// <returns>���[���h�ϊ��s��z��</returns>
			const ::Ishikawa::Math::Matrix* GetWorldArray() const { return this->worldArray; }

		public:
			// �ǂݍ���
			void Load(const char* const _csvFilepath);
			// �f�[�^�j��
			void Clear();
		};
	}
}

#endif
