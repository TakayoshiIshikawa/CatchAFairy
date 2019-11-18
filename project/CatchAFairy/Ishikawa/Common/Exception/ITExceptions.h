//=============================================================================
//	ITExceptions.h
//
//	自作例外のヘッダファイル
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
			/// 関数失敗
			/// </summary>
			class FunctionFailed final : public ::std::exception{
			public:
				// デフォルトコンストラクタ
				FunctionFailed();
				// コンストラクタ
				FunctionFailed(const char* const message);
				// デストラクタ
				~FunctionFailed();
			};
		}
	}
}

#endif
