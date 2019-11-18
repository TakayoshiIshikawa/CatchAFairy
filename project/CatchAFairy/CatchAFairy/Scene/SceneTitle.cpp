//=============================================================================
//	SceneTitle.cpp
//
//	�^�C�g���V�[���̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "SceneTitle.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Common/PatternBase/SingletonManager.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "Ishikawa/Input/ITKeyboard.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "Ishikawa/DirectX/ITViewport.h"
#include "Ishikawa/DirectX/Texture/ITTextureData.h"
#include "Ishikawa/CriAdx2Le/ITCriAdx2Le.h"
#include "CatchAFairy/Game.h"
#include "CatchAFairy/Shader/User/ShaderVertexTopology.h"
#include "SceneManager.h"
#include "ScenePlay.h"
#include "CatchAFairy/Sound/CueSheet_0.h"

using namespace CatchAFairy::Scene;


/// <summary>�w�i�C���f�b�N�X��</summary>
const unsigned int Title::BACKGROUND_INDEX_COUNT = 4U;
/// <summary>�{�^���C���f�b�N�X��</summary>
const unsigned int Title::BUTTON_INDEX_COUNT = 4U;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Title::Title() :
	outputRenderTargetTexture(),
	titleOutputShaderUser(),
	backgroundTexture(),
	backgroundVertexBuffer(nullptr),
	backgroundIndexBuffer(nullptr),
	buttonsTexture(),
	playButtonRect(-0.18f,-0.10f, 0.18f,-0.25f),//(-0.18f, 0.25f, 0.18f, 0.10f),
	ruleButtonRect(-0.18f, 0.075f, 0.18f,-0.075f),
	endButtonRect(-0.18f,-0.10f, 0.18f,-0.25f),
	playButtonVertexBuffer(nullptr),
	ruleButtonVertexBuffer(nullptr),
	endButtonVertexBuffer(nullptr),
	buttonIndexBuffer(nullptr),
	bgmSoundId(0U)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Title::~Title(){
	// �f�o�C�X�ˑ����\�[�X�J��
	this->ReleaseDeviceDependentResources();
}


