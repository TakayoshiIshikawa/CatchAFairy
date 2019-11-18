//=============================================================================
//	ITGameUseDirectXBase.cpp
//
//	DirectX�𗘗p���鎩��Q�[�����̃\�[�X�t�@�C��
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
/// �f�t�H���g�R���X�g���N�^
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
	// �������Ȃ�
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="_windowClassName">�E�B���h�E�N���X��</param>
/// <param name="_windowClassStyle">�E�B���h�E�N���X�X�^�C��</param>
/// <param name="_windowClassClassExtra">�E�B���h�E�N���X�N���X�G�N�X�g��</param>
/// <param name="_windowClassWindowExtra">�E�B���h�E�N���X�E�B���h�E�G�N�X�g��</param>
/// <param name="_windowTitle">�E�B���h�E�^�C�g��</param>
/// <param name="_windowStyle">�E�B���h�E�X�^�C��</param>
/// <param name="_windowExStyle">�E�B���h�EEX�X�^�C��</param>
/// <param name="_windowWidth">�E�B���h�E��</param>
/// <param name="_windowHeight">�E�B���h�E����</param>
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
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
UseDirectXBase::~UseDirectXBase(){
	// ���\�[�X���X�g����O���
	this->RemoveITWindowDependentResourceList();
	this->RemoveITDeviceDependentResourceList();
}


/// <summary>
/// ���s
/// </summary>
/// <returns>�Ō�̃��b�Z�[�W��wParam�̒l</returns>
int UseDirectXBase::Run(){
	MSG msg{0};

	// ���C�����[�v
	while(WM_QUIT != msg.message){
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			// �Q�[�����s
			this->Execute();
		}
	}

	return static_cast<int>(msg.wParam);
}

/// <summary>
/// �I��������
/// </summary>
void UseDirectXBase::Exit() const {
	::Ishikawa::Window::Window* const itWindow = this->GetDependentITWindow();
	if(itWindow != nullptr) itWindow->Exit();
}

/// <summary>
/// ������
/// </summary>
/// <param name="_hInstance">�C���X�^���X�n���h��</param>
/// <param name="_windowProcedure">�E�B���h�E�v���V�[�W��</param>
void UseDirectXBase::InitializeUseDirectXBase(
	HINSTANCE__* _hInstance,
	::Ishikawa::Window::WindowClass::ProcedureType _windowProcedure
){
	// ��ꕔ���̏�����
	this->InitializeITWindowDependentResource();
	this->InitializeITDeviceDependentResource();


	// �E�B���h�E�N���X�̏������A����
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

	// �E�B���h�E�̏������A����
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
	// ���������E�B���h�E���}�l�[�W���ɓo�^
	Ishikawa::Window::Manager& windowManager = Ishikawa::Common::SingletonManager::GetInstance<Ishikawa::Window::Manager>();
	windowManager.AddWindow(&window);

	// �f�o�C�X�̏������A����
	this->directXDevice.Initialize();
	this->directXDevice.Create();

	// �X���b�v�`�F�[���쐬
	this->swapChain.Create(&(this->directXDevice), &(this->window));
}

/// <summary>
/// �E�B���h�E�\��
/// </summary>
/// <param name="_nCmdShow">�\�����@</param>
void UseDirectXBase::ShowWindow(int _nCmdShow){
	this->window.Show(_nCmdShow);
}

/// <summary>
/// ���g�̈ˑ����ݒ肷��
/// </summary>
void UseDirectXBase::SettingDependentThis(){
	// �ˑ���ݒ�
	this->window.AddDependentResource(this);
	this->directXDevice.AddDependentResource(this);
}
