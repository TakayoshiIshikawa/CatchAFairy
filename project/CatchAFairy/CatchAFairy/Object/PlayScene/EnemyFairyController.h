//=============================================================================
//	EnemyFairyController.h
//
//	敵妖精コントローラのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __ENEMY_FAIRY_CONTROLLER_H__
#define __ENEMY_FAIRY_CONTROLLER_H__

#include "CatchAFairy/Object/UpdateObjectInterface.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "CatchAFairy/Data/EnemyFairyType.h"
#include "EnemyFairyConstance.h"

namespace CatchAFairy{
	namespace Object{
		namespace PlayScene{
			class EnemyFairies;
			class PointLight;
			class EnemyFairyConstance;

			/// <summary>
			/// 敵妖精コントローラ
			/// </summary>
			class EnemyFairyController final :
				public ::CatchAFairy::Object::UpdateInterface
			{
			private:
				/// <summary>
				/// 更新状態
				/// </summary>
				enum class UpdateState{
					/// <summary>休憩</summary>
					Resting,
					/// <summary>目的のチェックポイントに移動する</summary>
					MoveToTargetCheckpoint
				};

			private:
				// チェックポイントの範囲
				static const float CHECKPOINT_RANGE;
				// 敵妖精定数配列
				static const EnemyFairyConstance ENEMY_FAIRY_CONSTANCE[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)];


			private:
				/// <summary>種類</summary>
				CatchAFairy::Data::EnemyFairyType type;
				/// <summary>定数値</summary>
				const EnemyFairyConstance* CONSTANCE;
				/// <summary>妖精識別用ID</summary>
				unsigned int id;
				/// <summary>ワールド行列</summary>
				::Ishikawa::Math::Matrix* world;
				/// <summary>ワールド空間での目的地座標</summary>
				::Ishikawa::Math::Float4 target;
				/// <summary>敵妖精達</summary>
				EnemyFairies* enemyFairies;
				/// <summary>ポイントライト</summary>
				PointLight* pointLight;
				/// <summary>マナライト生成用タイマー</summary>
				float manaLightGenerationTimer;
				/// <summary>現在の更新関数</summary>
				void (EnemyFairyController::*currentUpdateFunction)(float);


			public:
				// デフォルトコンストラクタ
				EnemyFairyController();
				// デストラクタ
				virtual ~EnemyFairyController() override final;

			public:
				/// <summary>種類の取得</summary>
				/// <returns>種類</returns>
				CatchAFairy::Data::EnemyFairyType GetType() const { return this->type; }

			public:
				// 種類の設定
				void SetType(CatchAFairy::Data::EnemyFairyType _type);
				/// <summary>妖精識別用IDの設定</summary>
				/// <param name="_id">妖精識別用ID</param>
				void SetId(unsigned int _id){ this->id = _id; }
				/// <summary>ワールド行列の設定</summary>
				/// <param name="_world">ワールド行列</param>
				void SetWorld(::Ishikawa::Math::Matrix* const _world){ this->world = _world; }
				/// <summary>敵妖精達の設定</summary>
				/// <param name="_enemyFairies">敵妖精達</param>
				void SetEnemyFairies(EnemyFairies* const _enemyFairies){ this->enemyFairies = _enemyFairies; }
				//　ポイントライトの設定
				void SetPointLight(PointLight* const _pointLight);

			private:
				// ポイントライト設定の変更
				void ChangePointLightSetting();

			public:
				// リセット
				void Reset();
				// 更新
				virtual void Update(float _elapsedTime) override final;

			private:
				// 開始地点の選択
				::Ishikawa::Math::Float4 SelectStartPoint();
				// ポイントライトの更新
				void UpdatePointLight(float _elapsedTime);
				// 休憩する
				void UpdateOfRest(float _elapsedTime);
				// 目的地に移動する
				void UpdateOfMoveToTargetCheckpoint(float _elapsedTime);
				// 目的地を変更
				void ChangeTarget();
				// プレイヤを探す
				bool SearchPlayer(::Ishikawa::Math::Matrix* const _playerWorld);
			};
		}
	}
}

#endif
