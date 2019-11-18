//=============================================================================
//	OperationMethodView.cpp
//
//	妖精位置表示のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "OperationMethodView.h"
#include <math.h>
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "CatchAFairy/Data/GUIVerticesSetting.h"
#include "GUIManager.h"

using namespace ::CatchAFairy::Object::PlayScene::GUI;

/// <summary>位置</summary>
const ::Ishikawa::Math::Float2 OperationMethodView::POSITION(400.0f, 80.0f);
/// <summary>表示色</summary>
const ::Ishikawa::Math::Float3 OperationMethodView::COLOR(0.5f, 1.0f, 0.3f);
/// <summary>形状テクスチャコーディネート(左上と右下)</summary>
const ::Ishikawa::Math::Float4 OperationMethodView::SHAPE_TEXTURE_COORDINATE(0.75f, 0.0f, 1.0f, 0.125f);
/// <summary>表示テクスチャコーディネート(左上と右下)</summary>
const ::Ishikawa::Math::Float4 OperationMethodView::VIEW_TEXTURE_COORDINATE(0.0f, 0.0f, 1.0f/32.0f, 1.0f/32.0f);
/// <summary>大きさ</summary>
const ::Ishikawa::Math::Float2 OperationMethodView::SIZE(300.0f, 150.0f);
/// <summary>表示までの時間(一番初めのみ)</summary>
const float OperationMethodView::FIRST_CAST_TIME_VIEWING = 5.0f;
/// <summary>表示までの時間</summary>
const float OperationMethodView::CAST_TIME_VIEWING = 10.0f;
/// <summary>完全に表示されるまでの時間</summary>
const float OperationMethodView::START_VIEWING_TIME = 1.0f;
/// <summary>表示中の間隔</summary>
const float OperationMethodView::VIEWING_INTERVAL = 1.5f;
/// <summary>完全に消えるまでの時間</summary>
const float OperationMethodView::END_VIEWING_TIME = 1.0f;
/// <summary>表示するときの弱いアルファ値</summary>
const float OperationMethodView::WEAK_ALPHA = 0.5f;


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
OperationMethodView::OperationMethodView() :
	GUIInterface(),
	manager(nullptr),
	useVertex(nullptr),
	castTime(FIRST_CAST_TIME_VIEWING),
	viewValue(0.0f)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
OperationMethodView::~OperationMethodView(){
	// 何もしない
}


/// <summary>
/// マネージャの設定
/// </summary>
/// <param name="_manager">GUIマネージャ</param>
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
/// 頂点の設定
/// </summary>
/// <param name="_vertices">頂点配列</param>
/// <param name="_count">頂点数</param>
void OperationMethodView::SetVertices(
	::CatchAFairy::Shader::User::PlayGUI::VSInput* const _vertices,
	unsigned int _count
){
	NOT_USING(_count);

	this->useVertex = _vertices;

	if(this->useVertex != nullptr){
		// 設定初期化
		this->useVertex->position = POSITION;
		this->useVertex->color = ::Ishikawa::Math::Float4(COLOR, 0.0f);
		this->useVertex->shapeTextureCoordinate = SHAPE_TEXTURE_COORDINATE;
		this->useVertex->viewTextureCoordinate = VIEW_TEXTURE_COORDINATE;
		this->useVertex->size = SIZE;
	}
}


/// <summary>
/// 更新
/// </summary>
/// <param name="_elapsedTime">更新間隔</param>
void OperationMethodView::Update(float _elapsedTime){
	if(this->useVertex == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("useVertex is NULL.");
	}

	float alpha = 0.0f;

	static const float PI = 3.1415926535f;
	static const float PI2 = 2.0f * PI;

	if(this->castTime > 0.0f){
		this->castTime -= _elapsedTime;

		if(this->viewValue < 1.0f){
			static const float TIME_RATE = 1.0f / VIEWING_INTERVAL;

			// 表示終了
			this->viewValue -= TIME_RATE * _elapsedTime;
			if(this->viewValue < 0.0f) this->viewValue = 0.0f;
			alpha = 0.5f*(1.0f - cosf(PI*this->viewValue));
		}
		else{
			static const float TIME_RATE = 1.0f / VIEWING_INTERVAL;

			// ループ終了まではそのまま動く
			this->viewValue += TIME_RATE * _elapsedTime;
			if(this->viewValue > 2.0f) this->viewValue = 0.9999f;

			float rate = 0.5f*(1.0f + cosf(PI2*(this->viewValue-1.0f)));
			alpha = WEAK_ALPHA + (1.0f-WEAK_ALPHA)*rate;
		}
	}
	else{
		if(this->viewValue < 1.0f){
			static const float TIME_RATE = 1.0f / START_VIEWING_TIME;

			// 表示開始
			this->viewValue += TIME_RATE * _elapsedTime;
			alpha = 0.5f*(1.0f - cosf(PI*this->viewValue));
		}
		else{
			static const float TIME_RATE = 1.0f / VIEWING_INTERVAL;

			// ループ
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
