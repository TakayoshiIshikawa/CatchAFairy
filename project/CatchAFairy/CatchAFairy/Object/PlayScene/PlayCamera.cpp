//=============================================================================
//	PlayCamera.cpp
//
//	プレイカメラのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "PlayCamera.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "Ishikawa/DirectX/ITViewport.h"
#include "Ishikawa/DirectX/Texture/ITTextureData.h"
#include "CatchAFairy/Shader/User/ShaderVertexTopology.h"
#include "CatchAFairy/Object/RenderObjectInterface.h"
#include "CatchAFairy/Object/RenderSameObjectInterface.h"
#include "CatchAFairy/Object/PointLightObjectInterface.h"
#include "CatchAFairy/Scene/SceneManager.h"
#include "DirectionalLight.h"

using namespace CatchAFairy::Object::PlayScene;


/// <summary>最終インデックス数</summary>
const unsigned int PlayCamera::LAST_INDEX_COUNT = 4U;
/// <summary>追従比率</summary>
const float PlayCamera::FOLLOW_RATE = 0.1f;
/// <summary>視野角</summary>
const float PlayCamera::VIEW_ANGLE = 1.2f/*1.57f*/;
/// <summary>アスペクト比</summary>
const float PlayCamera::ASPECT_RATE = 0.75f;
/// <summary>ニアクリップ</summary>
const float PlayCamera::NEAR_CLIP = 0.5f;
/// <summary>ファークリップ</summary>
const float PlayCamera::FAR_CLIP = 30.0f;
/// <summary>クリップ中央</summary>
const float PlayCamera::CLIP_CENTER = 0.5f * (NEAR_CLIP + FAR_CLIP);


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
PlayCamera::PlayCamera() :
	::CatchAFairy::Object::UpdateInterface(),
	::CatchAFairy::Object::CameraInterface(),
	diffuseRenderTargetTexture(),
	normalRenderTargetTexture(),
	positionRenderTargetTexture(),
	firstDepthStencilTexture(),
	firstShaderUser(),
	outputTargetTexture(nullptr),
	lastShaderUser(),
	lastVertexBuffer(nullptr),
	lastIndexBuffer(nullptr),
	shadowRenderTargetTexture(),
	shadowShaderUser(),
	reflectionLightRenderTargetTexture(),
	reflectionLightDepthStencilTexture(),
	reflectionLightShaderUser(),
	pointLightRenderTargetTexture(),
	pointLightShaderUser(),
	local(::Ishikawa::Math::Matrix::IDENTITY),
	world(::Ishikawa::Math::Matrix::IDENTITY),
	view(::Ishikawa::Math::Matrix::IDENTITY),
	targetView(::Ishikawa::Math::Matrix::IDENTITY),
	projection(::Ishikawa::Math::Matrix::IDENTITY),
	directionalLight(nullptr)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
PlayCamera::~PlayCamera(){
	// 何もしない
}


/// <summary>
/// 平行ライトの設定
/// </summary>
/// <param name="_directionalLight">平行ライト</param>
void PlayCamera::SetDirectionalLight(DirectionalLight* const _directionalLight){
	this->directionalLight = _directionalLight;

	if(this->directionalLight != nullptr){
		this->directionalLight->SetLookPosition(this->world._41, this->world._42, this->world._43);
	}
}


/// <summary>
/// 追従オブジェクトの移動
/// </summary>
/// <param name="_followMatrix">追従するための行列</param>
void PlayCamera::OnMoveFollowingTheObject(const ::Ishikawa::Math::Matrix& _followMatrix){
	::Ishikawa::Math::Matrix invView = this->local * _followMatrix;
	this->targetView = invView.Invert();
}



