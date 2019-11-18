//=============================================================================
//	SceneResult.cpp
//
//	リザルトシーンのソースファイル
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


/// <summary>背景インデックス数</summary>
const unsigned int Result::BACKGROUND_INDEX_COUNT = 4U;

/// <summary>数字のテクスチャコーディネート [(x,y):左上 (z,w):右下]</summary>
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
/// <summary>各妖精のスコア</summary>
const unsigned int Result::FAIRY_SCORE[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)] = {
	100U,
	300U,
	500U
};

/// <summary>妖精カウント文字サイズ</summary>
const ::Ishikawa::Math::Float2 Result::FAIRY_COUNT_NUMBER_SIZE(0.1f, 0.1f);
/// <summary>スコア文字サイズ</summary>
const ::Ishikawa::Math::Float2 Result::SCORE_NUMBER_SIZE(0.13f, 0.13f);
/// <summary>妖精カウント座標</summary>
const ::Ishikawa::Math::Float2 Result::FAIRY_COUNT_POSITION[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)] = {
	::Ishikawa::Math::Float2(-0.33f, 0.58f),
	::Ishikawa::Math::Float2(-0.33f, 0.26f),
	::Ishikawa::Math::Float2(-0.33f,-0.06f)
};
/// <summary>妖精スコア座標</summary>
const ::Ishikawa::Math::Float2 Result::FAIRY_SCORE_POSITION[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)] = {
	::Ishikawa::Math::Float2(-0.05f, 0.60f),
	::Ishikawa::Math::Float2(-0.05f, 0.28f),
	::Ishikawa::Math::Float2(-0.05f,-0.04f)
};
/// <summary>合計スコア座標</summary>
const ::Ishikawa::Math::Float2 Result::TOTAL_SCORE_POSITION(-0.05f,-0.30f);

/// <summary>数字を表示するための頂点の合計</summary>
const unsigned int Result::TOTAL_VIEW_NUMBER_VERTEX_COUNT = 4U * Result::TOTAL_VIEW_NUMBER_COUNT;
/// <summary>数字を表示するためのインデックスの合計</summary>
const unsigned int Result::TOTAL_VIEW_NUMBER_INDEX_COUNT = 6U * Result::TOTAL_VIEW_NUMBER_COUNT;


/// <summary>
/// デフォルトコンストラクタ
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
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Result::~Result(){
	// デバイス依存リソース開放
	this->ReleaseDeviceDependentResources();
}


/// <summary>
/// ゲームの取得
/// </summary>
/// <returns>ゲーム</returns>
::CatchAFairy::Game* Result::GetGame() const {
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
::Ishikawa::DirectX::Device::Device* Result::GetGameDependentITDevice() const{
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
const ::Ishikawa::Input::Keyboard& Result::GetKeyboard() const {
	// ゲーム取得
	::CatchAFairy::Game* const game = this->GetGame();

	return (game->GetKeyboard());
}

/// <summary>
/// 数字の文字からテクスチャコーディネートを取得する
/// </summary>
/// <param name="_numberCharacter">数字の文字</param>
/// <returns>対応するテクスチャコーディネート</returns>
const ::Ishikawa::Math::Float4& Result::GetNumberTexcoord(const char _numberCharacter){
	if((_numberCharacter >= '0') && (_numberCharacter <= '9')){
		return Result::NUMBER_TEXTURECOORDINATE[(_numberCharacter - '0')];
	}
	return Result::NUMBER_TEXTURECOORDINATE[10];
}


/// <summary>
/// ゲームのデバイスが読み込まれた
/// </summary>
void Result::OnLoadedGameDevice(){
	// ゲームデバイスにテクスチャを追加
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetGameDependentITDevice();
	itDevice->AddDependentResource(&(this->outputRenderTargetTexture));
	itDevice->AddDependentResource(&(this->titleOutputShaderUser));
	itDevice->AddDependentResource(&(this->backgroundTexture));
	itDevice->AddDependentResource(&(this->numbersTexture));

	// デバイス依存リソース作成
	this->CreateDeviceDependentResources();
}

/// <summary>
/// ゲームのデバイスが復元された
/// </summary>
void Result::OnRestoredGameDevice(){
	// デバイス依存リソース作成
	this->CreateDeviceDependentResources();
}

/// <summary>
/// ゲームのデバイスが破棄された
/// </summary>
void Result::OnLostGameDevice(){
	// デバイス依存リソース開放
	this->ReleaseDeviceDependentResources();
}


/// <summary>
/// サイズ変更
/// </summary>
/// <param name="_width">幅</param>
/// <param name="_height">高さ</param>
void Result::ChangeSceneSize(unsigned int _width, unsigned int _height){
	this->outputRenderTargetTexture.ChangeSize(_width, _height);
}


/// <summary>
/// 初期化
/// </summary>
void Result::Initialize(){
	// 基底の初期化
	this->InitializeScene();

	// メンバの初期化
	this->outputRenderTargetTexture.Initialize();
	this->titleOutputShaderUser.Initialize();
	this->backgroundTexture.Initialize();
	this->numbersTexture.Initialize();
}


/// <summary>
/// 開始
/// </summary>
void Result::Start(){
	// BGM開始
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	this->bgmSoundId = itCriAdx2Le.Play(CRI_CUESHEET_0_BGM_MAOUDAMASHII_HEALING08);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="_elapsedTime">経過時間</param>
void Result::Update(float _elapsedTime){
	NOT_USING(_elapsedTime);
	// 何もしない

	// ZかEnter,Spaceが押されたらタイトルシーンへ
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

			manager->ChangeScene(new Title());
		}
	}
}

/// <summary>
/// 描画
/// </summary>
void Result::Render(){
	// 出力に描画
	this->RenderToOutput();
}

/// <summary>
/// 終了
/// </summary>
void Result::End(){
	// BGM停止
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	itCriAdx2Le.Stop(this->bgmSoundId);
}


/// <summary>
/// デバイス依存リソース作成
/// </summary>
void Result::CreateDeviceDependentResources(){
	// 出力テクスチャ作成
	this->CreateOutputTextures();
	// 背景画像読み込み
	this->backgroundTexture.Load(L"Resources/Textures/ResultBack.png");
	// 背景バッファ作成
	this->CreateBackgroundVertexBuffer();
	this->CreateBackgroundIndexBuffer();
	// 数字画像読み込み
	this->numbersTexture.Load(L"Resources/Textures/Numbers.png");
	// 数字バッファ作成
	this->CreateNumberVertexBuffer();
	this->CreateNumberIndexBuffer();
}

/// <summary>
/// デバイス依存リソース開放
/// </summary>
void Result::ReleaseDeviceDependentResources(){
	// 数字バッファ開放
	this->ReleaseNumberIndexBuffer();
	this->ReleaseNumberVertexBuffer();
	// 背景バッファ開放
	this->ReleaseBackgroundIndexBuffer();
	this->ReleaseBackgroundVertexBuffer();
}


/// <summary>
/// 出力テクスチャ作成
/// </summary>
void Result::CreateOutputTextures(){
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
void Result::CreateBackgroundVertexBuffer(){
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
void Result::CreateBackgroundIndexBuffer(){
	// インデックス情報
	const unsigned long indices[BACKGROUND_INDEX_COUNT] = { 0U, 1U, 2U, 3U };
	// インデックスバッファ作成
	this->backgroundIndexBuffer = this->titleOutputShaderUser.CreateIndexBuffer(indices, BACKGROUND_INDEX_COUNT);
}

/// <summary>
/// 背景頂点バッファ開放
/// </summary>
void Result::ReleaseBackgroundVertexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->backgroundVertexBuffer);
	this->backgroundVertexBuffer = nullptr;
}

/// <summary>
/// 背景インデックスバッファ開放
/// </summary>
void Result::ReleaseBackgroundIndexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->backgroundIndexBuffer);
	this->backgroundIndexBuffer = nullptr;
}


