//=============================================================================
//	PlayCamera.cpp
//
//	�v���C�J�����̃\�[�X�t�@�C��
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


/// <summary>�ŏI�C���f�b�N�X��</summary>
const unsigned int PlayCamera::LAST_INDEX_COUNT = 4U;
/// <summary>�Ǐ]�䗦</summary>
const float PlayCamera::FOLLOW_RATE = 0.1f;
/// <summary>����p</summary>
const float PlayCamera::VIEW_ANGLE = 1.2f/*1.57f*/;
/// <summary>�A�X�y�N�g��</summary>
const float PlayCamera::ASPECT_RATE = 0.75f;
/// <summary>�j�A�N���b�v</summary>
const float PlayCamera::NEAR_CLIP = 0.5f;
/// <summary>�t�@�[�N���b�v</summary>
const float PlayCamera::FAR_CLIP = 30.0f;
/// <summary>�N���b�v����</summary>
const float PlayCamera::CLIP_CENTER = 0.5f * (NEAR_CLIP + FAR_CLIP);


/// <summary>
/// �f�t�H���g�R���X�g���N�^
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
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayCamera::~PlayCamera(){
	// �������Ȃ�
}


/// <summary>
/// ���s���C�g�̐ݒ�
/// </summary>
/// <param name="_directionalLight">���s���C�g</param>
void PlayCamera::SetDirectionalLight(DirectionalLight* const _directionalLight){
	this->directionalLight = _directionalLight;

	if(this->directionalLight != nullptr){
		this->directionalLight->SetLookPosition(this->world._41, this->world._42, this->world._43);
	}
}


/// <summary>
/// �Ǐ]�I�u�W�F�N�g�̈ړ�
/// </summary>
/// <param name="_followMatrix">�Ǐ]���邽�߂̍s��</param>
void PlayCamera::OnMoveFollowingTheObject(const ::Ishikawa::Math::Matrix& _followMatrix){
	::Ishikawa::Math::Matrix invView = this->local * _followMatrix;
	this->targetView = invView.Invert();
}



