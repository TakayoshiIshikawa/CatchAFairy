//=============================================================================
//	TimeView.cpp
//
//	���ԕ\���̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "TimeView.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "CatchAFairy/Scene/ScenePlay.h"
#include "GUIManager.h"
#include "Numbers.h"

using namespace ::CatchAFairy::Object::PlayScene::GUI;

/// <summary>�ő�J�E���g</summary>
const unsigned int TimeView::MAX_COUNT = 120U;
/// <summary>�\���ʒu�z��</summary>
const ::Ishikawa::Math::Float2 TimeView::POSITION[VIEW_DIGIT] = {
	::Ishikawa::Math::Float2(420.0f, 575.0f),
	::Ishikawa::Math::Float2(400.0f, 575.0f),
	::Ishikawa::Math::Float2(380.0f, 575.0f)
};
/// <summary>�\���F</summary>
const ::Ishikawa::Math::Float3 TimeView::COLOR(1.0f, 1.0f, 1.0f);
/// <summary>�傫��</summary>
const ::Ishikawa::Math::Float2 TimeView::SIZE(40.0f, 40.0f);


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
TimeView::TimeView() :
	GUIInterface(),
	scenePlay(nullptr),
	manager(nullptr),
	useVertices(nullptr),
	currentTime(static_cast<float>(MAX_COUNT))
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TimeView::~TimeView(){
	// �������Ȃ�
}


/// <summary>
/// �}�l�[�W���̐ݒ�
/// </summary>
/// <param name="_manager">GUI�}�l�[�W��</param>
void TimeView::SetManager(
	::CatchAFairy::Object::PlayScene::GUI::GUIManager* const _manager
){
	this->manager = _manager;

	if(_manager == nullptr){
		this->SetVertices(nullptr, 0U);
	}
	else{
		this->SetVertices(
			this->manager->GetGUIVertices(
				::CatchAFairy::Data::GUIVerticesSetting::TIME_VIEW_START_INDEX,
				::CatchAFairy::Data::GUIVerticesSetting::TIME_VIEW_INDEX_COUNT
			),
			VIEW_DIGIT
		);
	}
}

/// <summary>
/// ���_�̐ݒ�
/// </summary>
/// <param name="_vertices">���_�z��</param>
/// <param name="_count">���_��</param>
void TimeView::SetVertices(
	::CatchAFairy::Shader::User::PlayGUI::VSInput* const _vertices,
	unsigned int _count
){
	this->useVertices = _vertices;

	if(this->useVertices != nullptr){
		// �ݒ菉����
		for(unsigned int i=0U; i<_count; ++i){
			this->useVertices[i].position = POSITION[i];
			this->useVertices[i].color = ::Ishikawa::Math::Float4(COLOR, 1.0f);
			this->useVertices[i].shapeTextureCoordinate = ::Ishikawa::Math::Float4::ZERO;
			this->useVertices[i].viewTextureCoordinate = Numbers::GetViewTextureCoordinate();
			this->useVertices[i].size = SIZE;
		}
	}
}


/// <summary>
/// �X�V
/// </summary>
/// <param name="_elapsedTime">�X�V�Ԋu</param>
void TimeView::Update(float _elapsedTime){
	if(this->useVertices == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("useVertices is NULL.");
	}
	
	if(this->currentTime > 0.0f){
		this->currentTime -= _elapsedTime;
	}
	else{
		this->currentTime = 0.0f;
		if(this->scenePlay != nullptr){
			this->scenePlay->OnEnd();
		}
	}

	{
		int iTime = static_cast<int>(this->currentTime);

		for(unsigned int i=0U; i<VIEW_DIGIT; ++i){
//			this->useVertices[i].position = POSITION[i];
//			this->useVertices[i].color = ::Ishikawa::Math::Float4(COLOR, 1.0f);
			this->useVertices[i].shapeTextureCoordinate = Numbers::GetShapeTextureCoordinate(iTime % 10);
//			this->useVertices[i].viewTextureCoordinate = Numbers::GetViewTextureCoordinate();
//			this->useVertices[i].size = SIZE;

			iTime /= 10;
		}
	}
}
