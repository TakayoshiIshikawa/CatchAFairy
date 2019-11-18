//=============================================================================
//	FairyPositionView.cpp
//
//	妖精位置表示のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "FairyPositionView.h"
#include <float.h>
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "CatchAFairy/Data/GUIVerticesSetting.h"
#include "GUIManager.h"
#include "CatchAFairy/Object/PlayScene/EnemyFairies.h"
#include "CatchAFairy/Object/PlayScene/Player.h"

using namespace ::CatchAFairy::Object::PlayScene::GUI;

/// <summary>表示色</summary>
const ::Ishikawa::Math::Float3 FairyPositionView::COLOR(1.0f, 1.0f, 1.0f);
/// <summary>形状テクスチャコーディネート(左上と右下)</summary>
const ::Ishikawa::Math::Float4 FairyPositionView::SHAPE_TEXTURE_COORDINATE(0.5f, 0.0f, 0.625f, 0.125f);
/// <summary>表示テクスチャコーディネート(左上と右下)</summary>
const ::Ishikawa::Math::Float4 FairyPositionView::VIEW_TEXTURE_COORDINATE(0.0f, 0.0f, 1.0f/32.0f, 1.0f/32.0f);
/// <summary>大きさ</summary>
const ::Ishikawa::Math::Float2 FairyPositionView::SIZE(30.0f, 30.0f);


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
FairyPositionView::FairyPositionView() :
	GUIInterface(),
	manager(nullptr),
	useVertex(nullptr),
	enemyFairies(nullptr),
	player(nullptr)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
FairyPositionView::~FairyPositionView(){
	// 何もしない
}


/// <summary>
/// マネージャの設定
/// </summary>
/// <param name="_manager">GUIマネージャ</param>
void FairyPositionView::SetManager(
	::CatchAFairy::Object::PlayScene::GUI::GUIManager* const _manager
){
	this->manager = _manager;

	if(_manager == nullptr){
		this->SetVertices(nullptr, 0U);
	}
	else{
		this->SetVertices(
			this->manager->GetGUIVertices(
				::CatchAFairy::Data::GUIVerticesSetting::FAIRY_START_POSITION_VIEW_INDEX,
				::CatchAFairy::Data::GUIVerticesSetting::FAIRY_POSITION_VIEW_INDEX_COUNT
			),
			::CatchAFairy::Data::GUIVerticesSetting::FAIRY_POSITION_VIEW_INDEX_COUNT
		);
	}
}

/// <summary>
/// 頂点の設定
/// </summary>
/// <param name="_vertices">頂点配列</param>
/// <param name="_count">頂点数</param>
void FairyPositionView::SetVertices(
	::CatchAFairy::Shader::User::PlayGUI::VSInput* const _vertices,
	unsigned int _count
){
	this->useVertex = _vertices;

	if(this->useVertex != nullptr){
		// 設定初期化
		for(unsigned int i=0U; i<_count; ++i){
			this->useVertex[i].position = ::Ishikawa::Math::Float2::ZERO;
			this->useVertex[i].color = ::Ishikawa::Math::Float4(COLOR, 0.0f);
			this->useVertex[i].shapeTextureCoordinate = SHAPE_TEXTURE_COORDINATE;
			this->useVertex[i].viewTextureCoordinate = VIEW_TEXTURE_COORDINATE;
			this->useVertex[i].size = SIZE;
		}
	}
}


/// <summary>
/// 更新
/// </summary>
/// <param name="_elapsedTime">更新間隔</param>
void FairyPositionView::Update(float _elapsedTime){
	NOT_USING(_elapsedTime);

	if(this->useVertex == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("useVertex is NULL.");
	}
	if(this->enemyFairies == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("fairy is NULL.");
	}
	if(this->player == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("player is NULL.");
	}

	const unsigned int ENEMY_FAIRY_COUNT = this->enemyFairies->GetWorldCount();
	const ::Ishikawa::Math::Matrix* const ENEMY_FAIRY_WORLD = this->enemyFairies->GetWorldArray();

	for(unsigned int i=0U; i<ENEMY_FAIRY_COUNT; ++i){
		::Ishikawa::Math::Matrix distance = ENEMY_FAIRY_WORLD[i] * this->player->GetWorld().Invert();
		::Ishikawa::Math::Float2 direction(distance._41, distance._43);
		{
			float div = 10000000.0f;
			if((direction.x*direction.x) > (direction.y*direction.y)){
				if(direction.x > 0.0f)	div = direction.x;
				else					div = -(direction.x);
			}
			else{
				if(direction.y > 0.0f)	div = direction.y;
				else					div = -(direction.y);
			}

			if(div > FLT_EPSILON) direction *= 1.0f / div;
		}
		float alpha = 1.0f;
		if(direction.y > 0.9f){
			alpha = direction.x * direction.x;
			alpha = alpha * alpha;
		}


		this->useVertex[i].position = ::Ishikawa::Math::Float2(
			0.5f*(::GUIManager::OUTPUT_SIZE.x + (::GUIManager::OUTPUT_SIZE.x-SIZE.x)*direction.x),
			0.5f*(::GUIManager::OUTPUT_SIZE.y + (::GUIManager::OUTPUT_SIZE.y-SIZE.y)*direction.y)
		);
		this->useVertex[i].color = ::Ishikawa::Math::Float4(COLOR, alpha);
//		this->useVertex[i].shapeTextureCoordinate = SHAPE_TEXTURE_COORDINATE;
//		this->useVertex[i].viewTextureCoordinate = VIEW_TEXTURE_COORDINATE;
//		this->useVertex[i].size = SIZE;
	}
}
