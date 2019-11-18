//=============================================================================
//	ITWindowListEdge.cpp
//
//	����E�B���h�E�̃��X�g�[�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITWindowListEdge.h"

using namespace Ishikawa::Window;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
ListEdge::ListEdge() :
	nextITWindow(nullptr),
	beforeITWindow(nullptr)
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
	this->InitializeITWindow();
}

/// <summary>
/// ����E�B���h�E�̏�����
/// </summary>
void ListEdge::InitializeITWindow(){
	// ���g�ŏz������
	this->nextITWindow = this;
	this->beforeITWindow = this;
}
