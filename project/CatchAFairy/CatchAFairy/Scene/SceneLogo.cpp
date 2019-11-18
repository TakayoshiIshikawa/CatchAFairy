//=============================================================================
//	SceneLogo.cpp
//
//	ロゴシーンのソースファイル
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
/// 更新関数の種類
/// </summary>
enum class Logo::UpdateFunctionType{
	/// <summary>出現</summary>
	Appear,
	/// <summary>表示</summary>
	Print,
	/// <summary>隠蔽</summary>
	Hide
};


/// <summary>出現時間</summary>
const float Logo::APPEARING_TIME = 1.0f;
/// <summary>表示時間</summary>
const float Logo::PRINT_TIME = 2.0f;
/// <summary>隠蔽時間</summary>
const float Logo::HIDING_TIME = 1.5f;
/// <summary>インデックス数</summary>
const unsigned int Logo::INDEX_COUNT = 4U;


/// <summary>
/// デフォルトコンストラクタ
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
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Logo::~Logo(){
	// バッファ開放
	this->ReleaseIndexBuffer();
	this->ReleaseVertexBuffer();
}


/// <summary>
/// ゲームの依存している自作デバイスの取得
/// </summary>
/// <returns>ゲームの依存している自作デバイス</returns>
::Ishikawa::DirectX::Device::Device* Logo::GetGameDependentITDevice() const{
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

	// 自作デバイス取得
	::Ishikawa::DirectX::Device::Device* const itDevice = game->GetDependentITDevice();
	if(itDevice == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("itDevice is NULL.");
	}

	return itDevice;
}


/// <summary>
/// ゲームのデバイスが読み込まれた
/// </summary>
void Logo::OnLoadedGameDevice(){
	// ゲームデバイスにテクスチャを追加
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetGameDependentITDevice();
	itDevice->AddDependentResource(&(this->outputRenderTargetTexture));
	itDevice->AddDependentResource(&(this->logoShaderUser));
	itDevice->AddDependentResource(&(this->logoTexture));

	// 出力テクスチャ作成
	this->CreateOutputTextures();
	// 画像読み込み
	this->logoTexture.Load(L"Resources/Textures/Logo.png");
	// バッファ作成
	this->CreateVertexBuffer();
	this->CreateIndexBuffer();
}

/// <summary>
/// ゲームのデバイスが復元された
/// </summary>
void Logo::OnRestoredGameDevice(){
	// 何もしない
}

/// <summary>
/// ゲームのデバイスが破棄された
/// </summary>
void Logo::OnLostGameDevice(){
	// 何もしない
}


/// <summary>
/// サイズ変更
/// </summary>
/// <param name="_width">幅</param>
/// <param name="_height">高さ</param>
void Logo::ChangeSceneSize(unsigned int _width, unsigned int _height){
	this->outputRenderTargetTexture.ChangeSize(_width, _height);
}


/// <summary>
/// 初期化
/// </summary>
void Logo::Initialize(){
	// 基底の初期化
	this->InitializeScene();

	// メンバの初期化
	this->outputRenderTargetTexture.Initialize();
	this->logoShaderUser.Initialize();
	this->logoTexture.Initialize();
}


/// <summary>
/// 開始
/// </summary>
void Logo::Start(){
	// 何もしない
}

/// <summary>
/// 更新
/// </summary>
/// <param name="_elapsedTime">経過時間</param>
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
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("current update type is unknown.");
	}
}

/// <summary>
/// 描画
/// </summary>
void Logo::Render(){
	// 出力リソース
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

	// 入力リソース
	::CatchAFairy::Shader::User::Logo::RenderUseResource inputResource;
	inputResource.constantBuffer.colorRate = ::Ishikawa::Math::Float4(this->viewColor, this->viewColor, this->viewColor, 1.0f);
	inputResource.topology = ::CatchAFairy::Shader::User::VertexTopology::TriangleStrip;
	inputResource.vertexBuffer = this->vertexBuffer;
	inputResource.indexBuffer = this->indexBuffer;
	inputResource.indexCount = INDEX_COUNT;
	inputResource.shaderResourceView = this->logoTexture.GetShaderResourceView();

	// 描画
	const ::Ishikawa::Math::Float4 CLEAR_COLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->logoShaderUser.DrawBefore(outputResource, &CLEAR_COLOR);
	this->logoShaderUser.Draw(inputResource);
	this->logoShaderUser.DrawAfter();
}

/// <summary>
/// 終了
/// </summary>
void Logo::End(){
	// 何もしない
}


/// <summary>
/// 出力テクスチャ作成
/// </summary>
void Logo::CreateOutputTextures(){
	// 設定取得
	unsigned int sampleDescCount = 1U;
	unsigned int sampleDescQuality = 0U;
	::Ishikawa::DirectX::Texture::Dimention textureDimention = ::Ishikawa::DirectX::Texture::Dimention::Texture2D;

	// マルチサンプリングテクスチャにする
//	::Ishikawa::DirectX::Texture::GetMultisampleAntiAliasingDescData(
//		&sampleDescCount, &sampleDescQuality,
//		&textureDimention,
//		itDevice->GetDevice()
//	);

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
/// 頂点バッファ作成
/// </summary>
void Logo::CreateVertexBuffer(){
	// 頂点情報
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
	// 頂点バッファ作成
	this->vertexBuffer = this->logoShaderUser.CreateVertexBuffer(vertices, vertexCount);
}

/// <summary>
/// インデックスバッファ作成
/// </summary>
void Logo::CreateIndexBuffer(){
	// インデックス情報
	const unsigned long indices[INDEX_COUNT] = { 0U, 1U, 2U, 3U };
	// インデックスバッファ作成
	this->indexBuffer = this->logoShaderUser.CreateIndexBuffer(indices, INDEX_COUNT);
}

/// <summary>
/// 頂点バッファ開放
/// </summary>
void Logo::ReleaseVertexBuffer(){
	this->logoShaderUser.ReleaseBuffer(this->vertexBuffer);
	this->vertexBuffer = nullptr;
}

/// <summary>
/// インデックスバッファ開放
/// </summary>
void Logo::ReleaseIndexBuffer(){
	this->logoShaderUser.ReleaseBuffer(this->indexBuffer);
	this->indexBuffer = nullptr;
}


/// <summary>
/// ロゴ出現
/// </summary>
/// <param name="_elapsedTime">更新時間</param>
void Logo::AppearLogo(float _elapsedTime){
	this->timer += _elapsedTime;
	if(this->timer >= APPEARING_TIME){
		this->viewColor = 1.0f;
		this->timer = 0.0f;
		this->currentUpdateType = UpdateFunctionType::Print;
		return;
	}

	// 段々と明るくする
	float rate = this->timer / APPEARING_TIME;
	this->viewColor = rate;
}

/// <summary>
/// ロゴ表示
/// </summary>
/// <param name="_elapsedTime">更新時間</param>
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
/// ロゴ隠蔽
/// </summary>
/// <param name="_elapsedTime">更新時間</param>
void Logo::HideLogo(float _elapsedTime){
	this->timer += _elapsedTime;
	if(this->timer >= HIDING_TIME){
		this->viewColor = 0.0f;
		this->timer = 0.0f;
		this->currentUpdateType = UpdateFunctionType::Appear;

		// タイトルシーンへ
		Manager* const manager = this->GetSceneManager();
		manager->ChangeScene(new Title());
		return;
	}

	// 段々と暗くする
	float rate = (HIDING_TIME - this->timer) / HIDING_TIME;
	this->viewColor = rate;
}