/// <summary>
/// �Q�[���̎擾
/// </summary>
/// <returns>�Q�[��</returns>
::CatchAFairy::Game* Title::GetGame() const {
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
::Ishikawa::DirectX::Device::Device* Title::GetGameDependentITDevice() const{
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
const ::Ishikawa::Input::Keyboard& Title::GetKeyboard() const {
	// �Q�[���擾
	::CatchAFairy::Game* const game = this->GetGame();

	return (game->GetKeyboard());
}


/// <summary>
/// �Q�[���̃f�o�C�X���ǂݍ��܂ꂽ
/// </summary>
void Title::OnLoadedGameDevice(){
	// �Q�[���f�o�C�X�Ƀe�N�X�`����ǉ�
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetGameDependentITDevice();
	itDevice->AddDependentResource(&(this->outputRenderTargetTexture));
	itDevice->AddDependentResource(&(this->titleOutputShaderUser));
	itDevice->AddDependentResource(&(this->backgroundTexture));
	itDevice->AddDependentResource(&(this->buttonsTexture));

	// �f�o�C�X�ˑ����\�[�X�쐬
	this->CreateDeviceDependentResources();
}

/// <summary>
/// �Q�[���̃f�o�C�X���������ꂽ
/// </summary>
void Title::OnRestoredGameDevice(){
	// �f�o�C�X�ˑ����\�[�X�쐬
	this->CreateDeviceDependentResources();
}

/// <summary>
/// �Q�[���̃f�o�C�X���j�����ꂽ
/// </summary>
void Title::OnLostGameDevice(){
	// �f�o�C�X�ˑ����\�[�X�J��
	this->ReleaseDeviceDependentResources();
}


/// <summary>
/// �T�C�Y�ύX
/// </summary>
/// <param name="_width">��</param>
/// <param name="_height">����</param>
void Title::ChangeSceneSize(unsigned int _width, unsigned int _height){
	this->outputRenderTargetTexture.ChangeSize(_width, _height);
}


/// <summary>
/// ������
/// </summary>
void Title::Initialize(){
	// ���̏�����
	this->InitializeScene();

	// �����o�̏�����
	this->outputRenderTargetTexture.Initialize();
	this->titleOutputShaderUser.Initialize();
	this->backgroundTexture.Initialize();
	this->buttonsTexture.Initialize();
}


/// <summary>
/// �J�n
/// </summary>
void Title::Start(){
	// BGM�J�n
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	this->bgmSoundId = itCriAdx2Le.Play(CRI_CUESHEET_0_BGM_MAOUDAMASHII_HEALING08);
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="_elapsedTime">�o�ߎ���</param>
void Title::Update(float _elapsedTime){
	NOT_USING(_elapsedTime);
	// �������Ȃ�

	// Z��Enter,Space�������ꂽ��v���C�V�[����
	const ::Ishikawa::Input::Keyboard& keyboard = this->GetKeyboard();
	if(
		keyboard.IsPressed(::Ishikawa::Input::KeyCode::Z) ||
		keyboard.IsPressed(::Ishikawa::Input::KeyCode::Enter)
	){
		Manager* const manager = this->GetSceneManager();
		if(manager != nullptr){
			// ���艹
			::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
			itCriAdx2Le.Play(CRI_CUESHEET_0_SE_MAOUDAMASHII_SYSTEM47);

			manager->ChangeScene(new Play());
		}
	}
}

/// <summary>
/// �`��
/// </summary>
void Title::Render(){
	// �o�͂ɕ`��
	this->RenderToOutput();
}

/// <summary>
/// �I��
/// </summary>
void Title::End(){
	// BGM��~
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	itCriAdx2Le.Stop(this->bgmSoundId);
}


/// <summary>
/// �f�o�C�X�ˑ����\�[�X�쐬
/// </summary>
void Title::CreateDeviceDependentResources(){
	// �o�̓e�N�X�`���쐬
	this->CreateOutputTextures();
	// �w�i�摜�ǂݍ���
	this->backgroundTexture.Load(L"Resources/Textures/TitleBack.png");
	// �w�i�o�b�t�@�쐬
	this->CreateBackgroundVertexBuffer();
	this->CreateBackgroundIndexBuffer();
	// �{�^���摜�ǂݍ���
	this->buttonsTexture.Load(L"Resources/Textures/TitleButtons.png");
	// �{�^���o�b�t�@�쐬
	this->CreatePlayButtonVertexBuffer();
	this->CreateRuleButtonVertexBuffer();
	this->CreateEndButtonVertexBuffer();
	this->CreateButtonIndexBuffer();
}

/// <summary>
/// �f�o�C�X�ˑ����\�[�X�J��
/// </summary>
void Title::ReleaseDeviceDependentResources(){
	// �{�^���o�b�t�@�J��
	this->ReleaseButtonIndexBuffer();
	this->ReleaseEndButtonVertexBuffer();
	this->ReleaseRuleButtonVertexBuffer();
	this->ReleasePlayButtonVertexBuffer();
	// �w�i�o�b�t�@�J��
	this->ReleaseBackgroundIndexBuffer();
	this->ReleaseBackgroundVertexBuffer();
}


/// <summary>
/// �o�̓e�N�X�`���쐬
/// </summary>
void Title::CreateOutputTextures(){
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
/// �w�i���_�o�b�t�@�쐬
/// </summary>
void Title::CreateBackgroundVertexBuffer(){
	// ���_���
	const unsigned int vertexCount = 4U;
	::CatchAFairy::Shader::User::TitleOutput::VSInput vertices[vertexCount];
	vertices[0].position			= ::Ishikawa::Math::Float2(-1.0f, 1.0f);
	vertices[0].textureCoordinate	= ::Ishikawa::Math::Float2( 0.0f, 0.0f);
	vertices[1].position			= ::Ishikawa::Math::Float2(-1.0f,-1.0f);
	vertices[1].textureCoordinate	= ::Ishikawa::Math::Float2( 0.0f, 1.0f);
	vertices[2].position			= ::Ishikawa::Math::Float2( 1.0f, 1.0f);
	vertices[2].textureCoordinate	= ::Ishikawa::Math::Float2( 1.0f, 0.0f);
	vertices[3].position			= ::Ishikawa::Math::Float2( 1.0f,-1.0f);
	vertices[3].textureCoordinate	= ::Ishikawa::Math::Float2( 1.0f, 1.0f);
	// ���_�o�b�t�@�쐬
	this->backgroundVertexBuffer = this->titleOutputShaderUser.CreateVertexBuffer(vertices, vertexCount);
}

/// <summary>
/// �w�i�C���f�b�N�X�o�b�t�@�쐬
/// </summary>
void Title::CreateBackgroundIndexBuffer(){
	// �C���f�b�N�X���
	const unsigned long indices[BACKGROUND_INDEX_COUNT] = { 0U, 1U, 2U, 3U };
	// �C���f�b�N�X�o�b�t�@�쐬
	this->backgroundIndexBuffer = this->titleOutputShaderUser.CreateIndexBuffer(indices, BACKGROUND_INDEX_COUNT);
}

/// <summary>
/// �w�i���_�o�b�t�@�J��
/// </summary>
void Title::ReleaseBackgroundVertexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->backgroundVertexBuffer);
	this->backgroundVertexBuffer = nullptr;
}

/// <summary>
/// �w�i�C���f�b�N�X�o�b�t�@�J��
/// </summary>
void Title::ReleaseBackgroundIndexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->backgroundIndexBuffer);
	this->backgroundIndexBuffer = nullptr;
}


