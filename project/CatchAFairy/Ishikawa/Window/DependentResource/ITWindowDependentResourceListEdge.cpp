//=============================================================================
//	ITWindowDependentResourceListEdge.cpp
//
//	����E�B���h�E�ˑ����\�[�X�̃��X�g�[�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITWindowDependentResourceListEdge.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"

using namespace Ishikawa::Window::DependentResource;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
ListEdge::ListEdge() :
	nextITWindowDependentResource(nullptr),
	beforeITWindowDependentResource(nullptr)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ListEdge::~ListEdge(){
	// �������Ȃ�
}


/// <summary>
/// ������
/// </summary>
void ListEdge::Initialize(){
	this->InitializeITWindowDependentResource();
}

/// <summary>
/// ����E�B���h�E�ˑ����\�[�X�̏�����
/// </summary>
void ListEdge::InitializeITWindowDependentResource(){
	// ���g�ŏz������
	this->nextITWindowDependentResource = this;
	this->beforeITWindowDependentResource = this;
}
