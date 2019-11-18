//=============================================================================
//	TimeView.cpp
//
//	時間表示のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "TimeView.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "CatchAFairy/Scene/ScenePlay.h"
#include "GUIManager.h"
#include "Numbers.h"

using namespace ::CatchAFairy::Object::PlayScene::GUI;

/// <summary>最大カウント</summary>
const unsigned int TimeView::MAX_COUNT = 120U;
/// <summary>表示位置配列</summary>
const ::Ishikawa::Math::Float2 TimeView::POSITION[VIEW_DIGIT] = {
	::Ishikawa::Math::Float2(420.0f, 575.0f),
	::Ishikawa::Math::Float2(400.0f, 575.0f),
	::Ishikawa::Math::Float2(380.0f, 575.0f)
};
/// <summary>表示色</summary>
const ::Ishikawa::Math::Float3 TimeView::COLOR(1.0f, 1.0f, 1.0f);
/// <summary>大きさ</summary>
const ::Ishikawa::Math::Float2 TimeView::SIZE(40.0f, 40.0f);


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
TimeView::TimeView() :
	GUIInterface(),
	scenePlay(nullptr),
	manager(nullptr),
	useVertices(nullptr),
	currentTime(static_cast<float>(MAX_COUNT))
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
TimeView::~TimeView(){
	// 何もしない
}


/// <summary>
/// マネージャの設定
/// </summary>
/// <param name="_manager">GUIマネージャ</param>
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
/// 頂点の設定
/// </summary>
/// <param name="_vertices">頂点配列</param>
/// <param name="_count">頂点数</param>
void TimeView::SetVertices(
	::CatchAFairy::Shader::User::PlayGUI::VSInput* const _vertices,
	unsigned int _count
){
	this->useVertices = _vertices;

	if(this->useVertices != nullptr){
		// 設定初期化
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
/// 更新
/// </summary>
/// <param name="_elapsedTime">更新間隔</param>
void TimeView::Update(float _elapsedTime){
	if(this->useVertices == nullptr){
		// 異常終了
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
