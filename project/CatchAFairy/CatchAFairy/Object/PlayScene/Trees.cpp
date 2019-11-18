//=============================================================================
//	Trees.cpp
//
//	�؁X�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "Trees.h"
#include <math.h>
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Common/Random.h"
#include "Ishikawa/Math/ITCollision.h"
#include "Ishikawa/Model/ITObjectLoader.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "CatchAFairy/Shader/User/ShaderVertexTopology.h"
#include "CatchAFairy/Shader/User/PlayCameraEyeFirstShaderUser.h"

using namespace CatchAFairy::Object::PlayScene;


/// <summary>���샂�f���f�[�^�t�@�C���p�X</summary>
const char* const Trees::IT_MODEL_DATA_FILEPATH = "Resources/Models/TestTree004.itobj";
/// <summary>�g�U���˃e�N�X�`���t�@�C���p�X</summary>
const wchar_t* const Trees::DIFFUSE_TEXTURE_FILEPATH = L"Resources/Textures/TreeTexture01.png";
/// <summary>�@���e�N�X�`���t�@�C���p�X</summary>
const wchar_t* const Trees::NORMAL_TEXTURE_FILEPATH = L"Resources/Textures/TreeNormal02.png";
/// <summary>�I�t�Z�b�g�f�[�^�t�@�C���p�X</summary>
const char* const Trees::OFFSET_DATA_FILEPATH = "Resources/Data/TreesData.csv";
/// <summary>���莲����</summary>
const float Trees::COLLISION_AXIS_HEIGHT = 3.3f;
/// <summary>���蔼�a</summary>
const float Trees::COLLISION_RADIUS = 0.3f;
/// <summary>�ő�U��</summary>
const ::Ishikawa::Math::Float3 Trees::MAX_AMPLITUDE(0.01f, 0.005f, 0.01f);
/// <summary>�ŏ��h�ꑬ�x</summary>
const float Trees::MIN_SWING_SPEED = 0.25f * 3.1415926535f;
/// <summary>�ő�h�ꑬ�x</summary>
const float Trees::MAX_SWING_SPEED = 1.0f * 3.1415926535f;


