//=============================================================================
//	EnemyFairies.cpp
//
//	�G�d���B�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "EnemyFairies.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Common/Random.h"
#include "Ishikawa/Model/ITObjectLoader.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "CatchAFairy/Shader/User/ShaderVertexTopology.h"
#include "CatchAFairy/Shader/User/PlayCameraEyeFirstShaderUser.h"
#include "PlayCamera.h"
#include "PointLight.h"
#include "Player.h"

using namespace ::CatchAFairy::Object::PlayScene;


/// <summary>���샂�f���f�[�^�t�@�C���p�X</summary>
const char* const EnemyFairies::IT_MODEL_DATA_FILEPATH = "Resources/Models/TestFairy001.itobj";
/// <summary>�g�U���˃e�N�X�`���t�@�C���p�X</summary>
const wchar_t* const EnemyFairies::DIFFUSE_TEXTURE_FILEPATH = L"Resources/Textures/White.png";
/// <summary>�@���e�N�X�`���t�@�C���p�X</summary>
const wchar_t* const EnemyFairies::NORMAL_TEXTURE_FILEPATH = L"Resources/Textures/DefaultNormal.png";

/// <summary>�G�d�������p�z��</summary>
const unsigned int EnemyFairies::ENEMY_FAIRY_GENERATING_COUNT[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)] = {
	1U,
	5U,
	16U
};


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
EnemyFairies::EnemyFairies() :
	::Ishikawa::DirectX::Device::DependentResource::Base(),
	::CatchAFairy::Object::UpdateInterface(),
	::CatchAFairy::Object::RenderSameInterface(),
	useShader(nullptr),
	itObjectVertices(nullptr),
	itObjectVertexCount(0U),
	itObjectIndices(nullptr),
	itObjectIndexCount(0U),
	topology(::CatchAFairy::Shader::User::VertexTopology::TriangleList),
	vertexBuffer(nullptr),
	indexBuffer(nullptr),
	indexCount(0U),
	circumscribedCircleCenterPosition(::Ishikawa::Math::Float3::ZERO),
	squareCircumscribedCircleRadius(0.0f),
	diffuseTexture(),
	normalTexture(),
	worldArray{},
	controllerArray{},
	player(nullptr),
	catchCount(0U)
{
	// ����I�u�W�F�N�g�f�[�^�ǂݍ���
	this->LoadItObjectData();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyFairies::~EnemyFairies(){
	// ����I�u�W�F�N�g�f�[�^�J��
	this->ReleaseItObjectData();
}


/// <summary>
/// �g�|���W�[�^�C�v�̎擾
/// </summary>
/// <returns>�g�|���W�[�^�C�v</returns>
::CatchAFairy::Shader::User::VertexTopology EnemyFairies::GetTopology() const {
	return this->topology;
}

/// <summary>
/// ���_�o�b�t�@�̎擾
/// </summary>
/// <returns>���_�o�b�t�@</returns>
ID3D11Buffer* EnemyFairies::GetVertexBuffer() const {
	return this->vertexBuffer;
}

/// <summary>
/// �C���f�b�N�X�o�b�t�@�̎擾
/// </summary>
/// <returns>�C���f�b�N�X�o�b�t�@</returns>
ID3D11Buffer* EnemyFairies::GetIndexBuffer() const {
	return this->indexBuffer;
}

/// <summary>
/// �C���f�b�N�X���̎擾
/// </summary>
/// <returns>�C���f�b�N�X��</returns>
unsigned int EnemyFairies::GetIndexCount() const {
	return this->indexCount;
}

/// <summary>
/// �O�ډ~���S���W�̎擾
/// </summary>
/// <returns>�O�ډ~���S���W</returns>
const ::Ishikawa::Math::Float3& EnemyFairies::GetCircumscribedCircleCenterPosition() const {
	return this->circumscribedCircleCenterPosition;
}

/// <summary>
/// �O�ډ~���a���̎擾
/// </summary>
/// <returns>�O�ډ~���a���</returns>
float EnemyFairies::GetSquareCircumscribedCircleRadius() const {
	return this->squareCircumscribedCircleRadius;
}

/// <summary>
/// �g�U���˃V�F�[�_���\�[�X�r���[�̎擾
/// </summary>
/// <returns>�g�U���˃V�F�[�_���\�[�X�r���[</returns>
ID3D11ShaderResourceView* EnemyFairies::GetDiffuseShaderResourceView() const {
	return this->diffuseTexture.GetShaderResourceView();
}

/// <summary>
/// �@���V�F�[�_���\�[�X�r���[�̎擾
/// </summary>
/// <returns>�@���V�F�[�_���\�[�X�r���[</returns>
ID3D11ShaderResourceView* EnemyFairies::GetNormalShaderResourceView() const {
	return this->normalTexture.GetShaderResourceView();
}

/// <summary>
/// ���[���h�ϊ��s�񐔂̎擾
/// </summary>
/// <returns>���[���h�ϊ��s��</returns>
unsigned int EnemyFairies::GetWorldCount() const {
	return ENEMY_FAIRY_COUNT;
}

/// <summary>
/// ���[���h�ϊ��s��z��̎擾
/// </summary>
/// <returns>���[���h�ϊ��s��z��</returns>
const ::Ishikawa::Math::Matrix* EnemyFairies::GetWorldArray() const {
	return this->worldArray;
}

/// <summary>
/// �v���C���̎擾
/// </summary>
/// <returns>�v���C��</returns>
const Player* EnemyFairies::GetPlayer() const {
	return this->player;
}

/// <summary>
/// �w��d���̎�ނ̎擾
/// </summary>
/// <param name="_id">�G�d��ID</param>
/// <returns>�w��d���̎��</returns>
const CatchAFairy::Data::EnemyFairyType EnemyFairies::GetFairyType(unsigned int _id) const {
	return this->controllerArray[_id].GetType();
}


/// <summary>
/// �|�C���g���C�g�̐ݒ�
/// </summary>
/// <param name="_pointLight">�|�C���g���C�g</param>
void EnemyFairies::SetPointLight(PointLight* const _pointLight){
	for(unsigned int i=0U; i<ENEMY_FAIRY_COUNT; ++i){
		this->controllerArray[i].SetPointLight(_pointLight);
	}
}

/// <summary>
/// �v���C���̐ݒ�
/// </summary>
/// <param name="_player">�v���C��</param>
void EnemyFairies::SetPlayer(const Player* const _player){
	this->player = _player;
}


/// <summary>
/// �f�o�C�X�ɓǂݍ��܂ꂽ
/// </summary>
void EnemyFairies::OnDeviceLoaded(){
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetDependentITDevice();

	// �����o��o�^
	itDevice->AddDependentResource(&(this->diffuseTexture));
	itDevice->AddDependentResource(&(this->normalTexture));

	// �e�N�X�`���ǂݍ���
	this->diffuseTexture.Load(DIFFUSE_TEXTURE_FILEPATH);
	this->normalTexture.Load(NORMAL_TEXTURE_FILEPATH);
}

/// <summary>
/// �f�o�C�X���������ꂽ
/// </summary>
void EnemyFairies::OnDeviceRestored(){
	// �������Ȃ�
}

/// <summary>
/// �f�o�C�X���j�����ꂽ
/// </summary>
void EnemyFairies::OnDeviceLost(){
	// �������Ȃ�
}

/// <summary>
/// �ǂꂩ���߂܂���
/// </summary>
/// <param name="_id">�߂܂����d����ID</param>
void EnemyFairies::OnCatched(unsigned int _id){
	if(_id >= ENEMY_FAIRY_COUNT){
		return;
	}

	++(this->catchCount);

	unsigned int generateIndex = static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Normal);
	for(unsigned int i=static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)-1; i>0; --i){
		if((this->catchCount % ENEMY_FAIRY_GENERATING_COUNT[i]) == 0U){
			generateIndex = i;
			break;
		}
	}
	this->controllerArray[_id].SetType(static_cast<CatchAFairy::Data::EnemyFairyType>(generateIndex));
	this->controllerArray[_id].Reset();
}


