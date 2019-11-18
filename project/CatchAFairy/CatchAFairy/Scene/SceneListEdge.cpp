//=============================================================================
//	SceneListEdge.cpp
//
//	�V�[�����X�g�̒[�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "SceneListEdge.h"

using namespace CatchAFairy::Scene;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
ListEdge::ListEdge() :
	nextScene(nullptr),
	beforeScene(nullptr)
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
	this->InitializeScene();
}

/// <summary>
/// �V�[���̏�����
/// </summary>
void ListEdge::InitializeScene(){
	// ���g�ŏz������
	this->nextScene = this;
	this->beforeScene = this;
}
