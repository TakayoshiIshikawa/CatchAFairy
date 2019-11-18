//=============================================================================
//	SceneResult.cpp
//
//	���U���g�V�[���̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "SceneResult.h"
#include <stdio.h>
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
#include "SceneTitle.h"
#include "CatchAFairy/Sound/CueSheet_0.h"

using namespace CatchAFairy::Scene;


/// <summary>�w�i�C���f�b�N�X��</summary>
const unsigned int Result::BACKGROUND_INDEX_COUNT = 4U;

/// <summary>�����̃e�N�X�`���R�[�f�B�l�[�g [(x,y):���� (z,w):�E��]</summary>
const ::Ishikawa::Math::Float4 Result::NUMBER_TEXTURECOORDINATE[11] = {
	::Ishikawa::Math::Float4(0.0f/16.0f, 0.0f, 1.0f/16.0f, 1.0f),
	::Ishikawa::Math::Float4(1.0f/16.0f, 0.0f, 2.0f/16.0f, 1.0f),
	::Ishikawa::Math::Float4(2.0f/16.0f, 0.0f, 3.0f/16.0f, 1.0f),
	::Ishikawa::Math::Float4(3.0f/16.0f, 0.0f, 4.0f/16.0f, 1.0f),
	::Ishikawa::Math::Float4(4.0f/16.0f, 0.0f, 5.0f/16.0f, 1.0f),
	::Ishikawa::Math::Float4(5.0f/16.0f, 0.0f, 6.0f/16.0f, 1.0f),
	::Ishikawa::Math::Float4(6.0f/16.0f, 0.0f, 7.0f/16.0f, 1.0f),
	::Ishikawa::Math::Float4(7.0f/16.0f, 0.0f, 8.0f/16.0f, 1.0f),
	::Ishikawa::Math::Float4(8.0f/16.0f, 0.0f, 9.0f/16.0f, 1.0f),
	::Ishikawa::Math::Float4(9.0f/16.0f, 0.0f, 10.0f/16.0f, 1.0f),
	::Ishikawa::Math::Float4(10.0f/16.0f, 0.0f, 11.0f/16.0f, 1.0f)
};
/// <summary>�e�d���̃X�R�A</summary>
const unsigned int Result::FAIRY_SCORE[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)] = {
	100U,
	300U,
	500U
};

/// <summary>�d���J�E���g�����T�C�Y</summary>
const ::Ishikawa::Math::Float2 Result::FAIRY_COUNT_NUMBER_SIZE(0.1f, 0.1f);
/// <summary>�X�R�A�����T�C�Y</summary>
const ::Ishikawa::Math::Float2 Result::SCORE_NUMBER_SIZE(0.13f, 0.13f);
/// <summary>�d���J�E���g���W</summary>
const ::Ishikawa::Math::Float2 Result::FAIRY_COUNT_POSITION[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)] = {
	::Ishikawa::Math::Float2(-0.33f, 0.58f),
	::Ishikawa::Math::Float2(-0.33f, 0.26f),
	::Ishikawa::Math::Float2(-0.33f,-0.06f)
};
/// <summary>�d���X�R�A���W</summary>
const ::Ishikawa::Math::Float2 Result::FAIRY_SCORE_POSITION[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)] = {
	::Ishikawa::Math::Float2(-0.05f, 0.60f),
	::Ishikawa::Math::Float2(-0.05f, 0.28f),
	::Ishikawa::Math::Float2(-0.05f,-0.04f)
};
/// <summary>���v�X�R�A���W</summary>
const ::Ishikawa::Math::Float2 Result::TOTAL_SCORE_POSITION(-0.05f,-0.30f);

