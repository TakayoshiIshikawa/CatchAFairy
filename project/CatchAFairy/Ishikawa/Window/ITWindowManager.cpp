//=============================================================================
//	ITWindowManager.cpp
//
//	自作ウィンドウマネージャのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITWindowManager.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Common/PatternBase/SingletonManager.h"

using namespace Ishikawa::Window;

/// <summary>シングルトンインスタンス</summary>
Manager* Manager::instance = nullptr;


/// <summary>
/// インスタンスの取得[自作ウィンドウマネージャ]
/// </summary>
/// <returns>自作ウィンドウマネージャのインスタンス</returns>
template<>
Manager& Ishikawa::Common::SingletonManager::GetInstance<Manager>(){
	if(Manager::instance == nullptr){
		Manager::instance = new Manager();
		(Manager::instance)->Initialize();
	}
	return (*(Manager::instance));
}


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
Manager::Manager() :
	windowListEdge()
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Manager::~Manager(){
	// シングルトンインスタンスのリセット
	instance = nullptr;
}


/// <summary>
/// 初期化
/// </summary>
void Manager::Initialize(){
	// メンバの初期化
	this->windowListEdge.Initialize();
}


/// <summary>
/// ウィンドウの追加
/// </summary>
/// <param name="_window">追加ウィンドウ</param>
void Manager::AddWindow(Interface* const _window){
	if(_window == nullptr){
		throw Ishikawa::Common::Exception::FunctionFailed("You can't add NULL to list of window.");
	}

	// 追加するデータをリストから外す
	_window->RemoveITWindowList();

	Interface* const tail = this->windowListEdge.GetBeforeITWindow();
	// 循環リストの一番後ろに追加
	_window->SetBeforeITWindow(tail);
	_window->SetNextITWindow(&(this->windowListEdge));
	tail->SetNextITWindow(_window);
	this->windowListEdge.SetBeforeITWindow(_window);
}
