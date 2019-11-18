//=============================================================================
//	SceneTitle.cpp
//
//	タイトルシーンのソースファイル
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


/// <summary>背景インデックス数</summary>
const unsigned int Title::BACKGROUND_INDEX_COUNT = 4U;
/// <summary>ボタンインデックス数</summary>
const unsigned int Title::BUTTON_INDEX_COUNT = 4U;


/// <summary>
/// デフォルトコンストラクタ
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
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Title::~Title(){
	// デバイス依存リソース開放
	this->ReleaseDeviceDependentResources();
}


/// <summary>
/// ゲームの取得
/// </summary>
/// <returns>ゲーム</returns>
::CatchAFairy::Game* Title::GetGame() const {
	// マネージャ取得
	Manager* const manager = this->GetSceneManager();
	if(manager == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("manager is NULL.");
	}

	// ゲーム取得
	::CatchAFairy::Game* const game = manager->GetGame();
	if(game == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("game is NULL.");
	}

	return game;
}

/// <summary>
/// ゲームの依存している自作デバイスの取得
/// </summary>
/// <returns>ゲームの依存している自作デバイス</returns>
::Ishikawa::DirectX::Device::Device* Title::GetGameDependentITDevice() const{
	// ゲーム取得
	::CatchAFairy::Game* const game = this->GetGame();

	// 自作デバイス取得
	::Ishikawa::DirectX::Device::Device* const itDevice = game->GetDependentITDevice();
	if(itDevice == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("itDevice is NULL.");
	}

	return itDevice;
}

/// <summary>
/// キーボードの取得
/// </summary>
/// <returns></returns>
const ::Ishikawa::Input::Keyboard& Title::GetKeyboard() const {
	// ゲーム取得
	::CatchAFairy::Game* const game = this->GetGame();

	return (game->GetKeyboard());
}


/// <summary>
/// ゲームのデバイスが読み込まれた
/// </summary>
void Title::OnLoadedGameDevice(){
	// ゲームデバイスにテクスチャを追加
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetGameDependentITDevice();
	itDevice->AddDependentResource(&(this->outputRenderTargetTexture));
	itDevice->AddDependentResource(&(this->titleOutputShaderUser));
	itDevice->AddDependentResource(&(this->backgroundTexture));
	itDevice->AddDependentResource(&(this->buttonsTexture));

	// デバイス依存リソース作成
	this->CreateDeviceDependentResources();
}

/// <summary>
/// ゲームのデバイスが復元された
/// </summary>
void Title::OnRestoredGameDevice(){
	// デバイス依存リソース作成
	this->CreateDeviceDependentResources();
}

/// <summary>
/// ゲームのデバイスが破棄された
/// </summary>
void Title::OnLostGameDevice(){
	// デバイス依存リソース開放
	this->ReleaseDeviceDependentResources();
}


/// <summary>
/// サイズ変更
/// </summary>
/// <param name="_width">幅</param>
/// <param name="_height">高さ</param>
void Title::ChangeSceneSize(unsigned int _width, unsigned int _height){
	this->outputRenderTargetTexture.ChangeSize(_width, _height);
}


/// <summary>
/// 初期化
/// </summary>
void Title::Initialize(){
	// 基底の初期化
	this->InitializeScene();

	// メンバの初期化
	this->outputRenderTargetTexture.Initialize();
	this->titleOutputShaderUser.Initialize();
	this->backgroundTexture.Initialize();
	this->buttonsTexture.Initialize();
}


