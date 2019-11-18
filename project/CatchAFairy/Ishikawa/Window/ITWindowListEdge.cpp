//=============================================================================
//	ITWindowListEdge.cpp
//
//	自作ウィンドウのリスト端のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITWindowListEdge.h"

using namespace Ishikawa::Window;


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
ListEdge::ListEdge() :
	nextITWindow(nullptr),
	beforeITWindow(nullptr)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
ListEdge::~ListEdge(){
	// 何もしない
}


/// <summary>
/// 初期化
/// </summary>
void ListEdge::Initialize(){
	this->InitializeITWindow();
}

/// <summary>
/// 自作ウィンドウの初期化
/// </summary>
void ListEdge::InitializeITWindow(){
	// 自身で循環させる
	this->nextITWindow = this;
	this->beforeITWindow = this;
}