/// <summary>
/// �v���C�{�^�����_�o�b�t�@�쐬
/// </summary>
void Title::CreatePlayButtonVertexBuffer(){
	// ���_���
	const unsigned int vertexCount = 4U;
	const float uvTop = 0.0f;
	const float uvBottom = 1.0f / 3.0f;
	::CatchAFairy::Shader::User::TitleOutput::VSInput vertices[vertexCount];
	vertices[0].position			= ::Ishikawa::Math::Float2(this->playButtonRect.x, this->playButtonRect.y);
	vertices[0].textureCoordinate	= ::Ishikawa::Math::Float2( 0.0f, uvTop);
	vertices[1].position			= ::Ishikawa::Math::Float2(this->playButtonRect.x, this->playButtonRect.w);
	vertices[1].textureCoordinate	= ::Ishikawa::Math::Float2( 0.0f, uvBottom);
	vertices[2].position			= ::Ishikawa::Math::Float2(this->playButtonRect.z, this->playButtonRect.y);
	vertices[2].textureCoordinate	= ::Ishikawa::Math::Float2( 1.0f, uvTop);
	vertices[3].position			= ::Ishikawa::Math::Float2(this->playButtonRect.z, this->playButtonRect.w);
	vertices[3].textureCoordinate	= ::Ishikawa::Math::Float2( 1.0f, uvBottom);
	// ���_�o�b�t�@�쐬
	this->playButtonVertexBuffer = this->titleOutputShaderUser.CreateVertexBuffer(vertices, vertexCount);
}

