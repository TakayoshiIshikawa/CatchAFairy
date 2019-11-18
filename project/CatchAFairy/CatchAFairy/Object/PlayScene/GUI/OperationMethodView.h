//=============================================================================
//	OperationMethodView.h
//
//	操作方法表示のヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __OPERATION_METHOD_VIEW_H__
#define __OPERATION_METHOD_VIEW_H__

#include "GUIObjectInterface.h"
#include "Ishikawa/Math/ITFloatN.h"

namespace CatchAFairy{
	namespace Object{
		namespace PlayScene{
			namespace GUI{
				/// <summary>
				/// 操作方法表示
				/// </summary>
				class OperationMethodView final : public GUIInterface{
				private:
					// 位置
					static const ::Ishikawa::Math::Float2 POSITION;
					// 表示色
					static const ::Ishikawa::Math::Float3 COLOR;
					// 形状テクスチャコーディネート(左上と右下)
					static const ::Ishikawa::Math::Float4 SHAPE_TEXTURE_COORDINATE;
					// 表示テクスチャコーディネート(左上と右下)
					static const ::Ishikawa::Math::Float4 VIEW_TEXTURE_COORDINATE;
					// 大きさ
					static const ::Ishikawa::Math::Float2 SIZE;
					// 表示までの時間(一番初めのみ)
					static const float FIRST_CAST_TIME_VIEWING;
					// 表示までの時間
					static const float CAST_TIME_VIEWING;
					// 完全に表示されるまでの時間
					static const float START_VIEWING_TIME;
					// 表示中の間隔
					static const float VIEWING_INTERVAL;
					// 完全に消えるまでの時間
					static const float END_VIEWING_TIME;
					// 表示するときの弱いアルファ値
					static const float WEAK_ALPHA;

				private:
					/// <summary>GUIマネージャ</summary>
					::CatchAFairy::Object::PlayScene::GUI::GUIManager* manager;
					/// <summary>利用頂点</summary>
					::CatchAFairy::Shader::User::PlayGUI::VSInput* useVertex;
					/// <summary>表示までの時間</summary>
					float castTime;
					/// <summary>表示用の値 [0.0～1.0:移行状態 1.0～:ループ]</summary>
					float viewValue;


				public:
					// デフォルトコンストラクタ
					OperationMethodView();
					// デストラクタ
					virtual ~OperationMethodView() override final;


				public:
					// マネージャの設定
					virtual void SetManager(
						::CatchAFairy::Object::PlayScene::GUI::GUIManager* const _manager
					) override final;

				protected:
					// 頂点の設定
					virtual void SetVertices(
						::CatchAFairy::Shader::User::PlayGUI::VSInput* const _vertices,
						unsigned int _count
					) override final;

				public:
					/// <summary>プレイヤが行動した</summary>
					void OnActPlayer(){
						this->castTime = CAST_TIME_VIEWING;
					}

				public:
					// 更新
					virtual void Update(float _elapsedTime) override final;
				};
			}
		}
	}
}

#endif