/// <summary>
/// 数字頂点バッファ作成
/// </summary>
void Result::CreateNumberVertexBuffer(){
	// 頂点情報
	::CatchAFairy::Shader::User::TitleOutput::VSInput vertices[Result::TOTAL_VIEW_NUMBER_VERTEX_COUNT];

	char viewNumberString[16U] = {0};
	unsigned int settingOffset = 0U;
	unsigned int totalScore = 0U;
	// 各捕獲数
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
	// 各スコア
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
	// 合計スコア
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

	// 頂点バッファ作成
	this->numberVertexBuffer = this->titleOutputShaderUser.CreateVertexBuffer(vertices, Result::TOTAL_VIEW_NUMBER_VERTEX_COUNT);
}

/// <summary>
/// 数字インデックスバッファ作成
/// </summary>
void Result::CreateNumberIndexBuffer(){
	// インデックス情報
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

	// インデックスバッファ作成
	this->numberIndexBuffer = this->titleOutputShaderUser.CreateIndexBuffer(indices, Result::TOTAL_VIEW_NUMBER_INDEX_COUNT);
}

/// <summary>
/// 数字頂点バッファ開放
/// </summary>
void Result::ReleaseNumberVertexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->numberVertexBuffer);
	this->numberVertexBuffer = nullptr;
}

/// <summary>
/// 数字インデックスバッファ開放
/// </summary>
void Result::ReleaseNumberIndexBuffer(){
	this->titleOutputShaderUser.ReleaseBuffer(this->numberIndexBuffer);
	this->numberIndexBuffer = nullptr;
}


/// <summary>
/// 出力に描画
/// </summary>
void Result::RenderToOutput(){
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
	// 数字
	::CatchAFairy::Shader::User::TitleOutput::RenderUseResource playButtonResource;
	playButtonResource.constantBuffer.colorRate = ::Ishikawa::Math::Float4(1.0f, 1.0f, 1.0f, 1.0f);
	playButtonResource.topology = ::CatchAFairy::Shader::User::VertexTopology::TriangleList;
	playButtonResource.vertexBuffer = this->numberVertexBuffer;
	playButtonResource.indexBuffer = this->numberIndexBuffer;
	playButtonResource.indexCount = Result::TOTAL_VIEW_NUMBER_INDEX_COUNT;
	playButtonResource.shaderResourceView = this->numbersTexture.GetShaderResourceView();
	// 描画
	const ::Ishikawa::Math::Float4 CLEAR_COLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->titleOutputShaderUser.DrawBefore(outputResource, &CLEAR_COLOR);
	this->titleOutputShaderUser.Draw(backgroundResource);
	this->titleOutputShaderUser.Draw(playButtonResource);
	this->titleOutputShaderUser.DrawAfter();
}
