//=============================================================================
//	ITWindowDependentResourceBase.cpp
//
//	自作ウィンドウ依存リソースの基底のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITWindowDependentResourceBase.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"

using namespace Ishikawa::Window::DependentResource;


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
Base::Base() :
	dependentITWindow(nullptr),
	nextITWindowDependentResource(nullptr),
	beforeITWindowDependentResource(nullptr)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Base::~Base(){
	this->RemoveITWindowDependentResourceList();
}


/// <summary>
/// リストから外れる
/// </summary>
void Base::RemoveITWindowDependentResourceList(){
	// 所属している循環リストから外れる
	Interface* const next = this->nextITWindowDependentResource;
	Interface* const before = this->beforeITWindowDependentResource;
	next->SetBeforeITWindowDependentResource(before);
	before->SetNextITWindowDependentResource(next);
	// 自身で循環させる
	this->nextITWindowDependentResource = this;
	this->beforeITWindowDependentResource = this;
}


/// <summary>
/// 自作ウィンドウ依存リソースの初期化
/// </summary>
void Base::InitializeITWindowDependentResource(){
	// 自身で循環させる
	this->nextITWindowDependentResource = this;
	this->beforeITWindowDependentResource = this;
}
