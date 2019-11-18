//=============================================================================
//	ITDeviceDependentResourceListEdge.cpp
//
//	����f�o�C�X�ˑ����\�[�X�̃��X�g�[�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITDeviceDependentResourceListEdge.h"

using namespace Ishikawa::DirectX::Device::DependentResource;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
ListEdge::ListEdge() :
	nextITDeviceDependentResource(nullptr),
	beforeITDeviceDependentResource(nullptr)
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
	this->InitializeITDeviceDependentResource();
}

/// <summary>
/// ����f�o�C�X�ˑ����\�[�X�̏�����
/// </summary>
void ListEdge::InitializeITDeviceDependentResource(){
	// ���g�ŏz������
	this->nextITDeviceDependentResource = this;
	this->beforeITDeviceDependentResource = this;
}
