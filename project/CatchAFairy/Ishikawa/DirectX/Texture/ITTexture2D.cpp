//=============================================================================
//	ITTexture2D.cpp
//
//	����2D�e�N�X�`���̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITTexture2D.h"
#pragma warning(disable:4061)
#pragma warning(disable:4365)
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#pragma warning(disable:4917)
#include <d3d11_1.h>
#pragma warning(default:4917)
#pragma warning(default:4820)
#pragma warning(default:4668)
#pragma warning(default:4365)
#pragma warning(default:4061)
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "ITTextureData.h"

using namespace Ishikawa::DirectX::Texture;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Texture2D::Texture2D() :
	::Ishikawa::DirectX::Device::DependentResource::Base(),
	Texture2DInterface(),
	bindableFlag(BindableFlag::ShaderResource),
	currentBindFlag(BindFlag::NotBind),
	width(0U),
	height(0U),
	format(Format::Unknown),
	cpuAccessFlag(0U),
	sampleDescCount(0U),
	sampleDescQuality(0U),
	dimention(Dimention::Texture2D),
	texture(nullptr),
	shaderResourceView(nullptr),
	renderTargetView(nullptr),
	depthStencilView(nullptr)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Texture2D::~Texture2D(){
	// ���\�[�X�̊J��
	this->ReleaseAll();
}


/// <summary>
/// �V�F�[�_���\�[�X�r���[�̎擾
/// </summary>
/// <returns>�V�F�[�_���\�[�X�r���[</returns>
ID3D11ShaderResourceView* Texture2D::GetShaderResourceView() const {
	// �o�C���h�\���𒲂ׂ�
	if(!(this->IsBindable(BindFlag::ShaderResource))){
		// �o�C���h�ł��Ȃ�
		throw ::Ishikawa::Common::Exception::FunctionFailed("Can't bind texture to shader resource view.");
	}
	// �o�C���h��ؑւ�
	if(this->currentBindFlag != BindFlag::ShaderResource){
		Texture2D* const _this = const_cast<Texture2D*>(this);
		_this->ReleaseCurrentView();
		_this->CreateShaderResourceView();
		_this->currentBindFlag = BindFlag::ShaderResource;
	}

	return this->shaderResourceView;
}

/// <summary>
/// �����_�[�^�[�Q�b�g�r���[�̎擾
/// </summary>
/// <returns>�����_�[�^�[�Q�b�g�r���[</returns>
ID3D11RenderTargetView* Texture2D::GetRenderTargetView() const {
	// �o�C���h�\���𒲂ׂ�
	if(!(this->IsBindable(BindFlag::RenderTarget))){
		// �o�C���h�ł��Ȃ�
		throw ::Ishikawa::Common::Exception::FunctionFailed("Can't bind texture to render target view.");
	}
	// �o�C���h��ؑւ�
	if(this->currentBindFlag != BindFlag::RenderTarget){
		Texture2D* const _this = const_cast<Texture2D*>(this);
		_this->ReleaseCurrentView();
		_this->CreateRenderTargetView();
		_this->currentBindFlag = BindFlag::RenderTarget;
	}
	return this->renderTargetView;
}

/// <summary>
/// �f�v�X�X�e���V���r���[�̎擾
/// </summary>
/// <returns>�f�v�X�X�e���V���r���[</returns>
ID3D11DepthStencilView* Texture2D::GetDepthStencilView() const {
	// �o�C���h�\���𒲂ׂ�
	if(!(this->IsBindable(BindFlag::DepthStencil))){
		// �o�C���h�ł��Ȃ�
		throw ::Ishikawa::Common::Exception::FunctionFailed("Can't bind texture to depth stencil view.");
	}
	// �o�C���h��ؑւ�
	if(this->currentBindFlag != BindFlag::DepthStencil){
		Texture2D* const _this = const_cast<Texture2D*>(this);
		_this->ReleaseCurrentView();
		_this->CreateDepthStencilView();
		_this->currentBindFlag = BindFlag::DepthStencil;
	}

	return this->depthStencilView;
}

/// <summary>
/// �o�C���h�\��?
/// </summary>
/// <param name="_bindFlag">�o�C���h�t���O</param>
/// <returns>true:�\ false:�s�\</returns>
bool Texture2D::IsBindable(BindFlag _bindFlag) const{
	switch(_bindFlag){
	case BindFlag::NotBind:
		return false;

	case BindFlag::ShaderResource:
		return	((this->bindableFlag == BindableFlag::ShaderResource) ||
				 (this->bindableFlag == BindableFlag::SR_RT) ||
				 (this->bindableFlag == BindableFlag::SR_DS));

	case BindFlag::RenderTarget:
		return	((this->bindableFlag == BindableFlag::RenderTarget) ||
				 (this->bindableFlag == BindableFlag::SR_RT));

	case BindFlag::DepthStencil:
		return	((this->bindableFlag == BindableFlag::DepthStencil) ||
				 (this->bindableFlag == BindableFlag::SR_DS));

	default:
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_bindFlag is unknown.");
	}
}

