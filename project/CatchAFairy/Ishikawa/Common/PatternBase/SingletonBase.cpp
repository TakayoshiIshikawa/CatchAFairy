//=============================================================================
//	SingletonBase.cpp
//
//	シングルトンの基底のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "SingletonBase.h"
#include "SingletonManager.h"

using namespace Ishikawa::Common;


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
SingletonBase::SingletonBase() :
	beforeSingleton(nullptr)
{
	// マネージャに追加
	SingletonManager::GetInstance().AddInstance(this);
}

/// <summary>
/// デストラクタ
/// </summary>
SingletonBase::~SingletonBase(){
	// 何もしない
}
