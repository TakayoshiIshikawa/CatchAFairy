//=============================================================================
//	EnemyFairyController.cpp
//
//	敵妖精コントローラのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "EnemyFairyController.h"
#include "Ishikawa/Common/Random.h"
#include "EnemyFairyConstance.h"
#include "EnemyFairies.h"
#include "PointLight.h"
#include "Player.h"

using namespace ::CatchAFairy::Object::PlayScene;


/// <summary>チェックポイントの範囲</summary>
const float EnemyFairyController::CHECKPOINT_RANGE = 0.2f;
/// <summary>敵妖精定数配列</summary>
const EnemyFairyConstance EnemyFairyController::ENEMY_FAIRY_CONSTANCE[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)] = {
	EnemyFairyConstance(
		1.5f,
		1.0f,
		0.6f,
		::Ishikawa::Math::Float3(0.0f, 0.1f, 0.0f),
		::Ishikawa::Math::Float4(0.7f, 1.0f, 0.7f, 0.7f),
		0.05f,
		0.3f,
		::Ishikawa::Math::Float4(0.7f, 1.0f, 0.7f, 0.5f),
		0.01f,
		0.15f,
		0.2f,
		1U
	),
	EnemyFairyConstance(
		2.5f,
		1.5f,
		0.6f,
		::Ishikawa::Math::Float3(0.0f, 0.1f, 0.0f),
		::Ishikawa::Math::Float4(1.0f, 1.0f, 0.6f, 0.7f),
		0.05f,
		0.3f,
		::Ishikawa::Math::Float4(1.0f, 1.0f, 0.6f, 0.5f),
		0.01f,
		0.15f,
		0.2f,
		1U
	),
	EnemyFairyConstance(
		3.0f,
		2.3f,
		0.6f,
		::Ishikawa::Math::Float3(0.0f, 0.1f, 0.0f),
		::Ishikawa::Math::Float4(0.6f, 0.9f, 1.0f, 0.7f),
		0.05f,
		0.3f,
		::Ishikawa::Math::Float4(0.6f, 0.9f, 1.0f, 0.6f),
		0.01f,
		0.15f,
		0.2f,
		1U
	)
};


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
EnemyFairyController::EnemyFairyController() :
	::CatchAFairy::Object::UpdateInterface(),
	CONSTANCE(nullptr),
	id(0U),
	world(nullptr),
	target(::Ishikawa::Math::Float4::ZERO),
	enemyFairies(nullptr),
	pointLight(nullptr),
	manaLightGenerationTimer(0.0f),
	currentUpdateFunction(nullptr)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyFairyController::~EnemyFairyController(){
	// 何もしない
}


/// <summary>
/// 種類の設定
/// </summary>
/// <param name="_type">種類</param>
void EnemyFairyController::SetType(CatchAFairy::Data::EnemyFairyType _type){
	this->type = _type;
	if(this->type == CatchAFairy::Data::EnemyFairyType::Count){
		this->type = CatchAFairy::Data::EnemyFairyType::Normal;
	}

	this->CONSTANCE = &(EnemyFairyController::ENEMY_FAIRY_CONSTANCE[static_cast<unsigned int>(this->type)]);
}

/// <summary>
/// ポイントライトの設定
/// </summary>
/// <param name="_pointLight">ポイントライト</param>
void EnemyFairyController::SetPointLight(PointLight* const _pointLight){
	this->pointLight = _pointLight;
	this->ChangePointLightSetting();
}


