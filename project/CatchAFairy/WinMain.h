//=============================================================================
//	WinMain.h
//
//	エントリポイントのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __WINMAIN_H__
#define __WINMAIN_H__

struct HINSTANCE__;


namespace Main{
	// 実際の実行部分
	// [他クラスからのfriendが通らなかったので仕方なく別にした]
	int Execute(HINSTANCE__* hInstance, HINSTANCE__* hPrevInstance, char* lpCmdLine, int nCmdShow);
}

#endif
