//=============================================================================
//	ITWindow.cpp
//
//	����E�B���h�E�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITWindow.h"
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#include <windows.h>
#pragma warning(default:4820)
#pragma warning(default:4668)
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "DependentResource/ITWindowDependentResourceInterface.h"

using namespace Ishikawa::Window;


/// <summary>���f���t���O</summary>
const unsigned long Window::WINDOW_SUSPENDING_FLAG	= 0x00000001;
/// <summary>��ʃT�C�Y�ύX���t���O</summary>
const unsigned long Window::WINDOW_SIZE_MOVING_FLAG	= 0x00000002;
/// <summary>�ŏ����t���O</summary>
const unsigned long Window::WINDOW_MINIMIZING_FLAG	= 0x00000004;
/// <summary>�ő剻�t���O</summary>
const unsigned long Window::WINDOW_MAXIMIZING_FLAG	= 0x00000008;
/// <summary>�t���X�N���[���t���O</summary>
const unsigned long Window::WINDOW_FULLSCREEN_FLAG	= 0x00000010;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Window::Window() :
	nextITWindow(nullptr),
	beforeITWindow(nullptr),
	handle(nullptr),
	style(0UL),
	exStyle(0UL),
	width(0L),
	height(0L),
	widthOfBeforeMaximize(0L),
	heightOfBeforeMaximize(0L),
	windowFlag(0UL),
	windowDependentResourceListEdge()
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Window::~Window(){
	this->RemoveITWindowList();
}


/// <summary>
/// ���f����?
/// </summary>
/// <returns>true:���f�� false:���f���łȂ�</returns>
bool Window::IsSuspending() const{
	return ((this->windowFlag & WINDOW_SUSPENDING_FLAG) != 0);
}

/// <summary>
/// ��ʃT�C�Y�ύX����?
/// </summary>
/// <returns>true:��ʃT�C�Y�ύX�� false:��ʃT�C�Y�ύX���łȂ�</returns>
bool Window::IsSizeMoving() const{
	return ((this->windowFlag & WINDOW_SIZE_MOVING_FLAG) != 0);
}

/// <summary>
/// �ŏ�������?
/// </summary>
/// <returns>true:�ŏ����� false:�ŏ������łȂ�</returns>
bool Window::IsMinimizing() const{
	return ((this->windowFlag & WINDOW_MINIMIZING_FLAG) != 0);
}

/// <summary>
/// �ő剻����?
/// </summary>
/// <returns>true:�ő剻�� false:�ő剻���łȂ�</returns>
bool Window::IsMaximizing() const{
	return ((this->windowFlag & WINDOW_MAXIMIZING_FLAG) != 0);
}

/// <summary>
/// �t���X�N���[������?
/// </summary>
/// <returns>true:�t���X�N���[���� false:�t���X�N���[�����łȂ�</returns>
bool Window::IsFullscreen() const{
	return ((this->windowFlag & WINDOW_FULLSCREEN_FLAG) != 0);
}


/// <summary>
/// ���f���t���O�̐ݒ�
/// </summary>
/// <param name="_flag">���f���t���O</param>
void Window::SetSuspendFlag(bool _flag){
	if(_flag)	this->windowFlag |= WINDOW_SUSPENDING_FLAG;
	else		this->windowFlag &= (~WINDOW_SUSPENDING_FLAG);
}

/// <summary>
/// ��ʃT�C�Y�ύX���t���O�̐ݒ�
/// </summary>
/// <param name="_flag">��ʃT�C�Y�ύX���t���O</param>
void Window::SetSizeMoveFlag(bool _flag){
	if(_flag)	this->windowFlag |= WINDOW_SIZE_MOVING_FLAG;
	else		this->windowFlag &= (~WINDOW_SIZE_MOVING_FLAG);
}

/// <summary>
/// �ŏ������t���O�̐ݒ�
/// </summary>
/// <param name="_flag">�ŏ������t���O</param>
void Window::SetMinimizeFlag(bool _flag){
	if(_flag)	this->windowFlag |= WINDOW_MINIMIZING_FLAG;
	else		this->windowFlag &= (~WINDOW_MINIMIZING_FLAG);
}

/// <summary>
/// �ő剻���t���O�̐ݒ�
/// </summary>
/// <param name="_flag">�ő剻���t���O</param>
void Window::SetMaximizeFlag(bool _flag){
	if(_flag)	this->windowFlag |= WINDOW_MAXIMIZING_FLAG;
	else		this->windowFlag &= (~WINDOW_MAXIMIZING_FLAG);
}

