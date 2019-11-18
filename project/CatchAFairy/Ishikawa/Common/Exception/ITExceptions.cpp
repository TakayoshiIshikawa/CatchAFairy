//=============================================================================
//	ITExceptions.cpp
//
//	自作例外のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITExceptions.h"

using namespace Ishikawa::Common::Exception;


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
FunctionFailed::FunctionFailed() :
	::std::exception()
{
	// 何もしない
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="message">メッセージ</param>
FunctionFailed::FunctionFailed(const char* const message) :
	::std::exception(message)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
FunctionFailed::~FunctionFailed(){
	// 何もしない
}