/// <summary>
/// ゲームのデバイスに依存するリソースの追加
/// </summary>
/// <param name="_itDevice">自作デバイス</param>
void PlayCamera::AddITDeviceDependentResources(::Ishikawa::DirectX::Device::Device* const _itDevice){
	if(_itDevice == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_itDevice is NULL.");
	}

	// ゲームデバイスにリソースを追加
	_itDevice->AddDependentResource(&(this->diffuseRenderTargetTexture));
	_itDevice->AddDependentResource(&(this->normalRenderTargetTexture));
	_itDevice->AddDependentResource(&(this->positionRenderTargetTexture));
	_itDevice->AddDependentResource(&(this->firstDepthStencilTexture));
	_itDevice->AddDependentResource(&(this->firstShaderUser));
	_itDevice->AddDependentResource(&(this->lastShaderUser));
	_itDevice->AddDependentResource(&(this->shadowRenderTargetTexture));
	_itDevice->AddDependentResource(&(this->shadowShaderUser));
	_itDevice->AddDependentResource(&(this->reflectionLightRenderTargetTexture));
	_itDevice->AddDependentResource(&(this->reflectionLightDepthStencilTexture));
	_itDevice->AddDependentResource(&(this->reflectionLightShaderUser));
	_itDevice->AddDependentResource(&(this->pointLightRenderTargetTexture));
	_itDevice->AddDependentResource(&(this->pointLightShaderUser));
}

/// <summary>
/// デバイス依存リソース作成
/// </summary>
void PlayCamera::CreateDeviceDependentResources(){
	this->CreateFirstTextures();
	this->CreateShadowTextures();
	this->CreateReflectionLightTextures();
	this->CreatePointLightTextures();
	this->CreateLastVertexBuffer();
	this->CreateLastIndexBuffer();
}

/// <summary>
/// デバイス依存リソース開放
/// </summary>
void PlayCamera::ReleaseDeviceDependentResources(){
	this->ReleaseLastIndexBuffer();
	this->ReleaseLastVertexBuffer();
}


