//=============================================================================
//	AllUse.h
//
//	全てにインクルードされるヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __ALLUSE_H__
#define __ALLUSE_H__

//=========================================================================
//	コンパイラ警告[ファイルの最後で警告が出るもの]
//=========================================================================
#pragma warning(disable:4514)	// 参照されていないインライン関数の削除
#pragma warning(disable:4710)	// 関数f()はインライン関数ではない

//=========================================================================
//	コンパイラ警告[リリースのみ]
//=========================================================================
#if defined(NDEBUG)

#pragma warning(disable:4711)	// 関数f()に自動インライン展開を選択した
#pragma warning(disable:4738)	// メモリに32bit浮動結果を格納します。パフォーマンスが低下する可能性有

#endif


//=========================================================================
//	デバッグ用
//=========================================================================
#if defined(DEBUG) || defined(_DEBUG) || defined(__DEBUG)

#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)

#define __IS_DEBUG (true)

#else

#define __IS_DEBUG (false)

#endif


//=========================================================================
//	マクロ
//=========================================================================
#define NOT_USING(x) ((void)x)


//=========================================================================
//	ヘッダファイル
//=========================================================================


#endif
