//=============================================================================
//	DirectionalLight.cpp
//
//	���s���C�g�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "DirectionalLight.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "Ishikawa/DirectX/Texture/ITTextureData.h"
#include "Ishikawa/DirectX/ITViewport.h"
#include "CatchAFairy/Object/RenderObjectInterface.h"
#include "CatchAFairy/Object/RenderSameObjectInterface.h"

using namespace ::CatchAFairy::Object::PlayScene;


/// <summary>�e�N�X�`���T�C�Y</summary>
const unsigned int DirectionalLight::TEXTURE_SIZE = 2048U;
/// <summary>�����_����̋���</summary>
const float DirectionalLight::DISTANCE = 15.0f;
/// <summary>Y���̉�]�p[���p������]</summary>
const float DirectionalLight::ROTATE_Y = -1.5f;
/// <summary>X���̉�]�p[����������]</summary>
const float DirectionalLight::ROTATE_X = -1.4f;
/// <summary>�\���T�C�Y</summary>
const float DirectionalLight::VIEW_SIZE = 30.0f;
/// <summary>�j�A�N���b�v</summary>
const float DirectionalLight::NEAR_CLIP = 0.1f;
/// <summary>�t�@�[�N���b�v</summary>
const float DirectionalLight::FAR_CLIP = 110.0f;
/// <summary>�f�t�H���g�F</summary>
const ::Ishikawa::Math::Float4 DirectionalLight::DEFAULT_COLOR(1.0f, 1.0f, 1.0f, 1.0f);


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
DirectionalLight::DirectionalLight() :
	::CatchAFairy::Object::CameraInterface(),
	renderTargetTexture(),
	depthStencilTexture(),
	directionalLightShaderUser(),
	local(::Ishikawa::Math::Matrix::IDENTITY),
	world(::Ishikawa::Math::Matrix::IDENTITY),
	view(::Ishikawa::Math::Matrix::IDENTITY),
	projection(::Ishikawa::Math::Matrix::IDENTITY),
	color(DEFAULT_COLOR)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
DirectionalLight::~DirectionalLight(){
	// �������Ȃ�
}


/// <summary>
/// �����_�̐ݒ�
/// </summary>
/// <param name="_x">�����_X���W</param>
/// <param name="_y">�����_Y���W</param>
/// <param name="_z">�����_Z���W</param>
void DirectionalLight::SetLookPosition(float _x, float _y, float _z){
	::Ishikawa::Math::Matrix translate = ::Ishikawa::Math::Matrix::IDENTITY;
	translate._41 += _x;
	translate._42 += _y;
	translate._43 += _z;

	this->world = this->local * translate;
	this->view = this->world.Invert();
}


/// <summary>
/// �Q�[���̃f�o�C�X�Ɉˑ����郊�\�[�X�̒ǉ�
/// </summary>
/// <param name="_itDevice">����f�o�C�X</param>
void DirectionalLight::AddITDeviceDependentResources(::Ishikawa::DirectX::Device::Device* const _itDevice){
	// �����o��o�^
	_itDevice->AddDependentResource(&(this->renderTargetTexture));
	_itDevice->AddDependentResource(&(this->depthStencilTexture));
	_itDevice->AddDependentResource(&(this->directionalLightShaderUser));
}

/// <summary>
/// �f�o�C�X�ˑ����\�[�X�쐬
/// </summary>
void DirectionalLight::CreateDeviceDependentResources(){
	this->CreateTextures();
}

/// <summary>
/// �f�o�C�X�ˑ����\�[�X�J��
/// </summary>
void DirectionalLight::ReleaseDeviceDependentResources(){
	// �������Ȃ�
}



/// <summary>
/// ������
/// </summary>
void DirectionalLight::Initialize(){
	// �����o�̏�����
	this->renderTargetTexture.Initialize();
	this->depthStencilTexture.Initialize();
	this->directionalLightShaderUser.Initialize();

	::Ishikawa::Math::Matrix translate = ::Ishikawa::Math::CreateTranslationMatrix(0.0f, 0.0f,-DISTANCE);
	::Ishikawa::Math::Matrix rotateY = ::Ishikawa::Math::CreateRotationMatrixOfAxisY(ROTATE_Y);
	::Ishikawa::Math::Matrix rotateX = ::Ishikawa::Math::CreateRotationMatrixOfAxisX(ROTATE_X);
	this->local = translate * rotateX * rotateY;

	this->world = this->local;
	this->view = this->world.Invert();

	this->projection = ::Ishikawa::Math::CreateOrthographicMatrix(VIEW_SIZE, VIEW_SIZE, NEAR_CLIP, FAR_CLIP);
}


