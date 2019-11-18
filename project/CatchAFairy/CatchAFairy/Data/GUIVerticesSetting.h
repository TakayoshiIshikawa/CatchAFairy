//=============================================================================
//	GUIVerticesSetting.h
//
//	GUI頂点設定データのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __GUI_VERTICES_SETTING_H__
#define __GUI_VERTICES_SETTING_H__

#include "PointLightsSetting.h"

namespace CatchAFairy{
	namespace Data{
		/// <summary>
		/// GUI頂点設定データ
		/// </summary>
		namespace GUIVerticesSetting{
			/// <summary>妖精位置表示用インデックスの先頭</summary>
			const unsigned int FAIRY_START_POSITION_VIEW_INDEX = 0U;
			/// <summary>妖精位置表示用インデックス数</summary>
			const unsigned int FAIRY_POSITION_VIEW_INDEX_COUNT = PointLightsSetting::ENEMY_FAIRY_COUNT;
			/// <summary>時間表示用インデックスの先頭</summary>
			const unsigned int TIME_VIEW_START_INDEX = FAIRY_POSITION_VIEW_INDEX_COUNT;
			/// <summary>時間表示用インデックス数 (表示桁数)</summary>
			const unsigned int TIME_VIEW_INDEX_COUNT = 3U;
			/// <summary>操作方法表示用インデックス</summary>
			const unsigned int OPERATION_METHOD_VIEW_INDEX = TIME_VIEW_START_INDEX + TIME_VIEW_INDEX_COUNT;
			/// <summary>GUI全頂点数</summary>
			const unsigned int ALL_VERTEX_COUNT = OPERATION_METHOD_VIEW_INDEX + 1U;
		}
	}
}

#endif
