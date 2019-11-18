//=============================================================================
//	SingletonBase.cpp
//
//	�V���O���g���̊��̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "SingletonBase.h"
#include "SingletonManager.h"

using namespace Ishikawa::Common;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
SingletonBase::SingletonBase() :
	beforeSingleton(nullptr)
{
	// �}�l�[�W���ɒǉ�
	SingletonManager::GetInstance().AddInstance(this);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SingletonBase::~SingletonBase(){
	// �������Ȃ�
}
