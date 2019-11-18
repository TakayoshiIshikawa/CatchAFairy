//=============================================================================
//	ITWindowClass.cpp
//
//	����E�B���h�E�N���X�̃\�[�X�t�@�C��
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
/// �f�t�H���g�R���X�g���N�^
/// </summary>
WindowClass::WindowClass() :
	name(nullptr)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
WindowClass::~WindowClass(){
	// �������Ȃ�
}


/// <summary>
/// �쐬
/// </summary>
/// <param name="_name">���O</param>
/// <param name="_menuName">���j���[���\�[�X��</param>
/// <param name="_style">�X�^�C��</param>
/// <param name="_classExtra">���̍\���̂Ɋ��蓖�Ă�]���ȃ�����(0�ŏ����������)[�Ƃ肠����0�ɂ��Ă����Ƃ���]</param>
/// <param name="_windowExtra">�E�B���h�E�C���X�^���X�Ɋ��蓖�Ă���]���ȃ�����(0�ŏ����������)</param>
/// <param name="_hInstance">�A�v���P�[�V�����C���X�^���X�n���h��</param>
/// <param name="_icon">�A�C�R���̃n���h��</param>
/// <param name="_smallIcon">�X���[���A�C�R���̃n���h��</param>
/// <param name="_cursor">�J�[�\���̃n���h��</param>
/// <param name="_backgroundBrush">�w�i�u���V�̃n���h��</param>
/// <param name="_procedure">�E�B���h�E�v���V�[�W���ւ̊֐��|�C���^</param>
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

	// �o�^
	if(!RegisterClassExW(&windowClass)){
		// �o�^���s
		throw Common::Exception::FunctionFailed("Couldn't register window class.");
	}
}
