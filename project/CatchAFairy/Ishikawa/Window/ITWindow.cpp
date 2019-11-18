//=============================================================================
//	ITWindow.cpp
//
//	自作ウィンドウのソースファイル
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


/// <summary>中断中フラグ</summary>
const unsigned long Window::WINDOW_SUSPENDING_FLAG	= 0x00000001;
/// <summary>画面サイズ変更中フラグ</summary>
const unsigned long Window::WINDOW_SIZE_MOVING_FLAG	= 0x00000002;
/// <summary>最小化フラグ</summary>
const unsigned long Window::WINDOW_MINIMIZING_FLAG	= 0x00000004;
/// <summary>最大化フラグ</summary>
const unsigned long Window::WINDOW_MAXIMIZING_FLAG	= 0x00000008;
/// <summary>フルスクリーンフラグ</summary>
const unsigned long Window::WINDOW_FULLSCREEN_FLAG	= 0x00000010;


/// <summary>
/// デフォルトコンストラクタ
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
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Window::~Window(){
	this->RemoveITWindowList();
}


/// <summary>
/// 中断中か?
/// </summary>
/// <returns>true:中断中 false:中断中でない</returns>
bool Window::IsSuspending() const{
	return ((this->windowFlag & WINDOW_SUSPENDING_FLAG) != 0);
}

/// <summary>
/// 画面サイズ変更中か?
/// </summary>
/// <returns>true:画面サイズ変更中 false:画面サイズ変更中でない</returns>
bool Window::IsSizeMoving() const{
	return ((this->windowFlag & WINDOW_SIZE_MOVING_FLAG) != 0);
}

/// <summary>
/// 最小化中か?
/// </summary>
/// <returns>true:最小化中 false:最小化中でない</returns>
bool Window::IsMinimizing() const{
	return ((this->windowFlag & WINDOW_MINIMIZING_FLAG) != 0);
}

/// <summary>
/// 最大化中か?
/// </summary>
/// <returns>true:最大化中 false:最大化中でない</returns>
bool Window::IsMaximizing() const{
	return ((this->windowFlag & WINDOW_MAXIMIZING_FLAG) != 0);
}

/// <summary>
/// フルスクリーン中か?
/// </summary>
/// <returns>true:フルスクリーン中 false:フルスクリーン中でない</returns>
bool Window::IsFullscreen() const{
	return ((this->windowFlag & WINDOW_FULLSCREEN_FLAG) != 0);
}


/// <summary>
/// 中断中フラグの設定
/// </summary>
/// <param name="_flag">中断中フラグ</param>
void Window::SetSuspendFlag(bool _flag){
	if(_flag)	this->windowFlag |= WINDOW_SUSPENDING_FLAG;
	else		this->windowFlag &= (~WINDOW_SUSPENDING_FLAG);
}

/// <summary>
/// 画面サイズ変更中フラグの設定
/// </summary>
/// <param name="_flag">画面サイズ変更中フラグ</param>
void Window::SetSizeMoveFlag(bool _flag){
	if(_flag)	this->windowFlag |= WINDOW_SIZE_MOVING_FLAG;
	else		this->windowFlag &= (~WINDOW_SIZE_MOVING_FLAG);
}

/// <summary>
/// 最小化中フラグの設定
/// </summary>
/// <param name="_flag">最小化中フラグ</param>
void Window::SetMinimizeFlag(bool _flag){
	if(_flag)	this->windowFlag |= WINDOW_MINIMIZING_FLAG;
	else		this->windowFlag &= (~WINDOW_MINIMIZING_FLAG);
}

/// <summary>
/// 最大化中フラグの設定
/// </summary>
/// <param name="_flag">最大化中フラグ</param>
void Window::SetMaximizeFlag(bool _flag){
	if(_flag)	this->windowFlag |= WINDOW_MAXIMIZING_FLAG;
	else		this->windowFlag &= (~WINDOW_MAXIMIZING_FLAG);
}

/// <summary>
/// フルスクリーン中フラグの設定
/// </summary>
/// <param name="_flag">フルスクリーン中フラグ</param>
void Window::SetFullscreenFlag(bool _flag){
	if(_flag)	this->windowFlag |= WINDOW_FULLSCREEN_FLAG;
	else		this->windowFlag &= (~WINDOW_FULLSCREEN_FLAG);
}


/// <summary>
/// リストから外れる
/// </summary>
void Window::RemoveITWindowList(){
	// 所属している循環リストから外れる
	Interface* const next = this->nextITWindow;
	Interface* const before = this->beforeITWindow;
	next->SetBeforeITWindow(before);
	before->SetNextITWindow(next);
	// 自身で循環させる
	this->nextITWindow = this;
	this->beforeITWindow = this;
}


