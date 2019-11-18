//=============================================================================
//	ITKeyboard.cpp
//
//	����L�[�{�[�h�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITKeyboard.h"
#include <string.h>
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#include <windows.h>
#pragma warning(default:4820)
#pragma warning(default:4668)
#include "Ishikawa/Common/Exception/ITExceptions.h"

using namespace Ishikawa::Input;


/// <summary>
/// W�p�����[�^����L�[�R�[�h�ɕϊ�
/// </summary>
/// <param name="_wParam">W�p�����[�^</param>
/// <returns>�L�[�R�[�h</returns>
KeyCode Keyboard::ConvertWPalamToKeyCode(unsigned int _wParam){
	switch(_wParam){
	case VK_ESCAPE:
		return KeyCode::Escape;

	case VK_RETURN:
		return KeyCode::Enter;

	case VK_SPACE:
		return KeyCode::Space;

	case 'Z':
		return KeyCode::Z;

	case 'X':
		return KeyCode::X;

	case 'C':
		return KeyCode::C;

	case 'A':
		return KeyCode::A;

	case 'S':
		return KeyCode::S;

	case 'D':
		return KeyCode::D;

	case 'Q':
		return KeyCode::Q;

	case 'W':
		return KeyCode::W;

	case 'E':
		return KeyCode::E;

	case VK_SHIFT:
		return KeyCode::Shift;

	case VK_LEFT:
		return KeyCode::LeftArrow;

	case VK_RIGHT:
		return KeyCode::RightArrow;

	case VK_UP:
		return KeyCode::UpArrow;

	case VK_DOWN:
		return KeyCode::DownArrow;

	default:
		return KeyCode::None;
	};
}


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Keyboard::Keyboard() :
	beforeState(),
	currentState()
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Keyboard::~Keyboard(){
	// �������Ȃ�
}


/// <summary>
/// �L�[��������Ă��邩?
/// </summary>
/// <param name="_key">�L�[</param>
/// <returns>true:������Ă��� false:������Ă��Ȃ�</returns>
bool Keyboard::IsPressing(KeyCode _key) const {
	return this->currentState.GetState(_key);
}

/// <summary>
/// �L�[��������Ă��邩?
/// </summary>
/// <param name="_key">�L�[</param>
/// <returns>true:������Ă��� false:������Ă��Ȃ�</returns>
bool Keyboard::IsReleasing(KeyCode _key) const {
	return (!(this->currentState.GetState(_key)));
}

/// <summary>
/// �L�[�������ꂽ��?
/// </summary>
/// <param name="_key">�L�[</param>
/// <returns>true:�����ꂽ false:������Ă��Ȃ�</returns>
bool Keyboard::IsPressed(KeyCode _key) const {
	return (
		(this->currentState.GetState(_key)) &&
		(!(this->beforeState.GetState(_key)))
	);
}

/// <summary>
/// �L�[�������ꂽ��?
/// </summary>
/// <param name="_key">�L�[</param>
/// <returns>true:�����ꂽ false:������Ă��Ȃ�</returns>
bool Keyboard::IsReleased(KeyCode _key) const {
	return (
		(!(this->currentState.GetState(_key))) &&
		(this->beforeState.GetState(_key))
	);
}


/// <summary>
/// ��Ԃ��ύX����
/// </summary>
/// <param name="_message">���b�Z�[�W</param>
/// <param name="_wParam">���b�Z�[�W�̕t�����</param>
/// <param name="_lParam">���b�Z�[�W�̕t�����</param>
void Keyboard::OnChangedState(unsigned int _message, unsigned int _wParam, long _lParam){
	NOT_USING(_lParam);

	switch(_message){
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		KeyCode key = ConvertWPalamToKeyCode(_wParam);
		if(key < KeyCode::SingleKeyCount){
			this->currentState.Pressed(key);
		}
		break;
	}

	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		KeyCode key = ConvertWPalamToKeyCode(_wParam);
		if(key < KeyCode::SingleKeyCount){
			this->currentState.Released(key);
		}
		break;
	}

	default:
		// �������Ȃ�
		break;
	};
}


/// <summary>
/// �X�V
/// </summary>
void Keyboard::Update(){
	this->beforeState = this->currentState;
}



/// <summary>
/// �L�[�����Ԃ��擾���邽�߂̃C���f�b�N�X�ƃ}�X�N�f�[�^�ɕϊ�
/// </summary>
/// <param name="_index">�C���f�b�N�X</param>
/// <param name="_mask">�}�X�N�f�[�^</param>
/// <param name="_key">�P�̃L�[</param>
void Keyboard::KeyState::ConvertKeyToKeyIndexAndMask(
	unsigned int* const _index,
	StateType* const _mask,
	KeyCode _key
){
	if(_index == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_index is NULL.");
	}
	if(_mask == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_index is NULL.");
	}

	if(_key >= KeyCode::SingleKeyCount){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Can't convert compound key code to index and mask data.");
	}

	unsigned int index = static_cast<unsigned int>(_key) / STATE_TYPE_BIT_COUNT;
	unsigned int maskBit = static_cast<unsigned int>(_key) % STATE_TYPE_BIT_COUNT;

	(*_index) = index;
	(*_mask) = (1UL << maskBit);
}


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Keyboard::KeyState::KeyState() :
	state{0}
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Keyboard::KeyState::~KeyState(){
	// �������Ȃ�
}


/// <summary>
/// ������Z�q
/// </summary>
/// <param name="_original">�I���W�i���f�[�^</param>
/// <returns>���g�̃f�[�^</returns>
Keyboard::KeyState& Keyboard::KeyState::operator=(const KeyState& _original){
	// ��Ԃ��R�s�[
	memcpy(this->state, _original.state, sizeof(_original.state));

	return (*this);
}


/// <summary>
/// �L�[��Ԃ̎擾
/// </summary>
/// <param name="_key">�L�[</param>
/// <returns>true:������Ă��� false:������Ă��Ȃ�</returns>
bool Keyboard::KeyState::GetState(KeyCode _key) const {
	if(_key < KeyCode::SingleKeyCount){
		// �P�̃L�[�R�[�h
		unsigned int index = 0U;
		StateType mask = 0UL;
		ConvertKeyToKeyIndexAndMask(&index, &mask, _key);

		return ((this->state[index] & mask) != 0);
	}
	else{
		// �����L�[�R�[�h�͒P�̃L�[�R�[�h�̑g�ݍ��킹�ŏ�������
		return false;
	}
}

/// <summary>
/// �L�[�������ꂽ
/// </summary>
/// <param name="_key">�L�[</param>
void Keyboard::KeyState::Pressed(KeyCode _key){
	if(_key >= KeyCode::SingleKeyCount){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Compound keys can not be pressed.");
	}

	unsigned int index = 0U;
	StateType mask = 0UL;
	ConvertKeyToKeyIndexAndMask(&index, &mask, _key);

	// �Ή�����r�b�g�𗧂Ă�
	this->state[index] |= mask;
}

/// <summary>
/// �L�[�������ꂽ
/// </summary>
/// <param name="_key">�L�[</param>
void Keyboard::KeyState::Released(KeyCode _key){
	if(_key >= KeyCode::SingleKeyCount){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Compound keys can not be pressed.");
	}

	unsigned int index = 0U;
	StateType mask = 0UL;
	ConvertKeyToKeyIndexAndMask(&index, &mask, _key);

	// �Ή�����r�b�g���~�낷
	this->state[index] &= (~mask);
}
