//=============================================================================
//	ScenePlay.cpp
//
//	�v���C�V�[���̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ScenePlay.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Common/PatternBase/SingletonManager.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "Ishikawa/Math/ITCollision.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "Ishikawa/DirectX/ITViewport.h"
#include "Ishikawa/DirectX/Texture/ITTextureData.h"
#include "Ishikawa/CriAdx2Le/ITCriAdx2Le.h"
#include "CatchAFairy/Game.h"
#include "CatchAFairy/Shader/User/ShaderVertexTopology.h"
#include "SceneManager.h"
#include "CatchAFairy/Object/RenderObjectInterface.h"
#include "CatchAFairy/Object/RenderSameObjectInterface.h"
#include "CatchAFairy/Object/CameraObjectInterface.h"
#include "SceneResult.h"
#include "CatchAFairy/Sound/CueSheet_0.h"

using namespace CatchAFairy::Scene;


/// <summary>�o�̓C���f�b�N�X��</summary>
const unsigned int Play::OUTPUT_INDEX_COUNT = 4U;
/// <summary>�X�e�[�W�T�C�Y�̔���</summary>
const float Play::STAGE_SIZE_HALF = 10.0f;
/// <summary>�f�B���N�V���i�����C�g�̐F</summary>
const ::Ishikawa::Math::Float4 Play::DIRECTIONAL_LIGHT_COLOR(0.95f, 1.0f, 1.0f, 1.0f);


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Play::Play() :
	outputRenderTargetTexture(),
	directionalLight(),
	playCamera(),
	ground(),
	player(),
	enemyFairies(),
	trees(),
	pointLight(),
	guiManager(),
	fairyPositionView(),
	timeView(),
	operationMethodView(),
	bgmSoundId(0U),
	isEnd(0)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Play::~Play(){
	// �f�o�C�X�ˑ����\�[�X�J��
	this->ReleaseDeviceDependentResources();
}


/// <summary>
/// �Q�[���̎擾
/// </summary>
/// <returns>�Q�[��</returns>
::CatchAFairy::Game* Play::GetGame() const {
	// �}�l�[�W���擾
	Manager* const manager = this->GetSceneManager();
	if(manager == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("manager is NULL.");
	}

	// �Q�[���擾
	::CatchAFairy::Game* const game = manager->GetGame();
	if(game == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("game is NULL.");
	}

	return game;
}

/// <summary>
/// �Q�[���̈ˑ����Ă��鎩��f�o�C�X�̎擾
/// </summary>
/// <returns>�Q�[���̈ˑ����Ă��鎩��f�o�C�X</returns>
::Ishikawa::DirectX::Device::Device* Play::GetGameDependentITDevice() const{
	// �Q�[���擾
	::CatchAFairy::Game* const game = this->GetGame();

	// ����f�o�C�X�擾
	::Ishikawa::DirectX::Device::Device* const itDevice = game->GetDependentITDevice();
	if(itDevice == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("itDevice is NULL.");
	}

	return itDevice;
}

/// <summary>
/// �L�[�{�[�h�̎擾
/// </summary>
/// <returns></returns>
const ::Ishikawa::Input::Keyboard& Play::GetKeyboard() const {
	// �Q�[���擾
	::CatchAFairy::Game* const game = this->GetGame();

	return (game->GetKeyboard());
}


/// <summary>
/// �Q�[���̃f�o�C�X���ǂݍ��܂ꂽ
/// </summary>
void Play::OnLoadedGameDevice(){
	// �Q�[���f�o�C�X�Ƀ��\�[�X��ǉ�
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetGameDependentITDevice();
	itDevice->AddDependentResource(&(this->outputRenderTargetTexture));
	itDevice->AddDependentResource(&(this->ground));
	itDevice->AddDependentResource(&(this->player));
	itDevice->AddDependentResource(&(this->enemyFairies));
	itDevice->AddDependentResource(&(this->trees));
	this->playCamera.AddITDeviceDependentResources(itDevice);
	this->directionalLight.AddITDeviceDependentResources(itDevice);
	this->guiManager.AddITDeviceDependentResources(itDevice);

	// �f�o�C�X�ˑ����\�[�X�쐬
	this->CreateDeviceDependentResources();
}

/// <summary>
/// �Q�[���̃f�o�C�X���������ꂽ
/// </summary>
void Play::OnRestoredGameDevice(){
	// �f�o�C�X�ˑ����\�[�X�쐬
	this->CreateDeviceDependentResources();
}

