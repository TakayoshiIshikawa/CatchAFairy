//=============================================================================
//	Ground.cpp
//
//	�n�ʂ̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "Ground.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Model/ITObjectLoader.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "CatchAFairy/Shader/User/ShaderVertexTopology.h"
#include "CatchAFairy/Shader/User/PlayCameraEyeFirstShaderUser.h"

using namespace CatchAFairy::Object::PlayScene;


/// <summary>���샂�f���f�[�^�t�@�C���p�X</summary>
const char* const Ground::IT_MODEL_DATA_FILEPATH = "Resources/Models/TestGround001.itobj";
/// <summary>�g�U���˃e�N�X�`���t�@�C���p�X</summary>
const wchar_t* const Ground::DIFFUSE_TEXTURE_FILEPATH = L"Resources/Textures/GroundTexture.png";
/// <summary>�@���e�N�X�`���t�@�C���p�X</summary>
const wchar_t* const Ground::NORMAL_TEXTURE_FILEPATH = L"Resources/Textures/GroundNormal01.png";


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Ground::Ground() :
	::CatchAFairy::Object::RenderInterface(),
	::Ishikawa::DirectX::Device::DependentResource::Base(),
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
	world(::Ishikawa::Math::Matrix::IDENTITY),
	diffuseTexture(),
	normalTexture()
{
	// ����I�u�W�F�N�g�f�[�^�ǂݍ���
	this->LoadItObjectData();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Ground::~Ground(){
	// ����I�u�W�F�N�g�f�[�^�J��
	this->ReleaseItObjectData();
}


/// <summary>
/// �g�|���W�[�^�C�v�̎擾
/// </summary>
/// <returns>�g�|���W�[�^�C�v</returns>
::CatchAFairy::Shader::User::VertexTopology Ground::GetTopology() const {
	return this->topology;
}

/// <summary>
/// ���_�o�b�t�@�̎擾
/// </summary>
/// <returns>���_�o�b�t�@</returns>
ID3D11Buffer* Ground::GetVertexBuffer() const {
	return this->vertexBuffer;
}

/// <summary>
/// �C���f�b�N�X�o�b�t�@�̎擾
/// </summary>
/// <returns>�C���f�b�N�X�o�b�t�@</returns>
ID3D11Buffer* Ground::GetIndexBuffer() const {
	return this->indexBuffer;
}

/// <summary>
/// �C���f�b�N�X���̎擾
/// </summary>
/// <returns>�C���f�b�N�X��</returns>
unsigned int Ground::GetIndexCount() const {
	return this->indexCount;
}

/// <summary>
/// �O�ډ~���S���W�̎擾
/// </summary>
/// <returns>�O�ډ~���S���W</returns>
const ::Ishikawa::Math::Float3& Ground::GetCircumscribedCircleCenterPosition() const {
	return this->circumscribedCircleCenterPosition;
}

/// <summary>
/// �O�ډ~���a���̎擾
/// </summary>
/// <returns>�O�ډ~���a���</returns>
float Ground::GetSquareCircumscribedCircleRadius() const {
	return this->squareCircumscribedCircleRadius;
}

/// <summary>
/// �g�U���˃V�F�[�_���\�[�X�r���[�̎擾
/// </summary>
/// <returns>�g�U���˃V�F�[�_���\�[�X�r���[</returns>
ID3D11ShaderResourceView* Ground::GetDiffuseShaderResourceView() const {
	return this->diffuseTexture.GetShaderResourceView();
}

/// <summary>
/// �@���V�F�[�_���\�[�X�r���[�̎擾
/// </summary>
/// <returns>�@���V�F�[�_���\�[�X�r���[</returns>
ID3D11ShaderResourceView* Ground::GetNormalShaderResourceView() const {
	return this->normalTexture.GetShaderResourceView();
}

/// <summary>
/// ���[���h�ϊ��s��̎擾
/// </summary>
/// <returns>���[���h�ϊ��s��</returns>
const ::Ishikawa::Math::Matrix& Ground::GetWorld() const {
	return this->world;
}


/// <summary>
/// �f�o�C�X�ɓǂݍ��܂ꂽ
/// </summary>
void Ground::OnDeviceLoaded(){
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
void Ground::OnDeviceRestored(){
	// �������Ȃ�
}

/// <summary>
/// �f�o�C�X���j�����ꂽ
/// </summary>
void Ground::OnDeviceLost(){
	// �������Ȃ�
}


/// <summary>
/// ������
/// </summary>
void Ground::Initialize(){
	// ���̏�����
	this->InitializeITDeviceDependentResource();

	// �����o�̏�����
	this->diffuseTexture.Initialize();
	this->normalTexture.Initialize();
}


/// <summary>
/// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�쐬
/// </summary>
/// <param name="_useShader">���p����V�F�[�_</param>
void Ground::CreateVertexBufferAndIndexBuffer(
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
void Ground::ReleaseVertexBufferAndIndexBuffer(){
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
void Ground::LoadItObjectData(){
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
void Ground::ReleaseItObjectData(){
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
