//=============================================================================
//	SystemFunctions.cpp
//
//	システム関数群のソースファイル
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
/// 処理の一時停止
/// </summary>
/// <param name="_milliSeconds">停止時間[ミリ秒単位]</param>
void System::Sleep(unsigned long _milliSeconds){
	::Sleep(_milliSeconds);
}