/// <summary>
/// �t���X�N���[�����t���O�̐ݒ�
/// </summary>
/// <param name="_flag">�t���X�N���[�����t���O</param>
void Window::SetFullscreenFlag(bool _flag){
	if(_flag)	this->windowFlag |= WINDOW_FULLSCREEN_FLAG;
	else		this->windowFlag &= (~WINDOW_FULLSCREEN_FLAG);
}


/// <summary>
/// ���X�g����O���
/// </summary>
void Window::RemoveITWindowList(){
	// �������Ă���z���X�g����O���
	Interface* const next = this->nextITWindow;
	Interface* const before = this->beforeITWindow;
	next->SetBeforeITWindow(before);
	before->SetNextITWindow(next);
	// ���g�ŏz������
	this->nextITWindow = this;
	this->beforeITWindow = this;
}


/// <summary>
/// ������
/// </summary>
void Window::Initialize(){
	this->InitializeITWindow();
}

/// <summary>
/// ����E�B���h�E�̏�����
/// </summary>
void Window::InitializeITWindow(){
	// ���g�ŏz������
	this->nextITWindow = this;
	this->beforeITWindow = this;
	// �����o�̏�����
	this->windowDependentResourceListEdge.Initialize();
}


/// <summary>
/// �쐬
/// </summary>
/// <param name="_className">�E�B���h�E�N���X��</param>
/// <param name="_title">�^�C�g��</param>
/// <param name="_style">�X�^�C��</param>
/// <param name="_exStyle">EX�X�^�C��</param>
/// <param name="_x">x���W</param>
/// <param name="_y">y���W</param>
/// <param name="_width">��</param>
/// <param name="_height">����</param>
/// <param name="_parent">�e�E�B���h�E�̃n���h��[�e�Ȃ��̏ꍇ��NULL]</param>
/// <param name="_menu">���j���[�n���h��</param>
/// <param name="_hInstance">�A�v���P�[�V�����C���X�^���X�n���h��</param>
/// <param name="_param">����f�[�^�p</param>
void Window::Create(
	const wchar_t* const _className,
	const wchar_t* const _title,
	const unsigned long _style,
	const unsigned long _exStyle,
	const long _x,
	const long _y,
	const long _width,
	const long _height,
	HWND__* const _parent,
	HMENU__* const _menu,
	HINSTANCE__* const _hInstance,
	void* const _param
){
	// ��`�̐ݒ�
	RECT rect = {0, 0, _width, _height};
	// �w�肳�ꂽ�N���C�A���g�̈�m�ۂׂ̈ɕK�v�ȍ��W���v�Z����
	AdjustWindowRectEx(&rect, _style, FALSE, _exStyle);
	long w = rect.right - rect.left;
	long h = rect.bottom - rect.top;

	// �����o�ϐ��̍X�V
	this->style = _style;
	this->exStyle = _exStyle;
	this->width = _width;
	this->height = _height;

	// �E�B���h�E����
	this->handle = CreateWindowExW(
		_exStyle,
		_className,
		_title,
		_style,
		_x,
		_y,
		w,
		h,
		_parent,
		_menu,
		_hInstance,
		_param
	);

	if(this->handle == nullptr){
		// �쐬���s
		throw Common::Exception::FunctionFailed("Couldn't create window.");
	}
}

/// <summary>
/// �\��
/// </summary>
/// <param name="nCmdShow">�����\�����@</param>
void Window::Show(int nCmdShow){
	ShowWindow(this->handle, nCmdShow);
}

/// <summary>
/// �I������
/// </summary>
void Window::Exit() const {
	PostMessageA(this->handle, WM_CLOSE, 0U, 0L);
}


/// <summary>
/// �ˑ����\�[�X�̒ǉ�(1����)
/// </summary>
/// <param name="_dependentResource">�ǉ�����ˑ����\�[�X</param>
void Window::AddDependentResource(DependentResource::Interface* const _dependentResource){
	if(_dependentResource == nullptr){
		throw Ishikawa::Common::Exception::FunctionFailed("You can't add NULL to list of window dependent resource.");
	}

	// �ǉ�����f�[�^�����X�g����O��
	_dependentResource->RemoveITWindowDependentResourceList();

	DependentResource::Interface* const tail = this->windowDependentResourceListEdge.GetBeforeITWindowDependentResource();
	// �z���X�g�̈�Ԍ��ɒǉ�
	_dependentResource->SetBeforeITWindowDependentResource(tail);
	_dependentResource->SetNextITWindowDependentResource(&(this->windowDependentResourceListEdge));
	tail->SetNextITWindowDependentResource(_dependentResource);
	this->windowDependentResourceListEdge.SetBeforeITWindowDependentResource(_dependentResource);

	// �ˑ��E�B���h�E��ݒ肵�Ēʒm
	_dependentResource->SetDependentITWindow(this);
	_dependentResource->OnWindowLoaded();
}

