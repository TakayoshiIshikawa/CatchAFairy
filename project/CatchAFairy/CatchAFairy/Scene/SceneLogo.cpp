//=============================================================================
//	SceneLogo.cpp
//
//	���S�V�[���̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "SceneLogo.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "Ishikawa/DirectX/ITViewport.h"
#include "Ishikawa/DirectX/Texture/ITTextureData.h"
#include "CatchAFairy/Game.h"
#include "CatchAFairy/Shader/User/ShaderVertexTopology.h"
#include "SceneManager.h"
#include "SceneTitle.h"

using namespace CatchAFairy::Scene;


/// <summary>
/// �X�V�֐��̎��
/// </summary>
enum class Logo::UpdateFunctionType{
	/// <summary>�o��</summary>
	Appear,
	/// <summary>�\��</summary>
	Print,
	/// <summary>�B��</summary>
	Hide
};


/// <summary>�o������</summary>
const float Logo::APPEARING_TIME = 1.0f;
/// <summary>�\������</summary>
const float Logo::PRINT_TIME = 2.0f;
/// <summary>�B������</summary>
const float Logo::HIDING_TIME = 1.5f;
/// <summary>�C���f�b�N�X��</summary>
const unsigned int Logo::INDEX_COUNT = 4U;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Logo::Logo() :
	outputRenderTargetTexture(),
	logoShaderUser(),
	logoTexture(),
	viewColor(0.0f),
	currentUpdateType(UpdateFunctionType::Appear),
	timer(0.0f),
	vertexBuffer(nullptr),
	indexBuffer(nullptr)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Logo::~Logo(){
	// �o�b�t�@�J��
	this->ReleaseIndexBuffer();
	this->ReleaseVertexBuffer();
}


/// <summary>
/// �Q�[���̈ˑ����Ă��鎩��f�o�C�X�̎擾
/// </summary>
/// <returns>�Q�[���̈ˑ����Ă��鎩��f�o�C�X</returns>
::Ishikawa::DirectX::Device::Device* Logo::GetGameDependentITDevice() const{
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

	// ����f�o�C�X�擾
	::Ishikawa::DirectX::Device::Device* const itDevice = game->GetDependentITDevice();
	if(itDevice == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("itDevice is NULL.");
	}

	return itDevice;
}


/// <summary>
/// �Q�[���̃f�o�C�X���ǂݍ��܂ꂽ
/// </summary>
void Logo::OnLoadedGameDevice(){
	// �Q�[���f�o�C�X�Ƀe�N�X�`����ǉ�
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetGameDependentITDevice();
	itDevice->AddDependentResource(&(this->outputRenderTargetTexture));
	itDevice->AddDependentResource(&(this->logoShaderUser));
	itDevice->AddDependentResource(&(this->logoTexture));

	// �o�̓e�N�X�`���쐬
	this->CreateOutputTextures();
	// �摜�ǂݍ���
	this->logoTexture.Load(L"Resources/Textures/Logo.png");
	// �o�b�t�@�쐬
	this->CreateVertexBuffer();
	this->CreateIndexBuffer();
}

/// <summary>
/// �Q�[���̃f�o�C�X���������ꂽ
/// </summary>
void Logo::OnRestoredGameDevice(){
	// �������Ȃ�
}

/// <summary>
/// �Q�[���̃f�o�C�X���j�����ꂽ
/// </summary>
void Logo::OnLostGameDevice(){
	// �������Ȃ�
}


/// <summary>
/// �T�C�Y�ύX
/// </summary>
/// <param name="_width">��</param>
/// <param name="_height">����</param>
void Logo::ChangeSceneSize(unsigned int _width, unsigned int _height){
	this->outputRenderTargetTexture.ChangeSize(_width, _height);
}


/// <summary>
/// ������
/// </summary>
void Logo::Initialize(){
	// ���̏�����
	this->InitializeScene();

	// �����o�̏�����
	this->outputRenderTargetTexture.Initialize();
	this->logoShaderUser.Initialize();
	this->logoTexture.Initialize();
}


