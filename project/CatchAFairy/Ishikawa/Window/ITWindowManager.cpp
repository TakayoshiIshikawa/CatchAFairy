//=============================================================================
//	ITWindowManager.cpp
//
//	����E�B���h�E�}�l�[�W���̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITWindowManager.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Common/PatternBase/SingletonManager.h"

using namespace Ishikawa::Window;

/// <summary>�V���O���g���C���X�^���X</summary>
Manager* Manager::instance = nullptr;


/// <summary>
/// �C���X�^���X�̎擾[����E�B���h�E�}�l�[�W��]
/// </summary>
/// <returns>����E�B���h�E�}�l�[�W���̃C���X�^���X</returns>
template<>
Manager& Ishikawa::Common::SingletonManager::GetInstance<Manager>(){
	if(Manager::instance == nullptr){
		Manager::instance = new Manager();
		(Manager::instance)->Initialize();
	}
	return (*(Manager::instance));
}


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Manager::Manager() :
	windowListEdge()
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Manager::~Manager(){
	// �V���O���g���C���X�^���X�̃��Z�b�g
	instance = nullptr;
}


/// <summary>
/// ������
/// </summary>
void Manager::Initialize(){
	// �����o�̏�����
	this->windowListEdge.Initialize();
}


/// <summary>
/// �E�B���h�E�̒ǉ�
/// </summary>
/// <param name="_window">�ǉ��E�B���h�E</param>
void Manager::AddWindow(Interface* const _window){
	if(_window == nullptr){
		throw Ishikawa::Common::Exception::FunctionFailed("You can't add NULL to list of window.");
	}

	// �ǉ�����f�[�^�����X�g����O��
	_window->RemoveITWindowList();

	Interface* const tail = this->windowListEdge.GetBeforeITWindow();
	// �z���X�g�̈�Ԍ��ɒǉ�
	_window->SetBeforeITWindow(tail);
	_window->SetNextITWindow(&(this->windowListEdge));
	tail->SetNextITWindow(_window);
	this->windowListEdge.SetBeforeITWindow(_window);
}