/// <summary>
/// �ˑ����\�[�X�̒ǉ�(�z���X�g����)
/// �ǉ�������͌����X�g�͋�ɂȂ�܂�
/// </summary>
/// <param name="_dependentResourceList">�ǉ�����ˑ����\�[�X�̏z���X�g�̒[</param>
void Window::AddDependentResourceList(DependentResource::ListEdge* const _dependentResourceListEdge){
	DependentResource::Interface* const headToBeAdded = _dependentResourceListEdge->GetNextITWindowDependentResource();
	// �ǉ�������̂��Ȃ��̂ŉ��������I���
	if(headToBeAdded->IsITWindowDependentResourceListEdge()) return;
	DependentResource::Interface* const tailToBeAdded = _dependentResourceListEdge->GetBeforeITWindowDependentResource();

	// �ǉ����X�g���璆�g��؂藣��
	_dependentResourceListEdge->SetNextITWindowDependentResource(_dependentResourceListEdge);
	_dependentResourceListEdge->SetBeforeITWindowDependentResource(_dependentResourceListEdge);

	DependentResource::Interface* const tail = this->windowDependentResourceListEdge.GetBeforeITWindowDependentResource();
	// �؂藣�������g���z���X�g�̈�Ԍ��ɒǉ�
	headToBeAdded->SetBeforeITWindowDependentResource(tail);
	tailToBeAdded->SetNextITWindowDependentResource(&(this->windowDependentResourceListEdge));
	tail->SetNextITWindowDependentResource(headToBeAdded);
	this->windowDependentResourceListEdge.SetBeforeITWindowDependentResource(tailToBeAdded);

	// �ˑ��E�B���h�E��ݒ肵�Ēʒm
	DependentResource::Interface* resource = headToBeAdded;
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->SetDependentITWindow(this);
		resource->OnWindowLoaded();

		resource = resource->GetNextITWindowDependentResource();
	}
}