/// <summary>
/// �Q�[���̃f�o�C�X���j�����ꂽ
/// </summary>
void Play::OnLostGameDevice(){
	// �f�o�C�X�ˑ����\�[�X�J��
	this->ReleaseDeviceDependentResources();
}

/// <summary>
/// ���U�鉹�̍Đ�
/// </summary>
void Play::OnPlaySoundOfKaraburi() const {
	::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>().Play(CRI_CUESHEET_0_TEASI_KARABURI03);
}

/// <summary>
/// �ߊl���̍Đ�
/// </summary>
void Play::OnPlaySoundOfCatched() const {
	::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>().Play(CRI_CUESHEET_0_SE_MAOUDAMASHII_CHIME13);
}


/// <summary>
/// �T�C�Y�ύX
/// </summary>
/// <param name="_width">��</param>
/// <param name="_height">����</param>
void Play::ChangeSceneSize(unsigned int _width, unsigned int _height){
	this->outputRenderTargetTexture.ChangeSize(_width, _height);
}


/// <summary>
/// ������
/// </summary>
void Play::Initialize(){
	// ���̏�����
	this->InitializeScene();

	// �����o�̏�����
	this->InitializeOutputResources();
	this->InitializeObjectResources();
}


/// <summary>
/// �J�n
/// </summary>
void Play::Start(){
	// BGM�J�n
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	this->bgmSoundId = itCriAdx2Le.Play(CRI_CUESHEET_0_WINDYBIRD);
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="_elapsedTime">�o�ߎ���</param>
void Play::Update(float _elapsedTime){
	// �e�����o�X�V
	this->player.Update(_elapsedTime);
	this->enemyFairies.Update(_elapsedTime);
	this->trees.Update(_elapsedTime);
	this->playCamera.Update(_elapsedTime);
	this->pointLight.Update(_elapsedTime);

	this->fairyPositionView.Update(_elapsedTime);
	this->timeView.Update(_elapsedTime);
	this->operationMethodView.Update(_elapsedTime);

	// �I���Ȃ烊�U���g�V�[���w
	if(this->IsEnd()){
		Manager* const manager = this->GetSceneManager();
		if(manager != nullptr){
			Result* const result = new Result();
			// �ߊl�����󂯓n��
			for(unsigned int i=0; i<static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count); ++i){
				CatchAFairy::Data::EnemyFairyType type = static_cast<CatchAFairy::Data::EnemyFairyType>(i);
				result->SetCatchedFairyCount(type, this->player.GetCatchedFairyCount(type));
			}
			manager->ChangeScene(result);
		}
	}
}

/// <summary>
/// �`��
/// </summary>
void Play::Render(){
	// �e�`��
	this->RenderOfCameraEyeFirst();
	this->RenderOfDirectionalLight();
	this->playCamera.RenderOfShadowShader();
	this->RenderOfCameraEyeReflectionLight();
	this->RenderOfCameraEyePointLight();
	this->playCamera.RenderOfLastShader();

	// GUI�`��
	this->guiManager.Render();
}

/// <summary>
/// �I��
/// </summary>
void Play::End(){
	// BGM��~
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	itCriAdx2Le.Stop(this->bgmSoundId);
}