/// <summary>
/// ���s���C�g�V�F�[�_�`��O����
/// </summary>
void DirectionalLight::DirectionalLightShaderDrawBefore(){
	// �o�̓��\�[�X
	::CatchAFairy::Shader::User::DirectionalLight::OutputResource outputResource;
	outputResource.positionRenderTargetView = this->renderTargetTexture.GetRenderTargetView();
	outputResource.depthStencilView = this->depthStencilTexture.GetDepthStencilView();

	unsigned int width = 0U, height = 0U;
	this->renderTargetTexture.GetTextureSize(&width, &height);
	::Ishikawa::DirectX::Viewport viewport;
	viewport.leftTopX	= 0.0f;
	viewport.leftTopY	= 0.0f;
	viewport.width		= static_cast<float>(width);
	viewport.height		= static_cast<float>(height);
	viewport.minDepth	= 0.0f;
	viewport.maxDepth	= 1.0f;
	outputResource.viewport = &viewport;

	// �`��
	const float CLEAR_COLOR[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
	this->directionalLightShaderUser.DrawBefore(outputResource, CLEAR_COLOR);
}

/// <summary>
/// ���s���C�g�V�F�[�_�ŃI�u�W�F�N�g��`��
/// </summary>
/// <param name="_renderObject">�`��I�u�W�F�N�g</param>
void DirectionalLight::RenderObjectToDirectionalLightShader(
	const ::CatchAFairy::Object::RenderInterface& _renderObject
){
	// ���̓��\�[�X
	::CatchAFairy::Shader::User::DirectionalLight::RenderUseResource inputResource;
	inputResource.checkCenterPosition = _renderObject.GetCircumscribedCircleCenterPosition();
	inputResource.checkSquareRadius = _renderObject.GetSquareCircumscribedCircleRadius();
	inputResource.checkModelToProjectionMatrix = _renderObject.GetWorld() * this->view * this->projection;
	inputResource.constantBuffer.modelToProjectionMatrix = inputResource.checkModelToProjectionMatrix.Transpose();
	inputResource.topology = _renderObject.GetTopology();
	inputResource.vertexBuffer = _renderObject.GetVertexBuffer();
	inputResource.indexBuffer = _renderObject.GetIndexBuffer();
	inputResource.indexCount = _renderObject.GetIndexCount();
	inputResource.diffuseShaderResourceView = _renderObject.GetDiffuseShaderResourceView();

	// �`��
	this->directionalLightShaderUser.Draw(inputResource);
}

/// <summary>
/// ���s���C�g�V�F�[�_�ŕ����I�u�W�F�N�g��`��
/// </summary>
/// <param name="_renderObjects">�`��I�u�W�F�N�g</param>
void DirectionalLight::RenderSameObjectToDirectionalLightShader(
	const ::CatchAFairy::Object::RenderSameInterface& _renderObjects
){
	// ���̓��\�[�X
	::CatchAFairy::Shader::User::DirectionalLight::MultipleRenderUseResource inputResource;
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

	// �`��
	this->directionalLightShaderUser.DrawMultiple(inputResource);
}

/// <summary>
/// ���s���C�g�V�F�[�_�`��㏈��
/// </summary>
void DirectionalLight::DirectionalLightShaderDrawAfter(){
	this->directionalLightShaderUser.DrawAfter();
}



/// <summary>
/// �e�N�X�`���쐬
/// </summary>
void DirectionalLight::CreateTextures(){
	// �ݒ�擾
	unsigned int sampleDescCount = 1U;
	unsigned int sampleDescQuality = 0U;
	::Ishikawa::DirectX::Texture::Dimention textureDimention = ::Ishikawa::DirectX::Texture::Dimention::Texture2D;

	// �e�N�X�`���쐬
	this->renderTargetTexture.Create(
		::Ishikawa::DirectX::Texture::BindableFlag::SR_RT,
		TEXTURE_SIZE,
		TEXTURE_SIZE,
		::Ishikawa::DirectX::Texture::Format::R8G8B8A8UNorm,
		0U,
		sampleDescCount,
		sampleDescQuality,
		textureDimention
	);
	this->depthStencilTexture.Create(
		::Ishikawa::DirectX::Texture::BindableFlag::DepthStencil,
		TEXTURE_SIZE,
		TEXTURE_SIZE,
		::Ishikawa::DirectX::Texture::Format::D24UNormS8UInt,
		0U,
		sampleDescCount,
		sampleDescQuality,
		textureDimention
	);
}
