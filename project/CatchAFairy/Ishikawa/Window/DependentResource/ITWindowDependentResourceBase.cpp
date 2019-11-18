//=============================================================================
//	ITWindowDependentResourceBase.cpp
//
//	����E�B���h�E�ˑ����\�[�X�̊��̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITWindowDependentResourceBase.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"

using namespace Ishikawa::Window::DependentResource;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Base::Base() :
	dependentITWindow(nullptr),
	nextITWindowDependentResource(nullptr),
	beforeITWindowDependentResource(nullptr)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Base::~Base(){
	this->RemoveITWindowDependentResourceList();
}


/// <summary>
/// ���X�g����O���
/// </summary>
void Base::RemoveITWindowDependentResourceList(){
	// �������Ă���z���X�g����O���
	Interface* const next = this->nextITWindowDependentResource;
	Interface* const before = this->beforeITWindowDependentResource;
	next->SetBeforeITWindowDependentResource(before);
	before->SetNextITWindowDependentResource(next);
	// ���g�ŏz������
	this->nextITWindowDependentResource = this;
	this->beforeITWindowDependentResource = this;
}


/// <summary>
/// ����E�B���h�E�ˑ����\�[�X�̏�����
/// </summary>
void Base::InitializeITWindowDependentResource(){
	// ���g�ŏz������
	this->nextITWindowDependentResource = this;
	this->beforeITWindowDependentResource = this;
}