/// <summary>
/// �Q�[���̃f�o�C�X�Ɉˑ����郊�\�[�X�̒ǉ�
/// </summary>
/// <param name="_itDevice">����f�o�C�X</param>
void PlayCamera::AddITDeviceDependentResources(::Ishikawa::DirectX::Device::Device* const _itDevice){
	if(_itDevice == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_itDevice is NULL.");
	}

	// �Q�[���f�o�C�X�Ƀ��\�[�X��ǉ�
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
/// �f�o�C�X�ˑ����\�[�X�쐬
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
/// �f�o�C�X�ˑ����\�[�X�J��
/// </summary>
void PlayCamera::ReleaseDeviceDependentResources(){
	this->ReleaseLastIndexBuffer();
	this->ReleaseLastVertexBuffer();
}


/// <summary>
/// ������
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
/// �X�V
/// </summary>
/// <param name="_elapsedTime">�X�V�Ԋu</param>
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
/// 1�ԃV�F�[�_�̕`��O����
/// </summary>
void PlayCamera::DrawBeforeOfFirstShader(){
	// �o�̓��\�[�X
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

	// ��񃊃Z�b�g
	const float CLEAR_COLOR[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->firstShaderUser.DrawBefore(outputResource, CLEAR_COLOR);
}

/// <summary>
/// 1�ԃV�F�[�_�ɃI�u�W�F�N�g��`��
/// </summary>
/// <param name="_renderObject">�`��I�u�W�F�N�g</param>
void PlayCamera::RenderObjectToFirstShader(const ::CatchAFairy::Object::RenderInterface& _renderObject){
	// ���̓��\�[�X
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

	// �`��
	this->firstShaderUser.Draw(inputResource);
}

/// <summary>
/// 1�ԃV�F�[�_�ɕ����I�u�W�F�N�g��`��
/// </summary>
/// <param name="_renderObjects">�`��I�u�W�F�N�g</param>
void PlayCamera::RenderSameObjectToFirstShader(const ::CatchAFairy::Object::RenderSameInterface& _renderObjects){
	// ���̓��\�[�X
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

	// �`��
	this->firstShaderUser.DrawMultiple(inputResource);
}

/// <summary>
/// 1�ԃV�F�[�_�̕`��㏈��
/// </summary>
void PlayCamera::DrawAfterOfFirstShader(){
	this->firstShaderUser.DrawAfter();
}

/// <summary>
/// �e�V�F�[�_�`��
/// </summary>
void PlayCamera::RenderOfShadowShader(){
	if(this->directionalLight == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("directional light is NULL.");
	}

	// �o�̓��\�[�X
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

	// ���̓��\�[�X
	::CatchAFairy::Shader::User::CameraEyeShadow::RenderUseResource inputResource;
	inputResource.constantBuffer.cameraProjectionToLightProjectionMatrix =
		(this->projection.Invert() * this->world *
			this->directionalLight->GetView() * this->directionalLight->GetProjection()).Transpose();
	inputResource.constantBuffer.directionalLightDirection = this->directionalLight->GetDirection() * this->view;
	inputResource.cameraPositionResourceView = this->positionRenderTargetTexture.GetShaderResourceView();
	inputResource.cameraNormalResourceView = this->normalRenderTargetTexture.GetShaderResourceView();
	inputResource.lightPositionResourceView = this->directionalLight->GetLastShaderResourceView();

	// �`��
	const ::Ishikawa::Math::Float4 CLEAR_COLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->shadowShaderUser.DrawBefore(outputResource, &CLEAR_COLOR);
	this->shadowShaderUser.Draw(inputResource);
	this->shadowShaderUser.DrawAfter();
}

/// <summary>
/// ���ˌ��V�F�[�_�̕`��O����
/// </summary>
void PlayCamera::DrawBeforeOfReflectionLightShader(){
	// �o�̓��\�[�X
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
/// ���ˌ��V�F�[�_�`��
/// </summary>
/// <param name="_pointLightObject">�|�C���g���C�g�I�u�W�F�N�g</param>
void PlayCamera::RenderOfReflectionLightShader(const ::CatchAFairy::Object::PointLightInterface& _pointLightObject){
	// ���̓��\�[�X
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

	// �`��
	this->reflectionLightShaderUser.Draw(inputResource);
}

/// <summary>
/// ���ˌ��V�F�[�_�̕`��㏈��
/// </summary>
void PlayCamera::DrawAfterOfReflectionLightShader(){
	this->reflectionLightShaderUser.DrawAfter();
}

/// <summary>
/// �|�C���g���C�g�V�F�[�_�̕`��O����
/// </summary>
void PlayCamera::DrawBeforeOfPointLightShader(){
	// �o�̓��\�[�X
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
/// �|�C���g���C�g�V�F�[�_�`��
/// </summary>
/// <param name="_pointLightObject">�|�C���g���C�g�I�u�W�F�N�g</param>
void PlayCamera::RenderOfPointLightShader(const ::CatchAFairy::Object::PointLightInterface& _pointLightObject){
	// ���̓��\�[�X
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

	// �`��
	this->pointLightShaderUser.Draw(inputResource);
}

/// <summary>
/// �|�C���g���C�g�V�F�[�_�̕`��㏈��
/// </summary>
void PlayCamera::DrawAfterOfPointLightShader(){
	this->pointLightShaderUser.DrawAfter();
}

/// <summary>
/// �ŏI�V�F�[�_�`��
/// </summary>
void PlayCamera::RenderOfLastShader(){
	if(this->outputTargetTexture == nullptr){
		// �ُ�I��
		::Ishikawa::Common::Exception::FunctionFailed("outputTargetTexture is NULL.");
	}

	// �o�̓��\�[�X
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

	// ���̓��\�[�X
	// �J�������_
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

	// �`��
	const ::Ishikawa::Math::Float4 CLEAR_COLOR(0.0f, 0.0f, 0.0f, 1.0f);
	this->lastShaderUser.DrawBefore(outputResource, &CLEAR_COLOR);
	this->lastShaderUser.Draw(inputResource);
	this->lastShaderUser.DrawAfter();
}


/// <summary>
/// 1�ԃe�N�X�`���쐬
/// </summary>
void PlayCamera::CreateFirstTextures(){
	// �ݒ�擾
	unsigned int sampleDescCount = 1U;
	unsigned int sampleDescQuality = 0U;
	::Ishikawa::DirectX::Texture::Dimention textureDimention = ::Ishikawa::DirectX::Texture::Dimention::Texture2D;

	// �e�N�X�`���쐬
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
/// �e�e�N�X�`���쐬
/// </summary>
void PlayCamera::CreateShadowTextures(){
	// �ݒ�擾
	unsigned int sampleDescCount = 1U;
	unsigned int sampleDescQuality = 0U;
	::Ishikawa::DirectX::Texture::Dimention textureDimention = ::Ishikawa::DirectX::Texture::Dimention::Texture2D;

	// �e�N�X�`���쐬
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
/// ���ˌ��e�N�X�`���쐬
/// </summary>
void PlayCamera::CreateReflectionLightTextures(){
	// �ݒ�擾
	unsigned int sampleDescCount = 1U;
	unsigned int sampleDescQuality = 0U;
	::Ishikawa::DirectX::Texture::Dimention textureDimention = ::Ishikawa::DirectX::Texture::Dimention::Texture2D;

	// �e�N�X�`���쐬
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
/// �|�C���g���C�g�e�N�X�`���쐬
/// </summary>
void PlayCamera::CreatePointLightTextures(){
	// �ݒ�擾
	unsigned int sampleDescCount = 1U;
	unsigned int sampleDescQuality = 0U;
	::Ishikawa::DirectX::Texture::Dimention textureDimention = ::Ishikawa::DirectX::Texture::Dimention::Texture2D;

	// �e�N�X�`���쐬
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
/// �ŏI���_�o�b�t�@�쐬
/// </summary>
void PlayCamera::CreateLastVertexBuffer(){
	// ���_���
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
	// ���_�o�b�t�@�쐬
	this->lastVertexBuffer = this->lastShaderUser.CreateVertexBuffer(vertices, vertexCount);
}

/// <summary>
/// �ŏI�C���f�b�N�X�o�b�t�@�쐬
/// </summary>
void PlayCamera::CreateLastIndexBuffer(){
	// �C���f�b�N�X���
	const unsigned long indices[LAST_INDEX_COUNT] = { 0U, 1U, 2U, 3U };
	// �C���f�b�N�X�o�b�t�@�쐬
	this->lastIndexBuffer = this->lastShaderUser.CreateIndexBuffer(indices, LAST_INDEX_COUNT);
}

/// <summary>
/// �ŏI���_�o�b�t�@�J��
/// </summary>
void PlayCamera::ReleaseLastVertexBuffer(){
	this->lastShaderUser.ReleaseBuffer(this->lastVertexBuffer);
	this->lastVertexBuffer = nullptr;
}

/// <summary>
/// �ŏI�C���f�b�N�X�o�b�t�@�J��
/// </summary>
void PlayCamera::ReleaseLastIndexBuffer(){
	this->lastShaderUser.ReleaseBuffer(this->lastIndexBuffer);
	this->lastIndexBuffer = nullptr;
}


/// <summary>
/// 1�ԃ��\�[�X������
/// </summary>
void PlayCamera::InitializeFirstResources(){
	this->diffuseRenderTargetTexture.Initialize();
	this->normalRenderTargetTexture.Initialize();
	this->positionRenderTargetTexture.Initialize();
	this->firstDepthStencilTexture.Initialize();
	this->firstShaderUser.Initialize();
}

/// <summary>
/// �ŏI���\�[�X������
/// </summary>
void PlayCamera::InitializeLastResources(){
	this->lastShaderUser.Initialize();
}

/// <summary>
/// �e���\�[�X������
/// </summary>
void PlayCamera::InitializeShadowResources(){
	this->shadowRenderTargetTexture.Initialize();
	this->shadowShaderUser.Initialize();
}

/// <summary>
/// ���ˌ����\�[�X������
/// </summary>
void PlayCamera::InitializeReflectionLightResources(){
	this->reflectionLightRenderTargetTexture.Initialize();
	this->reflectionLightDepthStencilTexture.Initialize();
	this->reflectionLightShaderUser.Initialize();
}

/// <summary>
/// �|�C���g���C�g���\�[�X������
/// </summary>
void PlayCamera::InitializePointLightResources(){
	this->pointLightRenderTargetTexture.Initialize();
	this->pointLightShaderUser.Initialize();
}
