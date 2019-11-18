//=============================================================================
//	DebugLog.cpp
//
//	デバッグログのソースファイル
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
/// ログを残す
/// </summary>
/// <param name="_log">ログ</param>
void Debug::Log(const char* const _log){
#if __IS_DEBUG
	OutputDebugStringA(_log);
#else
	NOT_USING(_log);
#endif
}


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="_functionName">関数名</param>
Debug::FunctionLog::FunctionLog(const char* const _functionName) :
	functionName(_functionName)
{
#if __IS_DEBUG
	OutputDebugStringA(_functionName);
	OutputDebugStringA(" start\n");
#endif
}

/// <summary>
/// デストラクタ
/// </summary>
Debug::FunctionLog::~FunctionLog(){
#if __IS_DEBUG
	OutputDebugStringA(this->functionName);
	OutputDebugStringA(" end\n");
#endif
}
