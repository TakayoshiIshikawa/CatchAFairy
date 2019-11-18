//=============================================================================
//	Player.cpp
//
//	�v���C���̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "Player.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Model/ITObjectLoader.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "CatchAFairy/Shader/User/ShaderVertexTopology.h"
#include "CatchAFairy/Shader/User/PlayCameraEyeFirstShaderUser.h"
#include "CatchAFairy/Game.h"
#include "CatchAFairy/Scene/ScenePlay.h"
#include "CatchAFairy/Object/PlayScene/GUI/OperationMethodView.h"
#include "PlayCamera.h"

using namespace CatchAFairy::Object::PlayScene;


/// <summary>���샂�f���f�[�^�t�@�C���p�X</summary>
const char* const Player::IT_MODEL_DATA_FILEPATH = "Resources/Models/TestFairy001.itobj";
/// <summary>�g�U���˃e�N�X�`���t�@�C���p�X</summary>
const wchar_t* const Player::DIFFUSE_TEXTURE_FILEPATH = L"Resources/Textures/White.png";
/// <summary>�@���e�N�X�`���t�@�C���p�X</summary>
const wchar_t* const Player::NORMAL_TEXTURE_FILEPATH = L"Resources/Textures/DefaultNormal.png";
/// <summary>�d��ID</summary>
const unsigned int Player::FAIRY_ID = 0U;
/// <summary>�{�̃��C�g�̂���</summary>
const ::Ishikawa::Math::Float3 Player::FAIRY_LIGHT_ANCHOR(0.0f, 0.1f, 0.0f);
/// <summary>�{�̃��C�g�̐F</summary>
const ::Ishikawa::Math::Float4 Player::FAIRY_LIGHT_COLOR(0.9f, 0.9f, 1.0f, 0.7f);
/// <summary>�{�̃��C�g�̔��a</summary>
const float Player::FAIRY_LIGHT_RADIUS = 0.03f;
/// <summary>�{�̃��C�g�̌���</summary>
const float Player::FAIRY_LIGHT_LUMEN = 0.4f;
/// <summary>��{�̃}�i���C�g�̐F</summary>
const ::Ishikawa::Math::Float4 Player::DEFAULT_MANA_LIGHT_COLOR(0.9f, 0.9f, 1.0f, 0.6f);
/// <summary>�e�d����߂܂����Ƃ��̃}�i���C�g�̐F</summary>
const ::Ishikawa::Math::Float4 Player::CATCHED_FAIRY_MANA_LIGHT_COLOR[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)] = {
	::Ishikawa::Math::Float4(0.0f, 1.0f, 0.0f, 0.9f),
	::Ishikawa::Math::Float4(1.0f, 1.0f, 0.0f, 0.9f),
	::Ishikawa::Math::Float4(0.0f, 0.0f, 1.0f, 0.9f)
};
/// <summary>�}�i���C�g�̔��a</summary>
const float Player::MANA_LIGHT_RADIUS = 0.002f;
/// <summary>�}�i���C�g�̌���</summary>
const float Player::MANA_LIGHT_LUMEN = 0.04f;
/// <summary>�ŏ��̃}�i���C�g�����J�E���g�����䗦</summary>
const float Player::MINIMUM_MANA_LIGHT_GENERATE_COUNT_ADDITIONAL_RATIO = 0.5f;
/// <summary>�ő�̃}�i���C�g�����J�E���g�����䗦</summary>
const float Player::MAXIMUM_MANA_LIGHT_GENERATE_COUNT_ADDITIONAL_RATIO = 10.0f;
/// <summary>�J�n�ʒu</summary>
const ::Ishikawa::Math::Float3 Player::START_POSITION(0.0f, 1.5f, 0.0f);
/// <summary>�ړ����x</summary>
const float Player::MOVE_SPEED = 3.0f;
/// <summary>��]���x</summary>
const float Player::ROTATE_SPEED = 2.5f;
/// <summary>�ߊl�Ԋu</summary>
const float Player::CATCH_INTERVAL = 1.5f;

