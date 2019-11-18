//=============================================================================
//	SceneManager.cpp
//
//	�V�[���}�l�[�W���̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "SceneManager.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "Ishikawa/DirectX/ITViewport.h"
#include "SceneBase.h"
#include "CatchAFairy/Game.h"
#include "CatchAFairy/Shader/User/ShaderVertexTopology.h"

using namespace CatchAFairy::Scene;


/// <summary>�V�[���o�̓e�N�X�`���T�C�Y</summary>
const unsigned int Manager::SCENE_OUTPUT_TEXTURE_SIZE = 1024U;
/// <summary>�V�[���o�̓e�N�X�`���̕`�敝</summary>
const unsigned int Manager::SCENE_OUTPUT_TEXTURE_WIDTH = 800U;
/// <summary>�V�[���o�̓e�N�X�`���̕`�捂��</summary>
const unsigned int Manager::SCENE_OUTPUT_TEXTURE_HEIGHT = 600U;
/// <summary>�V�[���o�̓e�N�X�`���̃A�X�y�N�g��</summary>
const float Manager::SCENE_OUTPUT_TEXTURE_ASPECT = static_cast<float>(SCENE_OUTPUT_TEXTURE_HEIGHT) / SCENE_OUTPUT_TEXTURE_WIDTH;
/// <summary>�]���̐F</summary>
const ::Ishikawa::Math::Float4 Manager::MARGIN_COLOR(0.0f, 0.0f, 0.0f, 1.0f);


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Manager::Manager() :
	game(nullptr),
	sceneListEdge(),
	outputScreenShaderUser(),
	isChangedSceneFrame(0)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Manager::~Manager(){
	// �������Ȃ�
}


/// <summary>
/// �Q�[�����ˑ����Ă��鎩��f�o�C�X�̎擾
/// </summary>
/// <returns>�Q�[�����ˑ����Ă��鎩��f�o�C�X</returns>
::Ishikawa::DirectX::Device::Device* Manager::GetGameDependentITDevice() const{
	if(this->game == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("this game is NULL.");
	}

	// �Q�[�����ˑ����Ă��鎩��f�o�C�X�̎擾
	::Ishikawa::DirectX::Device::Device* const itDevice = this->game->GetDependentITDevice();
	if(itDevice == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("itDevice is NULL.");
	}

	return itDevice;
}

/// <summary>
/// ���݂̃V�[�����擾
/// </summary>
/// <returns>���݂̃V�[��</returns>
Interface* Manager::GetCurrentScene() const{
	// �V�[�����X�g�̈�Ԍ������݂̃V�[���Ƃ���
	return this->sceneListEdge.GetBeforeScene();
}


/// <summary>
/// �Q�[���̃f�o�C�X���ǂݍ��܂ꂽ
/// </summary>
void Manager::OnLoadedGameDevice(){
	// �����o��o�^
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetGameDependentITDevice();
	itDevice->AddDependentResource(&(this->outputScreenShaderUser));

	// �S�ẴV�[���ɒʒm
	Interface* scene = this->sceneListEdge.GetNextScene();
	while(!(scene->IsSceneListEdge())){
		scene->OnLoadedGameDevice();

		scene = scene->GetNextScene();
	}
}

/// <summary>
/// �Q�[���̃f�o�C�X���������ꂽ
/// </summary>
void Manager::OnRestoredGameDevice(){
	// �S�ẴV�[���ɒʒm
	Interface* scene = this->sceneListEdge.GetNextScene();
	while(!(scene->IsSceneListEdge())){
		scene->OnRestoredGameDevice();

		scene = scene->GetNextScene();
	}
}

/// <summary>
/// �Q�[���̃f�o�C�X���j�����ꂽ
/// </summary>
void Manager::OnLostGameDevice(){
	// �S�ẴV�[���ɒʒm
	Interface* scene = this->sceneListEdge.GetNextScene();
	while(!(scene->IsSceneListEdge())){
		scene->OnLostGameDevice();

		scene = scene->GetNextScene();
	}
}