/// <summary>
/// �f�o�C�X�̎擾
/// </summary>
/// <returns>�f�o�C�X</returns>
ID3D11Device* Texture2D::GetDevice() const{
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetDependentITDevice();
	if(itDevice == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("itDevice is NULL.");
	}
	ID3D11Device* const device = itDevice->GetDevice();
	if(device == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("device is NULL.");
	}

	return device;
}


/// <summary>
/// ������
/// </summary>
void Texture2D::Initialize(){
	// �����o�̏�����
	this->InitializeITDeviceDependentResource();
}


/// <summary>
/// �f�o�C�X�ɓǂݍ��܂ꂽ
/// </summary>
void Texture2D::OnDeviceLoaded(){
	// �������Ȃ�
}

/// <summary>
/// �f�o�C�X���������ꂽ
/// </summary>
void Texture2D::OnDeviceRestored(){
	// �������Ȃ�
}

/// <summary>
/// �f�o�C�X���j�����ꂽ
/// </summary>
void Texture2D::OnDeviceLost(){
	// ���\�[�X�̊J��
	this->ReleaseAll();
}


/// <summary>
/// �쐬
/// </summary>
/// <param name="_bindableFlag">�o�C���h�\�t���O</param>
/// <param name="_width">��</param>
/// <param name="_height">����</param>
/// <param name="_format">�t�H�[�}�b�g</param>
/// <param name="_cpuAccessFlag">CPU�A�N�Z�X�t���O</param>
/// <param name="_sampleDescCount">�T���v���ݒ�J�E���g</param>
/// <param name="_sampleDescQuality">�T���v���ݒ�N�H���e�B</param>
/// <param name="_dimention">�f�B�����V����</param>
void Texture2D::Create(
	BindableFlag _bindableFlag,
	unsigned int _width,
	unsigned int _height,
	Format _format,
	unsigned int _cpuAccessFlag,
	unsigned int _sampleDescCount,
	unsigned int _sampleDescQuality,
	Dimention _dimention
){
	// ���\�[�X�̊J��
	this->ReleaseAll();

	// �����o�ύX
	this->bindableFlag = _bindableFlag;
	this->width = _width;
	this->height = _height;
	this->format = _format;
	this->cpuAccessFlag = _cpuAccessFlag;
	this->sampleDescCount = _sampleDescCount;
	this->sampleDescQuality = _sampleDescQuality;
	this->dimention = _dimention;

	// �e�N�X�`���쐬
	this->CreateTexture();
}

/// <summary>
/// �T�C�Y�ύX
/// </summary>
/// <param name="_width">��</param>
/// <param name="_height">����</param>
void Texture2D::ChangeSize(unsigned int _width, unsigned int _height){
	// ���\�[�X�̊J��
	this->ReleaseAll();

	// �����o�ύX
	this->width = _width;
	this->height = _height;

	// �e�N�X�`���쐬
	this->CreateTexture();
}

/// <summary>
/// �e�N�X�`���̍쐬
/// </summary>
void Texture2D::CreateTexture(){
	// �ݒ�
	CD3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Format = ConvertFormatToDxgiFormat(this->format);
	textureDesc.Width = this->width;
	textureDesc.Height = this->height;
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.BindFlags = ConvertBindableFlagToUInt(this->bindableFlag);
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.CPUAccessFlags = this->cpuAccessFlag;
	textureDesc.SampleDesc.Count = this->sampleDescCount;
	textureDesc.SampleDesc.Quality = this->sampleDescQuality;
	textureDesc.MiscFlags = 0;

	ID3D11Device* const device = this->GetDevice();
	// �쐬
	HRESULT hr = device->CreateTexture2D(
		&textureDesc,
		nullptr,
		&(this->texture)
	);
	if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create depth stencil texture.");
	}
}

