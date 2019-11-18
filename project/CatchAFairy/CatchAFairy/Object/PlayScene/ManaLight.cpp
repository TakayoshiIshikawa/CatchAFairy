//=============================================================================
//	ManaLight.cpp
//
//	マナライトのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ManaLight.h"
#include "Ishikawa/Common/Random.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"

using namespace ::CatchAFairy::Object::PlayScene;

/// <summary>加速度</summary>
const ::Ishikawa::Math::Float3 ManaLight::ACCELERATION(0.0f, 0.3f, 0.0f);
/// <summary>左右方向の減速係数</summary>
const float ManaLight::DECELERATION_FACTOR_XZ = 0.98f;
/// <summary>各方向の最大初期化速さ</summary>
const ::Ishikawa::Math::Float3 ManaLight::MAX_INITIALIZE_SPEED(0.3f, 0.2f, 0.3f);


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
ManaLight::ManaLight() :
	::CatchAFairy::Object::UpdateInterface(),
	lightVertex(nullptr),
	velocity(::Ishikawa::Math::Float3::ZERO)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
ManaLight::~ManaLight(){
	// 何もしない
}


/// <summary>
/// 生成
/// </summary>
/// <param name="_position">座標</param>
/// <param name="_color">色</param>
/// <param name="_radiusAndLumen">半径と光度</param>
void ManaLight::Generate(
	const ::Ishikawa::Math::Float3& _position,
	const ::Ishikawa::Math::Float4& _color,
	const ::Ishikawa::Math::Float2& _radiusAndLumen
){
	if(this->lightVertex == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("light vertex is NULL.");
	}

	this->lightVertex->position = _position;
	this->lightVertex->color = _color;
	this->lightVertex->radiusAndLumen = _radiusAndLumen;
	this->RandomizeVelocity();
}

/// <summary>
/// 更新
/// </summary>
/// <param name="_elapsedTime">経過時間</param>
void ManaLight::Update(float _elapsedTime){
	if(this->lightVertex == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("light vertex is NULL.");
	}

	// 移動
	this->lightVertex->position += this->velocity * _elapsedTime;
	this->velocity += ACCELERATION * _elapsedTime;
	// 左右方向には遅くなる
	this->velocity.x *= DECELERATION_FACTOR_XZ;
	this->velocity.y *= DECELERATION_FACTOR_XZ;
	// 時間経過で暗くなる
	this->lightVertex->radiusAndLumen.y *= (1.0f - 0.02f*(60.0f*_elapsedTime));
}


/// <summary>
/// [-1.0f,1.0f)の乱数値を取得
/// </summary>
/// <returns>[-1.0f,1.0f)の乱数値</returns>
inline float ManaLight::Random(){
	return static_cast<float>(2.0 * (::Ishikawa::Common::Random::RealRandom() - 0.5));
}

/// <summary>
/// 速度をランダマイズ
/// </summary>
void ManaLight::RandomizeVelocity(){
	this->velocity.x = MAX_INITIALIZE_SPEED.x * Random();
	this->velocity.y = MAX_INITIALIZE_SPEED.y * Random();
	this->velocity.z = MAX_INITIALIZE_SPEED.z * Random();
}
