//=============================================================================
//	ITDeviceDependentResourceBase.cpp
//
//	自作デバイス依存リソースの基底のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITDeviceDependentResourceBase.h"

using namespace Ishikawa::DirectX::Device::DependentResource;


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
Base::Base() :
	dependentDevice(nullptr),
	nextITDeviceDependentResource(nullptr),
	beforeITDeviceDependentResource(nullptr)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Base::~Base(){
	this->RemoveITDeviceDependentResourceList();
}


/// <summary>
/// リストから外れる
/// </summary>
void Base::RemoveITDeviceDependentResourceList(){
	// 所属している循環リストから外れる
	Interface* const next = this->nextITDeviceDependentResource;
	Interface* const before = this->beforeITDeviceDependentResource;
	next->SetBeforeITDeviceDependentResource(before);
	before->SetNextITDeviceDependentResource(next);
	// 自身で循環させる
	this->nextITDeviceDependentResource = this;
	this->beforeITDeviceDependentResource = this;
}


/// <summary>
/// 自作デバイス依存リソースの初期化
/// </summary>
void Base::InitializeITDeviceDependentResource(){
	// 自身で循環させる
	this->nextITDeviceDependentResource = this;
	this->beforeITDeviceDependentResource = this;
}
