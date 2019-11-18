//=============================================================================
//	Game.cpp
//
//	�Q�[���̃\�[�X�t�@�C��
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

/// <summary>1�񓖂���̍ő�o�ߎ���</summary>
const float Game::MAX_ELAPSED_TIME = 0.5f;
/// <summary>�E�B���h�E�^�C�g����</summary>
const wchar_t* const Game::WINDOW_TITLE_NAME = L"�d���S������";
/// <summary>acf�t�@�C���p�X</summary>
const char* const Game::SOUND_ACF_FILEPATH = "Resources\\Sounds\\CatchAFairySounds.acf";
/// <summary>acb�t�@�C���p�X</summary>
const char* const Game::SOUND_ACB_FILEPATH = "Resources\\Sounds\\CueSheet_0.acb";
/// <summary>awb�t�@�C���p�X</summary>
const char* const Game::SOUND_AWB_FILEPATH = "Resources\\Sounds\\CueSheet_0.awb";


/// <summary>
/// �f�t�H���g�R���X�g���N�^
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
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Game::~Game(){
	// �������Ȃ�
}


/// <summary>
/// ���s���邩?
/// </summary>
/// <returns>true:���s���� false:���s���Ȃ�</returns>
bool Game::IsExecute() const{
	return (this->state == State::Active);
}


#pragma region IT window messages
/// <summary>
/// �E�B���h�E�ɓǂݍ��܂ꂽ
/// </summary>
void Game::OnWindowLoaded(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowLoaded()");
}

/// <summary>
/// �E�B���h�E������ꂽ
/// </summary>
void Game::OnWindowClosed(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowClosed()");
}

/// <summary>
/// �E�B���h�E�T�C�Y���ύX���ꂽ
/// </summary>
void Game::OnWindowSizeChanged(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowSizeChanged()");

	::Ishikawa::Window::Window* itWindow = this->GetDependentITWindow();

	// �X���b�v�`�F�[���T�C�Y�ύX
	long windowWidth_ = 1L;
	long windowHeight_ = 1L;
	itWindow->GetSize(&windowWidth_, &windowHeight_);
	this->GetSwapChain().ChangeSize(static_cast<unsigned int>(windowWidth_), static_cast<unsigned int>(windowHeight_));
}

/// <summary>
/// �E�B���h�E���ő剻���ꂽ
/// </summary>
void Game::OnWindowMaximized(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowMaximized()");
}

/// <summary>
/// �E�B���h�E�̍ő剻���������ꂽ
/// </summary>
void Game::OnCancelOfWindowMaximized(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnCancelOfWindowMaximized()");
}

/// <summary>
/// �E�B���h�E���ŏ������ꂽ
/// </summary>
void Game::OnWindowMinimized(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowMinimized()");
}

/// <summary>
/// �E�B���h�E�̍ŏ������������ꂽ
/// </summary>
void Game::OnCancelOfWindowMinimized(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnCancelOfWindowMinimized()");
}

/// <summary>
/// �E�B���h�E�����f���ꂽ
/// </summary>
void Game::OnWindowSuspended(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowSuspended()");
}

/// <summary>
/// �E�B���h�E���ĊJ���ꂽ
/// </summary>
void Game::OnWindowResumed(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowResumed()");
}

/// <summary>
/// �E�B���h�E������������
/// </summary>
void Game::OnWindowActivated(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowActivated()");

	// ������Ԃɂ���
	this->state = State::Active;
}

/// <summary>
/// �E�B���h�E���񊈐�������
/// </summary>
void Game::OnWindowDeactivated(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnWindowDeactivated()");

	// �񊈐���Ԃɂ���
	this->state = State::Deactive;
}

/// <summary>
/// �L�[�{�[�h�X�e�[�g���ύX���ꂽ
/// </summary>
/// <param name="_message">���b�Z�[�W</param>
/// <param name="_wParam">���b�Z�[�W�̕t�����</param>
/// <param name="_lParam">���b�Z�[�W�̕t�����</param>
void Game::OnChangedKeyboardState(unsigned int _message, unsigned int _wParam, long _lParam){
	this->keyboard.OnChangedState(_message, _wParam, _lParam);
}

/// <summary>
/// �}�E�X�X�e�[�g���ύX���ꂽ
/// </summary>
/// <param name="_message">���b�Z�[�W</param>
/// <param name="_wParam">���b�Z�[�W�̕t�����</param>
/// <param name="_lParam">���b�Z�[�W�̕t�����</param>
void Game::OnChangedMouseState(unsigned int _message, unsigned int _wParam, long _lParam){
	NOT_USING(_message);
	NOT_USING(_wParam);
	NOT_USING(_lParam);
	// TODO
}
#pragma endregion IT window messages