/// <summary>
/// ������
/// </summary>
void EnemyFairies::Initialize(){
	// ���̏�����
	this->InitializeITDeviceDependentResource();

	// �����o�̏�����
	this->diffuseTexture.Initialize();
	this->normalTexture.Initialize();

	// ���ׂĂ̗d����������
	for(unsigned int i=0U; i<ENEMY_FAIRY_COUNT; ++i){
		this->controllerArray[i].SetType(CatchAFairy::Data::EnemyFairyType::Normal);
		this->controllerArray[i].SetId(::CatchAFairy::Data::PointLightsSetting::PLAYER_FAIRY_COUNT + i);
		this->worldArray[i] = ::Ishikawa::Math::Matrix::IDENTITY;
		this->controllerArray[i].SetWorld(&(this->worldArray[i]));
		this->controllerArray[i].SetEnemyFairies(this);
		this->controllerArray[i].Reset();
	}
}


/// <summary>
/// �X�V
/// </summary>
/// <param name="_elapsedTime">�X�V�Ԋu</param>
void EnemyFairies::Update(float _elapsedTime){
	for(unsigned int i=0U; i<ENEMY_FAIRY_COUNT; ++i){
		this->controllerArray[i].Update(_elapsedTime);
	}
}


/// <summary>
/// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�쐬
/// </summary>
/// <param name="_useShader">���p����V�F�[�_</param>
void EnemyFairies::CreateVertexBufferAndIndexBuffer(
	const ::CatchAFairy::Shader::User::PlayCameraEyeFirst* const _useShader
){
	if(_useShader == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_useShader is NULL.");
	}

	this->useShader = _useShader;

	// ���_���
	unsigned int vertexCount = this->itObjectVertexCount;
	::CatchAFairy::Shader::User::PlayCameraEyeFirst::VSInput* vertices =
		new ::CatchAFairy::Shader::User::PlayCameraEyeFirst::VSInput[vertexCount];
	if(vertices == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't new player's vertex shader input memory.");
	}
	for(unsigned int i=0U; i<vertexCount; ++i){
		vertices[i].position			= this->itObjectVertices[i].position;
		vertices[i].normal				= this->itObjectVertices[i].normal;
		vertices[i].textureCoordinate	= this->itObjectVertices[i].textureCoordinate;
	}
	// ���_�o�b�t�@�쐬
	this->vertexBuffer = _useShader->CreateVertexBuffer(vertices, vertexCount);
	// ���_�z��폜
	delete[] vertices;
	vertices = nullptr;

	// �C���f�b�N�X���
	// �C���f�b�N�X�o�b�t�@�쐬
	this->indexBuffer = _useShader->CreateIndexBuffer(this->itObjectIndices, this->itObjectIndexCount);
	this->indexCount = this->itObjectIndexCount;

	// �g�|���W�[�ݒ�
	this->topology = ::CatchAFairy::Shader::User::VertexTopology::TriangleList;
}