/// <summary>������\�����邽�߂̒��_�̍��v</summary>
const unsigned int Result::TOTAL_VIEW_NUMBER_VERTEX_COUNT = 4U * Result::TOTAL_VIEW_NUMBER_COUNT;
/// <summary>������\�����邽�߂̃C���f�b�N�X�̍��v</summary>
const unsigned int Result::TOTAL_VIEW_NUMBER_INDEX_COUNT = 6U * Result::TOTAL_VIEW_NUMBER_COUNT;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Result::Result() :
	outputRenderTargetTexture(),
	titleOutputShaderUser(),
	backgroundTexture(),
	backgroundVertexBuffer(nullptr),
	backgroundIndexBuffer(nullptr),
	numbersTexture(),
	numberVertexBuffer(nullptr),
	numberIndexBuffer(nullptr),
	catchedFairyCount{0U},
	bgmSoundId(0U)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Result::~Result(){
	// �f�o�C�X�ˑ����\�[�X�J��
	this->ReleaseDeviceDependentResources();
}


/// <summary>
/// �Q�[���̎擾
/// </summary>
/// <returns>�Q�[��</returns>
::CatchAFairy::Game* Result::GetGame() const {
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
::Ishikawa::DirectX::Device::Device* Result::GetGameDependentITDevice() const{
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
const ::Ishikawa::Input::Keyboard& Result::GetKeyboard() const {
	// �Q�[���擾
	::CatchAFairy::Game* const game = this->GetGame();

	return (game->GetKeyboard());
}

/// <summary>
/// �����̕�������e�N�X�`���R�[�f�B�l�[�g���擾����
/// </summary>
/// <param name="_numberCharacter">�����̕���</param>
/// <returns>�Ή�����e�N�X�`���R�[�f�B�l�[�g</returns>
const ::Ishikawa::Math::Float4& Result::GetNumberTexcoord(const char _numberCharacter){
	if((_numberCharacter >= '0') && (_numberCharacter <= '9')){
		return Result::NUMBER_TEXTURECOORDINATE[(_numberCharacter - '0')];
	}
	return Result::NUMBER_TEXTURECOORDINATE[10];
}


/// <summary>
/// �Q�[���̃f�o�C�X���ǂݍ��܂ꂽ
/// </summary>
void Result::OnLoadedGameDevice(){
	// �Q�[���f�o�C�X�Ƀe�N�X�`����ǉ�
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetGameDependentITDevice();
	itDevice->AddDependentResource(&(this->outputRenderTargetTexture));
	itDevice->AddDependentResource(&(this->titleOutputShaderUser));
	itDevice->AddDependentResource(&(this->backgroundTexture));
	itDevice->AddDependentResource(&(this->numbersTexture));

	// �f�o�C�X�ˑ����\�[�X�쐬
	this->CreateDeviceDependentResources();
}

/// <summary>
/// �Q�[���̃f�o�C�X���������ꂽ
/// </summary>
void Result::OnRestoredGameDevice(){
	// �f�o�C�X�ˑ����\�[�X�쐬
	this->CreateDeviceDependentResources();
}

/// <summary>
/// �Q�[���̃f�o�C�X���j�����ꂽ
/// </summary>
void Result::OnLostGameDevice(){
	// �f�o�C�X�ˑ����\�[�X�J��
	this->ReleaseDeviceDependentResources();
}


/// <summary>
/// �T�C�Y�ύX
/// </summary>
/// <param name="_width">��</param>
/// <param name="_height">����</param>
void Result::ChangeSceneSize(unsigned int _width, unsigned int _height){
	this->outputRenderTargetTexture.ChangeSize(_width, _height);
}


/// <summary>
/// ������
/// </summary>
void Result::Initialize(){
	// ���̏�����
	this->InitializeScene();

	// �����o�̏�����
	this->outputRenderTargetTexture.Initialize();
	this->titleOutputShaderUser.Initialize();
	this->backgroundTexture.Initialize();
	this->numbersTexture.Initialize();
}


/// <summary>
/// �J�n
/// </summary>
void Result::Start(){
	// BGM�J�n
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	this->bgmSoundId = itCriAdx2Le.Play(CRI_CUESHEET_0_BGM_MAOUDAMASHII_HEALING08);
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="_elapsedTime">�o�ߎ���</param>
void Result::Update(float _elapsedTime){
	NOT_USING(_elapsedTime);
	// �������Ȃ�

	// Z��Enter,Space�������ꂽ��^�C�g���V�[����
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

			manager->ChangeScene(new Title());
		}
	}
}

/// <summary>
/// �`��
/// </summary>
void Result::Render(){
	// �o�͂ɕ`��
	this->RenderToOutput();
}

/// <summary>
/// �I��
/// </summary>
void Result::End(){
	// BGM��~
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	itCriAdx2Le.Stop(this->bgmSoundId);
}


/// <summary>
/// �f�o�C�X�ˑ����\�[�X�쐬
/// </summary>
void Result::CreateDeviceDependentResources(){
	// �o�̓e�N�X�`���쐬
	this->CreateOutputTextures();
	// �w�i�摜�ǂݍ���
	this->backgroundTexture.Load(L"Resources/Textures/ResultBack.png");
	// �w�i�o�b�t�@�쐬
	this->CreateBackgroundVertexBuffer();
	this->CreateBackgroundIndexBuffer();
	// �����摜�ǂݍ���
	this->numbersTexture.Load(L"Resources/Textures/Numbers.png");
	// �����o�b�t�@�쐬
	this->CreateNumberVertexBuffer();
	this->CreateNumberIndexBuffer();
}

/// <summary>
/// �f�o�C�X�ˑ����\�[�X�J��
/// </summary>
void Result::ReleaseDeviceDependentResources(){
	// �����o�b�t�@�J��
	this->ReleaseNumberIndexBuffer();
	this->ReleaseNumberVertexBuffer();
	// �w�i�o�b�t�@�J��
	this->ReleaseBackgroundIndexBuffer();
	this->ReleaseBackgroundVertexBuffer();
}


/// <summary>
/// �o�̓e�N�X�`���쐬
/// </summary>
void Result::CreateOutputTextures(){
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
void Result::CreateBackgroundVertexBuffer(){
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
void Result::CreateBackgroundIndexBuffer(){
	// �C���f�b�N�X���
	const unsigned long indices[BACKGROUND_INDEX_COUNT] = { 0U, 1U, 2U, 3U };
	// �C���f�b�N�X�o�b�t�@�쐬
	this->backgroundIndexBuffer = this->titleOutputShaderUser.CreateIndexBuffer(indices, BACKGROUND_INDEX_COUNT);
}

/// <summary>
/// �w�i���_�o�b�t�@�J��
/// </summary>
void Result::ReleaseBackgroundVertexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->backgroundVertexBuffer);
	this->backgroundVertexBuffer = nullptr;
}

/// <summary>
/// �w�i�C���f�b�N�X�o�b�t�@�J��
/// </summary>
void Result::ReleaseBackgroundIndexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->backgroundIndexBuffer);
	this->backgroundIndexBuffer = nullptr;
}


