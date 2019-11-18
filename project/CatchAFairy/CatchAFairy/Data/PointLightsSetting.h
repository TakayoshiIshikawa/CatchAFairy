//=============================================================================
//	PointLightsSetting.h
//
//	ポイントライト設定データのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __POINT_LIGHTS_SETTING_H__
#define __POINT_LIGHTS_SETTING_H__

namespace CatchAFairy{
	namespace Data{
		/// <summary>
		/// ポイントライト設定データ
		/// </summary>
		namespace PointLightsSetting{
			/// <summary>妖精1体の本体につくライトの数</summary>
			static const unsigned int ONE_FAIRY_BODY_LIGHT_COUNT = 2U;
			/// <summary>妖精1体から出るマナライトの数</summary>
			static const unsigned int ONE_FAIRY_MANA_LIGHT_COUNT = 8U;
			/// <summary>妖精1体あたりのライトの総数</summary>
			static const unsigned int ONE_FAIRY_LIGHT_ALL_COUNT = ONE_FAIRY_BODY_LIGHT_COUNT + ONE_FAIRY_MANA_LIGHT_COUNT;
			/// <summary>プレイヤ妖精の数</summary>
			static const unsigned int PLAYER_FAIRY_COUNT = 1U;
			/// <summary>敵妖精の数</summary>
			static const unsigned int ENEMY_FAIRY_COUNT = 5U;
			/// <summary>妖精の総数</summary>
			static const unsigned int FAIRY_COUNT = PLAYER_FAIRY_COUNT + ENEMY_FAIRY_COUNT;

			/// <summary>ライトの最大数</summary>
			static const unsigned int MAX_COUNT = FAIRY_COUNT * ONE_FAIRY_LIGHT_ALL_COUNT;
		}
	}
}

#endif
