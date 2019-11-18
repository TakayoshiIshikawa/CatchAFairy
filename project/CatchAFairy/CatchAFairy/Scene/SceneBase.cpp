//=============================================================================
//	SceneBase.cpp
//
//	�V�[�����̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "SceneBase.h"

using namespace CatchAFairy::Scene;


/// <summary>�X�V�t���O</summary>
const unsigned long Base::SCENE_UPDATE_FLAG	= 0x00000001;
/// <summary>�`��t���O</summary>
const unsigned long Base::SCENE_RENDER_FLAG	= 0x00000002;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Base::Base() :
	nextScene(nullptr),
	beforeScene(nullptr)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Base::~Base(){
	this->RemoveSceneList();
}


/// <summary>
/// �X�V���邩?
/// </summary>
/// <returns>true:�X�V���� false:�X�V���Ȃ�</returns>
bool Base::IsUpdateScene() const{
	return ((this->sceneFlag & SCENE_UPDATE_FLAG) != 0);
}

/// <summary>
/// �`�悷�邩?
/// </summary>
/// <returns>true:�`�悷�� false:�`�悵�Ȃ�</returns>
bool Base::IsRenderScene() const{
	return ((this->sceneFlag & SCENE_RENDER_FLAG) != 0);
}


/// <summary>
/// �X�V�t���O�̐ݒ�
/// </summary>
/// <param name="_flag">�X�V�t���O</param>
void Base::SetUpdateFlag(bool _flag){
	if(_flag)	this->sceneFlag |= SCENE_UPDATE_FLAG;
	else		this->sceneFlag &= (~SCENE_UPDATE_FLAG);
}

/// <summary>
/// �`��t���O�̐ݒ�
/// </summary>
/// <param name="_flag">�`��t���O</param>
void Base::SetRenderFlag(bool _flag){
	if(_flag)	this->sceneFlag |= SCENE_RENDER_FLAG;
	else		this->sceneFlag &= (~SCENE_RENDER_FLAG);
}


/// <summary>
/// ���X�g����O���
/// </summary>
void Base::RemoveSceneList(){
	// �������Ă���z���X�g����O���
	Interface* const next = this->nextScene;
	Interface* const before = this->beforeScene;
	next->SetBeforeScene(before);
	before->SetNextScene(next);
	// ���g�ŏz������
	this->nextScene = this;
	this->beforeScene = this;
}


/// <summary>
/// �V�[���̏�����
/// </summary>
void Base::InitializeScene(){
	// ���g�ŏz������
	this->nextScene = this;
	this->beforeScene = this;
}