#pragma region IT device messages
/// <summary>
/// �f�o�C�X�ɓǂݍ��܂ꂽ
/// </summary>
void Game::OnDeviceLoaded(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnDeviceLoaded()");

	this->sceneManager.OnLoadedGameDevice();
}

/// <summary>
/// �f�o�C�X���������ꂽ
/// </summary>
void Game::OnDeviceRestored(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnDeviceRestored()");

	this->sceneManager.OnRestoredGameDevice();
}

/// <summary>
/// �f�o�C�X���j�����ꂽ
/// </summary>
void Game::OnDeviceLost(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::OnDeviceLost()");

	this->sceneManager.OnLostGameDevice();
}
#pragma endregion IT device messages


/// <summary>
/// ������
/// </summary>
/// <param name="_hInstance">�C���X�^���X�n���h��</param>
/// <param name="_windowProcedure">�E�B���h�E�v���V�[�W��</param>
void Game::Initialize(
	HINSTANCE__* _hInstance,
	::Ishikawa::Window::WindowClass::ProcedureType _windowProcedure
){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::Initialize()");

	// CRI������
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	itCriAdx2Le.Initialize(SOUND_ACF_FILEPATH, SOUND_ACB_FILEPATH, SOUND_AWB_FILEPATH);

	// ��ꕔ���̏�����
	this->InitializeUseDirectXBase(_hInstance, _windowProcedure);

	// �����o�̏�����
	this->sceneManager.SetGame(this);
	this->sceneManager.Initialize();

	// �f�t�H���g�^�C�}�[�̏�����
	::Ishikawa::Common::Timer::DEFAULT_TIMER.Initialize();

#if __IS_DEBUG
	// �r���̃V�[���Ŏn�߂�
	this->sceneManager.PushScene(new Scene::Result());
#else
	// ���S�V�[���Ŏn�߂�
	this->sceneManager.PushScene(new Scene::Logo());
#endif

	// �ˑ���ݒ�
	this->SettingDependentThis();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="_nCmdShow">�\�����@</param>
void Game::Activate(int _nCmdShow){
	this->state = State::Active;

	// �E�B���h�E�\��
	this->ShowWindow(_nCmdShow);
}

/// <summary>
/// �I��
/// </summary>
void Game::Finalize(){
	::Ishikawa::Common::Debug::FunctionLog fLog("CatchAFairy::Game::Finalize()");

	// �I����Ԃɂ���
	this->state = State::Finalize;

	// �V�[���̏I��
	this->sceneManager.Finalize();

	// CRI�I��
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	itCriAdx2Le.Finalize();
}

/// <summary>
/// ���s
/// </summary>
void Game::Execute(){
	if(this->IsExecute()){
		this->Update();
		this->Render();
	}
	else{
		// 0.1�b�ҋ@
		::Ishikawa::Common::System::Sleep(100U);
	}
}


/// <summary>
/// �X�V
/// </summary>
void Game::Update(){
	// Escape���͂ŏI��������
	if(this->keyboard.IsPressing(::Ishikawa::Input::KeyCode::Escape)){
		this->Exit();
	}

	// CRI�X�V
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	itCriAdx2Le.Update();

	// �f�t�H���g�^�C�}�[�X�V
	::Ishikawa::Common::Timer::DEFAULT_TIMER.Update();

	// �o�ߎ��Ԃ��V�~�����[�g�p�ɒ���
	float elapsedTime = ::Ishikawa::Common::Timer::DEFAULT_TIMER.GetElapsedSeconds();
	elapsedTime = (elapsedTime > MAX_ELAPSED_TIME) ? (MAX_ELAPSED_TIME) : (elapsedTime);
	// �V�[���X�V
	this->sceneManager.Update(elapsedTime);

	// �L�[�{�[�h�X�V
	this->keyboard.Update();
}

/// <summary>
/// �`��
/// </summary>
void Game::Render(){

	// �V�[���̕`��
	this->sceneManager.Render();


	// ��ʂ�����[RenderToScreen()���̃V�F�[�_�ŏ��������Ă���̂ŉ������Ȃ�]
//	this->GetSwapChain().ClearScreen();

	// ��ʂ֕`��
	unsigned int width = 1U, height = 1U;
	this->GetSwapChain().GetSize(&width, &height);

	this->sceneManager.RenderToScreen(
		this->GetSwapChain().GetRenderTargetView(),
		this->GetSwapChain().GetDepthStencilView(),
		width, height
	);

	// ��ʂɔ��f
	this->GetSwapChain().Present();
}
