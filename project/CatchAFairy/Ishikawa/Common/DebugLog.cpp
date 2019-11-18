//=============================================================================
//	DebugLog.cpp
//
//	�f�o�b�O���O�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "DebugLog.h"
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#include <windows.h>
#pragma warning(default:4820)
#pragma warning(default:4668)

using namespace Ishikawa::Common;


/// <summary>
/// ���O���c��
/// </summary>
/// <param name="_log">���O</param>
void Debug::Log(const char* const _log){
#if __IS_DEBUG
	OutputDebugStringA(_log);
#else
	NOT_USING(_log);
#endif
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="_functionName">�֐���</param>
Debug::FunctionLog::FunctionLog(const char* const _functionName) :
	functionName(_functionName)
{
#if __IS_DEBUG
	OutputDebugStringA(_functionName);
	OutputDebugStringA(" start\n");
#endif
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Debug::FunctionLog::~FunctionLog(){
#if __IS_DEBUG
	OutputDebugStringA(this->functionName);
	OutputDebugStringA(" end\n");
#endif
}
