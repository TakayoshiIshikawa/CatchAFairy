//=============================================================================
//	PointLight.cpp
//
//	�|�C���g���C�g�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "PointLight.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"

using namespace CatchAFairy::Object::PlayScene;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
PointLight::PointLight() :
	::CatchAFairy::Object::UpdateInterface(),
	::CatchAFairy::Object::PointLightInterface(),
	vertexArray{},
	vertexBuffer(nullptr),
	indexBuffer(nullptr),
	indexCount(0U),
	reflectionLightShaderUser(nullptr),
	manaLightColor{},
	manaLightRadiusAndLumen{},
	manaLightCreateIndex{0U},
	manaLightArray{}
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PointLight::~PointLight(){
	// �������Ȃ�
}


/// <summary>
/// �d���{�̂ɂ����C�g�̍��W�̐ݒ�
/// </summary>
/// <param name="_id">�ݒ肷��d����ID</param>
/// <param name="_x">X���W</param>
/// <param name="_y">Y���W</param>
/// <param name="_z">Z���W</param>
void PointLight::SetFairyBodyLightPosition(unsigned int _id, float _x, float _y, float _z){
	if(_id >= FAIRY_COUNT) return;

	const unsigned int OFFSET_INDEX = _id * ONE_FAIRY_LIGHT_ALL_COUNT;

	for(unsigned int i=0; i<ONE_FAIRY_BODY_LIGHT_COUNT; ++i){
		::Ishikawa::Math::Float3& position = this->vertexArray[OFFSET_INDEX+i].position;
		position.x = _x;
		position.y = _y;
		position.z = _z;
	}
}

/// <summary>
/// �d���{�̂ɂ����C�g�̐F�̐ݒ�
/// </summary>
/// <param name="_id">�ݒ肷��d����ID</param>
/// <param name="_red">�Ԑ���</param>
/// <param name="_green">�ΐ���</param>
/// <param name="_blue">����</param>
/// <param name="_alpha">�s��������</param>
void PointLight::SetFairyBodyLightColor(unsigned int _id, float _red, float _green, float _blue, float _alpha){
	if(_id >= FAIRY_COUNT) return;

	const unsigned int OFFSET_INDEX = _id * ONE_FAIRY_LIGHT_ALL_COUNT;

	for(unsigned int i=0; i<ONE_FAIRY_BODY_LIGHT_COUNT; ++i){
		::Ishikawa::Math::Float4& color = this->vertexArray[OFFSET_INDEX+i].color;
		color.x = _red;
		color.y = _green;
		color.z = _blue;
		color.w = _alpha;
	}
}

/// <summary>
/// �d���{�̂ɂ����C�g�̔��a�̐ݒ�
/// </summary>
/// <param name="_id">�ݒ肷��d����ID</param>
/// <param name="_radius">���a</param>
void PointLight::SetFairyBodyLightRadius(unsigned int _id, float _radius){
	if(_id >= FAIRY_COUNT) return;

	const unsigned int OFFSET_INDEX = _id * ONE_FAIRY_LIGHT_ALL_COUNT;

	for(unsigned int i=0; i<ONE_FAIRY_BODY_LIGHT_COUNT; ++i){
		this->vertexArray[OFFSET_INDEX+i].radiusAndLumen.x = _radius;
	}
}

/// <summary>
/// �d���{�̂ɂ����C�g�̌��x�̐ݒ�
/// </summary>
/// <param name="_id">�ݒ肷��d����ID</param>
/// <param name="_lumen">���x</param>
void PointLight::SetFairyBodyLightLumen(unsigned int _id, float _lumen){
	if(_id >= FAIRY_COUNT) return;

	const unsigned int OFFSET_INDEX = _id * ONE_FAIRY_LIGHT_ALL_COUNT;

	for(unsigned int i=0; i<ONE_FAIRY_BODY_LIGHT_COUNT; ++i){
		this->vertexArray[OFFSET_INDEX+i].radiusAndLumen.y = _lumen;
	}
}

/// <summary>
/// �}�i���C�g�F�̐ݒ�
/// </summary>
/// <param name="_id">�ݒ肷��d����ID</param>
/// <param name="_red">�Ԑ���</param>
/// <param name="_green">�ΐ���</param>
/// <param name="_blue">����</param>
/// <param name="_alpha">�s��������</param>
void PointLight::SetFairyManaLightColor(unsigned int _id, float _red, float _green, float _blue, float _alpha){
	if(_id >= FAIRY_COUNT) return;
	
	this->manaLightColor[_id].x = _red;
	this->manaLightColor[_id].y = _green;
	this->manaLightColor[_id].z = _blue;
	this->manaLightColor[_id].w = _alpha;
}

/// <summary>
/// �}�i���C�g���a�̐ݒ�
/// </summary>
/// <param name="_id">�ݒ肷��d����ID</param>
/// <param name="_radius">���a</param>
void PointLight::SetFairyManaLightRadius(unsigned int _id, float _radius){
	if(_id >= FAIRY_COUNT) return;

	this->manaLightRadiusAndLumen[_id].x = _radius;
}

