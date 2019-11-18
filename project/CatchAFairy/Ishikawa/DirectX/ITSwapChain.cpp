//=============================================================================
//	ITSwapChain.cpp
//
//	����X���b�v�`�F�[���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITSwapChain.h"
#pragma warning(disable:4626)
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#include <wrl/client.h>
#pragma warning(default:4820)
#pragma warning(default:4668)
#pragma warning(default:4626)
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
#include "Ishikawa/Common/DebugLog.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "Ishikawa/Window/ITWindow.h"

using namespace Ishikawa::DirectX;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
SwapChain::SwapChain() :
	sampleDescCount(0U),
	sampleDescQuality(0U),
	swapChain(nullptr),
	bufferCount(2U),
	bufferFormat(DXGI_FORMAT_B8G8R8A8_UNORM),
	width(1U),
	height(1U),
	renderTargetTexture(nullptr),
	renderTargetView(nullptr),
	depthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT),
	depthStencilViewDimention(D3D11_DSV_DIMENSION_TEXTURE2D),
	depthStencilTexture(nullptr),
	depthStencilView(nullptr),
	dependentDevice(nullptr)
{
	// �T���v���ݒ���f�t�H���g�ɂ��Ă���
	this->ChangeDefaultSampleDesc();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SwapChain::~SwapChain(){
	this->ReleaseDepthStencilView();
	this->ReleaseDepthStencilTexture();
	this->ReleaseRenderTargetView();
	this->ReleaseRenderTargetTexture();
	this->ReleaseSwapChain();
}


/// <summary>
/// �쐬
/// </summary>
/// <param name="_device">�f�o�C�X</param>
/// <param name="_window">�o�̓E�B���h�E�̃n���h��</param>
void SwapChain::Create(Device::Device* const _device, ::Ishikawa::Window::Window* const _window){
	if(_device == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Input is NULL. [_device]");
	}
	if(_device->GetDevice() == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Input is NULL. [_device->GetDevice()]");
	}
	if(_window == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Input is NULL. [_window]");
	}
	if(_window->GetHandle() == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Input is NULL. [_window->GetHandle()]");
	}

	// �����o�ϐ��̐ݒ�
	this->dependentDevice = _device;

	// �T���v���ݒ�̎擾
//	this->ChangeDefaultSampleDesc();
	this->ChangeMultisampleAntiAliasing(_device->GetDevice());

	// �f�o�C�X�쐬�Ɏg�p���ꂽDXGI�t�@�N�g�����擾
	Microsoft::WRL::ComPtr<IDXGIFactory> factory;
	this->GetFactory(factory.GetAddressOf(), _device->GetDevice());
	if(factory.Get() == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("factory is NULL.");
	}

	// �E�B���h�E�T�C�Y���擾���Đݒ�
	long windowWidth = 1L;
	long windowHeight = 1L;
	_window->GetSize(&windowWidth, &windowHeight);
	this->width = static_cast<unsigned int>(windowWidth);
	this->height = static_cast<unsigned int>(windowHeight);

	// �X���b�v�`�F�[���쐬
	this->CreateSwapChain(factory.Get(), _device->GetDevice(), _window->GetHandle());

	if(FAILED(factory->MakeWindowAssociation(_window->GetHandle(), DXGI_MWA_NO_ALT_ENTER))){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("factory->MakeWindowAssociation() failed.");
	}

	// �����_�[�^�[�Q�b�g�쐬
	this->CreateRenderTargetTexture();
	this->CreateRenderTargetView(_device->GetDevice());
	// �f�v�X�X�e���V���쐬
	this->CreateDepthStencilTexture(_device->GetDevice());
	this->CreateDepthStencilView(_device->GetDevice());
}

/// <summary>
/// �T�C�Y�ύX
/// </summary>
/// <param name="_width">��</param>
/// <param name="_height">����</param>
void SwapChain::ChangeSize(unsigned int _width, unsigned int _height){
	// �f�v�X�X�e���V�������Z�b�g
	this->ReleaseDepthStencilView();
	this->ReleaseDepthStencilTexture();
	// �����_�[�^�[�Q�b�g�����Z�b�g
	this->ReleaseRenderTargetView();
	this->ReleaseRenderTargetTexture();


	// �T�C�Y�ύX
	this->width = _width;
	this->height = _height;
	HRESULT hr = this->swapChain->ResizeBuffers(
		this->bufferCount,
		_width,
		_height,
		this->bufferFormat,
		0U
	);

	if((hr == DXGI_ERROR_DEVICE_REMOVED) || (hr == DXGI_ERROR_DEVICE_RESET)){
#if __IS_DEBUG
		OutputDebugStringA("Device lost on ResizeBuffers()\n");
#endif
		this->dependentDevice->Lost();

		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Lost device.");
	}
	else if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't resize swap chain buffers.");
	}

	// �����_�[�^�[�Q�b�g��蒼��
	this->CreateRenderTargetTexture();
	this->CreateRenderTargetView(this->dependentDevice->GetDevice());
	// �f�v�X�X�e���V����蒼��
	this->CreateDepthStencilTexture(this->dependentDevice->GetDevice());
	this->CreateDepthStencilView(this->dependentDevice->GetDevice());
}


