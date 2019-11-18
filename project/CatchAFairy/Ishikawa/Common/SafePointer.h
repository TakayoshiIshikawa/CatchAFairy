//=============================================================================
//	SafePointer.h
//
//	�Z�[�t�|�C���^�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SAFEPOINTER_H__
#define __SAFEPOINTER_H__

namespace Ishikawa{
	namespace Common{
		/// <summary>
		/// �Z�[�t�|�C���^
		/// </summary>
		template<typename DataType>
		class SafePointer{
		private:
			/// <summary>�f�[�^</summary>
			DataType* data;


		public:
			/// <summary>�f�t�H���g�R���X�g���N�^</summary>
			SafePointer():data(nullptr){}
			/// <summary>�R���X�g���N�^</summary>
			/// <param name="_data">�f�[�^</param>
			SafePointer(DataType* const _data):data(_data){}
			/// <summary>�f�X�g���N�^</summary>
			~SafePointer(){ if(this->data != nullptr) delete this->data; }
		private:
			/// <summary>�R�s�[�R���X�g���N�^[�폜]</summary>
			SafePointer(const SafePointer<DataType>&) = delete;
			/// <summary>������Z�q[�폜]</summary>
			SafePointer<DataType>& operator=(const SafePointer<DataType>&) = delete;

		public:
			/// <summary>�|�C���^�Q��</summary>
			/// <returns>�Ǘ��f�[�^</returns>
			DataType& operator*(){ return (*(this->data)); }
			/// <summary>�A���[���Z�q</summary>
			/// <returns>�Ǘ��f�[�^</returns>
			DataType& operator->(){ return (*(this->data)); }
			/// <summary>bool���Z�q</summary>
			/// <returns>true:�f�[�^�L false:�f�[�^��</returns>
			operator bool() const { return (this->data != nullptr); }
			/// <summary>not���Z�q</summary>
			/// <returns>true:�f�[�^�� false:�f�[�^�L</returns>
			bool operator!() const { return (this->data == nullptr); }

		public:
			/// <summary>�f�[�^�̎擾</summary>
			/// <returns>�f�[�^</returns>
			DataType* Get() const { return this->data; }
			/// <summary>�f�[�^�̃��Z�b�g</summary>
			/// <param name="_data">�f�[�^</param>
			void Reset(DataType* const _data = nullptr){
				if(this->data != nullptr) delete this->data;
				this->data = _data;
			}
			/// <summary>�f�[�^�̈ړ�</summary>
			/// <returns>�f�[�^</returns>
			DataType* Move(){
				DataType* const returnData = this->data;
				this->data = nullptr;
				return returnData;
			}
			/// <summary>�f�[�^���󂩂ǂ���</summary>
			/// <returns>true:�f�[�^�� false:�f�[�^�L</returns>
			bool Empty() const { return (this->data == nullptr); }
		};
	}
}

#endif