/// <summary>
/// 開始
/// </summary>
void Title::Start(){
	// BGM開始
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	this->bgmSoundId = itCriAdx2Le.Play(CRI_CUESHEET_0_BGM_MAOUDAMASHII_HEALING08);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="_elapsedTime">経過時間</param>
void Title::Update(float _elapsedTime){
	NOT_USING(_elapsedTime);
	// 何もしない

	// ZかEnter,Spaceが押されたらプレイシーンへ
	const ::Ishikawa::Input::Keyboard& keyboard = this->GetKeyboard();
	if(
		keyboard.IsPressed(::Ishikawa::Input::KeyCode::Z) ||
		keyboard.IsPressed(::Ishikawa::Input::KeyCode::Enter)
	){
		Manager* const manager = this->GetSceneManager();
		if(manager != nullptr){
			// 決定音
			::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
			itCriAdx2Le.Play(CRI_CUESHEET_0_SE_MAOUDAMASHII_SYSTEM47);

			manager->ChangeScene(new Play());
		}
	}
}

/// <summary>
/// 描画
/// </summary>
void Title::Render(){
	// 出力に描画
	this->RenderToOutput();
}

/// <summary>
/// 終了
/// </summary>
void Title::End(){
	// BGM停止
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	itCriAdx2Le.Stop(this->bgmSoundId);
}


/// <summary>
/// デバイス依存リソース作成
/// </summary>
void Title::CreateDeviceDependentResources(){
	// 出力テクスチャ作成
	this->CreateOutputTextures();
	// 背景画像読み込み
	this->backgroundTexture.Load(L"Resources/Textures/TitleBack.png");
	// 背景バッファ作成
	this->CreateBackgroundVertexBuffer();
	this->CreateBackgroundIndexBuffer();
	// ボタン画像読み込み
	this->buttonsTexture.Load(L"Resources/Textures/TitleButtons.png");
	// ボタンバッファ作成
	this->CreatePlayButtonVertexBuffer();
	this->CreateRuleButtonVertexBuffer();
	this->CreateEndButtonVertexBuffer();
	this->CreateButtonIndexBuffer();
}

/// <summary>
/// デバイス依存リソース開放
/// </summary>
void Title::ReleaseDeviceDependentResources(){
	// ボタンバッファ開放
	this->ReleaseButtonIndexBuffer();
	this->ReleaseEndButtonVertexBuffer();
	this->ReleaseRuleButtonVertexBuffer();
	this->ReleasePlayButtonVertexBuffer();
	// 背景バッファ開放
	this->ReleaseBackgroundIndexBuffer();
	this->ReleaseBackgroundVertexBuffer();
}


/// <summary>
/// 出力テクスチャ作成
/// </summary>
void Title::CreateOutputTextures(){
	// 設定取得
	unsigned int sampleDescCount = 1U;
	unsigned int sampleDescQuality = 0U;
	::Ishikawa::DirectX::Texture::Dimention textureDimention = ::Ishikawa::DirectX::Texture::Dimention::Texture2D;

	// テクスチャ作成
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
/// 背景頂点バッファ作成
/// </summary>
void Title::CreateBackgroundVertexBuffer(){
	// 頂点情報
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
	// 頂点バッファ作成
	this->backgroundVertexBuffer = this->titleOutputShaderUser.CreateVertexBuffer(vertices, vertexCount);
}

/// <summary>
/// 背景インデックスバッファ作成
/// </summary>
void Title::CreateBackgroundIndexBuffer(){
	// インデックス情報
	const unsigned long indices[BACKGROUND_INDEX_COUNT] = { 0U, 1U, 2U, 3U };
	// インデックスバッファ作成
	this->backgroundIndexBuffer = this->titleOutputShaderUser.CreateIndexBuffer(indices, BACKGROUND_INDEX_COUNT);
}

/// <summary>
/// 背景頂点バッファ開放
/// </summary>
void Title::ReleaseBackgroundVertexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->backgroundVertexBuffer);
	this->backgroundVertexBuffer = nullptr;
}

/// <summary>
/// 背景インデックスバッファ開放
/// </summary>
void Title::ReleaseBackgroundIndexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->backgroundIndexBuffer);
	this->backgroundIndexBuffer = nullptr;
}


/// <summary>
/// プレイボタン頂点バッファ作成
/// </summary>
void Title::CreatePlayButtonVertexBuffer(){
	// 頂点情報
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
	// 頂点バッファ作成
	this->playButtonVertexBuffer = this->titleOutputShaderUser.CreateVertexBuffer(vertices, vertexCount);
}

/// <summary>
/// ルールボタン頂点バッファ作成
/// </summary>
void Title::CreateRuleButtonVertexBuffer(){
	// 頂点情報
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
	// 頂点バッファ作成
	this->ruleButtonVertexBuffer = this->titleOutputShaderUser.CreateVertexBuffer(vertices, vertexCount);
}

/// <summary>
/// プレイボタン頂点バッファ作成
/// </summary>
void Title::CreateEndButtonVertexBuffer(){
	// 頂点情報
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
	// 頂点バッファ作成
	this->endButtonVertexBuffer = this->titleOutputShaderUser.CreateVertexBuffer(vertices, vertexCount);
}