/// <summary>
/// ����
/// </summary>
void Window::Close(){
	// �ʒm
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnWindowClosed();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// ���f
/// </summary>
void Window::Suspend(){
	this->SetSuspendFlag(true);
	// �ʒm
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnWindowSuspended();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// �ĊJ
/// </summary>
void Window::Resume(){
	this->SetSuspendFlag(false);
	// �ʒm
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnWindowResumed();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// �T�C�Y�ύX
/// </summary>
/// <param name="_width">��</param>
/// <param name="_height">����</param>
void Window::ChangeSize(long _width, long _height){
	this->width = _width;
	this->height = _height;

	// �ʒm
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnWindowSizeChanged();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// ������
/// </summary>
void Window::Activate(){
	// �ʒm
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnWindowActivated();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// �񊈐���
/// </summary>
void Window::Deactivate(){
	// �ʒm
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnWindowDeactivated();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// �ŏ���
/// </summary>
void Window::Minimize(){
	this->SetMinimizeFlag(true);
	// �ʒm
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnWindowMinimized();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// �ŏ�������
/// </summary>
void Window::CancelOfMinimize(){
	this->SetMinimizeFlag(false);
	// �ʒm
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnCancelOfWindowMinimized();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// �ő剻
/// </summary>
void Window::Maximize(){
	this->SetMaximizeFlag(true);
	// �ʒm
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnWindowMaximized();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// �ő剻����
/// </summary>
void Window::CancelOfMaximize(){
	this->SetMaximizeFlag(false);
	// �ʒm
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnCancelOfWindowMaximized();
		resource = resource->GetNextITWindowDependentResource();
	}
}


/// <summary>
/// �v���V�[�W��
/// </summary>
/// <param name="_message">���b�Z�[�W</param>
/// <param name="_wParam">���b�Z�[�W�̕t�����</param>
/// <param name="_lParam">���b�Z�[�W�̕t�����</param>
/// <returns>true:�Ăяo�����Ƃ�TRUE��Ԃ� false:�f�t�H���g�E�B���h�E�v���V�[�W����</returns>
bool Window::Procedure(unsigned int _message, unsigned int _wParam, long _lParam){
	switch(_message){
	case WM_DESTROY:
	{
		this->Close();
		PostQuitMessage(0);
		break;
	}

	case WM_ENTERSIZEMOVE:
	{
		this->SetSizeMoveFlag(true);
		break;
	}

	case WM_EXITSIZEMOVE:
	{
		this->SetSizeMoveFlag(false);

		RECT rect;
		GetClientRect(this->handle, &rect);

		long w = rect.right - rect.left;
		long h = rect.bottom - rect.top;

		if((w != this->width) || (h != this->height)){
			this->ChangeSize(w, h);
		}
		break;
	}

	case WM_SIZE:
	{
		// �L�����Z���������ɂ���
		if((_wParam != SIZE_MINIMIZED) && this->IsMinimizing()){
			this->CancelOfMinimize();
		}
		if((_wParam != SIZE_MAXIMIZED) && this->IsMaximizing()){
			this->CancelOfMaximize();
			// �ő剻�O�̃T�C�Y�ɕύX
			this->ChangeSize(this->widthOfBeforeMaximize, this->heightOfBeforeMaximize);
		}

		if(_wParam == SIZE_MINIMIZED){
			this->Minimize();
		}
		else if(_wParam == SIZE_MAXIMIZED){
			// �ő剻�O�̕��ƍ�����ۑ�
			this->widthOfBeforeMaximize = this->width;
			this->heightOfBeforeMaximize = this->height;
			// �ő剻
			this->Maximize();
			// �T�C�Y�ύX
			this->ChangeSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		}
		break;
	}

	case WM_ACTIVATEAPP:
	{
		if(_wParam != 0){
			this->Activate();
		}
		else{
			this->Deactivate();
		}
		break;
	}

	case WM_POWERBROADCAST:
	{
		switch(_wParam){
		case PBT_APMQUERYSUSPEND:
		{
			if(!(this->IsSuspending())){
				this->Suspend();
			}
			return true;
		}

		case PBT_APMRESUMESUSPEND:
		{
			if(!(this->IsMinimizing())){
				if(this->IsSuspending()){
					this->Resume();
				}
			}
			return true;
		}

		default:
			// �������Ȃ�
			break;
		}
		break;
	}

	case WM_SYSKEYDOWN:
	{
		if((_wParam == VK_RETURN) && ((_lParam & 0x60000000) == 0x20000000)){
			// Alt + Enter �Ńt���X�N���[���̃g�O���Ƃ���
			if(this->IsFullscreen()){
				// �E�B���h�E���[�h�ɕύX
				SetWindowLongPtrW(this->handle, GWL_STYLE, static_cast<long>(this->style));
				SetWindowLongPtrW(this->handle, GWL_EXSTYLE, static_cast<long>(this->exStyle));

				ShowWindow(this->handle, SW_NORMAL);

				// ��`�̐ݒ�
				RECT rect = {0, 0, this->width, this->height};
				// �w�肳�ꂽ�N���C�A���g�̈�m�ۂׂ̈ɕK�v�ȍ��W���v�Z����
				AdjustWindowRectEx(&rect, this->style, FALSE, this->exStyle);
				long w = rect.right - rect.left;
				long h = rect.bottom - rect.top;

				SetWindowPos(
					this->handle,
					HWND_TOP,
					CW_USEDEFAULT, CW_USEDEFAULT,
					w, h,
					SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED
				);
			}
			else{
				// �t���X�N���[�����[�h�ɕύX
				SetWindowLongPtrW(this->handle, GWL_STYLE, 0);
				SetWindowLongPtrW(this->handle, GWL_EXSTYLE, WS_EX_TOPMOST);

				SetWindowPos(
					this->handle,
					HWND_TOP,
					0, 0,
					0, 0,
					SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
				);
				ShowWindow(this->handle, SW_MAXIMIZE);
			}

			// �g�O������@"this->windowFlag ^= WINDOW_FULLSCREEN_FLAG"�ł�����
			this->SetFullscreenFlag(!(this->IsFullscreen()));
		}
	}

	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP:
	{
		// �L�[�{�[�h�X�e�[�g�ύX�ʒm
		DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
		while(!(resource->IsITWindowDependentResourceListEdge())){
			resource->OnChangedKeyboardState(_message, _wParam, _lParam);
			resource = resource->GetNextITWindowDependentResource();
		}
		break;
	}

	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
	{
		// �L�[�{�[�h�X�e�[�g�ύX�ʒm
		DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
		while(!(resource->IsITWindowDependentResourceListEdge())){
			resource->OnChangedMouseState(_message, _wParam, _lParam);
			resource = resource->GetNextITWindowDependentResource();
		}
		break;
	}

	default:
		// �������Ȃ�
		break;
	}

	// �f�t�H���g�E�B���h�E�v���V�[�W���ɓ�����
	return false;
}