/// <summary>
/// ������
/// </summary>
void Manager::Initialize(){
	// �����o�̏�����
	this->outputScreenShaderUser.Initialize();
	this->sceneListEdge.Initialize();
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="_elapsedTime">�o�ߎ���</param>
void Manager::Update(float _elapsedTime){
	Interface* const currentScene = this->GetCurrentScene();
	currentScene->Update(_elapsedTime);
}

/// <summary>
/// �`��
/// </summary>
void Manager::Render(){
	// �V�[���ύX�����t���[���ł͕`������Ȃ�
	if(this->isChangedSceneFrame != 0){
		this->isChangedSceneFrame = 0;
		return;
	}

	Interface* const currentScene = this->GetCurrentScene();
	currentScene->Render();
}

/// <summary>
/// ��ʂ֕`��
/// </summary>
/// <param name="_renderTargetView">�X�N���[���̃����_�[�^�[�Q�b�g�r���[</param>
/// <param name="_depthStencilView">�X�N���[���̃f�v�X�X�e���V���r���[</param>
/// <param name="_width">�X�N���[���̕�</param>
/// <param name="_height">�X�N���[���̍���</param>
void Manager::RenderToScreen(
	ID3D11RenderTargetView* const _renderTargetView,
	ID3D11DepthStencilView* const _depthStencilView,
	unsigned int _width,
	unsigned int _height
){
	if(_renderTargetView == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_renderTargetView is NULL.");
	}
	if(_depthStencilView == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_depthStencilView is NULL.");
	}

	// ���݂̃V�[���̏o�͉摜���擾
	Interface* const currentScene = this->GetCurrentScene();
	ID3D11ShaderResourceView* const currentSceneSRV = currentScene->GetShaderResourceView();
	// �o�͉摜���Ȃ���Ή������Ȃ�
	if(currentSceneSRV == nullptr) return;


	// �o�̓��\�[�X
	::CatchAFairy::Shader::User::OutputScreen::OutputResource outputResource;
	outputResource.renderTargetView = _renderTargetView;
//	outputResource.depthStencilView = _depthStencilView;
	::Ishikawa::DirectX::Viewport viewport;
	viewport.leftTopX	= 0.0f;
	viewport.leftTopY	= 0.0f;
	viewport.width		= static_cast<float>(_width);
	viewport.height		= static_cast<float>(_height);
	viewport.minDepth	= 0.0f;
	viewport.maxDepth	= 1.0f;
	outputResource.viewport = &viewport;

	// �V�[���o�͂̃A�X�y�N�g�䂪�ς��Ȃ��悤�ɒ��_�ʒu�𒲐�����
	float widthRate = viewport.width / SCENE_OUTPUT_TEXTURE_WIDTH;
	float heightRate = viewport.height / SCENE_OUTPUT_TEXTURE_HEIGHT;
	float x = 1.0f, y = 1.0f;
	if(widthRate > heightRate){
		x = heightRate / widthRate;
	}
	else{
		y = widthRate / heightRate;
	}
	// ���_���
	const unsigned int vertexCount = 4U;
	::CatchAFairy::Shader::User::OutputScreen::VSInput vertices[vertexCount];
	vertices[0].position			= ::Ishikawa::Math::Float2(-x, y);
	vertices[0].textureCoordinate	= ::Ishikawa::Math::Float2( 0.0f, 0.0f);
	vertices[1].position			= ::Ishikawa::Math::Float2(-x,-y);
	vertices[1].textureCoordinate	= ::Ishikawa::Math::Float2( 0.0f, 1.0f);
	vertices[2].position			= ::Ishikawa::Math::Float2( x, y);
	vertices[2].textureCoordinate	= ::Ishikawa::Math::Float2( 1.0f, 0.0f);
	vertices[3].position			= ::Ishikawa::Math::Float2( x,-y);
	vertices[3].textureCoordinate	= ::Ishikawa::Math::Float2( 1.0f, 1.0f);
	// ���_�o�b�t�@�쐬
	ID3D11Buffer* const vertexBuffer = this->outputScreenShaderUser.CreateVertexBuffer(vertices, vertexCount);

	// �C���f�b�N�X���
	const unsigned int indexCount = 4U;
	const unsigned long indices[indexCount] = { 0U, 1U, 2U, 3U };
	// �C���f�b�N�X�o�b�t�@�쐬
	ID3D11Buffer* const indexBuffer = this->outputScreenShaderUser.CreateIndexBuffer(indices, indexCount);

	// ���̓��\�[�X
	::CatchAFairy::Shader::User::OutputScreen::RenderUseResource inputResource;
	inputResource.topology = ::CatchAFairy::Shader::User::VertexTopology::TriangleStrip;
	inputResource.vertexBuffer = vertexBuffer;
	inputResource.indexBuffer = indexBuffer;
	inputResource.indexCount = indexCount;
	inputResource.shaderResourceView = currentSceneSRV;

	// �`��
	this->outputScreenShaderUser.DrawBefore(outputResource, &MARGIN_COLOR);
	this->outputScreenShaderUser.Draw(inputResource);
	this->outputScreenShaderUser.DrawAfter();

	// �o�b�t�@�J��
	this->outputScreenShaderUser.ReleaseBuffer(indexBuffer);
	this->outputScreenShaderUser.ReleaseBuffer(vertexBuffer);
}

/// <summary>
/// �I��
/// </summary>
void Manager::Finalize(){
	this->ClearSceneList();
}


/// <summary>
/// �v�b�V��
/// </summary>
/// <param name="_scene">�V�[��</param>
void Manager::PushScene(Base* const _scene){
	if(_scene == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_scene is NULL.");
	}
	// �V�[���̏�����
	_scene->Initialize();

	// ���݂̃V�[�����~�߂�
	Interface* const currentScene = this->GetCurrentScene();
	currentScene->SetUpdateFlag(false);
	currentScene->SetRenderFlag(false);

	// �}�l�[�W���̐ݒ�
	_scene->SetSceneManager(this);
	// �f�o�C�X���ǂݍ��܂�Ă���Ȃ�A�f�o�C�X�ǂݍ��ݕ����𓮂���
	if(this->game != nullptr){
		if(this->game->GetDependentITDevice() != nullptr){
			_scene->OnLoadedGameDevice();
		}
	}

	// �V�[�����X�g�̈�Ԍ��ɒǉ�
	Interface* const tail = this->sceneListEdge.GetBeforeScene();
	_scene->SetBeforeScene(tail);
	_scene->SetNextScene(&(this->sceneListEdge));
	tail->SetNextScene(_scene);
	this->sceneListEdge.SetBeforeScene(_scene);

	// �V�[���J�n
	_scene->Start();

	// �V�[���ύX���L�^
	this->isChangedSceneFrame = 1;
}

/// <summary>
/// �|�b�v
/// </summary>
void Manager::PopScene(){
	// ���݂̃V�[�����I��
	Interface* const currentScene = this->GetCurrentScene();
	currentScene->End();

	// �O�̃V�[�����ĊJ����
	Interface* const beforeScene = currentScene->GetBeforeScene();
	beforeScene->SetUpdateFlag(true);
	beforeScene->SetRenderFlag(true);

	// �V�[���폜
	delete currentScene;
}

/// <summary>
/// �`�F���W
/// </summary>
/// <param name="_scene">�V�[��</param>
void Manager::ChangeScene(Base* const _scene){
	if(_scene == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_scene is NULL.");
	}

	// ���݂̃V�[�����|�b�v���āA�V�����V�[�����v�b�V��
	this->PopScene();
	this->PushScene(_scene);
}

/// <summary>
/// �S�폜
/// </summary>
void Manager::ClearSceneList(){
	// �S�ẴV�[�����I�������Ă���
	Interface* scene = this->sceneListEdge.GetBeforeScene();
	while(!(scene->IsSceneListEdge())){
		scene->End();

		Interface* const deleteScene = scene;
		scene = scene->GetBeforeScene();

		delete deleteScene;
	}
}
