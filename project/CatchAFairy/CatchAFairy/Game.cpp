//=============================================================================
//	Game.cpp
//
//	ゲームのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "Game.h"
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#include <windows.h>
#pragma warning(default:4820)
#pragma warning(default:4668)
#include "Ishikawa/Common/DebugLog.h"
#include "Ishikawa/Common/SystemFunctions.h"
#include "Ishikawa/Common/Timer.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Common/PatternBase/SingletonManager.h"
#include "Ishikawa/Window/ITWindow.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "Ishikawa/CriAdx2Le/ITCriAdx2Le.h"
#include "Scene/SceneLogo.h"
#if __IS_DEBUG
#include "Scene/SceneResult.h"
#endif

using namespace CatchAFairy;

/// <summary>1回当たりの最大経過時間</summary>
const float Game::MAX_ELAPSED_TIME = 0.5f;
/// <summary>ウィンドウタイトル名</summary>
const wchar_t* const Game::WINDOW_TITLE_NAME = L"妖精鬼ごっこ";
/// <summary>acfファイルパス</summary>
const char* const Game::SOUND_ACF_FILEPATH = "Resources\\Sounds\\CatchAFairySounds.acf";
/// <summary>acbファイルパス</summary>
const char* const Game::SOUND_ACB_FILEPATH = "Resources\\Sounds\\CueSheet_0.acb";
/// <summary>awbファイルパス</summary>
const char* const Game::SOUND_AWB_FILEPATH = "Resources\\Sounds\\CueSheet_0.awb";


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
Game::Game() :
	::Ishikawa::Game::UseDirectXBase(
		L"CatchAFairyWindowClass",
		CS_HREDRAW | CS_VREDRAW,
		0L,
		0L,
		WINDOW_TITLE_NAME,
		WS_OVERLAPPEDWINDOW,
		0UL,
		800L,
		600L
	),
	state(State::Initalize),
	sceneManager(),
	keyboard()
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Game::~Game(){
	// 何もしない
}


/// <summary>
/// 実行するか?
/// </summary>
/// <returns>true:実行する false:実行しない</returns>
bool Game::IsExecute() const{
	return (this->state == State::Active);
}


#pragma region IT window messages
/// <summary>
/// ウィンドウに読み込まれた
/// </summary>
void Game::OnWindowLoaded(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowLoaded()");
}

/// <summary>
/// ウィンドウが閉じられた
/// </summary>
void Game::OnWindowClosed(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowClosed()");
}

/// <summary>
/// ウィンドウサイズが変更された
/// </summary>
void Game::OnWindowSizeChanged(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowSizeChanged()");

	::Ishikawa::Window::Window* itWindow = this->GetDependentITWindow();

	// スワップチェーンサイズ変更
	long windowWidth_ = 1L;
	long windowHeight_ = 1L;
	itWindow->GetSize(&windowWidth_, &windowHeight_);
	this->GetSwapChain().ChangeSize(static_cast<unsigned int>(windowWidth_), static_cast<unsigned int>(windowHeight_));
}

/// <summary>
/// ウィンドウが最大化された
/// </summary>
void Game::OnWindowMaximized(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowMaximized()");
}

/// <summary>
/// ウィンドウの最大化が解除された
/// </summary>
void Game::OnCancelOfWindowMaximized(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnCancelOfWindowMaximized()");
}

/// <summary>
/// ウィンドウが最小化された
/// </summary>
void Game::OnWindowMinimized(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowMinimized()");
}

/// <summary>
/// ウィンドウの最小化が解除された
/// </summary>
void Game::OnCancelOfWindowMinimized(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnCancelOfWindowMinimized()");
}

/// <summary>
/// ウィンドウが中断された
/// </summary>
void Game::OnWindowSuspended(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowSuspended()");
}

/// <summary>
/// ウィンドウが再開された
/// </summary>
void Game::OnWindowResumed(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowResumed()");
}

/// <summary>
/// ウィンドウが活性化した
/// </summary>
void Game::OnWindowActivated(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowActivated()");

	// 活性状態にする
	this->state = State::Active;
}

/// <summary>
/// ウィンドウが非活性化した
/// </summary>
void Game::OnWindowDeactivated(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowDeactivated()");

	// 非活性状態にする
	this->state = State::Deactive;
}

/// <summary>
/// キーボードステートが変更された
/// </summary>
/// <param name="_message">メッセージ</param>
/// <param name="_wParam">メッセージの付加情報</param>
/// <param name="_lParam">メッセージの付加情報</param>
void Game::OnChangedKeyboardState(unsigned int _message, unsigned int _wParam, long _lParam){
	this->keyboard.OnChangedState(_message, _wParam, _lParam);
}

