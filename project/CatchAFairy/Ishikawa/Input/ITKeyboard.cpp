//=============================================================================
//	ITKeyboard.cpp
//
//	自作キーボードのソースファイル
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
/// Wパラメータからキーコードに変換
/// </summary>
/// <param name="_wParam">Wパラメータ</param>
/// <returns>キーコード</returns>
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
/// デフォルトコンストラクタ
/// </summary>
Keyboard::Keyboard() :
	beforeState(),
	currentState()
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Keyboard::~Keyboard(){
	// 何もしない
}


/// <summary>
/// キーが押されているか?
/// </summary>
/// <param name="_key">キー</param>
/// <returns>true:押されている false:押されていない</returns>
bool Keyboard::IsPressing(KeyCode _key) const {
	return this->currentState.GetState(_key);
}

/// <summary>
/// キーが離されているか?
/// </summary>
/// <param name="_key">キー</param>
/// <returns>true:離されている false:離されていない</returns>
bool Keyboard::IsReleasing(KeyCode _key) const {
	return (!(this->currentState.GetState(_key)));
}

/// <summary>
/// キーが押されたか?
/// </summary>
/// <param name="_key">キー</param>
/// <returns>true:押された false:押されていない</returns>
bool Keyboard::IsPressed(KeyCode _key) const {
	return (
		(this->currentState.GetState(_key)) &&
		(!(this->beforeState.GetState(_key)))
	);
}

/// <summary>
/// キーが離されたか?
/// </summary>
/// <param name="_key">キー</param>
/// <returns>true:離された false:離されていない</returns>
bool Keyboard::IsReleased(KeyCode _key) const {
	return (
		(!(this->currentState.GetState(_key))) &&
		(this->beforeState.GetState(_key))
	);
}


/// <summary>
/// 状態が変更した
/// </summary>
/// <param name="_message">メッセージ</param>
/// <param name="_wParam">メッセージの付加情報</param>
/// <param name="_lParam">メッセージの付加情報</param>
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
		// 何もしない
		break;
	};
}


/// <summary>
/// 更新
/// </summary>
void Keyboard::Update(){
	this->beforeState = this->currentState;
}



/// <summary>
/// キーから状態を取得するためのインデックスとマスクデータに変換
/// </summary>
/// <param name="_index">インデックス</param>
/// <param name="_mask">マスクデータ</param>
/// <param name="_key">単体キー</param>
void Keyboard::KeyState::ConvertKeyToKeyIndexAndMask(
	unsigned int* const _index,
	StateType* const _mask,
	KeyCode _key
){
	if(_index == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_index is NULL.");
	}
	if(_mask == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_index is NULL.");
	}

	if(_key >= KeyCode::SingleKeyCount){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Can't convert compound key code to index and mask data.");
	}

	unsigned int index = static_cast<unsigned int>(_key) / STATE_TYPE_BIT_COUNT;
	unsigned int maskBit = static_cast<unsigned int>(_key) % STATE_TYPE_BIT_COUNT;

	(*_index) = index;
	(*_mask) = (1UL << maskBit);
}


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
Keyboard::KeyState::KeyState() :
	state{0}
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Keyboard::KeyState::~KeyState(){
	// 何もしない
}


/// <summary>
/// 代入演算子
/// </summary>
/// <param name="_original">オリジナルデータ</param>
/// <returns>自身のデータ</returns>
Keyboard::KeyState& Keyboard::KeyState::operator=(const KeyState& _original){
	// 状態をコピー
	memcpy(this->state, _original.state, sizeof(_original.state));

	return (*this);
}


/// <summary>
/// キー状態の取得
/// </summary>
/// <param name="_key">キー</param>
/// <returns>true:押されている false:押されていない</returns>
bool Keyboard::KeyState::GetState(KeyCode _key) const {
	if(_key < KeyCode::SingleKeyCount){
		// 単体キーコード
		unsigned int index = 0U;
		StateType mask = 0UL;
		ConvertKeyToKeyIndexAndMask(&index, &mask, _key);

		return ((this->state[index] & mask) != 0);
	}
	else{
		// 複合キーコードは単体キーコードの組み合わせで処理する
		return false;
	}
}

/// <summary>
/// キーが押された
/// </summary>
/// <param name="_key">キー</param>
void Keyboard::KeyState::Pressed(KeyCode _key){
	if(_key >= KeyCode::SingleKeyCount){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Compound keys can not be pressed.");
	}

	unsigned int index = 0U;
	StateType mask = 0UL;
	ConvertKeyToKeyIndexAndMask(&index, &mask, _key);

	// 対応するビットを立てる
	this->state[index] |= mask;
}

/// <summary>
/// キーが離された
/// </summary>
/// <param name="_key">キー</param>
void Keyboard::KeyState::Released(KeyCode _key){
	if(_key >= KeyCode::SingleKeyCount){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Compound keys can not be pressed.");
	}

	unsigned int index = 0U;
	StateType mask = 0UL;
	ConvertKeyToKeyIndexAndMask(&index, &mask, _key);

	// 対応するビットを降ろす
	this->state[index] &= (~mask);
}