/// <summary>
/// �������_�o�b�t�@�쐬
/// </summary>
void Result::CreateNumberVertexBuffer(){
	// ���_���
	::CatchAFairy::Shader::User::TitleOutput::VSInput vertices[Result::TOTAL_VIEW_NUMBER_VERTEX_COUNT];

	char viewNumberString[16U] = {0};
	unsigned int settingOffset = 0U;
	unsigned int totalScore = 0U;
	// �e�ߊl��
	for(unsigned int i=0U; i<static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count); ++i){
		const ::Ishikawa::Math::Float2& positionOffset = Result::FAIRY_COUNT_POSITION[i];
		const ::Ishikawa::Math::Float2& size = Result::FAIRY_COUNT_NUMBER_SIZE;

#pragma warning(disable:4996)
		sprintf(viewNumberString, "%015d", this->catchedFairyCount[i]);
#pragma warning(default:4996)
		const unsigned int stringOffset = 15U - Result::FAIRY_COUNT_DIGIT;

		for(unsigned int j=0U; j<Result::FAIRY_COUNT_DIGIT; ++j){
			settingOffset = 4U*(Result::FAIRY_COUNT_NUMBER_FARST_INDEX + Result::FAIRY_COUNT_DIGIT*i + j);
			const ::Ishikawa::Math::Float4& texcoord = this->GetNumberTexcoord(viewNumberString[stringOffset+j]);

			vertices[settingOffset + 0U].position =
				::Ishikawa::Math::Float2(positionOffset.x + size.x*j, positionOffset.y);
			vertices[settingOffset + 0U].textureCoordinate =
				::Ishikawa::Math::Float2(texcoord.x, texcoord.y);

			vertices[settingOffset + 1U].position =
				::Ishikawa::Math::Float2(positionOffset.x + size.x*(j+1U), positionOffset.y);
			vertices[settingOffset + 1U].textureCoordinate =
				::Ishikawa::Math::Float2(texcoord.z, texcoord.y);

			vertices[settingOffset + 2U].position =
				::Ishikawa::Math::Float2(positionOffset.x + size.x*j, positionOffset.y - size.y);
			vertices[settingOffset + 2U].textureCoordinate =
				::Ishikawa::Math::Float2(texcoord.x, texcoord.w);

			vertices[settingOffset + 3U].position =
				::Ishikawa::Math::Float2(positionOffset.x + size.x*(j+1U), positionOffset.y - size.y);
			vertices[settingOffset + 3U].textureCoordinate =
				::Ishikawa::Math::Float2(texcoord.z, texcoord.w);
		}
	}
	// �e�X�R�A
	for(unsigned int i=0U; i<static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count); ++i){
		const ::Ishikawa::Math::Float2& positionOffset = Result::FAIRY_SCORE_POSITION[i];
		const ::Ishikawa::Math::Float2& size = Result::SCORE_NUMBER_SIZE;

		unsigned int score = this->catchedFairyCount[i] * Result::FAIRY_SCORE[i];
		totalScore += score;

#pragma warning(disable:4996)
		sprintf(viewNumberString, "%15d", score);
#pragma warning(default:4996)
		const unsigned int stringOffset = 15U - Result::FAIRY_SCORE_DIGIT;

		for(unsigned int j=0U; j<Result::FAIRY_SCORE_DIGIT; ++j){
			settingOffset = 4U*(Result::FAIRY_SCORE_NUMBER_FARST_INDEX + Result::FAIRY_SCORE_DIGIT*i + j);
			const ::Ishikawa::Math::Float4& texcoord = this->GetNumberTexcoord(viewNumberString[stringOffset+j]);

			vertices[settingOffset + 0U].position =
				::Ishikawa::Math::Float2(positionOffset.x + size.x*j, positionOffset.y);
			vertices[settingOffset + 0U].textureCoordinate =
				::Ishikawa::Math::Float2(texcoord.x, texcoord.y);

			vertices[settingOffset + 1U].position =
				::Ishikawa::Math::Float2(positionOffset.x + size.x*(j+1U), positionOffset.y);
			vertices[settingOffset + 1U].textureCoordinate =
				::Ishikawa::Math::Float2(texcoord.z, texcoord.y);

			vertices[settingOffset + 2U].position =
				::Ishikawa::Math::Float2(positionOffset.x + size.x*j, positionOffset.y - size.y);
			vertices[settingOffset + 2U].textureCoordinate =
				::Ishikawa::Math::Float2(texcoord.x, texcoord.w);

			vertices[settingOffset + 3U].position =
				::Ishikawa::Math::Float2(positionOffset.x + size.x*(j+1U), positionOffset.y - size.y);
			vertices[settingOffset + 3U].textureCoordinate =
				::Ishikawa::Math::Float2(texcoord.z, texcoord.w);
		}
	}
	// ���v�X�R�A
	{
		const ::Ishikawa::Math::Float2& positionOffset = Result::TOTAL_SCORE_POSITION;
		const ::Ishikawa::Math::Float2& size = Result::SCORE_NUMBER_SIZE;

#pragma warning(disable:4996)
		sprintf(viewNumberString, "%15d", totalScore);
#pragma warning(default:4996)
		const unsigned int stringOffset = 15U - Result::TOTAL_SCORE_DIGIT;

		for(unsigned int j=0U; j<Result::TOTAL_SCORE_DIGIT; ++j){
			settingOffset = 4U*(Result::TOTAL_SCORE_NUMBER_FARST_INDEX + j);
			const ::Ishikawa::Math::Float4& texcoord = this->GetNumberTexcoord(viewNumberString[stringOffset+j]);

			vertices[settingOffset + 0U].position =
				::Ishikawa::Math::Float2(positionOffset.x + size.x*j, positionOffset.y);
			vertices[settingOffset + 0U].textureCoordinate =
				::Ishikawa::Math::Float2(texcoord.x, texcoord.y);

			vertices[settingOffset + 1U].position =
				::Ishikawa::Math::Float2(positionOffset.x + size.x*(j+1U), positionOffset.y);
			vertices[settingOffset + 1U].textureCoordinate =
				::Ishikawa::Math::Float2(texcoord.z, texcoord.y);

			vertices[settingOffset + 2U].position =
				::Ishikawa::Math::Float2(positionOffset.x + size.x*j, positionOffset.y - size.y);
			vertices[settingOffset + 2U].textureCoordinate =
				::Ishikawa::Math::Float2(texcoord.x, texcoord.w);

			vertices[settingOffset + 3U].position =
				::Ishikawa::Math::Float2(positionOffset.x + size.x*(j+1U), positionOffset.y - size.y);
			vertices[settingOffset + 3U].textureCoordinate =
				::Ishikawa::Math::Float2(texcoord.z, texcoord.w);
		}
	}

	// ���_�o�b�t�@�쐬
	this->numberVertexBuffer = this->titleOutputShaderUser.CreateVertexBuffer(vertices, Result::TOTAL_VIEW_NUMBER_VERTEX_COUNT);
}

