//=============================================================================
//	SceneManager.cpp
//
//	シーンマネージャのソースファイル
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


/// <summary>シーン出力テクスチャサイズ</summary>
const unsigned int Manager::SCENE_OUTPUT_TEXTURE_SIZE = 1024U;
/// <summary>シーン出力テクスチャの描画幅</summary>
const unsigned int Manager::SCENE_OUTPUT_TEXTURE_WIDTH = 800U;
/// <summary>シーン出力テクスチャの描画高さ</summary>
const unsigned int Manager::SCENE_OUTPUT_TEXTURE_HEIGHT = 600U;
/// <summary>シーン出力テクスチャのアスペクト比</summary>
const float Manager::SCENE_OUTPUT_TEXTURE_ASPECT = static_cast<float>(SCENE_OUTPUT_TEXTURE_HEIGHT) / SCENE_OUTPUT_TEXTURE_WIDTH;
/// <summary>余白の色</summary>
const ::Ishikawa::Math::Float4 Manager::MARGIN_COLOR(0.0f, 0.0f, 0.0f, 1.0f);


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
Manager::Manager() :
	game(nullptr),
	sceneListEdge(),
	outputScreenShaderUser(),
	isChangedSceneFrame(0)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Manager::~Manager(){
	// 何もしない
}


/// <summary>
/// ゲームが依存している自作デバイスの取得
/// </summary>
/// <returns>ゲームが依存している自作デバイス</returns>
::Ishikawa::DirectX::Device::Device* Manager::GetGameDependentITDevice() const{
	if(this->game == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("this game is NULL.");
	}

	// ゲームが依存している自作デバイスの取得
	::Ishikawa::DirectX::Device::Device* const itDevice = this->game->GetDependentITDevice();
	if(itDevice == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("itDevice is NULL.");
	}

	return itDevice;
}

/// <summary>
/// 現在のシーンを取得
/// </summary>
/// <returns>現在のシーン</returns>
Interface* Manager::GetCurrentScene() const{
	// シーンリストの一番後ろを現在のシーンとする
	return this->sceneListEdge.GetBeforeScene();
}


/// <summary>
/// ゲームのデバイスが読み込まれた
/// </summary>
void Manager::OnLoadedGameDevice(){
	// メンバを登録
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetGameDependentITDevice();
	itDevice->AddDependentResource(&(this->outputScreenShaderUser));

	// 全てのシーンに通知
	Interface* scene = this->sceneListEdge.GetNextScene();
	while(!(scene->IsSceneListEdge())){
		scene->OnLoadedGameDevice();

		scene = scene->GetNextScene();
	}
}

/// <summary>
/// ゲームのデバイスが復元された
/// </summary>
void Manager::OnRestoredGameDevice(){
	// 全てのシーンに通知
	Interface* scene = this->sceneListEdge.GetNextScene();
	while(!(scene->IsSceneListEdge())){
		scene->OnRestoredGameDevice();

		scene = scene->GetNextScene();
	}
}

/// <summary>
/// ゲームのデバイスが破棄された
/// </summary>
void Manager::OnLostGameDevice(){
	// 全てのシーンに通知
	Interface* scene = this->sceneListEdge.GetNextScene();
	while(!(scene->IsSceneListEdge())){
		scene->OnLostGameDevice();

		scene = scene->GetNextScene();
	}
}


/// <summary>
/// 初期化
/// </summary>
void Manager::Initialize(){
	// メンバの初期化
	this->outputScreenShaderUser.Initialize();
	this->sceneListEdge.Initialize();
}

/// <summary>
/// 更新
/// </summary>
/// <param name="_elapsedTime">経過時間</param>
void Manager::Update(float _elapsedTime){
	Interface* const currentScene = this->GetCurrentScene();
	currentScene->Update(_elapsedTime);
}

/// <summary>
/// 描画
/// </summary>
void Manager::Render(){
	// シーン変更したフレームでは描画をしない
	if(this->isChangedSceneFrame != 0){
		this->isChangedSceneFrame = 0;
		return;
	}

	Interface* const currentScene = this->GetCurrentScene();
	currentScene->Render();
}

