//=============================================================================
//	ITExceptions.cpp
//
//	�����O�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITExceptions.h"

using namespace Ishikawa::Common::Exception;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
FunctionFailed::FunctionFailed() :
	::std::exception()
{
	// �������Ȃ�
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="message">���b�Z�[�W</param>
FunctionFailed::FunctionFailed(const char* const message) :
	::std::exception(message)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
FunctionFailed::~FunctionFailed(){
	// �������Ȃ�
}
