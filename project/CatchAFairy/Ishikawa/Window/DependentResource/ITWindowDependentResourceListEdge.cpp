//=============================================================================
//	ITWindowDependentResourceListEdge.cpp
//
//	自作ウィンドウ依存リソースのリスト端のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITWindowDependentResourceListEdge.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"

using namespace Ishikawa::Window::DependentResource;


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
ListEdge::ListEdge() :
	nextITWindowDependentResource(nullptr),
	beforeITWindowDependentResource(nullptr)
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
	this->InitializeITWindowDependentResource();
}

/// <summary>
/// 自作ウィンドウ依存リソースの初期化
/// </summary>
void ListEdge::InitializeITWindowDependentResource(){
	// 自身で循環させる
	this->nextITWindowDependentResource = this;
	this->beforeITWindowDependentResource = this;
}