/// <summary>
/// ���[���{�^�����_�o�b�t�@�쐬
/// </summary>
void Title::CreateRuleButtonVertexBuffer(){
	// ���_���
	const unsigned int vertexCount = 4U;
	const float uvTop = 1.0f / 3.0f;
	const float uvBottom = 2.0f / 3.0f;
	::CatchAFairy::Shader::User::TitleOutput::VSInput vertices[vertexCount];
	vertices[0].position			= ::Ishikawa::Math::Float2(this->ruleButtonRect.x, this->ruleButtonRect.y);
	vertices[0].textureCoordinate	= ::Ishikawa::Math::Float2( 0.0f, uvTop);
	vertices[1].position			= ::Ishikawa::Math::Float2(this->ruleButtonRect.x, this->ruleButtonRect.w);
	vertices[1].textureCoordinate	= ::Ishikawa::Math::Float2( 0.0f, uvBottom);
	vertices[2].position			= ::Ishikawa::Math::Float2(this->ruleButtonRect.z, this->ruleButtonRect.y);
	vertices[2].textureCoordinate	= ::Ishikawa::Math::Float2( 1.0f, uvTop);
	vertices[3].position			= ::Ishikawa::Math::Float2(this->ruleButtonRect.z, this->ruleButtonRect.w);
	vertices[3].textureCoordinate	= ::Ishikawa::Math::Float2( 1.0f, uvBottom);
	// ���_�o�b�t�@�쐬
	this->ruleButtonVertexBuffer = this->titleOutputShaderUser.CreateVertexBuffer(vertices, vertexCount);
}

/// <summary>
/// �v���C�{�^�����_�o�b�t�@�쐬
/// </summary>
void Title::CreateEndButtonVertexBuffer(){
	// ���_���
	const unsigned int vertexCount = 4U;
	const float uvTop = 2.0f / 3.0f;
	const float uvBottom = 1.0f;
	::CatchAFairy::Shader::User::TitleOutput::VSInput vertices[vertexCount];
	vertices[0].position			= ::Ishikawa::Math::Float2(this->endButtonRect.x, this->endButtonRect.y);
	vertices[0].textureCoordinate	= ::Ishikawa::Math::Float2( 0.0f, uvTop);
	vertices[1].position			= ::Ishikawa::Math::Float2(this->endButtonRect.x, this->endButtonRect.w);
	vertices[1].textureCoordinate	= ::Ishikawa::Math::Float2( 0.0f, uvBottom);
	vertices[2].position			= ::Ishikawa::Math::Float2(this->endButtonRect.z, this->endButtonRect.y);
	vertices[2].textureCoordinate	= ::Ishikawa::Math::Float2( 1.0f, uvTop);
	vertices[3].position			= ::Ishikawa::Math::Float2(this->endButtonRect.z, this->endButtonRect.w);
	vertices[3].textureCoordinate	= ::Ishikawa::Math::Float2( 1.0f, uvBottom);
	// ���_�o�b�t�@�쐬
	this->endButtonVertexBuffer = this->titleOutputShaderUser.CreateVertexBuffer(vertices, vertexCount);
}

/// <summary>
/// �{�^���C���f�b�N�X�o�b�t�@�쐬
/// </summary>
void Title::CreateButtonIndexBuffer(){
	// �C���f�b�N�X���
	const unsigned long indices[BUTTON_INDEX_COUNT] = { 0U, 1U, 2U, 3U };
	// �C���f�b�N�X�o�b�t�@�쐬
	this->buttonIndexBuffer = this->titleOutputShaderUser.CreateIndexBuffer(indices, BUTTON_INDEX_COUNT);
}

/// <summary>
/// �v���C�{�^�����_�o�b�t�@�J��
/// </summary>
void Title::ReleasePlayButtonVertexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->playButtonVertexBuffer);
	this->playButtonVertexBuffer = nullptr;
}

/// <summary>
/// ���[���{�^�����_�o�b�t�@�J��
/// </summary>
void Title::ReleaseRuleButtonVertexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->ruleButtonVertexBuffer);
	this->ruleButtonVertexBuffer = nullptr;
}

/// <summary>
/// �v���C�{�^�����_�o�b�t�@�J��
/// </summary>
void Title::ReleaseEndButtonVertexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->endButtonVertexBuffer);
	this->endButtonVertexBuffer = nullptr;
}

/// <summary>
/// �{�^���C���f�b�N�X�o�b�t�@�J��
/// </summary>
void Title::ReleaseButtonIndexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->buttonIndexBuffer);
	this->buttonIndexBuffer = nullptr;
}


