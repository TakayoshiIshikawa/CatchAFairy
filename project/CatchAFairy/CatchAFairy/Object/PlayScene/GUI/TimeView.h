//=============================================================================
//	TimeView.h
//
//	時間表示のヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __TIME_VIEW_H__
#define __TIME_VIEW_H__

#include "GUIObjectInterface.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "CatchAFairy/Data/GUIVerticesSetting.h"

namespace CatchAFairy{
	namespace Scene{
		class Play;
	}

	namespace Object{
		namespace PlayScene{
			namespace GUI{
				/// <summary>
				/// 時間表示
				/// </summary>
				class TimeView final : public GUIInterface{
				private:
					/// <summary>表示桁数</summary>
					static const unsigned int VIEW_DIGIT = ::CatchAFairy::Data::GUIVerticesSetting::TIME_VIEW_INDEX_COUNT;
					// 最大カウント
					static const unsigned int MAX_COUNT;
					// 表示位置配列
					static const ::Ishikawa::Math::Float2 POSITION[VIEW_DIGIT];
					// 表示色
					static const ::Ishikawa::Math::Float3 COLOR;
					// 大きさ
					static const ::Ishikawa::Math::Float2 SIZE;

				private:
					/// <summary>プレイシーン</summary>
					::CatchAFairy::Scene::Play* scenePlay;
					/// <summary>GUIマネージャ</summary>
					::CatchAFairy::Object::PlayScene::GUI::GUIManager* manager;
					/// <summary>利用頂点</summary>
					::CatchAFairy::Shader::User::PlayGUI::VSInput* useVertices;
					/// <summary>経過時間</summary>
					float currentTime;


				public:
					// デフォルトコンストラクタ
					TimeView();
					// デストラクタ
					virtual ~TimeView() override final;

				public:
					/// <summary>プレイシーンの設定</summary>
					/// <param name="_scenePlay">プレイシーン</param>
					void SetScenePlay(::CatchAFairy::Scene::Play* const _scenePlay){
						this->scenePlay = _scenePlay;
					}
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
					// 更新
					virtual void Update(float _elapsedTime) override final;
				};
			}
		}
	}
}

#endif
