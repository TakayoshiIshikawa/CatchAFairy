//=============================================================================
//	EnemyFairyController.cpp
//
//	�G�d���R���g���[���̃\�[�X�t�@�C��
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


/// <summary>�`�F�b�N�|�C���g�͈̔�</summary>
const float EnemyFairyController::CHECKPOINT_RANGE = 0.2f;
/// <summary>�G�d���萔�z��</summary>
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
/// �f�t�H���g�R���X�g���N�^
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
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyFairyController::~EnemyFairyController(){
	// �������Ȃ�
}


/// <summary>
/// ��ނ̐ݒ�
/// </summary>
/// <param name="_type">���</param>
void EnemyFairyController::SetType(CatchAFairy::Data::EnemyFairyType _type){
	this->type = _type;
	if(this->type == CatchAFairy::Data::EnemyFairyType::Count){
		this->type = CatchAFairy::Data::EnemyFairyType::Normal;
	}

	this->CONSTANCE = &(EnemyFairyController::ENEMY_FAIRY_CONSTANCE[static_cast<unsigned int>(this->type)]);
}

/// <summary>
/// �|�C���g���C�g�̐ݒ�
/// </summary>
/// <param name="_pointLight">�|�C���g���C�g</param>
void EnemyFairyController::SetPointLight(PointLight* const _pointLight){
	this->pointLight = _pointLight;
	this->ChangePointLightSetting();
}


/// <summary>
/// �|�C���g���C�g�ݒ�̕ύX
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
/// ���Z�b�g
/// </summary>
void EnemyFairyController::Reset(){
	if(this->world != nullptr) this->world->row4 = SelectStartPoint();
	this->ChangeTarget();
	this->manaLightGenerationTimer = 0.0f;
	this->currentUpdateFunction = &EnemyFairyController::UpdateOfMoveToTargetCheckpoint;
	this->ChangePointLightSetting();
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="_elapsedTime">�X�V�Ԋu</param>
void EnemyFairyController::Update(float _elapsedTime){
	if(this->currentUpdateFunction != nullptr) (this->*currentUpdateFunction)(_elapsedTime);

	this->UpdatePointLight(_elapsedTime);
}


/// <summary>
/// �J�n�n�_�̑I��
/// </summary>
/// <returns>�J�n�n�_</returns>
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
/// �|�C���g���C�g�̍X�V
/// </summary>
/// <param name="_elapsedTime">�X�V�Ԋu</param>
void EnemyFairyController::UpdatePointLight(float _elapsedTime){
	if(this->pointLight == nullptr) return;
	if(this->world == nullptr) return;

	// �{�̃��C�g�̈ړ�
	this->pointLight->SetFairyBodyLightPosition(
		this->id,
		this->world->_41 + this->CONSTANCE->FAIRY_LIGHT_ANCHOR.x,
		this->world->_42 + this->CONSTANCE->FAIRY_LIGHT_ANCHOR.y,
		this->world->_43 + this->CONSTANCE->FAIRY_LIGHT_ANCHOR.z
	);

	this->manaLightGenerationTimer += _elapsedTime;
	if(this->manaLightGenerationTimer >= this->CONSTANCE->MANA_LIGHT_GENERATION_INTERVAL){
		this->manaLightGenerationTimer -= this->CONSTANCE->MANA_LIGHT_GENERATION_INTERVAL;

		// �V�K�}�i���C�g����
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
/// �x�e����
/// </summary>
/// <param name="_elapsedTime">�X�V�Ԋu</param>
void EnemyFairyController::UpdateOfRest(float _elapsedTime){
	NOT_USING(_elapsedTime);
}

/// <summary>
/// �ړI�n�Ɉړ�����
/// </summary>
/// <param name="_elapsedTime">�X�V�Ԋu</param>
void EnemyFairyController::UpdateOfMoveToTargetCheckpoint(float _elapsedTime){
	if(this->world == nullptr) return;

	// �d�����猩���ړI�n
	::Ishikawa::Math::Float4 targetSeenFromTheFairy = this->target * this->world->Invert();
	// �ړI�n�ւ̋���
	::Ishikawa::Math::Float3 targetToCheckpoint(targetSeenFromTheFairy.x, targetSeenFromTheFairy.y, targetSeenFromTheFairy.z);

	// �ړI�n�ɓ��B���Ă�����`�F�b�N�|�C���g��ύX����
	if(targetToCheckpoint.LengthSquared() < (CHECKPOINT_RANGE * CHECKPOINT_RANGE)){
		this->ChangeTarget();
		return;
	}

	// �ړ�
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
	// ��](Y��)
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

	// �ϊ���������
	(*(this->world)) = rotateMatrix * moveMatrix * (*(this->world));
}


/// <summary>
/// �ړI�n��ύX
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
/// �v���C����T��
/// </summary>
/// <param name="_playerWorld">�v���C���̃��[���h�s��i�[��</param>
/// <returns>true:�T���� false:�T���Ȃ�����</returns>
bool EnemyFairyController::SearchPlayer(::Ishikawa::Math::Matrix* const _playerWorld){
	NOT_USING(_playerWorld);
	return false;
}