/// <summary>
/// �o�͂ɕ`��
/// </summary>
void Title::RenderToOutput(){
	// �o�̓��\�[�X
	::CatchAFairy::Shader::User::TitleOutput::OutputResource outputResource;
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
	// �w�i
	::CatchAFairy::Shader::User::TitleOutput::RenderUseResource backgroundResource;
	backgroundResource.constantBuffer.colorRate = ::Ishikawa::Math::Float4(1.0f, 1.0f, 1.0f, 1.0f);
	backgroundResource.topology = ::CatchAFairy::Shader::User::VertexTopology::TriangleStrip;
	backgroundResource.vertexBuffer = this->backgroundVertexBuffer;
	backgroundResource.indexBuffer = this->backgroundIndexBuffer;
	backgroundResource.indexCount = BACKGROUND_INDEX_COUNT;
	backgroundResource.shaderResourceView = this->backgroundTexture.GetShaderResourceView();
	// �v���C�{�^��
	::CatchAFairy::Shader::User::TitleOutput::RenderUseResource playButtonResource;
	static const unsigned int FLUSH_FRAME = 60U;
	static unsigned int frameCount = 0U;
	float alpha = 1.0f;
	{
		frameCount = (frameCount + 1) % FLUSH_FRAME;
		float rate = static_cast<float>(frameCount) / FLUSH_FRAME;
		alpha = 1.0f - (rate * rate);
	}
	playButtonResource.constantBuffer.colorRate = ::Ishikawa::Math::Float4(1.0f, 1.0f, 1.0f, alpha);
	playButtonResource.topology = ::CatchAFairy::Shader::User::VertexTopology::TriangleStrip;
	playButtonResource.vertexBuffer = this->playButtonVertexBuffer;
	playButtonResource.indexBuffer = this->buttonIndexBuffer;
	playButtonResource.indexCount = BUTTON_INDEX_COUNT;
	playButtonResource.shaderResourceView = this->buttonsTexture.GetShaderResourceView();
//	// ���[���{�^��
//	::CatchAFairy::Shader::User::TitleOutput::RenderUseResource ruleButtonResource;
//	ruleButtonResource.constantBuffer.colorRate = ::Ishikawa::Math::Float4(1.0f, 1.0f, 1.0f, 1.0f);
//	ruleButtonResource.topology = ::CatchAFairy::Shader::User::VertexTopology::TriangleStrip;
//	ruleButtonResource.vertexBuffer = this->ruleButtonVertexBuffer;
//	ruleButtonResource.indexBuffer = this->buttonIndexBuffer;
//	ruleButtonResource.indexCount = BUTTON_INDEX_COUNT;
//	ruleButtonResource.shaderResourceView = this->buttonsTexture.GetShaderResourceView();
//	// �I���{�^��
//	::CatchAFairy::Shader::User::TitleOutput::RenderUseResource endButtonResource;
//	endButtonResource.constantBuffer.colorRate = ::Ishikawa::Math::Float4(1.0f, 1.0f, 1.0f, 1.0f);
//	endButtonResource.topology = ::CatchAFairy::Shader::User::VertexTopology::TriangleStrip;
//	endButtonResource.vertexBuffer = this->endButtonVertexBuffer;
//	endButtonResource.indexBuffer = this->buttonIndexBuffer;
//	endButtonResource.indexCount = BUTTON_INDEX_COUNT;
//	endButtonResource.shaderResourceView = this->buttonsTexture.GetShaderResourceView();

	// �`��
	const ::Ishikawa::Math::Float4 CLEAR_COLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->titleOutputShaderUser.DrawBefore(outputResource, &CLEAR_COLOR);
	this->titleOutputShaderUser.Draw(backgroundResource);
	this->titleOutputShaderUser.Draw(playButtonResource);
//	this->titleOutputShaderUser.Draw(ruleButtonResource);
//	this->titleOutputShaderUser.Draw(endButtonResource);
	this->titleOutputShaderUser.DrawAfter();
}
