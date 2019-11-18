//=============================================================================
//	EnemyFairyConstance.cpp
//
//	敵妖精定数のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "EnemyFairyConstance.h"

using namespace ::CatchAFairy::Object::PlayScene;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="_MAX_MOVE_SPEED">最高移動速度</param>
/// <param name="_MAX_ROTATE_SPEED">最高回転速度</param>
/// <param name="_MAX_SPEED_ROTATION_ANGLE_COS">最高回転する角度のコサイン値</param>
/// <param name="_FAIRY_LIGHT_ANCHOR">本体光の基点</param>
/// <param name="_FAIRY_LIGHT_COLOR">本体光の色</param>
/// <param name="_FAIRY_LIGHT_RADIUS">本体光の半径</param>
/// <param name="_FAIRY_LIGHT_LUMEN">本体光の光度</param>
/// <param name="_MANA_LIGHT_COLOR">マナライトの色</param>
/// <param name="_MANA_LIGHT_RADIUS">マナライトの半径</param>
/// <param name="_MANA_LIGHT_LUMEN">マナライトの光度</param>
/// <param name="_MANA_LIGHT_GENERATION_INTERVAL">マナライト生成間隔</param>
/// <param name="_MANA_LIGHT_GENERATION_ONCE_COUNT">マナライト生成1回当たりの生成数</param>
EnemyFairyConstance::EnemyFairyConstance(
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
) :
	MAX_MOVE_SPEED(_MAX_MOVE_SPEED),
	MAX_ROTATE_SPEED(_MAX_ROTATE_SPEED),
	MAX_SPEED_ROTATION_ANGLE_COS(_MAX_SPEED_ROTATION_ANGLE_COS),
	FAIRY_LIGHT_ANCHOR(_FAIRY_LIGHT_ANCHOR),
	FAIRY_LIGHT_COLOR(_FAIRY_LIGHT_COLOR),
	FAIRY_LIGHT_RADIUS(_FAIRY_LIGHT_RADIUS),
	FAIRY_LIGHT_LUMEN(_FAIRY_LIGHT_LUMEN),
	MANA_LIGHT_COLOR(_MANA_LIGHT_COLOR),
	MANA_LIGHT_RADIUS(_MANA_LIGHT_RADIUS),
	MANA_LIGHT_LUMEN(_MANA_LIGHT_LUMEN),
	MANA_LIGHT_GENERATION_INTERVAL(_MANA_LIGHT_GENERATION_INTERVAL),
	MANA_LIGHT_GENERATION_ONCE_COUNT(_MANA_LIGHT_GENERATION_ONCE_COUNT)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyFairyConstance::~EnemyFairyConstance(){
	// 何もしない
}