/// <summary>�d����߂܂�����Œፂ��</summary>
const float Player::FAIRY_CATCHABLE_HEIGHT_MIN = -0.5f;
/// <summary>�d����߂܂�����ō�����</summary>
const float Player::FAIRY_CATCHABLE_HEIGHT_MAX = 0.5f;
/// <summary>�d����߂܂����锼�a</summary>
const float Player::FAIRY_CATCHABLE_RADIUS = 0.5f;
/// <summary>�d����߂܂������`���a</summary>
const float Player::FAIRY_CATCHABLE_ANGLE_RADIUS = 1.5f;
/// <summary>�d����߂܂�����p�x�̃R�T�C���l</summary>
const float Player::FAIRY_CATCHABLE_ANGLE_COS = 0.5f;

/// <summary>�O�i�L�[</summary>
const ::Ishikawa::Input::KeyCode Player::FRONT_MOVE_KEY = ::Ishikawa::Input::KeyCode::Z;
/// <summary>��i�L�[ [�g�p�Ȃ�]</summary>
const ::Ishikawa::Input::KeyCode Player::BACK_MOVE_KEY = ::Ishikawa::Input::KeyCode::DownArrow;
/// <summary>�E�i�L�[ [�g�p�Ȃ�]</summary>
const ::Ishikawa::Input::KeyCode Player::RIGHT_MOVE_KEY = ::Ishikawa::Input::KeyCode::A;
/// <summary>���i�L�[ [�g�p�Ȃ�]</summary>
const ::Ishikawa::Input::KeyCode Player::LEFT_MOVE_KEY = ::Ishikawa::Input::KeyCode::D;
/// <summary>��i�L�[</summary>
const ::Ishikawa::Input::KeyCode Player::UP_MOVE_KEY = ::Ishikawa::Input::KeyCode::UpArrow;
/// <summary>���i�L�[</summary>
const ::Ishikawa::Input::KeyCode Player::DOWN_MOVE_KEY = ::Ishikawa::Input::KeyCode::DownArrow;
/// <summary>�E����L�[</summary>
const ::Ishikawa::Input::KeyCode Player::RIGHT_ROTATE_KEY = ::Ishikawa::Input::KeyCode::RightArrow;
/// <summary>������L�[</summary>
const ::Ishikawa::Input::KeyCode Player::LEFT_ROTATE_KEY = ::Ishikawa::Input::KeyCode::LeftArrow;
/// <summary>�ߊl�L�[</summary>
const ::Ishikawa::Input::KeyCode Player::CATCH_KEY = ::Ishikawa::Input::KeyCode::X;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Player::Player() :
	::Ishikawa::DirectX::Device::DependentResource::Base(),
	::CatchAFairy::Object::UpdateInterface(),
	::CatchAFairy::Object::RenderInterface(),
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
	normalTexture(),
	playCamera(nullptr),
	scenePlay(nullptr),
	operationMethodView(nullptr),
	catchTime(0.0f),
	pointLight(nullptr),
	manaLightGenerationCountAdditionalRatio(Player::MINIMUM_MANA_LIGHT_GENERATE_COUNT_ADDITIONAL_RATIO),
	manaLightGenerationCount(0.0f),
	manaLightGenerationColor(Player::DEFAULT_MANA_LIGHT_COLOR),
	catchedFairyCount{0U}
{
	// ����I�u�W�F�N�g�f�[�^�ǂݍ���
	this->LoadItObjectData();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player(){
	// ����I�u�W�F�N�g�f�[�^�J��
	this->ReleaseItObjectData();
}


/// <summary>
/// �g�|���W�[�^�C�v�̎擾
/// </summary>
/// <returns>�g�|���W�[�^�C�v</returns>
::CatchAFairy::Shader::User::VertexTopology Player::GetTopology() const {
	return this->topology;
}

/// <summary>
/// ���_�o�b�t�@�̎擾
/// </summary>
/// <returns>���_�o�b�t�@</returns>
ID3D11Buffer* Player::GetVertexBuffer() const {
	return this->vertexBuffer;
}

/// <summary>
/// �C���f�b�N�X�o�b�t�@�̎擾
/// </summary>
/// <returns>�C���f�b�N�X�o�b�t�@</returns>
ID3D11Buffer* Player::GetIndexBuffer() const {
	return this->indexBuffer;
}

/// <summary>
/// �C���f�b�N�X���̎擾
/// </summary>
/// <returns>�C���f�b�N�X��</returns>
unsigned int Player::GetIndexCount() const {
	return this->indexCount;
}

/// <summary>
/// �O�ډ~���S���W�̎擾
/// </summary>
/// <returns>�O�ډ~���S���W</returns>
const ::Ishikawa::Math::Float3& Player::GetCircumscribedCircleCenterPosition() const {
	return this->circumscribedCircleCenterPosition;
}

/// <summary>
/// �O�ډ~���a���̎擾
/// </summary>
/// <returns>�O�ډ~���a���</returns>
float Player::GetSquareCircumscribedCircleRadius() const {
	return this->squareCircumscribedCircleRadius;
}

/// <summary>
/// �g�U���˃V�F�[�_���\�[�X�r���[�̎擾
/// </summary>
/// <returns>�g�U���˃V�F�[�_���\�[�X�r���[</returns>
ID3D11ShaderResourceView* Player::GetDiffuseShaderResourceView() const {
	return this->diffuseTexture.GetShaderResourceView();
}

/// <summary>
/// �@���V�F�[�_���\�[�X�r���[�̎擾
/// </summary>
/// <returns>�@���V�F�[�_���\�[�X�r���[</returns>
ID3D11ShaderResourceView* Player::GetNormalShaderResourceView() const {
	return this->normalTexture.GetShaderResourceView();
}

/// <summary>
/// ���[���h�ϊ��s��̎擾
/// </summary>
/// <returns>���[���h�ϊ��s��</returns>
const ::Ishikawa::Math::Matrix& Player::GetWorld() const {
	return this->world;
}

/// <summary>
/// �L�[�{�[�h�̎擾
/// </summary>
/// <returns>�L�[�{�[�h</returns>
const ::Ishikawa::Input::Keyboard& Player::GetKeyboard() const {
	if(this->scenePlay == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("scenePlay is null.");
	}

	::CatchAFairy::Scene::Manager* const sceneManager = this->scenePlay->GetSceneManager();
	if(sceneManager == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("sceneManager is null.");
	}

	::CatchAFairy::Game* const game = sceneManager->GetGame();
	if(game == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("game is null.");
	}

	return game->GetKeyboard();
}


/// <summary>
/// �v���C�J�����̐ݒ�
/// </summary>
/// <param name="_playCamera">�v���C�J����</param>
void Player::SetPlayCamera(PlayCamera* const _playCamera){
	this->playCamera = _playCamera;

	if(this->playCamera != nullptr){
		this->playCamera->OnMoveFollowingTheObject(this->world);
	}
}

/// <summary>
/// �|�C���g���C�g�̐ݒ�
/// </summary>
/// <param name="_pointLight">�|�C���g���C�g</param>
void Player::SetPointLight(PointLight* const _pointLight){
	this->pointLight = _pointLight;
	if(this->pointLight != nullptr){
		this->pointLight->SetFairyBodyLightPosition(
			Player::FAIRY_ID,
			this->world._41 + Player::FAIRY_LIGHT_ANCHOR.x,
			this->world._42 + Player::FAIRY_LIGHT_ANCHOR.y,
			this->world._43 + Player::FAIRY_LIGHT_ANCHOR.z
		);
		this->pointLight->SetFairyBodyLightColor(
			Player::FAIRY_ID,
			Player::FAIRY_LIGHT_COLOR.x,
			Player::FAIRY_LIGHT_COLOR.y,
			Player::FAIRY_LIGHT_COLOR.z,
			Player::FAIRY_LIGHT_COLOR.w
		);
		this->pointLight->SetFairyBodyLightRadius(Player::FAIRY_ID, Player::FAIRY_LIGHT_RADIUS);
		this->pointLight->SetFairyBodyLightLumen(Player::FAIRY_ID, Player::FAIRY_LIGHT_LUMEN);

		this->manaLightGenerationColor = Player::DEFAULT_MANA_LIGHT_COLOR;
		this->pointLight->SetFairyManaLightColor(
			Player::FAIRY_ID,
			Player::DEFAULT_MANA_LIGHT_COLOR.x,
			Player::DEFAULT_MANA_LIGHT_COLOR.y,
			Player::DEFAULT_MANA_LIGHT_COLOR.z,
			Player::DEFAULT_MANA_LIGHT_COLOR.w
		);
		this->pointLight->SetFairyManaLightRadius(Player::FAIRY_ID, Player::MANA_LIGHT_RADIUS);
		this->pointLight->SetFairyManaLightLumen(Player::FAIRY_ID, Player::MANA_LIGHT_LUMEN);
	}
}

/// <summary>
/// ���[���h�ϊ��s��̐ݒ�
/// </summary>
/// <param name="_world">���[���h�ϊ��s��</param>
void Player::SetWorld(const ::Ishikawa::Math::Matrix& _world){
	this->world = _world;

	if(this->playCamera != nullptr){
		this->playCamera->OnMoveFollowingTheObject(this->world);
	}
}


/// <summary>
/// �f�o�C�X�ɓǂݍ��܂ꂽ
/// </summary>
void Player::OnDeviceLoaded(){
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
void Player::OnDeviceRestored(){
	// �������Ȃ�
}

/// <summary>
/// �f�o�C�X���j�����ꂽ
/// </summary>
void Player::OnDeviceLost(){
	// �������Ȃ�
}


/// <summary>
/// �d����߂܂���
/// </summary>
/// <param name="_catchedFairyType">�߂܂����d���̎��</param>
void Player::OnCaughtAFairy(CatchAFairy::Data::EnemyFairyType _catchedFairyType){
	if(_catchedFairyType == CatchAFairy::Data::EnemyFairyType::Count){
		return;
	}

	// �߂܂�����ނ̕ߊl���𑝂₷
	++(this->catchedFairyCount[static_cast<unsigned int>(_catchedFairyType)]);

	// �}�i���C�g�����ʂ𑝂₷
	this->manaLightGenerationCountAdditionalRatio =
		Player::MAXIMUM_MANA_LIGHT_GENERATE_COUNT_ADDITIONAL_RATIO;
	// �}�i���C�g�̐F��߂܂����d���ɑΉ�����F�ɋ߂Â���
	this->manaLightGenerationColor =
		(this->manaLightGenerationColor * 0.3f) +
		(Player::CATCHED_FAIRY_MANA_LIGHT_COLOR[static_cast<unsigned int>(_catchedFairyType)] * 0.7f);
}


/// <summary>
/// ������
/// </summary>
void Player::Initialize(){
	// ���̏�����
	this->InitializeITDeviceDependentResource();

	// �����o�̏�����
	this->diffuseTexture.Initialize();
	this->normalTexture.Initialize();

	// �����ʒu�ݒ�
	this->world._41 = START_POSITION.x;
	this->world._42 = START_POSITION.y;
	this->world._43 = START_POSITION.z;
}


/// <summary>
/// �X�V
/// </summary>
/// <param name="_elapsedTime">�X�V�Ԋu</param>
void Player::Update(float _elapsedTime){
	this->Move(_elapsedTime);
	this->Rotate(_elapsedTime);

	if(this->catchTime > 0.0f){
		this->catchTime -= _elapsedTime;
	}

	this->Catch();

	this->UpdatePointLight(_elapsedTime);
}


/// <summary>
/// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�쐬
/// </summary>
/// <param name="_useShader">���p����V�F�[�_</param>
void Player::CreateVertexBufferAndIndexBuffer(
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
void Player::ReleaseVertexBufferAndIndexBuffer(){
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
void Player::LoadItObjectData(){
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
void Player::ReleaseItObjectData(){
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
/// �ړ�
/// </summary>
/// <param name="_elapsedTime">�X�V�Ԋu</param>
void Player::Move(float _elapsedTime){
	const ::Ishikawa::Input::Keyboard& keyboard = this->GetKeyboard();

	float deltaMoveDistance = _elapsedTime * MOVE_SPEED;
	int moveDirectionNumber = 13;
	if(keyboard.IsPressing(FRONT_MOVE_KEY)){
		// �O
		moveDirectionNumber += 3;
	}
//	if(keyboard.IsPressing(BACK_MOVE_KEY)){
//		// ���
//		moveDirectionNumber -= 3;
//	}
//	if(keyboard.IsPressing(LEFT_MOVE_KEY)){
//		// ��
//		moveDirectionNumber -= 1;
//	}
//	if(keyboard.IsPressing(RIGHT_MOVE_KEY)){
//		// �E
//		moveDirectionNumber += 1;
//	}
	if(keyboard.IsPressing(UP_MOVE_KEY)){
		// ��
		moveDirectionNumber += 9;
	}
	if(keyboard.IsPressing(DOWN_MOVE_KEY)){
		// ��
		moveDirectionNumber -= 9;
	}

	if(moveDirectionNumber != 13){
		int deltaX = moveDirectionNumber%3 - 1;
		int deltaY = moveDirectionNumber/9 - 1;
		int deltaZ = (moveDirectionNumber%9)/3 - 1;

		::Ishikawa::Math::Float3 velocity = ::Ishikawa::Math::Float3::ZERO;
		velocity.x = deltaMoveDistance * deltaX;
		velocity.y = deltaMoveDistance * deltaY;
		velocity.z = deltaMoveDistance * deltaZ;

		int check = deltaX*deltaX + deltaY*deltaY + deltaZ* deltaZ;

		// �΂߈ړ��̏ꍇ�͑��x�����킹��
		switch(check){
		case 2:
			velocity *= 0.7071067812f;
		case 3:
			velocity *= 0.5773502692f;
		}

		// �X�e�[�W�Ƃ̔���ňړ��x�N�g���𒲐�
		velocity = this->scenePlay->CollideMoveVectorAtStage(
			::Ishikawa::Math::Float3::ZERO,
			velocity,
			this->world,
			0.001f
		);

		::Ishikawa::Math::Matrix moveMatrix = ::Ishikawa::Math::Matrix::IDENTITY;
		moveMatrix._41 = velocity.x;  moveMatrix._42 = velocity.y;  moveMatrix._43 = velocity.z;
		this->SetWorld(moveMatrix * this->world);

		if(this->operationMethodView != nullptr){
			this->operationMethodView->OnActPlayer();
		}
	}
}

/// <summary>
/// ��]
/// </summary>
/// <param name="_elapsedTime">�X�V�Ԋu</param>
void Player::Rotate(float _elapsedTime){
	const ::Ishikawa::Input::Keyboard& keyboard = this->GetKeyboard();

	float deltaRotateRadian = _elapsedTime * ROTATE_SPEED;
	int rotateNumber = 0;
	if(keyboard.IsPressing(LEFT_ROTATE_KEY)){
		rotateNumber += 1;
	}
	if(keyboard.IsPressing(RIGHT_ROTATE_KEY)){
		rotateNumber -= 1;
	}

	if(rotateNumber != 0){
		::Ishikawa::Math::Matrix rotateMatrix = ::Ishikawa::Math::CreateRotationMatrixOfAxisY(rotateNumber * deltaRotateRadian);
		this->SetWorld(rotateMatrix * this->world);

		if(this->operationMethodView != nullptr){
			this->operationMethodView->OnActPlayer();
		}
	}
}

/// <summary>
/// �߂܂���
/// </summary>
void Player::Catch(){
	const ::Ishikawa::Input::Keyboard& keyboard = this->GetKeyboard();

	if( keyboard.IsPressed(CATCH_KEY) ||
		(keyboard.IsPressing(CATCH_KEY) && (this->catchTime <= 0.0f))
	){
		this->catchTime = CATCH_INTERVAL;

		this->TryCatchingFairies();

		if(this->operationMethodView != nullptr){
			this->operationMethodView->OnActPlayer();
		}
	}
}


/// <summary>
/// �|�C���g���C�g�̍X�V
/// </summary>
/// <param name="_elapsedTime">�X�V�Ԋu</param>
void Player::UpdatePointLight(float _elapsedTime){
	if(this->pointLight == nullptr) return;

	// �{�̃��C�g�̈ړ�
	this->pointLight->SetFairyBodyLightPosition(
		Player::FAIRY_ID,
		this->world._41 + Player::FAIRY_LIGHT_ANCHOR.x,
		this->world._42 + Player::FAIRY_LIGHT_ANCHOR.y,
		this->world._43 + Player::FAIRY_LIGHT_ANCHOR.z
	);

	this->manaLightGenerationCount += _elapsedTime * this->manaLightGenerationCountAdditionalRatio;
	while(this->manaLightGenerationCount >= 1.0f){
		this->manaLightGenerationCount -= 1.0f;

		// �}�i���C�g�����J�E���g�����䗦�����X�Ɍ��炷
		this->manaLightGenerationCountAdditionalRatio =
			(0.9f * this->manaLightGenerationCountAdditionalRatio) +
			(0.1f * Player::MINIMUM_MANA_LIGHT_GENERATE_COUNT_ADDITIONAL_RATIO);

		// ���X�Ɋ�{�F�ɋ߂Â���
		this->manaLightGenerationColor =
			(this->manaLightGenerationColor * 0.9f) +
			(Player::DEFAULT_MANA_LIGHT_COLOR * 0.1f);
		this->pointLight->SetFairyManaLightColor(
			Player::FAIRY_ID,
			this->manaLightGenerationColor.x,
			this->manaLightGenerationColor.y,
			this->manaLightGenerationColor.z,
			this->manaLightGenerationColor.w
		);

		// �V�K�}�i���C�g����
		this->pointLight->GenerateFairyManaLight(
			Player::FAIRY_ID,
			this->world._41 + Player::FAIRY_LIGHT_ANCHOR.x,
			this->world._42 + Player::FAIRY_LIGHT_ANCHOR.y,
			this->world._43 + Player::FAIRY_LIGHT_ANCHOR.z
		);
	}
}

/// <summary>
/// �d����߂܂��悤�Ƃ���
/// </summary>
void Player::TryCatchingFairies(){
	if(this->scenePlay != nullptr){
		// �r��U�鉹
		this->scenePlay->OnPlaySoundOfKaraburi();

		::CatchAFairy::Object::PlayScene::EnemyFairies& enemyFairies = this->scenePlay->GetEnemyFairies();
		const unsigned int ENEMY_FAIRY_COUNT = enemyFairies.GetWorldCount();
		const ::Ishikawa::Math::Matrix* const ENEMY_FAIRY_WORLD = enemyFairies.GetWorldArray();
		for(unsigned int i=0U; i<ENEMY_FAIRY_COUNT; ++i){
			// �d���̃��[���h���W
			const ::Ishikawa::Math::Float4& fairyWorldPosition = ENEMY_FAIRY_WORLD[i].row4;
			// �v���C�����猩���d���̍��W
			::Ishikawa::Math::Float4 fairyPositionAtPlayer = fairyWorldPosition * this->world.Invert();

			// �����I�ɕ߂܂����邩�ǂ���
			if((fairyPositionAtPlayer.y < FAIRY_CATCHABLE_HEIGHT_MIN)
				|| (fairyPositionAtPlayer.y > FAIRY_CATCHABLE_HEIGHT_MAX)
				){
				// �͂��Ȃ�
				continue;
			}
			// �����I�ɕ߂܂����邩�ǂ���
			::Ishikawa::Math::Float2 distanceOfPlayerLooksFairy(fairyPositionAtPlayer.x, fairyPositionAtPlayer.z);
			float lengthSquaredOfPlayerToFairy = distanceOfPlayerLooksFairy.LengthSquared();
			if(lengthSquaredOfPlayerToFairy < (FAIRY_CATCHABLE_RADIUS * FAIRY_CATCHABLE_RADIUS)){
				// �m�ۉ�
				this->scenePlay->OnPlaySoundOfCatched();
				// �m�ۂ�ʒm
				this->OnCaughtAFairy(enemyFairies.GetFairyType(i));
				enemyFairies.OnCatched(i);
				continue;
			}
			if(lengthSquaredOfPlayerToFairy > (FAIRY_CATCHABLE_ANGLE_RADIUS * FAIRY_CATCHABLE_ANGLE_RADIUS)){
				// �͂��Ȃ�
				continue;
			}
			// �p�x�I�ɕ߂܂����邩�ǂ���
			::Ishikawa::Math::Float2 directionOfPlayerLooksFairy;
			distanceOfPlayerLooksFairy.Normalize(&directionOfPlayerLooksFairy);
			if(directionOfPlayerLooksFairy.y < FAIRY_CATCHABLE_ANGLE_COS) continue;


			// ���ׂĂ̏�����ʉ߂����̂ŕ߂܂�����

			// �m�ۉ�
			this->scenePlay->OnPlaySoundOfCatched();
			// �m�ۂ�ʒm
			this->OnCaughtAFairy(enemyFairies.GetFairyType(i));
			enemyFairies.OnCatched(i);
		}
	}
}