/// <summary>
/// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�J��
/// </summary>
void EnemyFairies::ReleaseVertexBufferAndIndexBuffer(){
	if(this->useShader != nullptr){
		// ���_�o�b�t�@�J��
		this->useShader->ReleaseBuffer(this->vertexBuffer);
		this->vertexBuffer = nullptr;

		// �C���f�b�N�X�o�b�t�@�J��
		this->useShader->ReleaseBuffer(this->indexBuffer);
		this->indexBuffer = nullptr;

		this->indexCount = 0U;

		this->useShader = nullptr;
	}
}


/// <summary>
/// ����I�u�W�F�N�g�f�[�^�ǂݍ���
/// </summary>
void EnemyFairies::LoadItObjectData(){
	::Ishikawa::Model::ItObject::FileLoader itObjectFileLoader;

	itObjectFileLoader.Load(
		&(this->itObjectVertices),
		&(this->itObjectVertexCount),
		&(this->itObjectIndices),
		&(this->itObjectIndexCount),
		&(this->circumscribedCircleCenterPosition),
		&(this->squareCircumscribedCircleRadius),
		IT_MODEL_DATA_FILEPATH
	);
}

/// <summary>
/// ����I�u�W�F�N�g�f�[�^�J��
/// </summary>
void EnemyFairies::ReleaseItObjectData(){
	// ���_���
	if(this->itObjectVertices != nullptr){
		delete[] this->itObjectVertices;
		this->itObjectVertices = nullptr;
	}
	this->itObjectVertexCount = 0U;

	// �C���f�b�N�X���
	if(this->itObjectIndices != nullptr){
		delete[] this->itObjectIndices;
		this->itObjectIndices = nullptr;
	}
	this->itObjectIndexCount = 0U;
}