/// <summary>
/// �����C���f�b�N�X�o�b�t�@�쐬
/// </summary>
void Result::CreateNumberIndexBuffer(){
	// �C���f�b�N�X���
	unsigned long indices[Result::TOTAL_VIEW_NUMBER_INDEX_COUNT] = { 0U };

	for(unsigned int i=0U; i<Result::TOTAL_VIEW_NUMBER_COUNT; ++i){
		const unsigned int settingOffset = 6U * i;
		const unsigned int indexOffset = 4U * i;

		indices[settingOffset + 0U] = indexOffset + 0U;
		indices[settingOffset + 1U] = indexOffset + 2U;
		indices[settingOffset + 2U] = indexOffset + 1U;
		indices[settingOffset + 3U] = indexOffset + 1U;
		indices[settingOffset + 4U] = indexOffset + 2U;
		indices[settingOffset + 5U] = indexOffset + 3U;
	}

	// �C���f�b�N�X�o�b�t�@�쐬
	this->numberIndexBuffer = this->titleOutputShaderUser.CreateIndexBuffer(indices, Result::TOTAL_VIEW_NUMBER_INDEX_COUNT);
}

/// <summary>
/// �������_�o�b�t�@�J��
/// </summary>
void Result::ReleaseNumberVertexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->numberVertexBuffer);
	this->numberVertexBuffer = nullptr;
}