/// <summary>
/// �X�e�[�W�ƈړ��x�N�g���̔���
/// </summary>
/// <param name="_startPosition">�x�N�g���n�_</param>
/// <param name="_moveVector">�ړ��x�N�g��</param>
/// <param name="_localToWorld">�ړ��x�N�g����Ԃ��烏�[���h��Ԃւ̕ϊ��s��</param>
/// <param name="_reflectionRate">���˗�</param>
/// <returns>�����̈ړ��x�N�g��</returns>
::Ishikawa::Math::Float3 Play::CollideMoveVectorAtStage(
	const ::Ishikawa::Math::Float3& _startPosition,
	const ::Ishikawa::Math::Float3& _moveVector,
	const ::Ishikawa::Math::Matrix& _localToWorld,
	float _reflectionRate
) const {
	::Ishikawa::Math::Float3 returnVector = _moveVector;

	// �X�e�[�W�O�ɏo�Ȃ��悤�ɂ���
	// �E
	returnVector = ::Ishikawa::Math::Collision::ReflectVectorAtPlanar(
		_startPosition,
		returnVector,
		_localToWorld,
		::Ishikawa::Math::Float3( 0.0f, 0.0f, STAGE_SIZE_HALF),
		::Ishikawa::Math::Float3( 0.0f, 0.0f,-1.0f),
		_reflectionRate
	);
	// ��
	returnVector = ::Ishikawa::Math::Collision::ReflectVectorAtPlanar(
		_startPosition,
		returnVector,
		_localToWorld,
		::Ishikawa::Math::Float3( 0.0f, 0.0f,-STAGE_SIZE_HALF),
		::Ishikawa::Math::Float3( 0.0f, 0.0f, 1.0f),
		_reflectionRate
	);
	// ��O
	returnVector = ::Ishikawa::Math::Collision::ReflectVectorAtPlanar(
		_startPosition,
		returnVector,
		_localToWorld,
		::Ishikawa::Math::Float3( STAGE_SIZE_HALF, 0.0f, 0.0f),
		::Ishikawa::Math::Float3(-1.0f, 0.0f, 0.0f),
		_reflectionRate
	);
	// ��
	returnVector = ::Ishikawa::Math::Collision::ReflectVectorAtPlanar(
		_startPosition,
		returnVector,
		_localToWorld,
		::Ishikawa::Math::Float3(-STAGE_SIZE_HALF, 0.0f, 0.0f),
		::Ishikawa::Math::Float3( 1.0f, 0.0f, 0.0f),
		_reflectionRate
	);
	// ��
	returnVector = ::Ishikawa::Math::Collision::ReflectVectorAtPlanar(
		_startPosition,
		returnVector,
		_localToWorld,
		::Ishikawa::Math::Float3(0.0f, 4.4f, 0.0f),
		::Ishikawa::Math::Float3( 0.0f,-1.0f, 0.0f),
		_reflectionRate
	);
	// ��
	returnVector = ::Ishikawa::Math::Collision::ReflectVectorAtPlanar(
		_startPosition,
		returnVector,
		_localToWorld,
		::Ishikawa::Math::Float3(0.0f, 0.1f, 0.0f),
		::Ishikawa::Math::Float3( 0.0f, 1.0f, 0.0f),
		_reflectionRate
	);

	// �؁X�Ƃ̔���
	returnVector = this->trees.CollideVector(
		_startPosition,
		returnVector,
		_localToWorld,
		_reflectionRate
	);

	return returnVector;
}


/// <summary>
/// �f�o�C�X�ˑ����\�[�X�쐬
/// </summary>
void Play::CreateDeviceDependentResources(){
	// �o�̓e�N�X�`���쐬
	this->CreateOutputTextures();
	// �`��I�u�W�F�N�g�̃��\�[�X�쐬
	this->playCamera.CreateDeviceDependentResources();
	this->directionalLight.CreateDeviceDependentResources();
	this->ground.CreateVertexBufferAndIndexBuffer(this->playCamera.GetFirstShaderUser());
	this->player.CreateVertexBufferAndIndexBuffer(this->playCamera.GetFirstShaderUser());
	this->enemyFairies.CreateVertexBufferAndIndexBuffer(this->playCamera.GetFirstShaderUser());
	this->trees.CreateVertexBufferAndIndexBuffer(this->playCamera.GetFirstShaderUser());
	this->pointLight.CreateDeviceDependentResources();

	this->guiManager.CreateDeviceDependentResources();
}

/// <summary>
/// �f�o�C�X�ˑ����\�[�X�J��
/// </summary>
void Play::ReleaseDeviceDependentResources(){
	// �`��I�u�W�F�N�g�̃��\�[�X�J��
	this->pointLight.ReleaseDeviceDependentResources();
	this->trees.ReleaseVertexBufferAndIndexBuffer();
	this->enemyFairies.ReleaseVertexBufferAndIndexBuffer();
	this->player.ReleaseVertexBufferAndIndexBuffer();
	this->ground.ReleaseVertexBufferAndIndexBuffer();
	this->directionalLight.ReleaseDeviceDependentResources();
	this->playCamera.ReleaseDeviceDependentResources();

	this->guiManager.ReleaseDeviceDependentResources();
}


