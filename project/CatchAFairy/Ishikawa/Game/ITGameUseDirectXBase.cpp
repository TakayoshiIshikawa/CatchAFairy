//=============================================================================
//	ITGameUseDirectXBase.cpp
//
//	DirectXを利用する自作ゲーム基底のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITGameUseDirectXBase.h"
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#include <windows.h>
#pragma warning(default:4820)
#pragma warning(default:4668)
#include "Ishikawa/Common/PatternBase/SingletonManager.h"
#include "Ishikawa/Window/ITWindowManager.h"


using namespace ::Ishikawa::Game;

/// <summary>
/// デフォルトコンストラクタ
/// </summary>
UseDirectXBase::UseDirectXBase() :
	::Ishikawa::Window::DependentResource::Base(),
	::Ishikawa::DirectX::Device::DependentResource::Base(),
	windowClass(),
	window(),
	directXDevice(),
	windowClassName(L"GameUseDirectXClassDefault"),
	windowClassStyle(CS_HREDRAW | CS_VREDRAW),
	windowClassClassExtra(0L),
	windowClassWindowExtra(0L),
	windowTitle(L"NoTitle"),
	windowStyle(WS_OVERLAPPEDWINDOW),
	windowExStyle(0UL),
	windowWidth(800L),
	windowHeight(600L)
{
	// 何もしない
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="_windowClassName">ウィンドウクラス名</param>
/// <param name="_windowClassStyle">ウィンドウクラススタイル</param>
/// <param name="_windowClassClassExtra">ウィンドウクラスクラスエクストラ</param>
/// <param name="_windowClassWindowExtra">ウィンドウクラスウィンドウエクストラ</param>
/// <param name="_windowTitle">ウィンドウタイトル</param>
/// <param name="_windowStyle">ウィンドウスタイル</param>
/// <param name="_windowExStyle">ウィンドウEXスタイル</param>
/// <param name="_windowWidth">ウィンドウ幅</param>
/// <param name="_windowHeight">ウィンドウ高さ</param>
UseDirectXBase::UseDirectXBase(
	const wchar_t* _windowClassName,
	unsigned long _windowClassStyle,
	long _windowClassClassExtra,
	long _windowClassWindowExtra,
	const wchar_t* _windowTitle,
	unsigned long _windowStyle,
	unsigned long _windowExStyle,
	long _windowWidth,
	long _windowHeight
) :
	::Ishikawa::Window::DependentResource::Base(),
	::Ishikawa::DirectX::Device::DependentResource::Base(),
	windowClass(),
	window(),
	directXDevice(),
	windowClassName(_windowClassName),
	windowClassStyle(_windowClassStyle),
	windowClassClassExtra(_windowClassClassExtra),
	windowClassWindowExtra(_windowClassWindowExtra),
	windowTitle(_windowTitle),
	windowStyle(_windowStyle),
	windowExStyle(_windowExStyle),
	windowWidth(_windowWidth),
	windowHeight(_windowHeight)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
UseDirectXBase::~UseDirectXBase(){
	// リソースリストから外れる
	this->RemoveITWindowDependentResourceList();
	this->RemoveITDeviceDependentResourceList();
}


/// <summary>
/// 実行
/// </summary>
/// <returns>最後のメッセージのwParamの値</returns>
int UseDirectXBase::Run(){
	MSG msg{0};

	// メインループ
	while(WM_QUIT != msg.message){
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			// ゲーム実行
			this->Execute();
		}
	}

	return static_cast<int>(msg.wParam);
}

/// <summary>
/// 終了させる
/// </summary>
void UseDirectXBase::Exit() const {
	::Ishikawa::Window::Window* const itWindow = this->GetDependentITWindow();
	if(itWindow != nullptr) itWindow->Exit();
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="_hInstance">インスタンスハンドル</param>
/// <param name="_windowProcedure">ウィンドウプロシージャ</param>
void UseDirectXBase::InitializeUseDirectXBase(
	HINSTANCE__* _hInstance,
	::Ishikawa::Window::WindowClass::ProcedureType _windowProcedure
){
	// 基底部分の初期化
	this->InitializeITWindowDependentResource();
	this->InitializeITDeviceDependentResource();


	// ウィンドウクラスの初期化、生成
	this->windowClass.Create(
		this->windowClassName,
		nullptr,
		this->windowClassStyle,
		this->windowClassClassExtra,
		this->windowClassWindowExtra,
		_hInstance,
		LoadIcon(_hInstance, IDI_APPLICATION),
		LoadIcon(_hInstance, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		(HBRUSH)GetStockObject(WHITE_BRUSH),
		_windowProcedure
	);

	// ウィンドウの初期化、生成
	this->window.Initialize();
	this->window.Create(
		this->windowClassName,
		this->windowTitle,
		this->windowStyle,
		this->windowExStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		this->windowWidth,
		this->windowHeight,
		nullptr,
		nullptr,
		_hInstance,
		nullptr
	);
	// 生成したウィンドウをマネージャに登録
	Ishikawa::Window::Manager& windowManager = Ishikawa::Common::SingletonManager::GetInstance<Ishikawa::Window::Manager>();
	windowManager.AddWindow(&window);

	// デバイスの初期化、生成
	this->directXDevice.Initialize();
	this->directXDevice.Create();

	// スワップチェーン作成
	this->swapChain.Create(&(this->directXDevice), &(this->window));
}

/// <summary>
/// ウィンドウ表示
/// </summary>
/// <param name="_nCmdShow">表示方法</param>
void UseDirectXBase::ShowWindow(int _nCmdShow){
	this->window.Show(_nCmdShow);
}

/// <summary>
/// 自身の依存先を設定する
/// </summary>
void UseDirectXBase::SettingDependentThis(){
	// 依存先設定
	this->window.AddDependentResource(this);
	this->directXDevice.AddDependentResource(this);
}
