//=============================================================================
//	ITWindowClass.cpp
//
//	自作ウィンドウクラスのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITWindowClass.h"
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#include <windows.h>
#pragma warning(default:4820)
#pragma warning(default:4668)
#include "Ishikawa/Common/Exception/ITExceptions.h"

using namespace Ishikawa::Window;


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
WindowClass::WindowClass() :
	name(nullptr)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
WindowClass::~WindowClass(){
	// 何もしない
}


/// <summary>
/// 作成
/// </summary>
/// <param name="_name">名前</param>
/// <param name="_menuName">メニューリソース名</param>
/// <param name="_style">スタイル</param>
/// <param name="_classExtra">この構造体に割り当てる余分なメモリ(0で初期化される)[とりあえず0にしておくといい]</param>
/// <param name="_windowExtra">ウィンドウインスタンスに割り当てられる余分なメモリ(0で初期化される)</param>
/// <param name="_hInstance">アプリケーションインスタンスハンドル</param>
/// <param name="_icon">アイコンのハンドル</param>
/// <param name="_smallIcon">スモールアイコンのハンドル</param>
/// <param name="_cursor">カーソルのハンドル</param>
/// <param name="_backgroundBrush">背景ブラシのハンドル</param>
/// <param name="_procedure">ウィンドウプロシージャへの関数ポインタ</param>
void WindowClass::Create(
	const wchar_t* const _name,
	const wchar_t* const _menuName,
	const unsigned long _style,
	const long _classExtra,
	const long _windowExtra,
	HINSTANCE__* const _hInstance,
	HICON__* const _icon,
	HICON__* const _smallIcon,
	HICON__* const _cursor,
	HBRUSH__* const _backgroundBrush,
	ProcedureType _procedure
){
	this->name = _name;

	WNDCLASSEXW windowClass;

	windowClass.cbSize = sizeof(WNDCLASSEXW);
	windowClass.lpszClassName = this->name;
	windowClass.lpszMenuName = _menuName;
	windowClass.style = _style;
	windowClass.cbClsExtra = _classExtra;
	windowClass.cbWndExtra = _windowExtra;
	windowClass.hInstance = _hInstance;
	windowClass.hIcon = _icon;
	windowClass.hIconSm = _smallIcon;
	windowClass.hCursor = _cursor;
	windowClass.hbrBackground = _backgroundBrush;
	windowClass.lpfnWndProc = _procedure;

	// 登録
	if(!RegisterClassExW(&windowClass)){
		// 登録失敗
		throw Common::Exception::FunctionFailed("Couldn't register window class.");
	}
}