/// <summary>
/// �o�̓e�N�X�`���쐬
/// </summary>
void Play::CreateOutputTextures(){
	// �ݒ�擾
	unsigned int sampleDescCount = 1U;
	unsigned int sampleDescQuality = 0U;
	::Ishikawa::DirectX::Texture::Dimention textureDimention = ::Ishikawa::DirectX::Texture::Dimention::Texture2D;

	// �e�N�X�`���쐬
	this->outputRenderTargetTexture.Create(
		::Ishikawa::DirectX::Texture::BindableFlag::SR_RT,
		Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		::Ishikawa::DirectX::Texture::Format::R8G8B8A8UNorm,
		0U,
		sampleDescCount,
		sampleDescQuality,
		textureDimention
	);
}



/// <summary>
/// �o�̓��\�[�X������
/// </summary>
void Play::InitializeOutputResources(){
	this->outputRenderTargetTexture.Initialize();
}

/// <summary>
/// �I�u�W�F�N�g���\�[�X������
/// </summary>
void Play::InitializeObjectResources(){
	this->directionalLight.Initialize();
	this->playCamera.Initialize();
	this->ground.Initialize();
	this->player.Initialize();
	this->enemyFairies.Initialize();
	this->trees.Initialize();
	this->pointLight.Initialize();

	this->directionalLight.SetColor(DIRECTIONAL_LIGHT_COLOR);
	this->playCamera.SetDirectionalLight(&(this->directionalLight));
	this->playCamera.SetOutputTargetTexture(&(this->outputRenderTargetTexture));
	this->player.SetPlayCamera(&(this->playCamera));
	this->player.SetPointLight(&(this->pointLight));
	this->player.SetScenePlay(this);
	this->player.SetOperationMethodView(&(this->operationMethodView));
	this->pointLight.SetReflectionLightShaderUser(this->playCamera.GetReflectionLightShaderUser());
	this->enemyFairies.SetPointLight(&(this->pointLight));
	this->enemyFairies.SetPlayer(&(this->player));


	this->guiManager.Initialize();

	this->guiManager.SetOutputTargetTexture(&(this->outputRenderTargetTexture));
	this->fairyPositionView.SetManager(&(this->guiManager));
	this->fairyPositionView.SetFairy(&(this->enemyFairies));
	this->fairyPositionView.SetPlayer(&(this->player));
	this->timeView.SetScenePlay(this);
	this->timeView.SetManager(&(this->guiManager));
	this->operationMethodView.SetManager(&(this->guiManager));
}


/// <summary>
/// �J�������_1�Ԃ̕`��
/// </summary>
void Play::RenderOfCameraEyeFirst(){
	this->playCamera.DrawBeforeOfFirstShader();
	
	// �e�I�u�W�F�N�g�̕`��
	this->playCamera.RenderObjectToFirstShader(this->ground);
	this->playCamera.RenderObjectToFirstShader(this->player);
	this->playCamera.RenderSameObjectToFirstShader(this->enemyFairies);
	this->playCamera.RenderSameObjectToFirstShader(this->trees);
	
	this->playCamera.DrawAfterOfFirstShader();
}

/// <summary>
/// ���s���C�g�̕`��
/// </summary>
void Play::RenderOfDirectionalLight(){
	this->directionalLight.DirectionalLightShaderDrawBefore();

	this->directionalLight.RenderObjectToDirectionalLightShader(this->ground);
	this->directionalLight.RenderObjectToDirectionalLightShader(this->player);
	this->directionalLight.RenderSameObjectToDirectionalLightShader(this->enemyFairies);
	this->directionalLight.RenderSameObjectToDirectionalLightShader(this->trees);

	this->directionalLight.DirectionalLightShaderDrawAfter();
}

/// <summary>
/// �J�������_���ˌ��̕`��
/// </summary>
void Play::RenderOfCameraEyeReflectionLight(){
	this->playCamera.DrawBeforeOfReflectionLightShader();

	// �e�I�u�W�F�N�g�̕`��
	this->playCamera.RenderOfReflectionLightShader(this->pointLight);

	this->playCamera.DrawAfterOfReflectionLightShader();
}

/// <summary>
/// �J�������_�|�C���g���C�g�̕`��
/// </summary>
void Play::RenderOfCameraEyePointLight(){
	this->playCamera.DrawBeforeOfPointLightShader();

	// �e�I�u�W�F�N�g�̕`��
	this->playCamera.RenderOfPointLightShader(this->pointLight);

	this->playCamera.DrawAfterOfPointLightShader();
}
