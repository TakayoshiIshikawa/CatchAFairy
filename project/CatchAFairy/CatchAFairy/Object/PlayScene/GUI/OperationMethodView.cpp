//=============================================================================
//	OperationMethodView.cpp
//
//	�d���ʒu�\���̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "OperationMethodView.h"
#include <math.h>
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "CatchAFairy/Data/GUIVerticesSetting.h"
#include "GUIManager.h"

using namespace ::CatchAFairy::Object::PlayScene::GUI;

/// <summary>�ʒu</summary>
const ::Ishikawa::Math::Float2 OperationMethodView::POSITION(400.0f, 80.0f);
/// <summary>�\���F</summary>
const ::Ishikawa::Math::Float3 OperationMethodView::COLOR(0.5f, 1.0f, 0.3f);
/// <summary>�`��e�N�X�`���R�[�f�B�l�[�g(����ƉE��)</summary>
const ::Ishikawa::Math::Float4 OperationMethodView::SHAPE_TEXTURE_COORDINATE(0.75f, 0.0f, 1.0f, 0.125f);
/// <summary>�\���e�N�X�`���R�[�f�B�l�[�g(����ƉE��)</summary>
const ::Ishikawa::Math::Float4 OperationMethodView::VIEW_TEXTURE_COORDINATE(0.0f, 0.0f, 1.0f/32.0f, 1.0f/32.0f);
/// <summary>�傫��</summary>
const ::Ishikawa::Math::Float2 OperationMethodView::SIZE(300.0f, 150.0f);
/// <summary>�\���܂ł̎���(��ԏ��߂̂�)</summary>
const float OperationMethodView::FIRST_CAST_TIME_VIEWING = 5.0f;
/// <summary>�\���܂ł̎���</summary>
const float OperationMethodView::CAST_TIME_VIEWING = 10.0f;
/// <summary>���S�ɕ\�������܂ł̎���</summary>
const float OperationMethodView::START_VIEWING_TIME = 1.0f;
/// <summary>�\�����̊Ԋu</summary>
const float OperationMethodView::VIEWING_INTERVAL = 1.5f;
/// <summary>���S�ɏ�����܂ł̎���</summary>
const float OperationMethodView::END_VIEWING_TIME = 1.0f;
/// <summary>�\������Ƃ��̎ア�A���t�@�l</summary>
const float OperationMethodView::WEAK_ALPHA = 0.5f;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
OperationMethodView::OperationMethodView() :
	GUIInterface(),
	manager(nullptr),
	useVertex(nullptr),
	castTime(FIRST_CAST_TIME_VIEWING),
	viewValue(0.0f)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
OperationMethodView::~OperationMethodView(){
	// �������Ȃ�
}


/// <summary>
/// �}�l�[�W���̐ݒ�
/// </summary>
/// <param name="_manager">GUI�}�l�[�W��</param>
void OperationMethodView::SetManager(
	::CatchAFairy::Object::PlayScene::GUI::GUIManager* const _manager
){
	this->manager = _manager;

	if(_manager == nullptr){
		this->SetVertices(nullptr, 0U);
	}
	else{
		this->SetVertices(
			this->manager->GetGUIVertices(
				::CatchAFairy::Data::GUIVerticesSetting::OPERATION_METHOD_VIEW_INDEX,
				1U
			),
			1U
		);
	}
}

/// <summary>
/// ���_�̐ݒ�
/// </summary>
/// <param name="_vertices">���_�z��</param>
/// <param name="_count">���_��</param>
void OperationMethodView::SetVertices(
	::CatchAFairy::Shader::User::PlayGUI::VSInput* const _vertices,
	unsigned int _count
){
	NOT_USING(_count);

	this->useVertex = _vertices;

	if(this->useVertex != nullptr){
		// �ݒ菉����
		this->useVertex->position = POSITION;
		this->useVertex->color = ::Ishikawa::Math::Float4(COLOR, 0.0f);
		this->useVertex->shapeTextureCoordinate = SHAPE_TEXTURE_COORDINATE;
		this->useVertex->viewTextureCoordinate = VIEW_TEXTURE_COORDINATE;
		this->useVertex->size = SIZE;
	}
}


/// <summary>
/// �X�V
/// </summary>
/// <param name="_elapsedTime">�X�V�Ԋu</param>
void OperationMethodView::Update(float _elapsedTime){
	if(this->useVertex == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("useVertex is NULL.");
	}

	float alpha = 0.0f;

	static const float PI = 3.1415926535f;
	static const float PI2 = 2.0f * PI;

	if(this->castTime > 0.0f){
		this->castTime -= _elapsedTime;

		if(this->viewValue < 1.0f){
			static const float TIME_RATE = 1.0f / VIEWING_INTERVAL;

			// �\���I��
			this->viewValue -= TIME_RATE * _elapsedTime;
			if(this->viewValue < 0.0f) this->viewValue = 0.0f;
			alpha = 0.5f*(1.0f - cosf(PI*this->viewValue));
		}
		else{
			static const float TIME_RATE = 1.0f / VIEWING_INTERVAL;

			// ���[�v�I���܂ł͂��̂܂ܓ���
			this->viewValue += TIME_RATE * _elapsedTime;
			if(this->viewValue > 2.0f) this->viewValue = 0.9999f;

			float rate = 0.5f*(1.0f + cosf(PI2*(this->viewValue-1.0f)));
			alpha = WEAK_ALPHA + (1.0f-WEAK_ALPHA)*rate;
		}
	}
	else{
		if(this->viewValue < 1.0f){
			static const float TIME_RATE = 1.0f / START_VIEWING_TIME;

			// �\���J�n
			this->viewValue += TIME_RATE * _elapsedTime;
			alpha = 0.5f*(1.0f - cosf(PI*this->viewValue));
		}
		else{
			static const float TIME_RATE = 1.0f / VIEWING_INTERVAL;

			// ���[�v
			this->viewValue += TIME_RATE * _elapsedTime;
			while(this->viewValue > 2.0f){
				this->viewValue -= 1.0f;
			}

			float rate = 0.5f*(1.0f + cosf(PI2*(this->viewValue-1.0f)));
			alpha = WEAK_ALPHA + (1.0f-WEAK_ALPHA)*rate;
		}
	}

	this->useVertex->position = POSITION;
	this->useVertex->color = ::Ishikawa::Math::Float4(COLOR, alpha);
	this->useVertex->shapeTextureCoordinate = SHAPE_TEXTURE_COORDINATE;
	this->useVertex->viewTextureCoordinate = VIEW_TEXTURE_COORDINATE;
	this->useVertex->size = SIZE;
}