/// <summary>
/// �}�i���C�g���x�̐ݒ�
/// </summary>
/// <param name="_id">�ݒ肷��d����ID</param>
/// <param name="_lumen">���x</param>
void PointLight::SetFairyManaLightLumen(unsigned int _id, float _lumen){
	if(_id >= FAIRY_COUNT) return;

	this->manaLightRadiusAndLumen[_id].y = _lumen;
}


/// <summary>
/// ������
/// </summary>
void PointLight::Initialize(){
	this->InitializeVertexArray();

	// �e�}�i���C�g�ɒ��_��ݒ�
	for(unsigned int i=0U; i<FAIRY_COUNT; ++i){
		const unsigned int OFFSET_INDEX = i*ONE_FAIRY_LIGHT_ALL_COUNT + ONE_FAIRY_BODY_LIGHT_COUNT;

		for(unsigned int j=0U; j<ONE_FAIRY_MANA_LIGHT_COUNT; ++j){
			this->manaLightArray[i][j].SetLightVertex(&(this->vertexArray[OFFSET_INDEX+j]));
		}
	}
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="_elapsedTime">�X�V�Ԋu</param>
void PointLight::Update(float _elapsedTime){
	// �}�i���C�g���X�V
	for(unsigned int i=0U; i<FAIRY_COUNT; ++i){
		for(unsigned int j=0U; j<ONE_FAIRY_MANA_LIGHT_COUNT; ++j){
			this->manaLightArray[i][j].Update(_elapsedTime);
		}
	}

	// ���_�o�b�t�@�X�V
	this->ReleaseVertexBuffer();
	this->CreateVertexBuffer();
}


/// <summary>
/// �}�i���C�g����
/// </summary>
/// <param name="_id">�ݒ肷��d����ID</param>
/// <param name="_x">X���W</param>
/// <param name="_y">Y���W</param>
/// <param name="_z">Z���W</param>
void PointLight::GenerateFairyManaLight(unsigned int _id, float _x, float _y, float _z){
	if(_id >= FAIRY_COUNT) return;

	this->manaLightArray[_id][this->manaLightCreateIndex[_id]].Generate(
		::Ishikawa::Math::Float3(_x, _y, _z),
		this->manaLightColor[_id],
		this->manaLightRadiusAndLumen[_id]
	);

	this->manaLightCreateIndex[_id] = (this->manaLightCreateIndex[_id]+1) % ONE_FAIRY_MANA_LIGHT_COUNT;
}


/// <summary>
/// �f�o�C�X�ˑ����\�[�X�쐬
/// </summary>
void PointLight::CreateDeviceDependentResources(){
	this->CreateVertexBuffer();
	this->CreateIndexBuffer();
}

/// <summary>
/// �f�o�C�X�ˑ����\�[�X�J��
/// </summary>
void PointLight::ReleaseDeviceDependentResources(){
	this->ReleaseIndexBuffer();
	this->ReleaseVertexBuffer();
}


/// <summary>
/// ���_�z��̏�����
/// </summary>
void PointLight::InitializeVertexArray(){
	for(unsigned int i=0U; i<MAX_COUNT; ++i){
		this->vertexArray[i].position = ::Ishikawa::Math::Float3::ZERO;
		this->vertexArray[i].color = ::Ishikawa::Math::Float4::ZERO;
		this->vertexArray[i].radiusAndLumen = ::Ishikawa::Math::Float2(1.0f, 1.0f);
	}
}


/// <summary>
/// ���_�o�b�t�@�쐬
/// </summary>
void PointLight::CreateVertexBuffer(){
	if(this->reflectionLightShaderUser == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Reflection light shader user is NULL.");
	}

	this->vertexBuffer = this->reflectionLightShaderUser->CreateVertexBuffer(this->vertexArray, MAX_COUNT);
}

/// <summary>
/// �C���f�b�N�X�o�b�t�@�쐬
/// </summary>
void PointLight::CreateIndexBuffer(){
	if(this->reflectionLightShaderUser == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Reflection light shader user is NULL.");
	}

	unsigned long indexArray[MAX_COUNT]{0UL};
	for(unsigned long i=0UL; i<MAX_COUNT; ++i){
		indexArray[i] = i;
	}

	this->indexBuffer = this->reflectionLightShaderUser->CreateIndexBuffer(indexArray, MAX_COUNT);
	this->indexCount = static_cast<unsigned int>(MAX_COUNT);
}

/// <summary>
/// ���_�o�b�t�@�J��
/// </summary>
void PointLight::ReleaseVertexBuffer(){
	if(this->reflectionLightShaderUser == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Reflection light shader user is NULL.");
	}

	if(this->vertexBuffer != nullptr){
		this->reflectionLightShaderUser->ReleaseBuffer(this->vertexBuffer);
		this->vertexBuffer = nullptr;
	}
}

/// <summary>
/// �C���f�b�N�X�o�b�t�@�J��
/// </summary>
void PointLight::ReleaseIndexBuffer(){
	if(this->reflectionLightShaderUser == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Reflection light shader user is NULL.");
	}

	if(this->indexBuffer != nullptr){
		this->reflectionLightShaderUser->ReleaseBuffer(this->indexBuffer);
		this->indexBuffer = nullptr;
		this->indexCount = 0U;
	}
}
