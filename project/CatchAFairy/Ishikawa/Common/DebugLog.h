//=============================================================================
//	DebugLog.h
//
//	デバッグログのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __DEBUG_LOG_H__
#define __DEBUG_LOG_H__

namespace Ishikawa{
	namespace Common{
		namespace Debug{
			// ログを残す
			void Log(const char* const _log);

			/// <summary>
			/// 関数ログ
			/// </summary>
			class FunctionLog final{
			private:
				/// <summary>関数名</summary>
				const char* const functionName;


			public:
				// コンストラクタ
				FunctionLog(const char* const _functionName);
				// デストラクタ
				~FunctionLog();
			private:
				/// <summary>コピーコンストラクタ[削除]</summary>
				FunctionLog(const FunctionLog&) = delete;
				/// <summary>代入演算子[削除]</summary>
				FunctionLog& operator=(const FunctionLog&) = delete;
			};
		}
	}
}

#endif