/// <summary>
/// ポイントライト設定の変更
/// </summary>
void EnemyFairyController::ChangePointLightSetting(){
	if((this->pointLight != nullptr) && (this->world != nullptr) && (this->CONSTANCE != nullptr)){
		this->pointLight->SetFairyBodyLightPosition(
			this->id,
			this->world->_41 + this->CONSTANCE->FAIRY_LIGHT_ANCHOR.x,
			this->world->_42 + this->CONSTANCE->FAIRY_LIGHT_ANCHOR.y,
			this->world->_43 + this->CONSTANCE->FAIRY_LIGHT_ANCHOR.z
		);
		this->pointLight->SetFairyBodyLightColor(
			this->id,
			this->CONSTANCE->FAIRY_LIGHT_COLOR.x,
			this->CONSTANCE->FAIRY_LIGHT_COLOR.y,
			this->CONSTANCE->FAIRY_LIGHT_COLOR.z,
			this->CONSTANCE->FAIRY_LIGHT_COLOR.w
		);
		this->pointLight->SetFairyBodyLightRadius(this->id, this->CONSTANCE->FAIRY_LIGHT_RADIUS);
		this->pointLight->SetFairyBodyLightLumen(this->id, this->CONSTANCE->FAIRY_LIGHT_LUMEN);

		this->pointLight->SetFairyManaLightColor(
			this->id,
			this->CONSTANCE->MANA_LIGHT_COLOR.x,
			this->CONSTANCE->MANA_LIGHT_COLOR.y,
			this->CONSTANCE->MANA_LIGHT_COLOR.z,
			this->CONSTANCE->MANA_LIGHT_COLOR.w
		);
		this->pointLight->SetFairyManaLightRadius(this->id, this->CONSTANCE->MANA_LIGHT_RADIUS);
		this->pointLight->SetFairyManaLightLumen(this->id, this->CONSTANCE->MANA_LIGHT_LUMEN);
	}
}


/// <summary>
/// リセット
/// </summary>
void EnemyFairyController::Reset(){
	if(this->world != nullptr) this->world->row4 = SelectStartPoint();
	this->ChangeTarget();
	this->manaLightGenerationTimer = 0.0f;
	this->currentUpdateFunction = &EnemyFairyController::UpdateOfMoveToTargetCheckpoint;
	this->ChangePointLightSetting();
}

/// <summary>
/// 更新
/// </summary>
/// <param name="_elapsedTime">更新間隔</param>
void EnemyFairyController::Update(float _elapsedTime){
	if(this->currentUpdateFunction != nullptr) (this->*currentUpdateFunction)(_elapsedTime);

	this->UpdatePointLight(_elapsedTime);
}


/// <summary>
/// 開始地点の選択
/// </summary>
/// <returns>開始地点</returns>
::Ishikawa::Math::Float4 EnemyFairyController::SelectStartPoint(){
	::Ishikawa::Math::Float3 playerPosition;

	if(this->enemyFairies != nullptr){
		const Player* player_ = this->enemyFairies->GetPlayer();
		if(player_ != nullptr){
			const ::Ishikawa::Math::Matrix& playerMatrix = player_->GetWorld();
			playerPosition.x = playerMatrix._41;
			playerPosition.y = playerMatrix._42;
			playerPosition.z = playerMatrix._43;
		}
	}

	::Ishikawa::Math::Float3 delta;
	const float MAX_DISTANCE = 8.0f;
	const float MIN_DISTANCE = 5.0f;
	do{
		delta.x = MAX_DISTANCE * (2.0f*static_cast<float>(::Ishikawa::Common::Random::RealRandom()) - 1.0f);
		delta.y = 0.5f + 1.5f*static_cast<float>(::Ishikawa::Common::Random::RealRandom());
		delta.z = MAX_DISTANCE * (2.0f*static_cast<float>(::Ishikawa::Common::Random::RealRandom()) - 1.0f);
	}while(delta.LengthSquared() < (MIN_DISTANCE*MIN_DISTANCE));

	return ::Ishikawa::Math::Float4(playerPosition + delta, 1.0f);
}

/// <summary>
/// ポイントライトの更新
/// </summary>
/// <param name="_elapsedTime">更新間隔</param>
void EnemyFairyController::UpdatePointLight(float _elapsedTime){
	if(this->pointLight == nullptr) return;
	if(this->world == nullptr) return;

	// 本体ライトの移動
	this->pointLight->SetFairyBodyLightPosition(
		this->id,
		this->world->_41 + this->CONSTANCE->FAIRY_LIGHT_ANCHOR.x,
		this->world->_42 + this->CONSTANCE->FAIRY_LIGHT_ANCHOR.y,
		this->world->_43 + this->CONSTANCE->FAIRY_LIGHT_ANCHOR.z
	);

	this->manaLightGenerationTimer += _elapsedTime;
	if(this->manaLightGenerationTimer >= this->CONSTANCE->MANA_LIGHT_GENERATION_INTERVAL){
		this->manaLightGenerationTimer -= this->CONSTANCE->MANA_LIGHT_GENERATION_INTERVAL;

		// 新規マナライト生成
		for(unsigned int i=0U; i<this->CONSTANCE->MANA_LIGHT_GENERATION_ONCE_COUNT; ++i){
			this->pointLight->GenerateFairyManaLight(
				this->id,
				this->world->_41 + this->CONSTANCE->FAIRY_LIGHT_ANCHOR.x,
				this->world->_42 + this->CONSTANCE->FAIRY_LIGHT_ANCHOR.y,
				this->world->_43 + this->CONSTANCE->FAIRY_LIGHT_ANCHOR.z
			);
		}
	}
}

