//=============================================================================
//	SingletonManager.cpp
//
//	シングルトンマネージャのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "SingletonManager.h"
#include "SingletonBase.h"

using namespace Ishikawa::Common;

/// <summary>シングルトンインスタンス</summary>
SafePointer<SingletonManager> SingletonManager::instance(nullptr);


/// <summary>
/// インスタンスの取得[シングルトンマネージャ]
/// </summary>
/// <returns>シングルトンマネージャのインスタンス</returns>
SingletonManager& SingletonManager::GetInstance(){
	if(instance.Get() == nullptr){
		instance.Reset(new SingletonManager());
	}
	return *instance;
}


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
SingletonManager::SingletonManager() :
	lastSingletonInstance(nullptr)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
SingletonManager::~SingletonManager(){
	this->Finalize();
}

/// <summary>
/// シングルトンインスタンスの追加
/// </summary>
/// <param name="singleton">シングルトンインスタンス</param>
void SingletonManager::AddInstance(SingletonBase* const singleton){
	if(singleton == nullptr) return;

	singleton->SetBeforeSingleton(this->lastSingletonInstance);
	this->lastSingletonInstance = singleton;
}

/// <summary>
/// 終了処理
/// </summary>
void SingletonManager::Finalize(){
	// 自身以外の全シングルトンインスタンスを削除
	while(this->lastSingletonInstance != nullptr){
		SingletonBase* const singletonInstance = this->lastSingletonInstance;
		this->lastSingletonInstance = singletonInstance->GetBeforeSingleton();
		delete singletonInstance;
	}
	this->lastSingletonInstance = nullptr;
}