/// <summary>
/// 初期化
/// </summary>
void PlayCamera::Initialize(){
	this->InitializeFirstResources();
	this->InitializeLastResources();
	this->InitializeShadowResources();
	this->InitializeReflectionLightResources();
	this->InitializePointLightResources();

	::Ishikawa::Math::Matrix trans1 = ::Ishikawa::Math::CreateTranslationMatrix(0.0f, 0.1f, 0.0f);
	::Ishikawa::Math::Matrix rotate = ::Ishikawa::Math::CreateRotationMatrixOfAxisX(-0.15f);
	::Ishikawa::Math::Matrix trans2 = ::Ishikawa::Math::CreateTranslationMatrix(0.0f, 0.1f,-1.3f);
	this->local = trans2 * rotate * trans1;

	this->world = this->local;
	this->targetView = this->local.Invert();
	this->view = this->targetView;

	this->projection = ::Ishikawa::Math::CreatePerspectiveMatrix(VIEW_ANGLE, ASPECT_RATE, NEAR_CLIP, FAR_CLIP);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="_elapsedTime">更新間隔</param>
void PlayCamera::Update(float _elapsedTime){
	float followRate = 60.0f * _elapsedTime * FOLLOW_RATE;
	followRate = (followRate > 1.0f) ? 1.0f : followRate;

	this->view += (this->targetView - this->view) * followRate;
	this->world = this->view.Invert();

	if(this->directionalLight != nullptr){
		::Ishikawa::Math::Float4 look(0.0f, 0.0f, CLIP_CENTER, 1.0f);
		look *= this->world;
		this->directionalLight->SetLookPosition(look.x, look.y, look.z);
	}
}

/// <summary>
/// 1番シェーダの描画前処理
/// </summary>
void PlayCamera::DrawBeforeOfFirstShader(){
	// 出力リソース
	::CatchAFairy::Shader::User::PlayCameraEyeFirst::OutputResource outputResource;
	outputResource.diffuseRenderTargetView = this->diffuseRenderTargetTexture.GetRenderTargetView();
	outputResource.normalRenderTargetView = this->normalRenderTargetTexture.GetRenderTargetView();
	outputResource.positionRenderTargetView = this->positionRenderTargetTexture.GetRenderTargetView();
	outputResource.depthStencilView = this->firstDepthStencilTexture.GetDepthStencilView();

	unsigned int width = 0U, height = 0U;
	this->diffuseRenderTargetTexture.GetTextureSize(&width, &height);
	::Ishikawa::DirectX::Viewport viewport;
	viewport.leftTopX	= 0.0f;
	viewport.leftTopY	= 0.0f;
	viewport.width		= static_cast<float>(width);
	viewport.height		= static_cast<float>(height);
	viewport.minDepth	= 0.0f;
	viewport.maxDepth	= 1.0f;
	outputResource.viewport = &viewport;

	// 情報リセット
	const float CLEAR_COLOR[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->firstShaderUser.DrawBefore(outputResource, CLEAR_COLOR);
}

/// <summary>
/// 1番シェーダにオブジェクトを描画
/// </summary>
/// <param name="_renderObject">描画オブジェクト</param>
void PlayCamera::RenderObjectToFirstShader(const ::CatchAFairy::Object::RenderInterface& _renderObject){
	// 入力リソース
	::CatchAFairy::Shader::User::PlayCameraEyeFirst::RenderUseResource inputResource;
	inputResource.checkCenterPosition = _renderObject.GetCircumscribedCircleCenterPosition();
	inputResource.checkSquareRadius = _renderObject.GetSquareCircumscribedCircleRadius();
	inputResource.checkModelToProjectionMatrix = _renderObject.GetWorld() * this->view * this->projection;
	inputResource.constantBuffer.modelToProjectionMatrix = inputResource.checkModelToProjectionMatrix.Transpose();
	inputResource.topology = _renderObject.GetTopology();
	inputResource.vertexBuffer = _renderObject.GetVertexBuffer();
	inputResource.indexBuffer = _renderObject.GetIndexBuffer();
	inputResource.indexCount = _renderObject.GetIndexCount();
	inputResource.diffuseShaderResourceView = _renderObject.GetDiffuseShaderResourceView();
	inputResource.normalShaderResourceView = _renderObject.GetNormalShaderResourceView();

	// 描画
	this->firstShaderUser.Draw(inputResource);
}

/// <summary>
/// 1番シェーダに複数オブジェクトを描画
/// </summary>
/// <param name="_renderObjects">描画オブジェクト</param>
void PlayCamera::RenderSameObjectToFirstShader(const ::CatchAFairy::Object::RenderSameInterface& _renderObjects){
	// 入力リソース
	::CatchAFairy::Shader::User::PlayCameraEyeFirst::MultipleRenderUseResource inputResource;
	inputResource.checkCenterPosition = _renderObjects.GetCircumscribedCircleCenterPosition();
	inputResource.checkSquareRadius = _renderObjects.GetSquareCircumscribedCircleRadius();
	inputResource.worldMatrixCount = _renderObjects.GetWorldCount();
	inputResource.worldMatrixArray = _renderObjects.GetWorldArray();
	inputResource.viewMatrix = this->view;
	inputResource.projectionMatrix = this->projection;
	inputResource.topology = _renderObjects.GetTopology();
	inputResource.vertexBuffer = _renderObjects.GetVertexBuffer();
	inputResource.indexBuffer = _renderObjects.GetIndexBuffer();
	inputResource.indexCount = _renderObjects.GetIndexCount();
	inputResource.diffuseShaderResourceView = _renderObjects.GetDiffuseShaderResourceView();
	inputResource.normalShaderResourceView = _renderObjects.GetNormalShaderResourceView();

	// 描画
	this->firstShaderUser.DrawMultiple(inputResource);
}

/// <summary>
/// 1番シェーダの描画後処理
/// </summary>
void PlayCamera::DrawAfterOfFirstShader(){
	this->firstShaderUser.DrawAfter();
}

/// <summary>
/// 影シェーダ描画
/// </summary>
void PlayCamera::RenderOfShadowShader(){
	if(this->directionalLight == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("directional light is NULL.");
	}

	// 出力リソース
	::CatchAFairy::Shader::User::CameraEyeShadow::OutputResource outputResource;
	outputResource.shadowRenderTargetView = this->shadowRenderTargetTexture.GetRenderTargetView();

	unsigned int width = 0U, height = 0U;
	this->shadowRenderTargetTexture.GetTextureSize(&width, &height);
	::Ishikawa::DirectX::Viewport viewport;
	viewport.leftTopX	= 0.0f;
	viewport.leftTopY	= 0.0f;
	viewport.width		= static_cast<float>(width);
	viewport.height		= static_cast<float>(height);
	viewport.minDepth	= 0.0f;
	viewport.maxDepth	= 1.0f;
	outputResource.viewport = &viewport;

	// 入力リソース
	::CatchAFairy::Shader::User::CameraEyeShadow::RenderUseResource inputResource;
	inputResource.constantBuffer.cameraProjectionToLightProjectionMatrix =
		(this->projection.Invert() * this->world *
			this->directionalLight->GetView() * this->directionalLight->GetProjection()).Transpose();
	inputResource.constantBuffer.directionalLightDirection = this->directionalLight->GetDirection() * this->view;
	inputResource.cameraPositionResourceView = this->positionRenderTargetTexture.GetShaderResourceView();
	inputResource.cameraNormalResourceView = this->normalRenderTargetTexture.GetShaderResourceView();
	inputResource.lightPositionResourceView = this->directionalLight->GetLastShaderResourceView();

	// 描画
	const ::Ishikawa::Math::Float4 CLEAR_COLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->shadowShaderUser.DrawBefore(outputResource, &CLEAR_COLOR);
	this->shadowShaderUser.Draw(inputResource);
	this->shadowShaderUser.DrawAfter();
}

/// <summary>
/// 反射光シェーダの描画前処理
/// </summary>
void PlayCamera::DrawBeforeOfReflectionLightShader(){
	// 出力リソース
	::CatchAFairy::Shader::User::CameraEyeReflectionLight::OutputResource outputResource;
	outputResource.reflectionLightRenderTargetView = this->reflectionLightRenderTargetTexture.GetRenderTargetView();
	outputResource.depthStencilView = this->reflectionLightDepthStencilTexture.GetDepthStencilView();

	unsigned int width = 0U, height = 0U;
	this->reflectionLightRenderTargetTexture.GetTextureSize(&width, &height);
	::Ishikawa::DirectX::Viewport viewport;
	viewport.leftTopX	= 0.0f;
	viewport.leftTopY	= 0.0f;
	viewport.width		= static_cast<float>(width);
	viewport.height		= static_cast<float>(height);
	viewport.minDepth	= 0.0f;
	viewport.maxDepth	= 1.0f;
	outputResource.viewport = &viewport;

	const float CLEAR_COLOR[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	this->reflectionLightShaderUser.DrawBefore(outputResource, CLEAR_COLOR);
}

/// <summary>
/// 反射光シェーダ描画
/// </summary>
/// <param name="_pointLightObject">ポイントライトオブジェクト</param>
void PlayCamera::RenderOfReflectionLightShader(const ::CatchAFairy::Object::PointLightInterface& _pointLightObject){
	// 入力リソース
	::CatchAFairy::Shader::User::CameraEyeReflectionLight::RenderUseResource inputResource;
	::Ishikawa::Math::Matrix localToView = this->view;
	inputResource.constantBuffer.localToViewMatrix = localToView.Transpose();
	inputResource.constantBuffer.localToProjectionMatrix = (localToView * this->projection).Transpose();
	inputResource.constantBuffer.invertProjectionMatrix = this->projection.Invert().Transpose();
	inputResource.positionResourceView = this->positionRenderTargetTexture.GetShaderResourceView();
	inputResource.normalResourceView = this->normalRenderTargetTexture.GetShaderResourceView();
	inputResource.vertexBuffer = _pointLightObject.GetVertexBuffer();
	inputResource.indexBuffer = _pointLightObject.GetIndexBuffer();
	inputResource.indexCount = _pointLightObject.GetIndexCount();

	// 描画
	this->reflectionLightShaderUser.Draw(inputResource);
}

/// <summary>
/// 反射光シェーダの描画後処理
/// </summary>
void PlayCamera::DrawAfterOfReflectionLightShader(){
	this->reflectionLightShaderUser.DrawAfter();
}

/// <summary>
/// ポイントライトシェーダの描画前処理
/// </summary>
void PlayCamera::DrawBeforeOfPointLightShader(){
	// 出力リソース
	::CatchAFairy::Shader::User::CameraEyePointLight::OutputResource outputResource;
	outputResource.pointLightRenderTargetView = this->pointLightRenderTargetTexture.GetRenderTargetView();
	outputResource.firstDepthStencilView = this->firstDepthStencilTexture.GetDepthStencilView();

	unsigned int width = 0U, height = 0U;
	this->reflectionLightRenderTargetTexture.GetTextureSize(&width, &height);
	::Ishikawa::DirectX::Viewport viewport;
	viewport.leftTopX	= 0.0f;
	viewport.leftTopY	= 0.0f;
	viewport.width		= static_cast<float>(width);
	viewport.height		= static_cast<float>(height);
	viewport.minDepth	= 0.0f;
	viewport.maxDepth	= 1.0f;
	outputResource.viewport = &viewport;

	const float CLEAR_COLOR[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	this->pointLightShaderUser.DrawBefore(outputResource, CLEAR_COLOR);
}

/// <summary>
/// ポイントライトシェーダ描画
/// </summary>
/// <param name="_pointLightObject">ポイントライトオブジェクト</param>
void PlayCamera::RenderOfPointLightShader(const ::CatchAFairy::Object::PointLightInterface& _pointLightObject){
	// 入力リソース
	::CatchAFairy::Shader::User::CameraEyePointLight::RenderUseResource inputResource;
	::Ishikawa::Math::Matrix localToView = this->view;
	inputResource.constantBuffer.localToViewMatrix = localToView.Transpose();
	inputResource.constantBuffer.localToProjectionMatrix = (localToView * this->projection).Transpose();
	inputResource.constantBuffer.projectionMatrix = this->projection.Transpose();
	inputResource.constantBuffer.invertProjectionMatrix = this->projection.Invert().Transpose();
	inputResource.positionResourceView = this->positionRenderTargetTexture.GetShaderResourceView();
	inputResource.vertexBuffer = _pointLightObject.GetVertexBuffer();
	inputResource.indexBuffer = _pointLightObject.GetIndexBuffer();
	inputResource.indexCount = _pointLightObject.GetIndexCount();

	// 描画
	this->pointLightShaderUser.Draw(inputResource);
}

/// <summary>
/// ポイントライトシェーダの描画後処理
/// </summary>
void PlayCamera::DrawAfterOfPointLightShader(){
	this->pointLightShaderUser.DrawAfter();
}

/// <summary>
/// 最終シェーダ描画
/// </summary>
void PlayCamera::RenderOfLastShader(){
	if(this->outputTargetTexture == nullptr){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("outputTargetTexture is NULL.");
	}

	// 出力リソース
	::CatchAFairy::Shader::User::PlayCameraEyeLast::OutputResource outputResource;
	outputResource.renderTargetView = this->outputTargetTexture->GetRenderTargetView();

	unsigned int width = 0U, height = 0U;
	this->outputTargetTexture->GetTextureSize(&width, &height);
	::Ishikawa::DirectX::Viewport viewport;
	viewport.leftTopX	= 0.0f;
	viewport.leftTopY	= 0.0f;
	viewport.width		= static_cast<float>(width);
	viewport.height		= static_cast<float>(height);
	viewport.minDepth	= 0.0f;
	viewport.maxDepth	= 1.0f;
	outputResource.viewport = &viewport;

	// 入力リソース
	// カメラ視点
	::CatchAFairy::Shader::User::PlayCameraEyeLast::RenderUseResource inputResource;
	inputResource.constantBuffer.directionalLightColor = this->directionalLight->GetColor();
	inputResource.topology = ::CatchAFairy::Shader::User::VertexTopology::TriangleStrip;
	inputResource.vertexBuffer = this->lastVertexBuffer;
	inputResource.indexBuffer = this->lastIndexBuffer;
	inputResource.indexCount = LAST_INDEX_COUNT;
	inputResource.diffuseShaderResourceView = this->diffuseRenderTargetTexture.GetShaderResourceView();
	inputResource.shadowShaderResourceView = this->shadowRenderTargetTexture.GetShaderResourceView();
	inputResource.reflectionLightShaderResourceView = this->reflectionLightRenderTargetTexture.GetShaderResourceView();
	inputResource.pointLightShaderResourceView = this->pointLightRenderTargetTexture.GetShaderResourceView();

	// 描画
	const ::Ishikawa::Math::Float4 CLEAR_COLOR(0.0f, 0.0f, 0.0f, 1.0f);
	this->lastShaderUser.DrawBefore(outputResource, &CLEAR_COLOR);
	this->lastShaderUser.Draw(inputResource);
	this->lastShaderUser.DrawAfter();
}


/// <summary>
/// 1番テクスチャ作成
/// </summary>
void PlayCamera::CreateFirstTextures(){
	// 設定取得
	unsigned int sampleDescCount = 1U;
	unsigned int sampleDescQuality = 0U;
	::Ishikawa::DirectX::Texture::Dimention textureDimention = ::Ishikawa::DirectX::Texture::Dimention::Texture2D;

	// テクスチャ作成
	this->diffuseRenderTargetTexture.Create(
		::Ishikawa::DirectX::Texture::BindableFlag::SR_RT,
		::CatchAFairy::Scene::Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		::CatchAFairy::Scene::Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		::Ishikawa::DirectX::Texture::Format::R8G8B8A8UNorm,
		0U,
		sampleDescCount,
		sampleDescQuality,
		textureDimention
	);
	this->normalRenderTargetTexture.Create(
		::Ishikawa::DirectX::Texture::BindableFlag::SR_RT,
		::CatchAFairy::Scene::Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		::CatchAFairy::Scene::Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		::Ishikawa::DirectX::Texture::Format::R32G32B32A32Float,
		0U,
		sampleDescCount,
		sampleDescQuality,
		textureDimention
	);
	this->positionRenderTargetTexture.Create(
		::Ishikawa::DirectX::Texture::BindableFlag::SR_RT,
		::CatchAFairy::Scene::Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		::CatchAFairy::Scene::Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		::Ishikawa::DirectX::Texture::Format::R32G32B32A32Float,
		0U,
		sampleDescCount,
		sampleDescQuality,
		textureDimention
	);
	this->firstDepthStencilTexture.Create(
		::Ishikawa::DirectX::Texture::BindableFlag::DepthStencil,
		::CatchAFairy::Scene::Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		::CatchAFairy::Scene::Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		::Ishikawa::DirectX::Texture::Format::D24UNormS8UInt,
		0U,
		sampleDescCount,
		sampleDescQuality,
		textureDimention
	);
}

/// <summary>
/// 影テクスチャ作成
/// </summary>
void PlayCamera::CreateShadowTextures(){
	// 設定取得
	unsigned int sampleDescCount = 1U;
	unsigned int sampleDescQuality = 0U;
	::Ishikawa::DirectX::Texture::Dimention textureDimention = ::Ishikawa::DirectX::Texture::Dimention::Texture2D;

	// テクスチャ作成
	this->shadowRenderTargetTexture.Create(
		::Ishikawa::DirectX::Texture::BindableFlag::SR_RT,
		::CatchAFairy::Scene::Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		::CatchAFairy::Scene::Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		::Ishikawa::DirectX::Texture::Format::R8G8B8A8UNorm,
		0U,
		sampleDescCount,
		sampleDescQuality,
		textureDimention
	);
}

/// <summary>
/// 反射光テクスチャ作成
/// </summary>
void PlayCamera::CreateReflectionLightTextures(){
	// 設定取得
	unsigned int sampleDescCount = 1U;
	unsigned int sampleDescQuality = 0U;
	::Ishikawa::DirectX::Texture::Dimention textureDimention = ::Ishikawa::DirectX::Texture::Dimention::Texture2D;

	// テクスチャ作成
	this->reflectionLightRenderTargetTexture.Create(
		::Ishikawa::DirectX::Texture::BindableFlag::SR_RT,
		::CatchAFairy::Scene::Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		::CatchAFairy::Scene::Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		::Ishikawa::DirectX::Texture::Format::R8G8B8A8UNorm,
		0U,
		sampleDescCount,
		sampleDescQuality,
		textureDimention
	);
	this->reflectionLightDepthStencilTexture.Create(
		::Ishikawa::DirectX::Texture::BindableFlag::DepthStencil,
		::CatchAFairy::Scene::Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		::CatchAFairy::Scene::Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		::Ishikawa::DirectX::Texture::Format::D24UNormS8UInt,
		0U,
		sampleDescCount,
		sampleDescQuality,
		textureDimention
	);
}

/// <summary>
/// ポイントライトテクスチャ作成
/// </summary>
void PlayCamera::CreatePointLightTextures(){
	// 設定取得
	unsigned int sampleDescCount = 1U;
	unsigned int sampleDescQuality = 0U;
	::Ishikawa::DirectX::Texture::Dimention textureDimention = ::Ishikawa::DirectX::Texture::Dimention::Texture2D;

	// テクスチャ作成
	this->pointLightRenderTargetTexture.Create(
		::Ishikawa::DirectX::Texture::BindableFlag::SR_RT,
		::CatchAFairy::Scene::Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		::CatchAFairy::Scene::Manager::SCENE_OUTPUT_TEXTURE_SIZE,
		::Ishikawa::DirectX::Texture::Format::R8G8B8A8UNorm,
		0U,
		sampleDescCount,
		sampleDescQuality,
		textureDimention
	);
}


/// <summary>
/// 最終頂点バッファ作成
/// </summary>
void PlayCamera::CreateLastVertexBuffer(){
	// 頂点情報
	const unsigned int vertexCount = 4U;
	::CatchAFairy::Shader::User::PlayCameraEyeLast::VSInput vertices[vertexCount];
	vertices[0].position = ::Ishikawa::Math::Float2(-1.0f, 1.0f);
	vertices[0].textureCoordinate = ::Ishikawa::Math::Float2( 0.0f, 0.0f);
	vertices[1].position = ::Ishikawa::Math::Float2(-1.0f,-1.0f);
	vertices[1].textureCoordinate = ::Ishikawa::Math::Float2( 0.0f, 1.0f);
	vertices[2].position = ::Ishikawa::Math::Float2( 1.0f, 1.0f);
	vertices[2].textureCoordinate = ::Ishikawa::Math::Float2( 1.0f, 0.0f);
	vertices[3].position = ::Ishikawa::Math::Float2( 1.0f,-1.0f);
	vertices[3].textureCoordinate = ::Ishikawa::Math::Float2( 1.0f, 1.0f);
	// 頂点バッファ作成
	this->lastVertexBuffer = this->lastShaderUser.CreateVertexBuffer(vertices, vertexCount);
}

/// <summary>
/// 最終インデックスバッファ作成
/// </summary>
void PlayCamera::CreateLastIndexBuffer(){
	// インデックス情報
	const unsigned long indices[LAST_INDEX_COUNT] = { 0U, 1U, 2U, 3U };
	// インデックスバッファ作成
	this->lastIndexBuffer = this->lastShaderUser.CreateIndexBuffer(indices, LAST_INDEX_COUNT);
}

/// <summary>
/// 最終頂点バッファ開放
/// </summary>
void PlayCamera::ReleaseLastVertexBuffer(){
	this->lastShaderUser.ReleaseBuffer(this->lastVertexBuffer);
	this->lastVertexBuffer = nullptr;
}

/// <summary>
/// 最終インデックスバッファ開放
/// </summary>
void PlayCamera::ReleaseLastIndexBuffer(){
	this->lastShaderUser.ReleaseBuffer(this->lastIndexBuffer);
	this->lastIndexBuffer = nullptr;
}


/// <summary>
/// 1番リソース初期化
/// </summary>
void PlayCamera::InitializeFirstResources(){
	this->diffuseRenderTargetTexture.Initialize();
	this->normalRenderTargetTexture.Initialize();
	this->positionRenderTargetTexture.Initialize();
	this->firstDepthStencilTexture.Initialize();
	this->firstShaderUser.Initialize();
}

/// <summary>
/// 最終リソース初期化
/// </summary>
void PlayCamera::InitializeLastResources(){
	this->lastShaderUser.Initialize();
}

/// <summary>
/// 影リソース初期化
/// </summary>
void PlayCamera::InitializeShadowResources(){
	this->shadowRenderTargetTexture.Initialize();
	this->shadowShaderUser.Initialize();
}

/// <summary>
/// 反射光リソース初期化
/// </summary>
void PlayCamera::InitializeReflectionLightResources(){
	this->reflectionLightRenderTargetTexture.Initialize();
	this->reflectionLightDepthStencilTexture.Initialize();
	this->reflectionLightShaderUser.Initialize();
}

/// <summary>
/// ポイントライトリソース初期化
/// </summary>
void PlayCamera::InitializePointLightResources(){
	this->pointLightRenderTargetTexture.Initialize();
	this->pointLightShaderUser.Initialize();
}
