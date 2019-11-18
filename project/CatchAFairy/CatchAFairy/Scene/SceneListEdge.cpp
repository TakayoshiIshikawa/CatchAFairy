//=============================================================================
//	SceneListEdge.cpp
//
//	シーンリストの端のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "SceneListEdge.h"

using namespace CatchAFairy::Scene;


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
ListEdge::ListEdge() :
	nextScene(nullptr),
	beforeScene(nullptr)
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
	this->InitializeScene();
}

/// <summary>
/// シーンの初期化
/// </summary>
void ListEdge::InitializeScene(){
	// 自身で循環させる
	this->nextScene = this;
	this->beforeScene = this;
}
