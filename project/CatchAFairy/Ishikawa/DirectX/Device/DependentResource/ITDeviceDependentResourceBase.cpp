//=============================================================================
//	ITDeviceDependentResourceBase.cpp
//
//	����f�o�C�X�ˑ����\�[�X�̊��̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITDeviceDependentResourceBase.h"

using namespace Ishikawa::DirectX::Device::DependentResource;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Base::Base() :
	dependentDevice(nullptr),
	nextITDeviceDependentResource(nullptr),
	beforeITDeviceDependentResource(nullptr)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Base::~Base(){
	this->RemoveITDeviceDependentResourceList();
}


/// <summary>
/// ���X�g����O���
/// </summary>
void Base::RemoveITDeviceDependentResourceList(){
	// �������Ă���z���X�g����O���
	Interface* const next = this->nextITDeviceDependentResource;
	Interface* const before = this->beforeITDeviceDependentResource;
	next->SetBeforeITDeviceDependentResource(before);
	before->SetNextITDeviceDependentResource(next);
	// ���g�ŏz������
	this->nextITDeviceDependentResource = this;
	this->beforeITDeviceDependentResource = this;
}


/// <summary>
/// ����f�o�C�X�ˑ����\�[�X�̏�����
/// </summary>
void Base::InitializeITDeviceDependentResource(){
	// ���g�ŏz������
	this->nextITDeviceDependentResource = this;
	this->beforeITDeviceDependentResource = this;
}