/// <summary>
/// �s��ɕϊ�
/// </summary>
/// <param name="_matrix">�ۑ���̍s��</param>
void Trees::WorldOffset::ToMatrix(::Ishikawa::Math::Matrix* const _matrix) const {
	if(_matrix == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_matrix is NULL.");
	}

	// �ݒ�
	(*_matrix) = ::Ishikawa::Math::Matrix::IDENTITY;
	float sinValue = sinf(this->angle);
	float cosValue = cosf(this->angle);
	_matrix->_11 = this->scaleOfWidth * cosValue;
	_matrix->_13 = this->scaleOfWidth * sinValue;
	_matrix->_22 = this->scaleOfHeight;
	_matrix->_31 = this->scaleOfWidth * (-sinValue);
	_matrix->_33 = this->scaleOfWidth * cosValue;
	_matrix->_41 = this->positionX;
	_matrix->_42 = this->positionY;
	_matrix->_43 = this->positionZ;
}


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Trees::Trees() :
	::CatchAFairy::Object::RenderSameInterface(),
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
	amplitudeArray(nullptr),
	swingSpeedArray(nullptr),
	swingTimeArray(nullptr),
	treesOffset(),
	worldArray(nullptr),
	diffuseTexture(),
	normalTexture()
{
	// ����I�u�W�F�N�g�f�[�^�ǂݍ���
	this->LoadItObjectData();

	// ���[���h�ϊ��s��z��̏�����
	this->InitializeWorldArray();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Trees::~Trees(){
	// ���[���h�ϊ��s��z��J��
	this->FinalizeWorldArray();
	// ����I�u�W�F�N�g�f�[�^�J��
	this->ReleaseItObjectData();
}


/// <summary>
/// �g�|���W�[�^�C�v�̎擾
/// </summary>
/// <returns>�g�|���W�[�^�C�v</returns>
::CatchAFairy::Shader::User::VertexTopology Trees::GetTopology() const {
	return this->topology;
}

/// <summary>
/// ���_�o�b�t�@�̎擾
/// </summary>
/// <returns>���_�o�b�t�@</returns>
ID3D11Buffer* Trees::GetVertexBuffer() const {
	return this->vertexBuffer;
}

/// <summary>
/// �C���f�b�N�X�o�b�t�@�̎擾
/// </summary>
/// <returns>�C���f�b�N�X�o�b�t�@</returns>
ID3D11Buffer* Trees::GetIndexBuffer() const {
	return this->indexBuffer;
}

/// <summary>
/// �C���f�b�N�X���̎擾
/// </summary>
/// <returns>�C���f�b�N�X��</returns>
unsigned int Trees::GetIndexCount() const {
	return this->indexCount;
}

/// <summary>
/// �O�ډ~���S���W�̎擾
/// </summary>
/// <returns>�O�ډ~���S���W</returns>
const ::Ishikawa::Math::Float3& Trees::GetCircumscribedCircleCenterPosition() const {
	return this->circumscribedCircleCenterPosition;
}

/// <summary>
/// �O�ډ~���a���̎擾
/// </summary>
/// <returns>�O�ډ~���a���</returns>
float Trees::GetSquareCircumscribedCircleRadius() const {
	return this->squareCircumscribedCircleRadius;
}

/// <summary>
/// �g�U���˃V�F�[�_���\�[�X�r���[�̎擾
/// </summary>
/// <returns>�g�U���˃V�F�[�_���\�[�X�r���[</returns>
ID3D11ShaderResourceView* Trees::GetDiffuseShaderResourceView() const {
	return this->diffuseTexture.GetShaderResourceView();
}

/// <summary>
/// �@���V�F�[�_���\�[�X�r���[�̎擾
/// </summary>
/// <returns>�@���V�F�[�_���\�[�X�r���[</returns>
ID3D11ShaderResourceView* Trees::GetNormalShaderResourceView() const {
	return this->normalTexture.GetShaderResourceView();
}

/// <summary>
/// ���[���h�ϊ��s�񐔂̎擾
/// </summary>
/// <returns>���[���h�ϊ��s��</returns>
unsigned int Trees::GetWorldCount() const {
	return this->treesOffset.GetCount();
}

/// <summary>
/// ���[���h�ϊ��s��z��̎擾
/// </summary>
/// <returns>���[���h�ϊ��s��z��</returns>
const ::Ishikawa::Math::Matrix* Trees::GetWorldArray() const {
	return this->worldArray;
}


/// <summary>
/// �f�o�C�X�ɓǂݍ��܂ꂽ
/// </summary>
void Trees::OnDeviceLoaded(){
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
void Trees::OnDeviceRestored(){
	// �������Ȃ�
}

/// <summary>
/// �f�o�C�X���j�����ꂽ
/// </summary>
void Trees::OnDeviceLost(){
	// �������Ȃ�
}


/// <summary>
/// ������
/// </summary>
void Trees::Initialize(){
	// ���̏�����
	this->InitializeITDeviceDependentResource();

	// �����o�̏�����
	this->diffuseTexture.Initialize();
	this->normalTexture.Initialize();
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="_elapsedTime">�X�V����</param>
void Trees::Update(float _elapsedTime){
	this->UpdateWorldArray(_elapsedTime);
}


/// <summary>
/// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�쐬
/// </summary>
/// <param name="_useShader">���p����V�F�[�_</param>
void Trees::CreateVertexBufferAndIndexBuffer(
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
void Trees::ReleaseVertexBufferAndIndexBuffer(){
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
/// �x�N�g���Ƃ̔���
/// </summary>
/// <param name="_startPosition">�x�N�g���n�_</param>
/// <param name="_vector">�x�N�g��</param>
/// <param name="_matrixVectorToWorld">�x�N�g����Ԃ��烏�[���h��Ԃւ̕ϊ��s��</param>
/// <param name="_reflectionRate">���˗�</param>
/// <returns>���˃x�N�g��</returns>
::Ishikawa::Math::Float3 Trees::CollideVector(
	const ::Ishikawa::Math::Float3& _startPosition,
	const ::Ishikawa::Math::Float3& _vector,
	const ::Ishikawa::Math::Matrix& _matrixVectorToWorld,
	float _reflectionRate
) const {
	::Ishikawa::Math::Float3 returnVector = _vector;
	for(unsigned int i=0U; i<this->treesOffset.GetCount(); ++i){
		returnVector = ::Ishikawa::Math::Collision::ReflectVectorAtCapsule(
			_startPosition,
			returnVector,
			_matrixVectorToWorld * this->worldArray[i].Invert(),
			::Ishikawa::Math::Float3::ZERO,
			::Ishikawa::Math::Float3(0.0f, COLLISION_AXIS_HEIGHT, 0.0f),
			COLLISION_RADIUS,
			_reflectionRate
		);
	}
	return returnVector;
}


/// <summary>
/// ����I�u�W�F�N�g�f�[�^�ǂݍ���
/// </summary>
void Trees::LoadItObjectData(){
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
void Trees::ReleaseItObjectData(){
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

/// <summary>
/// ���[���h�ϊ��s��z��̏�����
/// </summary>
void Trees::InitializeWorldArray(){
	const float DELTA_SWING_SPEED = MAX_SWING_SPEED - MIN_SWING_SPEED;

	this->FinalizeWorldArray();

	this->treesOffset.Load(OFFSET_DATA_FILEPATH);
	const ::Ishikawa::Math::Matrix* const startWorldArray = this->treesOffset.GetWorldArray();

	const unsigned int TREE_COUNT = this->treesOffset.GetCount();
	this->amplitudeArray = new ::Ishikawa::Math::Float3[TREE_COUNT];
	this->swingSpeedArray = new ::Ishikawa::Math::Float3[TREE_COUNT];
	this->swingTimeArray = new ::Ishikawa::Math::Float3[TREE_COUNT];
	this->worldArray = new ::Ishikawa::Math::Matrix[TREE_COUNT];

	for(unsigned int i=0U; i<TREE_COUNT; ++i){
		// �U��
		this->amplitudeArray[i].x = MAX_AMPLITUDE.x * static_cast<float>(::Ishikawa::Common::Random::RealRandom());
		this->amplitudeArray[i].y = MAX_AMPLITUDE.y * static_cast<float>(::Ishikawa::Common::Random::RealRandom());
		this->amplitudeArray[i].z = MAX_AMPLITUDE.z * static_cast<float>(::Ishikawa::Common::Random::RealRandom());
		// �h�ꑬ�x
		this->swingSpeedArray[i].x = MIN_SWING_SPEED + DELTA_SWING_SPEED * static_cast<float>(::Ishikawa::Common::Random::RealRandom());
		this->swingSpeedArray[i].y = MIN_SWING_SPEED + DELTA_SWING_SPEED * static_cast<float>(::Ishikawa::Common::Random::RealRandom());
		this->swingSpeedArray[i].z = MIN_SWING_SPEED + DELTA_SWING_SPEED * static_cast<float>(::Ishikawa::Common::Random::RealRandom());
		// �h�ꎞ��
		this->swingTimeArray[i] = ::Ishikawa::Math::Float3::ZERO;

		// ���[���h�s��
		this->worldArray[i] = startWorldArray[i];
	}

}

/// <summary>
/// ���[���h�ϊ��s��z��̍X�V
/// </summary>
/// <param name="_elapsedTime">�X�V����</param>
void Trees::UpdateWorldArray(float _elapsedTime){
	const float PI_2 = 2.0f * 3.1415926535f;

	const ::Ishikawa::Math::Matrix* const startWorldArray = this->treesOffset.GetWorldArray();
	for(unsigned int i=0U; i<this->treesOffset.GetCount(); ++i){
		// �h�ꎞ�ԍX�V
		this->swingTimeArray[i] += this->swingSpeedArray[i] * _elapsedTime;
		// �e�h�ꎞ�Ԃ�2PI�𒴂��Ȃ��悤�ɂ���
		while(this->swingTimeArray[i].x > PI_2){ this->swingTimeArray[i].x -= PI_2; }
		while(this->swingTimeArray[i].y > PI_2){ this->swingTimeArray[i].y -= PI_2; }
		while(this->swingTimeArray[i].z > PI_2){ this->swingTimeArray[i].z -= PI_2; }

		// �h�ꕝ
		::Ishikawa::Math::Float3 amplitude;
		amplitude.x = this->amplitudeArray[i].x * cosf(this->swingTimeArray[i].x);
		amplitude.y = this->amplitudeArray[i].y * cosf(this->swingTimeArray[i].y);
		amplitude.z = this->amplitudeArray[i].z * cosf(this->swingTimeArray[i].z);

		// �h��s��
		::Ishikawa::Math::Matrix swingMatrix(
			1.0f, 0.0f, 0.0f, 0.0f,
			amplitude.x, 1.0f+amplitude.y, amplitude.z, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		// ���[���h�s��
		this->worldArray[i] = swingMatrix * startWorldArray[i];
	}
}

/// <summary>
/// ���[���h�ϊ��s��z��̏I��
/// </summary>
void Trees::FinalizeWorldArray(){
	if(this->worldArray != nullptr){
		delete[] this->worldArray;
		this->worldArray = nullptr;
	}
	if(this->swingTimeArray != nullptr){
		delete[] this->swingTimeArray;
		this->swingTimeArray = nullptr;
	}
	if(this->swingSpeedArray != nullptr){
		delete[] this->swingSpeedArray;
		this->swingSpeedArray = nullptr;
	}
	if(this->amplitudeArray != nullptr){
		delete[] this->amplitudeArray;
		this->amplitudeArray = nullptr;
	}
}