/// <summary>
/// �����C���f�b�N�X�o�b�t�@�J��
/// </summary>
void Result::ReleaseNumberIndexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->numberIndexBuffer);
	this->numberIndexBuffer = nullptr;
}


/// <summary>
/// �o�͂ɕ`��
/// </summary>
void Result::RenderToOutput(){
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
	// ����
	::CatchAFairy::Shader::User::TitleOutput::RenderUseResource playButtonResource;
	playButtonResource.constantBuffer.colorRate = ::Ishikawa::Math::Float4(1.0f, 1.0f, 1.0f, 1.0f);
	playButtonResource.topology = ::CatchAFairy::Shader::User::VertexTopology::TriangleList;
	playButtonResource.vertexBuffer = this->numberVertexBuffer;
	playButtonResource.indexBuffer = this->numberIndexBuffer;
	playButtonResource.indexCount = Result::TOTAL_VIEW_NUMBER_INDEX_COUNT;
	playButtonResource.shaderResourceView = this->numbersTexture.GetShaderResourceView();
	// �`��
	const ::Ishikawa::Math::Float4 CLEAR_COLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->titleOutputShaderUser.DrawBefore(outputResource, &CLEAR_COLOR);
	this->titleOutputShaderUser.Draw(backgroundResource);
	this->titleOutputShaderUser.Draw(playButtonResource);
	this->titleOutputShaderUser.DrawAfter();
}