/// <summary>
/// 画面へ描画
/// </summary>
/// <param name="_renderTargetView">スクリーンのレンダーターゲットビュー</param>
/// <param name="_depthStencilView">スクリーンのデプスステンシルビュー</param>
/// <param name="_width">スクリーンの幅</param>
/// <param name="_height">スクリーンの高さ</param>
void Manager::RenderToScreen(
	ID3D11RenderTargetView* const _renderTargetView,
	ID3D11DepthStencilView* const _depthStencilView,
	unsigned int _width,
	unsigned int _height
){
	if(_renderTargetView == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_renderTargetView is NULL.");
	}
	if(_depthStencilView == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_depthStencilView is NULL.");
	}

	// 現在のシーンの出力画像を取得
	Interface* const currentScene = this->GetCurrentScene();
	ID3D11ShaderResourceView* const currentSceneSRV = currentScene->GetShaderResourceView();
	// 出力画像がなければ何もしない
	if(currentSceneSRV == nullptr) return;


	// 出力リソース
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

	// シーン出力のアスペクト比が変わらないように頂点位置を調整する
	float widthRate = viewport.width / SCENE_OUTPUT_TEXTURE_WIDTH;
	float heightRate = viewport.height / SCENE_OUTPUT_TEXTURE_HEIGHT;
	float x = 1.0f, y = 1.0f;
	if(widthRate > heightRate){
		x = heightRate / widthRate;
	}
	else{
		y = widthRate / heightRate;
	}
	// 頂点情報
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
	// 頂点バッファ作成
	ID3D11Buffer* const vertexBuffer = this->outputScreenShaderUser.CreateVertexBuffer(vertices, vertexCount);

	// インデックス情報
	const unsigned int indexCount = 4U;
	const unsigned long indices[indexCount] = { 0U, 1U, 2U, 3U };
	// インデックスバッファ作成
	ID3D11Buffer* const indexBuffer = this->outputScreenShaderUser.CreateIndexBuffer(indices, indexCount);

	// 入力リソース
	::CatchAFairy::Shader::User::OutputScreen::RenderUseResource inputResource;
	inputResource.topology = ::CatchAFairy::Shader::User::VertexTopology::TriangleStrip;
	inputResource.vertexBuffer = vertexBuffer;
	inputResource.indexBuffer = indexBuffer;
	inputResource.indexCount = indexCount;
	inputResource.shaderResourceView = currentSceneSRV;

	// 描画
	this->outputScreenShaderUser.DrawBefore(outputResource, &MARGIN_COLOR);
	this->outputScreenShaderUser.Draw(inputResource);
	this->outputScreenShaderUser.DrawAfter();

	// バッファ開放
	this->outputScreenShaderUser.ReleaseBuffer(indexBuffer);
	this->outputScreenShaderUser.ReleaseBuffer(vertexBuffer);
}

/// <summary>
/// 終了
/// </summary>
void Manager::Finalize(){
	this->ClearSceneList();
}


/// <summary>
/// プッシュ
/// </summary>
/// <param name="_scene">シーン</param>
void Manager::PushScene(Base* const _scene){
	if(_scene == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_scene is NULL.");
	}
	// シーンの初期化
	_scene->Initialize();

	// 現在のシーンを止める
	Interface* const currentScene = this->GetCurrentScene();
	currentScene->SetUpdateFlag(false);
	currentScene->SetRenderFlag(false);

	// マネージャの設定
	_scene->SetSceneManager(this);
	// デバイスが読み込まれているなら、デバイス読み込み部分を動かす
	if(this->game != nullptr){
		if(this->game->GetDependentITDevice() != nullptr){
			_scene->OnLoadedGameDevice();
		}
	}

	// シーンリストの一番後ろに追加
	Interface* const tail = this->sceneListEdge.GetBeforeScene();
	_scene->SetBeforeScene(tail);
	_scene->SetNextScene(&(this->sceneListEdge));
	tail->SetNextScene(_scene);
	this->sceneListEdge.SetBeforeScene(_scene);

	// シーン開始
	_scene->Start();

	// シーン変更を記録
	this->isChangedSceneFrame = 1;
}

/// <summary>
/// ポップ
/// </summary>
void Manager::PopScene(){
	// 現在のシーンを終了
	Interface* const currentScene = this->GetCurrentScene();
	currentScene->End();

	// 前のシーンを再開する
	Interface* const beforeScene = currentScene->GetBeforeScene();
	beforeScene->SetUpdateFlag(true);
	beforeScene->SetRenderFlag(true);

	// シーン削除
	delete currentScene;
}

/// <summary>
/// チェンジ
/// </summary>
/// <param name="_scene">シーン</param>
void Manager::ChangeScene(Base* const _scene){
	if(_scene == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_scene is NULL.");
	}

	// 現在のシーンをポップして、新しいシーンをプッシュ
	this->PopScene();
	this->PushScene(_scene);
}

/// <summary>
/// 全削除
/// </summary>
void Manager::ClearSceneList(){
	// 全てのシーンを終了させていく
	Interface* scene = this->sceneListEdge.GetBeforeScene();
	while(!(scene->IsSceneListEdge())){
		scene->End();

		Interface* const deleteScene = scene;
		scene = scene->GetBeforeScene();

		delete deleteScene;
	}
}
