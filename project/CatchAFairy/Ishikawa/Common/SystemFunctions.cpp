//=============================================================================
//	SystemFunctions.cpp
//
//	�V�X�e���֐��Q�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "SystemFunctions.h"
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#include <windows.h>
#pragma warning(default:4820)
#pragma warning(default:4668)

using namespace ::Ishikawa::Common;


/// <summary>
/// �����̈ꎞ��~
/// </summary>
/// <param name="_milliSeconds">��~����[�~���b�P��]</param>
void System::Sleep(unsigned long _milliSeconds){
	::Sleep(_milliSeconds);
}