/// <summary>
/// 初期化
/// </summary>
void Window::Initialize(){
	this->InitializeITWindow();
}

/// <summary>
/// 自作ウィンドウの初期化
/// </summary>
void Window::InitializeITWindow(){
	// 自身で循環させる
	this->nextITWindow = this;
	this->beforeITWindow = this;
	// メンバの初期化
	this->windowDependentResourceListEdge.Initialize();
}


/// <summary>
/// 作成
/// </summary>
/// <param name="_className">ウィンドウクラス名</param>
/// <param name="_title">タイトル</param>
/// <param name="_style">スタイル</param>
/// <param name="_exStyle">EXスタイル</param>
/// <param name="_x">x座標</param>
/// <param name="_y">y座標</param>
/// <param name="_width">幅</param>
/// <param name="_height">高さ</param>
/// <param name="_parent">親ウィンドウのハンドル[親なしの場合はNULL]</param>
/// <param name="_menu">メニューハンドル</param>
/// <param name="_hInstance">アプリケーションインスタンスハンドル</param>
/// <param name="_param">特殊データ用</param>
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
	// 矩形の設定
	RECT rect = {0, 0, _width, _height};
	// 指定されたクライアント領域確保の為に必要な座標を計算する
	AdjustWindowRectEx(&rect, _style, FALSE, _exStyle);
	long w = rect.right - rect.left;
	long h = rect.bottom - rect.top;

	// メンバ変数の更新
	this->style = _style;
	this->exStyle = _exStyle;
	this->width = _width;
	this->height = _height;

	// ウィンドウ生成
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
		// 作成失敗
		throw Common::Exception::FunctionFailed("Couldn't create window.");
	}
}

/// <summary>
/// 表示
/// </summary>
/// <param name="nCmdShow">初期表示方法</param>
void Window::Show(int nCmdShow){
	ShowWindow(this->handle, nCmdShow);
}

/// <summary>
/// 終了する
/// </summary>
void Window::Exit() const {
	PostMessageA(this->handle, WM_CLOSE, 0U, 0L);
}


/// <summary>
/// 依存リソースの追加(1つだけ)
/// </summary>
/// <param name="_dependentResource">追加する依存リソース</param>
void Window::AddDependentResource(DependentResource::Interface* const _dependentResource){
	if(_dependentResource == nullptr){
		throw Ishikawa::Common::Exception::FunctionFailed("You can't add NULL to list of window dependent resource.");
	}

	// 追加するデータをリストから外す
	_dependentResource->RemoveITWindowDependentResourceList();

	DependentResource::Interface* const tail = this->windowDependentResourceListEdge.GetBeforeITWindowDependentResource();
	// 循環リストの一番後ろに追加
	_dependentResource->SetBeforeITWindowDependentResource(tail);
	_dependentResource->SetNextITWindowDependentResource(&(this->windowDependentResourceListEdge));
	tail->SetNextITWindowDependentResource(_dependentResource);
	this->windowDependentResourceListEdge.SetBeforeITWindowDependentResource(_dependentResource);

	// 依存ウィンドウを設定して通知
	_dependentResource->SetDependentITWindow(this);
	_dependentResource->OnWindowLoaded();
}

/// <summary>
/// 依存リソースの追加(循環リストごと)
/// 追加した後は元リストは空になります
/// </summary>
/// <param name="_dependentResourceList">追加する依存リソースの循環リストの端</param>
void Window::AddDependentResourceList(DependentResource::ListEdge* const _dependentResourceListEdge){
	DependentResource::Interface* const headToBeAdded = _dependentResourceListEdge->GetNextITWindowDependentResource();
	// 追加するものがないので何もせず終わる
	if(headToBeAdded->IsITWindowDependentResourceListEdge()) return;
	DependentResource::Interface* const tailToBeAdded = _dependentResourceListEdge->GetBeforeITWindowDependentResource();

	// 追加リストから中身を切り離す
	_dependentResourceListEdge->SetNextITWindowDependentResource(_dependentResourceListEdge);
	_dependentResourceListEdge->SetBeforeITWindowDependentResource(_dependentResourceListEdge);

	DependentResource::Interface* const tail = this->windowDependentResourceListEdge.GetBeforeITWindowDependentResource();
	// 切り離した中身を循環リストの一番後ろに追加
	headToBeAdded->SetBeforeITWindowDependentResource(tail);
	tailToBeAdded->SetNextITWindowDependentResource(&(this->windowDependentResourceListEdge));
	tail->SetNextITWindowDependentResource(headToBeAdded);
	this->windowDependentResourceListEdge.SetBeforeITWindowDependentResource(tailToBeAdded);

	// 依存ウィンドウを設定して通知
	DependentResource::Interface* resource = headToBeAdded;
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->SetDependentITWindow(this);
		resource->OnWindowLoaded();

		resource = resource->GetNextITWindowDependentResource();
	}
}