/// <summary>
/// ��ʂ̃N���A
/// </summary>
void SwapChain::ClearScreen(){
	ID3D11DeviceContext* const context = this->dependentDevice->GetContext();

	// �e�r���[�̏����폜
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };	// �����ŏ�����
	context->ClearRenderTargetView(this->renderTargetView, clearColor);
	context->ClearDepthStencilView(this->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &(this->renderTargetView), this->depthStencilView);
}

/// <summary>
/// �o�b�t�@�����ւ��ĕ\��
/// </summary>
void SwapChain::Present(){
	// ��ʂɓ���������
	HRESULT hr = this->swapChain->Present(1, 0);

	// �ؒf�A�܂��̓h���C�o�̃A�b�v�O���[�h�ɂ��f�o�C�X���폜���ꂽ�ꍇ
	// �f�o�C�X���\�[�X���Đ�������
	if((hr == DXGI_ERROR_DEVICE_REMOVED) || (hr == DXGI_ERROR_DEVICE_RESET)){
#if __IS_DEBUG
		OutputDebugStringA("Device lost on Present()\n");
#endif
		this->dependentDevice->Lost();

		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Lost device.");
	}
	else if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't present swap chain.");
	}
}


/// <summary>
/// �X���b�v�`�F�[���̍쐬
/// </summary>
/// <param name="_factory">�t�@�N�g��</param>
/// <param name="_device">�f�o�C�X</param>
/// <param name="_windowHandle">�o�̓E�B���h�E�̃n���h��</param>
void SwapChain::CreateSwapChain(
	IDXGIFactory* const _factory,
	ID3D11Device* const _device,
	HWND__* const _windowHandle
){
	// �쐬����Ă�����̂��폜
	this->ReleaseRenderTargetView();
	this->ReleaseRenderTargetTexture();
	this->ReleaseSwapChain();

	// �ݒ�
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width = this->width;
	swapChainDesc.BufferDesc.Height = this->height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0U;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1U;
	swapChainDesc.BufferDesc.Format = this->bufferFormat;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = this->bufferCount;
	swapChainDesc.SampleDesc.Count = this->sampleDescCount;
	swapChainDesc.SampleDesc.Quality = this->sampleDescQuality;
	swapChainDesc.OutputWindow = _windowHandle;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// �쐬
	HRESULT hr = _factory->CreateSwapChain(
		_device,
		&swapChainDesc,
		&(this->swapChain)
	);
	if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create swap chain.");
	}
}

/// <summary>
/// �t�@�N�g���̎擾
/// </summary>
/// <param name="_factory">�t�@�N�g���i�[��</param>
/// <param name="_device">�f�o�C�X</param>
void SwapChain::GetFactory(IDXGIFactory** const _factory, ID3D11Device* const _device){
	(*_factory) = nullptr;

	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	if(FAILED(_device->QueryInterface<IDXGIDevice>(dxgiDevice.GetAddressOf()))){
		throw ::Ishikawa::Common::Exception::FunctionFailed("_device as dxgiDevice1 failed.");
	}

	Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
	if(FAILED(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()))){
		throw ::Ishikawa::Common::Exception::FunctionFailed("dxgiDevice1 get adapter failed!!");
	}

	Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
	if(FAILED(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())))){
		throw ::Ishikawa::Common::Exception::FunctionFailed("dxgiAdapter getParent() failed!!");
	}

	(*_factory) = dxgiFactory.Detach();
}

/// <summary>
/// �f�t�H���g�T���v���ݒ�ɕύX
/// </summary>
void SwapChain::ChangeDefaultSampleDesc(){
	this->sampleDescCount = 1U;
	this->sampleDescQuality = 0U;
	this->depthStencilViewDimention = D3D11_DSV_DIMENSION_TEXTURE2D;
}

/// <summary>
/// �}���`�T���v���E�A���`�G�C���A�V���O�ݒ�ɕύX
/// </summary>
/// <param name="_device">�f�o�C�X</param>
void SwapChain::ChangeMultisampleAntiAliasing(ID3D11Device* const _device){
	// �ō��i����T��
	for(unsigned int i=D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i>0U; --i){
		unsigned int quality = 0U;
		HRESULT hr = _device->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &quality);
		if(SUCCEEDED(hr)){
			if(quality > 0U){
				this->sampleDescCount = i;
				this->sampleDescQuality = quality - 1U;
				this->depthStencilViewDimention = D3D11_DSV_DIMENSION_TEXTURE2DMS;
				return;
			}
		}
	}
	// �ł��Ȃ���΃��O���c��
	::Ishikawa::Common::Debug::Log("Couldn't change multisample anti-aliasing.\n");
}

