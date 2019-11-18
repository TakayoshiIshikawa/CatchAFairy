//=============================================================================
//	SingletonManager.cpp
//
//	�V���O���g���}�l�[�W���̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "SingletonManager.h"
#include "SingletonBase.h"

using namespace Ishikawa::Common;

/// <summary>�V���O���g���C���X�^���X</summary>
SafePointer<SingletonManager> SingletonManager::instance(nullptr);


/// <summary>
/// �C���X�^���X�̎擾[�V���O���g���}�l�[�W��]
/// </summary>
/// <returns>�V���O���g���}�l�[�W���̃C���X�^���X</returns>
SingletonManager& SingletonManager::GetInstance(){
	if(instance.Get() == nullptr){
		instance.Reset(new SingletonManager());
	}
	return *instance;
}


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
SingletonManager::SingletonManager() :
	lastSingletonInstance(nullptr)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SingletonManager::~SingletonManager(){
	this->Finalize();
}

/// <summary>
/// �V���O���g���C���X�^���X�̒ǉ�
/// </summary>
/// <param name="singleton">�V���O���g���C���X�^���X</param>
void SingletonManager::AddInstance(SingletonBase* const singleton){
	if(singleton == nullptr) return;

	singleton->SetBeforeSingleton(this->lastSingletonInstance);
	this->lastSingletonInstance = singleton;
}

/// <summary>
/// �I������
/// </summary>
void SingletonManager::Finalize(){
	// ���g�ȊO�̑S�V���O���g���C���X�^���X���폜
	while(this->lastSingletonInstance != nullptr){
		SingletonBase* const singletonInstance = this->lastSingletonInstance;
		this->lastSingletonInstance = singletonInstance->GetBeforeSingleton();
		delete singletonInstance;
	}
	this->lastSingletonInstance = nullptr;
}