/// <summary>
/// 休憩する
/// </summary>
/// <param name="_elapsedTime">更新間隔</param>
void EnemyFairyController::UpdateOfRest(float _elapsedTime){
	NOT_USING(_elapsedTime);
}

/// <summary>
/// 目的地に移動する
/// </summary>
/// <param name="_elapsedTime">更新間隔</param>
void EnemyFairyController::UpdateOfMoveToTargetCheckpoint(float _elapsedTime){
	if(this->world == nullptr) return;

	// 妖精から見た目的地
	::Ishikawa::Math::Float4 targetSeenFromTheFairy = this->target * this->world->Invert();
	// 目的地への距離
	::Ishikawa::Math::Float3 targetToCheckpoint(targetSeenFromTheFairy.x, targetSeenFromTheFairy.y, targetSeenFromTheFairy.z);

	// 目的地に到達していたらチェックポイントを変更する
	if(targetToCheckpoint.LengthSquared() < (CHECKPOINT_RANGE * CHECKPOINT_RANGE)){
		this->ChangeTarget();
		return;
	}

	// 移動
	::Ishikawa::Math::Matrix moveMatrix = ::Ishikawa::Math::Matrix::IDENTITY;
	{
		::Ishikawa::Math::Float2 moveVector(targetToCheckpoint.y, targetToCheckpoint.z);
		float speedRate = moveVector.LengthSquared();
		speedRate = (speedRate > 1.0f) ? (1.0f) : (speedRate);
		float speed = speedRate * this->CONSTANCE->MAX_MOVE_SPEED * _elapsedTime;

		moveVector.Normalize();

		moveMatrix._42 = speed * moveVector.x;
		moveMatrix._43 = speed * moveVector.y;
	}
	// 回転(Y軸)
	::Ishikawa::Math::Matrix rotateMatrix = ::Ishikawa::Math::Matrix::IDENTITY;
	{
		float rotateRate = targetToCheckpoint.x / this->CONSTANCE->MAX_SPEED_ROTATION_ANGLE_COS;
		rotateRate = (rotateRate > 1.0f) ? (1.0f) : ((rotateRate < -1.0f) ? (-1.0f) : (rotateRate));
		if(targetToCheckpoint.z < 0.0f){
			rotateRate = (targetToCheckpoint.x < 0.0f) ? (-1.0f) : (1.0f);
		}
		float rotateSpeed = rotateRate * this->CONSTANCE->MAX_ROTATE_SPEED * _elapsedTime;

		rotateMatrix = ::Ishikawa::Math::CreateRotationMatrixOfAxisY(rotateSpeed);
	}

	// 変換をかける
	(*(this->world)) = rotateMatrix * moveMatrix * (*(this->world));
}


/// <summary>
/// 目的地を変更
/// </summary>
void EnemyFairyController::ChangeTarget(){
	const float MAX_DISTANCE = 8.0f;
	float x=0.0f, y=0.0f, z=0.0f;
	x = MAX_DISTANCE * (2.0f*static_cast<float>(::Ishikawa::Common::Random::RealRandom()) - 1.0f);
	y = 0.5f + 1.5f*static_cast<float>(::Ishikawa::Common::Random::RealRandom());
	z = MAX_DISTANCE * (2.0f*static_cast<float>(::Ishikawa::Common::Random::RealRandom()) - 1.0f);

	this->target = ::Ishikawa::Math::Float4(x, y, z, 1.0f);
}

/// <summary>
/// プレイヤを探す
/// </summary>
/// <param name="_playerWorld">プレイヤのワールド行列格納先</param>
/// <returns>true:探せた false:探せなかった</returns>
bool EnemyFairyController::SearchPlayer(::Ishikawa::Math::Matrix* const _playerWorld){
	NOT_USING(_playerWorld);
	return false;
}
