//=============================================================================
//	ScenePlay.cpp
//
//	プレイシーンのソースファイル
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


/// <summary>出力インデックス数</summary>
const unsigned int Play::OUTPUT_INDEX_COUNT = 4U;
/// <summary>ステージサイズの半分</summary>
const float Play::STAGE_SIZE_HALF = 10.0f;
/// <summary>ディレクショナルライトの色</summary>
const ::Ishikawa::Math::Float4 Play::DIRECTIONAL_LIGHT_COLOR(0.95f, 1.0f, 1.0f, 1.0f);


/// <summary>
/// デフォルトコンストラクタ
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
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Play::~Play(){
	// デバイス依存リソース開放
	this->ReleaseDeviceDependentResources();
}


/// <summary>
/// ゲームの取得
/// </summary>
/// <returns>ゲーム</returns>
::CatchAFairy::Game* Play::GetGame() const {
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
::Ishikawa::DirectX::Device::Device* Play::GetGameDependentITDevice() const{
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
const ::Ishikawa::Input::Keyboard& Play::GetKeyboard() const {
	// ゲーム取得
	::CatchAFairy::Game* const game = this->GetGame();

	return (game->GetKeyboard());
}


/// <summary>
/// ゲームのデバイスが読み込まれた
/// </summary>
void Play::OnLoadedGameDevice(){
	// ゲームデバイスにリソースを追加
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetGameDependentITDevice();
	itDevice->AddDependentResource(&(this->outputRenderTargetTexture));
	itDevice->AddDependentResource(&(this->ground));
	itDevice->AddDependentResource(&(this->player));
	itDevice->AddDependentResource(&(this->enemyFairies));
	itDevice->AddDependentResource(&(this->trees));
	this->playCamera.AddITDeviceDependentResources(itDevice);
	this->directionalLight.AddITDeviceDependentResources(itDevice);
	this->guiManager.AddITDeviceDependentResources(itDevice);

	// デバイス依存リソース作成
	this->CreateDeviceDependentResources();
}

/// <summary>
/// ゲームのデバイスが復元された
/// </summary>
void Play::OnRestoredGameDevice(){
	// デバイス依存リソース作成
	this->CreateDeviceDependentResources();
}

/// <summary>
/// ゲームのデバイスが破棄された
/// </summary>
void Play::OnLostGameDevice(){
	// デバイス依存リソース開放
	this->ReleaseDeviceDependentResources();
}

/// <summary>
/// 手を振る音の再生
/// </summary>
void Play::OnPlaySoundOfKaraburi() const {
	::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>().Play(CRI_CUESHEET_0_TEASI_KARABURI03);
}

/// <summary>
/// 捕獲音の再生
/// </summary>
void Play::OnPlaySoundOfCatched() const {
	::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>().Play(CRI_CUESHEET_0_SE_MAOUDAMASHII_CHIME13);
}


/// <summary>
/// サイズ変更
/// </summary>
/// <param name="_width">幅</param>
/// <param name="_height">高さ</param>
void Play::ChangeSceneSize(unsigned int _width, unsigned int _height){
	this->outputRenderTargetTexture.ChangeSize(_width, _height);
}


/// <summary>
/// 初期化
/// </summary>
void Play::Initialize(){
	// 基底の初期化
	this->InitializeScene();

	// メンバの初期化
	this->InitializeOutputResources();
	this->InitializeObjectResources();
}


/// <summary>
/// 開始
/// </summary>
void Play::Start(){
	// BGM開始
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	this->bgmSoundId = itCriAdx2Le.Play(CRI_CUESHEET_0_WINDYBIRD);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="_elapsedTime">経過時間</param>
void Play::Update(float _elapsedTime){
	// 各メンバ更新
	this->player.Update(_elapsedTime);
	this->enemyFairies.Update(_elapsedTime);
	this->trees.Update(_elapsedTime);
	this->playCamera.Update(_elapsedTime);
	this->pointLight.Update(_elapsedTime);

	this->fairyPositionView.Update(_elapsedTime);
	this->timeView.Update(_elapsedTime);
	this->operationMethodView.Update(_elapsedTime);

	// 終了ならリザルトシーンヘ
	if(this->IsEnd()){
		Manager* const manager = this->GetSceneManager();
		if(manager != nullptr){
			Result* const result = new Result();
			// 捕獲数を受け渡す
			for(unsigned int i=0; i<static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count); ++i){
				CatchAFairy::Data::EnemyFairyType type = static_cast<CatchAFairy::Data::EnemyFairyType>(i);
				result->SetCatchedFairyCount(type, this->player.GetCatchedFairyCount(type));
			}
			manager->ChangeScene(result);
		}
	}
}

/// <summary>
/// 描画
/// </summary>
void Play::Render(){
	// 各描画
	this->RenderOfCameraEyeFirst();
	this->RenderOfDirectionalLight();
	this->playCamera.RenderOfShadowShader();
	this->RenderOfCameraEyeReflectionLight();
	this->RenderOfCameraEyePointLight();
	this->playCamera.RenderOfLastShader();

	// GUI描画
	this->guiManager.Render();
}

/// <summary>
/// 終了
/// </summary>
void Play::End(){
	// BGM停止
	::Ishikawa::CriAdx2Le::ITCriAdx2Le& itCriAdx2Le = ::Ishikawa::Common::SingletonManager::GetInstance< ::Ishikawa::CriAdx2Le::ITCriAdx2Le>();
	itCriAdx2Le.Stop(this->bgmSoundId);
}


/// <summary>
/// ステージと移動ベクトルの判定
/// </summary>
/// <param name="_startPosition">ベクトル始点</param>
/// <param name="_moveVector">移動ベクトル</param>
/// <param name="_localToWorld">移動ベクトル空間からワールド空間への変換行列</param>
/// <param name="_reflectionRate">反射率</param>
/// <returns>判定後の移動ベクトル</returns>
::Ishikawa::Math::Float3 Play::CollideMoveVectorAtStage(
	const ::Ishikawa::Math::Float3& _startPosition,
	const ::Ishikawa::Math::Float3& _moveVector,
	const ::Ishikawa::Math::Matrix& _localToWorld,
	float _reflectionRate
) const {
	::Ishikawa::Math::Float3 returnVector = _moveVector;

	// ステージ外に出ないようにする
	// 右
	returnVector = ::Ishikawa::Math::Collision::ReflectVectorAtPlanar(
		_startPosition,
		returnVector,
		_localToWorld,
		::Ishikawa::Math::Float3( 0.0f, 0.0f, STAGE_SIZE_HALF),
		::Ishikawa::Math::Float3( 0.0f, 0.0f,-1.0f),
		_reflectionRate
	);
	// 左
	returnVector = ::Ishikawa::Math::Collision::ReflectVectorAtPlanar(
		_startPosition,
		returnVector,
		_localToWorld,
		::Ishikawa::Math::Float3( 0.0f, 0.0f,-STAGE_SIZE_HALF),
		::Ishikawa::Math::Float3( 0.0f, 0.0f, 1.0f),
		_reflectionRate
	);
	// 手前
	returnVector = ::Ishikawa::Math::Collision::ReflectVectorAtPlanar(
		_startPosition,
		returnVector,
		_localToWorld,
		::Ishikawa::Math::Float3( STAGE_SIZE_HALF, 0.0f, 0.0f),
		::Ishikawa::Math::Float3(-1.0f, 0.0f, 0.0f),
		_reflectionRate
	);
	// 奥
	returnVector = ::Ishikawa::Math::Collision::ReflectVectorAtPlanar(
		_startPosition,
		returnVector,
		_localToWorld,
		::Ishikawa::Math::Float3(-STAGE_SIZE_HALF, 0.0f, 0.0f),
		::Ishikawa::Math::Float3( 1.0f, 0.0f, 0.0f),
		_reflectionRate
	);
	// 上
	returnVector = ::Ishikawa::Math::Collision::ReflectVectorAtPlanar(
		_startPosition,
		returnVector,
		_localToWorld,
		::Ishikawa::Math::Float3(0.0f, 4.4f, 0.0f),
		::Ishikawa::Math::Float3( 0.0f,-1.0f, 0.0f),
		_reflectionRate
	);
	// 下
	returnVector = ::Ishikawa::Math::Collision::ReflectVectorAtPlanar(
		_startPosition,
		returnVector,
		_localToWorld,
		::Ishikawa::Math::Float3(0.0f, 0.1f, 0.0f),
		::Ishikawa::Math::Float3( 0.0f, 1.0f, 0.0f),
		_reflectionRate
	);

	// 木々との判定
	returnVector = this->trees.CollideVector(
		_startPosition,
		returnVector,
		_localToWorld,
		_reflectionRate
	);

	return returnVector;
}


/// <summary>
/// デバイス依存リソース作成
/// </summary>
void Play::CreateDeviceDependentResources(){
	// 出力テクスチャ作成
	this->CreateOutputTextures();
	// 描画オブジェクトのリソース作成
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
/// デバイス依存リソース開放
/// </summary>
void Play::ReleaseDeviceDependentResources(){
	// 描画オブジェクトのリソース開放
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
/// 出力テクスチャ作成
/// </summary>
void Play::CreateOutputTextures(){
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
/// 出力リソース初期化
/// </summary>
void Play::InitializeOutputResources(){
	this->outputRenderTargetTexture.Initialize();
}

/// <summary>
/// オブジェクトリソース初期化
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
/// カメラ視点1番の描画
/// </summary>
void Play::RenderOfCameraEyeFirst(){
	this->playCamera.DrawBeforeOfFirstShader();
	
	// 各オブジェクトの描画
	this->playCamera.RenderObjectToFirstShader(this->ground);
	this->playCamera.RenderObjectToFirstShader(this->player);
	this->playCamera.RenderSameObjectToFirstShader(this->enemyFairies);
	this->playCamera.RenderSameObjectToFirstShader(this->trees);
	
	this->playCamera.DrawAfterOfFirstShader();
}

/// <summary>
/// 平行ライトの描画
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
/// カメラ視点反射光の描画
/// </summary>
void Play::RenderOfCameraEyeReflectionLight(){
	this->playCamera.DrawBeforeOfReflectionLightShader();

	// 各オブジェクトの描画
	this->playCamera.RenderOfReflectionLightShader(this->pointLight);

	this->playCamera.DrawAfterOfReflectionLightShader();
}

/// <summary>
/// カメラ視点ポイントライトの描画
/// </summary>
void Play::RenderOfCameraEyePointLight(){
	this->playCamera.DrawBeforeOfPointLightShader();

	// 各オブジェクトの描画
	this->playCamera.RenderOfPointLightShader(this->pointLight);

	this->playCamera.DrawAfterOfPointLightShader();
}