/// <summary>
/// �J�n
/// </summary>
void Logo::Start(){
	// �������Ȃ�
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="_elapsedTime">�o�ߎ���</param>
void Logo::Update(float _elapsedTime){
	switch(this->currentUpdateType){
	case UpdateFunctionType::Appear:
		this->AppearLogo(_elapsedTime);
		break;

	case UpdateFunctionType::Print:
		this->PrintLogo(_elapsedTime);
		break;

	case UpdateFunctionType::Hide:
		this->HideLogo(_elapsedTime);
		break;

	default:
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("current update type is unknown.");
	}
}

/// <summary>
/// �`��
/// </summary>
void Logo::Render(){
	// �o�̓��\�[�X
	::CatchAFairy::Shader::User::Logo::OutputResource outputResource;
	outputResource.renderTargetView = this->outputRenderTargetTexture.GetRenderTargetView();

	unsigned int width = 0U, height = 0U;
	this->outputRenderTargetTexture.GetTextureSize(&width, &height);
	::Ishikawa::DirectX::Viewport viewport;
	viewport.leftTopX	= 0.0f;
	viewport.leftTopY	= 0.0f;
	viewport.width		= static_cast<float>(width);
	viewport.height		= static_cast<float>(height);
	viewport.minDepth	= 0.0f;
	viewport.maxDepth	= 1.0f;
	outputResource.viewport = &viewport;

	// ���̓��\�[�X
	::CatchAFairy::Shader::User::Logo::RenderUseResource inputResource;
	inputResource.constantBuffer.colorRate = ::Ishikawa::Math::Float4(this->viewColor, this->viewColor, this->viewColor, 1.0f);
	inputResource.topology = ::CatchAFairy::Shader::User::VertexTopology::TriangleStrip;
	inputResource.vertexBuffer = this->vertexBuffer;
	inputResource.indexBuffer = this->indexBuffer;
	inputResource.indexCount = INDEX_COUNT;
	inputResource.shaderResourceView = this->logoTexture.GetShaderResourceView();

	// �`��
	const ::Ishikawa::Math::Float4 CLEAR_COLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->logoShaderUser.DrawBefore(outputResource, &CLEAR_COLOR);
	this->logoShaderUser.Draw(inputResource);
	this->logoShaderUser.DrawAfter();
}

/// <summary>
/// �I��
/// </summary>
void Logo::End(){
	// �������Ȃ�
}


/// <summary>
/// �o�̓e�N�X�`���쐬
/// </summary>
void Logo::CreateOutputTextures(){
	// �ݒ�擾
	unsigned int sampleDescCount = 1U;
	unsigned int sampleDescQuality = 0U;
	::Ishikawa::DirectX::Texture::Dimention textureDimention = ::Ishikawa::DirectX::Texture::Dimention::Texture2D;

	// �}���`�T���v�����O�e�N�X�`���ɂ���
//	::Ishikawa::DirectX::Texture::GetMultisampleAntiAliasingDescData(
//		&sampleDescCount, &sampleDescQuality,
//		&textureDimention,
//		itDevice->GetDevice()
//	);

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
/// ���_�o�b�t�@�쐬
/// </summary>
void Logo::CreateVertexBuffer(){
	// ���_���
	const unsigned int vertexCount = 4U;
	::CatchAFairy::Shader::User::Logo::VSInput vertices[vertexCount];
	vertices[0].position			= ::Ishikawa::Math::Float2(-1.0f, 1.0f);
	vertices[0].textureCoordinate	= ::Ishikawa::Math::Float2( 0.0f, 0.0f);
	vertices[1].position			= ::Ishikawa::Math::Float2(-1.0f,-1.0f);
	vertices[1].textureCoordinate	= ::Ishikawa::Math::Float2( 0.0f, 1.0f);
	vertices[2].position			= ::Ishikawa::Math::Float2( 1.0f, 1.0f);
	vertices[2].textureCoordinate	= ::Ishikawa::Math::Float2( 1.0f, 0.0f);
	vertices[3].position			= ::Ishikawa::Math::Float2( 1.0f,-1.0f);
	vertices[3].textureCoordinate	= ::Ishikawa::Math::Float2( 1.0f, 1.0f);
	// ���_�o�b�t�@�쐬
	this->vertexBuffer = this->logoShaderUser.CreateVertexBuffer(vertices, vertexCount);
}

/// <summary>
/// �C���f�b�N�X�o�b�t�@�쐬
/// </summary>
void Logo::CreateIndexBuffer(){
	// �C���f�b�N�X���
	const unsigned long indices[INDEX_COUNT] = { 0U, 1U, 2U, 3U };
	// �C���f�b�N�X�o�b�t�@�쐬
	this->indexBuffer = this->logoShaderUser.CreateIndexBuffer(indices, INDEX_COUNT);
}

/// <summary>
/// ���_�o�b�t�@�J��
/// </summary>
void Logo::ReleaseVertexBuffer(){
	this->logoShaderUser.ReleaseBuffer(this->vertexBuffer);
	this->vertexBuffer = nullptr;
}

/// <summary>
/// �C���f�b�N�X�o�b�t�@�J��
/// </summary>
void Logo::ReleaseIndexBuffer(){
	this->logoShaderUser.ReleaseBuffer(this->indexBuffer);
	this->indexBuffer = nullptr;
}


/// <summary>
/// ���S�o��
/// </summary>
/// <param name="_elapsedTime">�X�V����</param>
void Logo::AppearLogo(float _elapsedTime){
	this->timer += _elapsedTime;
	if(this->timer >= APPEARING_TIME){
		this->viewColor = 1.0f;
		this->timer = 0.0f;
		this->currentUpdateType = UpdateFunctionType::Print;
		return;
	}

	// �i�X�Ɩ��邭����
	float rate = this->timer / APPEARING_TIME;
	this->viewColor = rate;
}

/// <summary>
/// ���S�\��
/// </summary>
/// <param name="_elapsedTime">�X�V����</param>
void Logo::PrintLogo(float _elapsedTime){
	this->timer += _elapsedTime;
	if(this->timer >= PRINT_TIME){
		this->viewColor = 1.0f;
		this->timer = 0.0f;
		this->currentUpdateType = UpdateFunctionType::Hide;
		return;
	}
}

/// <summary>
/// ���S�B��
/// </summary>
/// <param name="_elapsedTime">�X�V����</param>
void Logo::HideLogo(float _elapsedTime){
	this->timer += _elapsedTime;
	if(this->timer >= HIDING_TIME){
		this->viewColor = 0.0f;
		this->timer = 0.0f;
		this->currentUpdateType = UpdateFunctionType::Appear;

		// �^�C�g���V�[����
		Manager* const manager = this->GetSceneManager();
		manager->ChangeScene(new Title());
		return;
	}

	// �i�X�ƈÂ�����
	float rate = (HIDING_TIME - this->timer) / HIDING_TIME;
	this->viewColor = rate;
}