/// <summary>
/// �V�F�[�_���\�[�X�r���[�̍쐬
/// </summary>
void Texture2D::CreateShaderResourceView(){
	// �ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
	shaderResourceViewDesc.Format = ConvertFormatToDxgiFormat(this->format);
	shaderResourceViewDesc.ViewDimension = ConvertDimentionToD3D11SRVDimention(this->dimention);
	shaderResourceViewDesc.Texture2D.MipLevels = 1U;

	ID3D11Device* const device = this->GetDevice();
	// �e�N�X�`������V�F�[�_���\�[�X�r���[���쐬
	HRESULT hr = device->CreateShaderResourceView(
		this->texture,
		&shaderResourceViewDesc,
		&(this->shaderResourceView)
	);
	if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create shader resouce view.");
	}
}

/// <summary>
/// �����_�[�^�[�Q�b�g�r���[�̍쐬
/// </summary>
void Texture2D::CreateRenderTargetView(){
	// �ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	renderTargetViewDesc.Format = ConvertFormatToDxgiFormat(this->format);
	renderTargetViewDesc.ViewDimension = ConvertDimentionToD3D11RTVDimention(this->dimention);
	renderTargetViewDesc.Texture2D.MipSlice = 0U;

	ID3D11Device* const device = this->GetDevice();
	// �e�N�X�`�����烌���_�[�^�[�Q�b�g�r���[���쐬
	HRESULT hr = device->CreateRenderTargetView(
		this->texture,
		&renderTargetViewDesc,
		&(this->renderTargetView)
	);
	if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create render target view.");
	}
}

/// <summary>
/// �f�v�X�X�e���V���r���[�̍쐬
/// </summary>
void Texture2D::CreateDepthStencilView(){
	// �ݒ�
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = ConvertFormatToDxgiFormat(this->format);
	depthStencilViewDesc.ViewDimension = ConvertDimentionToD3D11DSVDimention(this->dimention);
	depthStencilViewDesc.Flags = 0U;
	depthStencilViewDesc.Texture2D.MipSlice = 0U;

	ID3D11Device* const device = this->GetDevice();
	// �e�N�X�`������f�v�X�X�e���V���r���[���쐬
	HRESULT hr = device->CreateDepthStencilView(
		this->texture,
		&depthStencilViewDesc,
		&(this->depthStencilView)
	);
	if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create depth stencil view.");
	}
}


/// <summary>
/// �S���\�[�X�̊J��
/// </summary>
void Texture2D::ReleaseAll(){
	this->ReleaseDepthStencilView();
	this->ReleaseRenderTargetView();
	this->ReleaseShaderResourceView();
	this->ReleaseTexture();

	this->currentBindFlag = BindFlag::NotBind;
}

/// <summary>
/// �e�N�X�`���̊J��
/// </summary>
void Texture2D::ReleaseTexture(){
	if(this->texture != nullptr){
		this->texture->Release();
		this->texture = nullptr;
	}
}

/// <summary>
/// ���ݗ��p���̃r���[���J��
/// </summary>
void Texture2D::ReleaseCurrentView(){
	switch(this->currentBindFlag){
	case BindFlag::NotBind:
		// �������Ȃ�
		break;

	case BindFlag::ShaderResource:
		this->ReleaseShaderResourceView();
		this->currentBindFlag = BindFlag::NotBind;
		break;

	case BindFlag::RenderTarget:
		this->ReleaseRenderTargetView();
		this->currentBindFlag = BindFlag::NotBind;
		break;

	case BindFlag::DepthStencil:
		this->ReleaseDepthStencilView();
		this->currentBindFlag = BindFlag::NotBind;
		break;

	default:
		// �ςȒl�������Ă���̂Ńr���[��S�ĊJ��
		this->ReleaseShaderResourceView();
		this->ReleaseRenderTargetView();
		this->ReleaseDepthStencilView();
		this->currentBindFlag = BindFlag::NotBind;
		break;
	}
}

/// <summary>
/// �V�F�[�_���\�[�X�r���[�̊J��
/// </summary>
void Texture2D::ReleaseShaderResourceView(){
	if(this->shaderResourceView != nullptr){
		this->shaderResourceView->Release();
		this->shaderResourceView = nullptr;
	}
}

/// <summary>
/// �����_�[�^�[�Q�b�g�r���[�̊J��
/// </summary>
void Texture2D::ReleaseRenderTargetView(){
	if(this->renderTargetView != nullptr){
		this->renderTargetView->Release();
		this->renderTargetView = nullptr;
	}
}

/// <summary>
/// �f�v�X�X�e���V���r���[�̊J��
/// </summary>
void Texture2D::ReleaseDepthStencilView(){
	if(this->depthStencilView != nullptr){
		this->depthStencilView->Release();
		this->depthStencilView = nullptr;
	}
}