/// <summary>
/// ボタンインデックスバッファ作成
/// </summary>
void Title::CreateButtonIndexBuffer(){
	// インデックス情報
	const unsigned long indices[BUTTON_INDEX_COUNT] = { 0U, 1U, 2U, 3U };
	// インデックスバッファ作成
	this->buttonIndexBuffer = this->titleOutputShaderUser.CreateIndexBuffer(indices, BUTTON_INDEX_COUNT);
}

/// <summary>
/// プレイボタン頂点バッファ開放
/// </summary>
void Title::ReleasePlayButtonVertexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->playButtonVertexBuffer);
	this->playButtonVertexBuffer = nullptr;
}

/// <summary>
/// ルールボタン頂点バッファ開放
/// </summary>
void Title::ReleaseRuleButtonVertexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->ruleButtonVertexBuffer);
	this->ruleButtonVertexBuffer = nullptr;
}

/// <summary>
/// プレイボタン頂点バッファ開放
/// </summary>
void Title::ReleaseEndButtonVertexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->endButtonVertexBuffer);
	this->endButtonVertexBuffer = nullptr;
}

/// <summary>
/// ボタンインデックスバッファ開放
/// </summary>
void Title::ReleaseButtonIndexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->buttonIndexBuffer);
	this->buttonIndexBuffer = nullptr;
}


/// <summary>
/// 出力に描画
/// </summary>
void Title::RenderToOutput(){
	// 出力リソース
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

	// 入力リソース
	// 背景
	::CatchAFairy::Shader::User::TitleOutput::RenderUseResource backgroundResource;
	backgroundResource.constantBuffer.colorRate = ::Ishikawa::Math::Float4(1.0f, 1.0f, 1.0f, 1.0f);
	backgroundResource.topology = ::CatchAFairy::Shader::User::VertexTopology::TriangleStrip;
	backgroundResource.vertexBuffer = this->backgroundVertexBuffer;
	backgroundResource.indexBuffer = this->backgroundIndexBuffer;
	backgroundResource.indexCount = BACKGROUND_INDEX_COUNT;
	backgroundResource.shaderResourceView = this->backgroundTexture.GetShaderResourceView();
	// プレイボタン
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
//	// ルールボタン
//	::CatchAFairy::Shader::User::TitleOutput::RenderUseResource ruleButtonResource;
//	ruleButtonResource.constantBuffer.colorRate = ::Ishikawa::Math::Float4(1.0f, 1.0f, 1.0f, 1.0f);
//	ruleButtonResource.topology = ::CatchAFairy::Shader::User::VertexTopology::TriangleStrip;
//	ruleButtonResource.vertexBuffer = this->ruleButtonVertexBuffer;
//	ruleButtonResource.indexBuffer = this->buttonIndexBuffer;
//	ruleButtonResource.indexCount = BUTTON_INDEX_COUNT;
//	ruleButtonResource.shaderResourceView = this->buttonsTexture.GetShaderResourceView();
//	// 終了ボタン
//	::CatchAFairy::Shader::User::TitleOutput::RenderUseResource endButtonResource;
//	endButtonResource.constantBuffer.colorRate = ::Ishikawa::Math::Float4(1.0f, 1.0f, 1.0f, 1.0f);
//	endButtonResource.topology = ::CatchAFairy::Shader::User::VertexTopology::TriangleStrip;
//	endButtonResource.vertexBuffer = this->endButtonVertexBuffer;
//	endButtonResource.indexBuffer = this->buttonIndexBuffer;
//	endButtonResource.indexCount = BUTTON_INDEX_COUNT;
//	endButtonResource.shaderResourceView = this->buttonsTexture.GetShaderResourceView();

	// 描画
	const ::Ishikawa::Math::Float4 CLEAR_COLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->titleOutputShaderUser.DrawBefore(outputResource, &CLEAR_COLOR);
	this->titleOutputShaderUser.Draw(backgroundResource);
	this->titleOutputShaderUser.Draw(playButtonResource);
//	this->titleOutputShaderUser.Draw(ruleButtonResource);
//	this->titleOutputShaderUser.Draw(endButtonResource);
	this->titleOutputShaderUser.DrawAfter();
}