/// <summary>
/// マウスステートが変更された
/// </summary>
/// <param name="_message">メッセージ</param>
/// <param name="_wParam">メッセージの付加情報</param>
/// <param name="_lParam">メッセージの付加情報</param>
void Game::OnChangedMouseState(unsigned int _message, unsigned int _wParam, long _lParam){
	NOT_USING(_message);
	NOT_USING(_wParam);
	NOT_USING(_lParam);
	// TODO
}
#pragma endregion IT window messages

#pragma region IT device messages
/// <summary>
/// デバイスに読み込まれた
/// </summary>
void Game::OnDeviceLoaded(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnDeviceLoaded()");

	this->sceneManager.OnLoadedGameDevice();
}

/// <summary>
/// デバイスが復元された
/// </summary>
void Game::OnDeviceRestored(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnDeviceRestored()");

	this->sceneManager.OnRestoredGameDevice();
}

/// <summary>
/// デバイスが破棄された
/// </summary>
void Game::OnDeviceLost(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnDeviceLost()");

	this->sceneManager.OnLostGameDevice();
}
#pragma endregion IT device messages


/// <summary>
/// 初期化
/// </summary>
/// <param name="_hInstance">インスタンスハンドル</param>
/// <param name="_windowProcedure">ウィンドウプロシージャ</param>
void Game::Initialize(
	HINSTANCE__* _hInstance,
	::Ishikawa::Window::WindowClass::ProcedureType _windowProcedure
){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::Initialize()");

	// CRI初期化
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	itCriAdx2Le.Initialize(SOUND_ACF_FILEPATH, SOUND_ACB_FILEPATH, SOUND_AWB_FILEPATH);

	// 基底部分の初期化
	this->InitializeUseDirectXBase(_hInstance, _windowProcedure);

	// メンバの初期化
	this->sceneManager.SetGame(this);
	this->sceneManager.Initialize();

	// デフォルトタイマーの初期化
	::Ishikawa::Common::Timer::DEFAULT_TIMER.Initialize();

#if __IS_DEBUG
	// 途中のシーンで始める
	this->sceneManager.PushScene(new Scene::Result());
#else
	// ロゴシーンで始める
	this->sceneManager.PushScene(new Scene::Logo());
#endif

	// 依存先設定
	this->SettingDependentThis();
}

/// <summary>
/// 活性化する
/// </summary>
/// <param name="_nCmdShow">表示方法</param>
void Game::Activate(int _nCmdShow){
	this->state = State::Active;

	// ウィンドウ表示
	this->ShowWindow(_nCmdShow);
}

/// <summary>
/// 終了
/// </summary>
void Game::Finalize(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::Finalize()");

	// 終了状態にする
	this->state = State::Finalize;

	// シーンの終了
	this->sceneManager.Finalize();

	// CRI終了
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	itCriAdx2Le.Finalize();
}

/// <summary>
/// 実行
/// </summary>
void Game::Execute(){
	if(this->IsExecute()){
		this->Update();
		this->Render();
	}
	else{
		// 0.1秒待機
		::Ishikawa::Common::System::Sleep(100U);
	}
}


/// <summary>
/// 更新
/// </summary>
void Game::Update(){
	// Escape入力で終了させる
	if(this->keyboard.IsPressing(::Ishikawa::Input::KeyCode::Escape)){
		this->Exit();
	}

	// CRI更新
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	itCriAdx2Le.Update();

	// デフォルトタイマー更新
	::Ishikawa::Common::Timer::DEFAULT_TIMER.Update();

	// 経過時間をシミュレート用に調整
	float elapsedTime = ::Ishikawa::Common::Timer::DEFAULT_TIMER.GetElapsedSeconds();
	elapsedTime = (elapsedTime > MAX_ELAPSED_TIME) ? (MAX_ELAPSED_TIME) : (elapsedTime);
	// シーン更新
	this->sceneManager.Update(elapsedTime);

	// キーボード更新
	this->keyboard.Update();
}

/// <summary>
/// 描画
/// </summary>
void Game::Render(){

	// シーンの描画
	this->sceneManager.Render();


	// 画面を消す[RenderToScreen()内のシェーダで初期化しているので何もしない]
//	this->GetSwapChain().ClearScreen();

	// 画面へ描画
	unsigned int width = 1U, height = 1U;
	this->GetSwapChain().GetSize(&width, &height);

	this->sceneManager.RenderToScreen(
		this->GetSwapChain().GetRenderTargetView(),
		this->GetSwapChain().GetDepthStencilView(),
		width, height
	);

	// 画面に反映
	this->GetSwapChain().Present();
}
