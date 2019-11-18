//=============================================================================
//	ITDeviceDependentResourceListEdge.cpp
//
//	自作デバイス依存リソースのリスト端のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITDeviceDependentResourceListEdge.h"

using namespace Ishikawa::DirectX::Device::DependentResource;


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
ListEdge::ListEdge() :
	nextITDeviceDependentResource(nullptr),
	beforeITDeviceDependentResource(nullptr)
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
	this->InitializeITDeviceDependentResource();
}

/// <summary>
/// 自作デバイス依存リソースの初期化
/// </summary>
void ListEdge::InitializeITDeviceDependentResource(){
	// 自身で循環させる
	this->nextITDeviceDependentResource = this;
	this->beforeITDeviceDependentResource = this;
}
