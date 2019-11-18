//=============================================================================
//	FairyPositionView.h
//
//	妖精位置表示のヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __FAIRY_POSITION_VIEW_H__
#define __FAIRY_POSITION_VIEW_H__

#include "GUIObjectInterface.h"
#include "Ishikawa/Math/ITFloatN.h"

namespace CatchAFairy{
	namespace Object{
		namespace PlayScene{
			class EnemyFairies;
			class Player;

			namespace GUI{
				/// <summary>
				/// 妖精位置表示
				/// </summary>
				class FairyPositionView final : public GUIInterface{
				private:
					// 表示色
					static const ::Ishikawa::Math::Float3 COLOR;
					// 形状テクスチャコーディネート(左上と右下)
					static const ::Ishikawa::Math::Float4 SHAPE_TEXTURE_COORDINATE;
					// 表示テクスチャコーディネート(左上と右下)
					static const ::Ishikawa::Math::Float4 VIEW_TEXTURE_COORDINATE;
					// 大きさ
					static const ::Ishikawa::Math::Float2 SIZE;

				private:
					/// <summary>GUIマネージャ</summary>
					::CatchAFairy::Object::PlayScene::GUI::GUIManager* manager;
					/// <summary>利用頂点</summary>
					::CatchAFairy::Shader::User::PlayGUI::VSInput* useVertex;
					/// <summary>敵妖精</summary>
					const ::CatchAFairy::Object::PlayScene::EnemyFairies* enemyFairies;
					/// <summary>プレイヤ</summary>
					const ::CatchAFairy::Object::PlayScene::Player* player;


				public:
					// デフォルトコンストラクタ
					FairyPositionView();
					// デストラクタ
					virtual ~FairyPositionView() override final;


				public:
					// マネージャの設定
					virtual void SetManager(
						::CatchAFairy::Object::PlayScene::GUI::GUIManager* const _manager
					) override final;
					/// <summary>妖精の設定</summary>
					/// <param name="_enemyFairies">敵妖精</param>
					void SetFairy(const ::CatchAFairy::Object::PlayScene::EnemyFairies* const _enemyFairies){
						this->enemyFairies = _enemyFairies;
					}
					/// <summary>プレイヤの設定</summary>
					/// <param name="_player">プレイヤ</param>
					void SetPlayer(const ::CatchAFairy::Object::PlayScene::Player* const _player){
						this->player = _player;
					}

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