/// <summary>
/// �����_�[�^�[�Q�b�g�e�N�X�`���̍쐬
/// </summary>
void SwapChain::CreateRenderTargetTexture(){
	this->ReleaseRenderTargetTexture();

	// �o�b�N�o�b�t�@�̃X���b�v�`�F�[�����烌���_�[�^�[�Q�b�g�r���[���쐬����
	if(FAILED(this->swapChain->GetBuffer(0, IID_PPV_ARGS(&this->renderTargetTexture)))){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't get render target texture of swap chain.");
	}
}

/// <summary>
/// �����_�[�^�[�Q�b�g�r���[�̍쐬
/// </summary>
/// <param name="_device">�f�o�C�X</param>
void SwapChain::CreateRenderTargetView(ID3D11Device* const _device){
	this->ReleaseRenderTargetView();

	// �e�N�X�`�����烌���_�[�^�[�Q�b�g�r���[���쐬
	HRESULT hr = _device->CreateRenderTargetView(
		this->renderTargetTexture,
		nullptr,
		&(this->renderTargetView)
	);
	if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create render target view.");
	}
}

/// <summary>
/// �f�v�X�X�e���V���e�N�X�`���̍쐬
/// </summary>
/// <param name="_device">�f�o�C�X</param>
void SwapChain::CreateDepthStencilTexture(ID3D11Device* const _device){
	this->ReleaseDepthStencilTexture();

	// �ݒ�
	CD3D11_TEXTURE2D_DESC depthStencilDesc{};
	depthStencilDesc.Format = this->depthStencilFormat;
	depthStencilDesc.Width = this->width;
	depthStencilDesc.Height = this->height;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.SampleDesc.Count = this->sampleDescCount;
	depthStencilDesc.SampleDesc.Quality = this->sampleDescQuality;
	depthStencilDesc.MiscFlags = 0;

	// �쐬
	HRESULT hr = _device->CreateTexture2D(
		&depthStencilDesc,
		nullptr,
		&(this->depthStencilTexture)
	);
	if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create depth stencil texture.");
	}
}

/// <summary>
/// �f�v�X�X�e���V���r���[�̍쐬
/// </summary>
/// <param name="_device">�f�o�C�X</param>
void SwapChain::CreateDepthStencilView(ID3D11Device* const _device){
	this->ReleaseDepthStencilView();

	// �ݒ�
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = this->depthStencilFormat;
	depthStencilViewDesc.ViewDimension = this->depthStencilViewDimention;
	depthStencilViewDesc.Flags = 0U;
	depthStencilViewDesc.Texture2D.MipSlice = 0U;

	// �e�N�X�`������f�v�X�X�e���V���r���[���쐬
	HRESULT hr = _device->CreateDepthStencilView(
		this->depthStencilTexture,
		&depthStencilViewDesc,
		&(this->depthStencilView)
	);
	if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create depth stencil view.");
	}
}


/// <summary>
/// �X���b�v�`�F�[���̊J��
/// </summary>
void SwapChain::ReleaseSwapChain(){
	if(this->swapChain != nullptr){
		this->swapChain->Release();
		this->swapChain = nullptr;
	}
}

/// <summary>
/// �����_�[�^�[�Q�b�g�e�N�X�`���̊J��
/// </summary>
void SwapChain::ReleaseRenderTargetTexture(){
	if(this->renderTargetTexture != nullptr){
		this->renderTargetTexture->Release();
		this->renderTargetTexture = nullptr;
	}
}

/// <summary>
/// �����_�[�^�[�Q�b�g�r���[�̊J��
/// </summary>
void SwapChain::ReleaseRenderTargetView(){
	if(this->renderTargetView != nullptr){
		this->renderTargetView->Release();
		this->renderTargetView = nullptr;
	}
}

/// <summary>
/// �f�v�X�X�e���V���e�N�X�`���̊J��
/// </summary>
void SwapChain::ReleaseDepthStencilTexture(){
	if(this->depthStencilTexture != nullptr){
		this->depthStencilTexture->Release();
		this->depthStencilTexture = nullptr;
	}
}

/// <summary>
/// �f�v�X�X�e���V���r���[�̊J��
/// </summary>
void SwapChain::ReleaseDepthStencilView(){
	if(this->depthStencilView != nullptr){
		this->depthStencilView->Release();
		this->depthStencilView = nullptr;
	}
}
