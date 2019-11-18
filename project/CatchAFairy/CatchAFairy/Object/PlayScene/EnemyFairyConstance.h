//=============================================================================
//	EnemyFairyConstance.h
//
//	敵妖精定数のヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __ENEMY_FAIRY_CONSTANCE_H__
#define __ENEMY_FAIRY_CONSTANCE_H__

#include "Ishikawa/Math/ITFloatN.h"

namespace CatchAFairy{
	namespace Object{
		namespace PlayScene{
			/// <summary>
			/// 敵妖精定数値
			/// </summary>
			class EnemyFairyConstance final {
			public:
				// 最高移動速度
				const float MAX_MOVE_SPEED;
				// 最高回転速度
				const float MAX_ROTATE_SPEED;
				// 最高回転する角度のコサイン値
				const float MAX_SPEED_ROTATION_ANGLE_COS;
				// 本体光の基点
				const ::Ishikawa::Math::Float3 FAIRY_LIGHT_ANCHOR;
				// 本体光の色
				const ::Ishikawa::Math::Float4 FAIRY_LIGHT_COLOR;
				// 本体光の半径
				const float FAIRY_LIGHT_RADIUS;
				// 本体光の光度
				const float FAIRY_LIGHT_LUMEN;
				// マナライトの色
				const ::Ishikawa::Math::Float4 MANA_LIGHT_COLOR;
				// マナライトの半径
				const float MANA_LIGHT_RADIUS;
				// マナライトの光度
				const float MANA_LIGHT_LUMEN;
				// マナライト生成間隔
				const float MANA_LIGHT_GENERATION_INTERVAL;
				// マナライト生成1回当たりの生成数
				const unsigned int MANA_LIGHT_GENERATION_ONCE_COUNT;

			public:
				// コンストラクタ
				EnemyFairyConstance(
					const float _MAX_MOVE_SPEED,
					const float _MAX_ROTATE_SPEED,
					const float _MAX_SPEED_ROTATION_ANGLE_COS,
					const ::Ishikawa::Math::Float3& _FAIRY_LIGHT_ANCHOR,
					const ::Ishikawa::Math::Float4& _FAIRY_LIGHT_COLOR,
					const float _FAIRY_LIGHT_RADIUS,
					const float _FAIRY_LIGHT_LUMEN,
					const ::Ishikawa::Math::Float4& _MANA_LIGHT_COLOR,
					const float _MANA_LIGHT_RADIUS,
					const float _MANA_LIGHT_LUMEN,
					const float _MANA_LIGHT_GENERATION_INTERVAL,
					const unsigned int _MANA_LIGHT_GENERATION_ONCE_COUNT
				);
				// デストラクタ
				~EnemyFairyConstance();
			private:
				/// <summary>代入演算子 [削除]</summary>
				EnemyFairyConstance& operator=(const EnemyFairyConstance&) = delete;
			};
		}
	}
}

#endif
