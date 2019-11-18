//=============================================================================
//	ITExceptions.h
//
//	�����O�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_EXCEPTIONS_H__
#define __IT_EXCEPTIONS_H__

#pragma warning(disable:4548)
#include <exception>
#pragma warning(default:4548)


namespace Ishikawa{
	namespace Common{
		namespace Exception{
			/// <summary>
			/// �֐����s
			/// </summary>
			class FunctionFailed final : public ::std::exception{
			public:
				// �f�t�H���g�R���X�g���N�^
				FunctionFailed();
				// �R���X�g���N�^
				FunctionFailed(const char* const message);
				// �f�X�g���N�^
				~FunctionFailed();
			};
		}
	}
}

#endif
