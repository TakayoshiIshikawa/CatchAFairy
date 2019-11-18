//=============================================================================
//	ManaLight.cpp
//
//	�}�i���C�g�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ManaLight.h"
#include "Ishikawa/Common/Random.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"

using namespace ::CatchAFairy::Object::PlayScene;

/// <summary>�����x</summary>
const ::Ishikawa::Math::Float3 ManaLight::ACCELERATION(0.0f, 0.3f, 0.0f);
/// <summary>���E�����̌����W��</summary>
const float ManaLight::DECELERATION_FACTOR_XZ = 0.98f;
/// <summary>�e�����̍ő叉��������</summary>
const ::Ishikawa::Math::Float3 ManaLight::MAX_INITIALIZE_SPEED(0.3f, 0.2f, 0.3f);


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
ManaLight::ManaLight() :
	::CatchAFairy::Object::UpdateInterface(),
	lightVertex(nullptr),
	velocity(::Ishikawa::Math::Float3::ZERO)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ManaLight::~ManaLight(){
	// �������Ȃ�
}


/// <summary>
/// ����
/// </summary>
/// <param name="_position">���W</param>
/// <param name="_color">�F</param>
/// <param name="_radiusAndLumen">���a�ƌ��x</param>
void ManaLight::Generate(
	const ::Ishikawa::Math::Float3& _position,
	const ::Ishikawa::Math::Float4& _color,
	const ::Ishikawa::Math::Float2& _radiusAndLumen
){
	if(this->lightVertex == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("light vertex is NULL.");
	}

	this->lightVertex->position = _position;
	this->lightVertex->color = _color;
	this->lightVertex->radiusAndLumen = _radiusAndLumen;
	this->RandomizeVelocity();
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="_elapsedTime">�o�ߎ���</param>
void ManaLight::Update(float _elapsedTime){
	if(this->lightVertex == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("light vertex is NULL.");
	}

	// �ړ�
	this->lightVertex->position += this->velocity * _elapsedTime;
	this->velocity += ACCELERATION * _elapsedTime;
	// ���E�����ɂ͒x���Ȃ�
	this->velocity.x *= DECELERATION_FACTOR_XZ;
	this->velocity.y *= DECELERATION_FACTOR_XZ;
	// ���Ԍo�߂ňÂ��Ȃ�
	this->lightVertex->radiusAndLumen.y *= (1.0f - 0.02f*(60.0f*_elapsedTime));
}


/// <summary>
/// [-1.0f,1.0f)�̗����l���擾
/// </summary>
/// <returns>[-1.0f,1.0f)�̗����l</returns>
inline float ManaLight::Random(){
	return static_cast<float>(2.0 * (::Ishikawa::Common::Random::RealRandom() - 0.5));
}

/// <summary>
/// ���x�������_�}�C�Y
/// </summary>
void ManaLight::RandomizeVelocity(){
	this->velocity.x = MAX_INITIALIZE_SPEED.x * Random();
	this->velocity.y = MAX_INITIALIZE_SPEED.y * Random();
	this->velocity.z = MAX_INITIALIZE_SPEED.z * Random();
}