/// <summary>
/// 閉じる
/// </summary>
void Window::Close(){
	// 通知
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnWindowClosed();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// 中断
/// </summary>
void Window::Suspend(){
	this->SetSuspendFlag(true);
	// 通知
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnWindowSuspended();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// 再開
/// </summary>
void Window::Resume(){
	this->SetSuspendFlag(false);
	// 通知
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnWindowResumed();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// サイズ変更
/// </summary>
/// <param name="_width">幅</param>
/// <param name="_height">高さ</param>
void Window::ChangeSize(long _width, long _height){
	this->width = _width;
	this->height = _height;

	// 通知
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnWindowSizeChanged();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// 活性化
/// </summary>
void Window::Activate(){
	// 通知
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnWindowActivated();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// 非活性化
/// </summary>
void Window::Deactivate(){
	// 通知
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnWindowDeactivated();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// 最小化
/// </summary>
void Window::Minimize(){
	this->SetMinimizeFlag(true);
	// 通知
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnWindowMinimized();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// 最小化解除
/// </summary>
void Window::CancelOfMinimize(){
	this->SetMinimizeFlag(false);
	// 通知
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnCancelOfWindowMinimized();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// 最大化
/// </summary>
void Window::Maximize(){
	this->SetMaximizeFlag(true);
	// 通知
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnWindowMaximized();
		resource = resource->GetNextITWindowDependentResource();
	}
}

/// <summary>
/// 最大化解除
/// </summary>
void Window::CancelOfMaximize(){
	this->SetMaximizeFlag(false);
	// 通知
	DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
	while(!(resource->IsITWindowDependentResourceListEdge())){
		resource->OnCancelOfWindowMaximized();
		resource = resource->GetNextITWindowDependentResource();
	}
}


/// <summary>
/// プロシージャ
/// </summary>
/// <param name="_message">メッセージ</param>
/// <param name="_wParam">メッセージの付加情報</param>
/// <param name="_lParam">メッセージの付加情報</param>
/// <returns>true:呼び出しもとでTRUEを返す false:デフォルトウィンドウプロシージャへ</returns>
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
		// キャンセル処理を先にする
		if((_wParam != SIZE_MINIMIZED) && this->IsMinimizing()){
			this->CancelOfMinimize();
		}
		if((_wParam != SIZE_MAXIMIZED) && this->IsMaximizing()){
			this->CancelOfMaximize();
			// 最大化前のサイズに変更
			this->ChangeSize(this->widthOfBeforeMaximize, this->heightOfBeforeMaximize);
		}

		if(_wParam == SIZE_MINIMIZED){
			this->Minimize();
		}
		else if(_wParam == SIZE_MAXIMIZED){
			// 最大化前の幅と高さを保存
			this->widthOfBeforeMaximize = this->width;
			this->heightOfBeforeMaximize = this->height;
			// 最大化
			this->Maximize();
			// サイズ変更
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
			// 何もしない
			break;
		}
		break;
	}

	case WM_SYSKEYDOWN:
	{
		if((_wParam == VK_RETURN) && ((_lParam & 0x60000000) == 0x20000000)){
			// Alt + Enter でフルスクリーンのトグルとする
			if(this->IsFullscreen()){
				// ウィンドウモードに変更
				SetWindowLongPtrW(this->handle, GWL_STYLE, static_cast<long>(this->style));
				SetWindowLongPtrW(this->handle, GWL_EXSTYLE, static_cast<long>(this->exStyle));

				ShowWindow(this->handle, SW_NORMAL);

				// 矩形の設定
				RECT rect = {0, 0, this->width, this->height};
				// 指定されたクライアント領域確保の為に必要な座標を計算する
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
				// フルスクリーンモードに変更
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

			// トグルする　"this->windowFlag ^= WINDOW_FULLSCREEN_FLAG"でもいい
			this->SetFullscreenFlag(!(this->IsFullscreen()));
		}
	}

	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP:
	{
		// キーボードステート変更通知
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
		// キーボードステート変更通知
		DependentResource::Interface* resource = this->windowDependentResourceListEdge.GetNextITWindowDependentResource();
		while(!(resource->IsITWindowDependentResourceListEdge())){
			resource->OnChangedMouseState(_message, _wParam, _lParam);
			resource = resource->GetNextITWindowDependentResource();
		}
		break;
	}

	default:
		// 何もしない
		break;
	}

	// デフォルトウィンドウプロシージャに投げる
	return false;
}
