//=============================================================================
//	ITSwapChain.cpp
//
//	自作スワップチェーンのヘッダファイル
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
/// デフォルトコンストラクタ
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
	// サンプラ設定をデフォルトにしておく
	this->ChangeDefaultSampleDesc();
}

/// <summary>
/// デストラクタ
/// </summary>
SwapChain::~SwapChain(){
	this->ReleaseDepthStencilView();
	this->ReleaseDepthStencilTexture();
	this->ReleaseRenderTargetView();
	this->ReleaseRenderTargetTexture();
	this->ReleaseSwapChain();
}


/// <summary>
/// 作成
/// </summary>
/// <param name="_device">デバイス</param>
/// <param name="_window">出力ウィンドウのハンドル</param>
void SwapChain::Create(Device::Device* const _device, ::Ishikawa::Window::Window* const _window){
	if(_device == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Input is NULL. [_device]");
	}
	if(_device->GetDevice() == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Input is NULL. [_device->GetDevice()]");
	}
	if(_window == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Input is NULL. [_window]");
	}
	if(_window->GetHandle() == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Input is NULL. [_window->GetHandle()]");
	}

	// メンバ変数の設定
	this->dependentDevice = _device;

	// サンプル設定の取得
//	this->ChangeDefaultSampleDesc();
	this->ChangeMultisampleAntiAliasing(_device->GetDevice());

	// デバイス作成に使用されたDXGIファクトリを取得
	Microsoft::WRL::ComPtr<IDXGIFactory> factory;
	this->GetFactory(factory.GetAddressOf(), _device->GetDevice());
	if(factory.Get() == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("factory is NULL.");
	}

	// ウィンドウサイズを取得して設定
	long windowWidth = 1L;
	long windowHeight = 1L;
	_window->GetSize(&windowWidth, &windowHeight);
	this->width = static_cast<unsigned int>(windowWidth);
	this->height = static_cast<unsigned int>(windowHeight);

	// スワップチェーン作成
	this->CreateSwapChain(factory.Get(), _device->GetDevice(), _window->GetHandle());

	if(FAILED(factory->MakeWindowAssociation(_window->GetHandle(), DXGI_MWA_NO_ALT_ENTER))){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("factory->MakeWindowAssociation() failed.");
	}

	// レンダーターゲット作成
	this->CreateRenderTargetTexture();
	this->CreateRenderTargetView(_device->GetDevice());
	// デプスステンシル作成
	this->CreateDepthStencilTexture(_device->GetDevice());
	this->CreateDepthStencilView(_device->GetDevice());
}

/// <summary>
/// サイズ変更
/// </summary>
/// <param name="_width">幅</param>
/// <param name="_height">高さ</param>
void SwapChain::ChangeSize(unsigned int _width, unsigned int _height){
	// デプスステンシルをリセット
	this->ReleaseDepthStencilView();
	this->ReleaseDepthStencilTexture();
	// レンダーターゲットをリセット
	this->ReleaseRenderTargetView();
	this->ReleaseRenderTargetTexture();


	// サイズ変更
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

		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Lost device.");
	}
	else if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't resize swap chain buffers.");
	}

	// レンダーターゲット作り直し
	this->CreateRenderTargetTexture();
	this->CreateRenderTargetView(this->dependentDevice->GetDevice());
	// デプスステンシル作り直し
	this->CreateDepthStencilTexture(this->dependentDevice->GetDevice());
	this->CreateDepthStencilView(this->dependentDevice->GetDevice());
}


/// <summary>
/// 画面のクリア
/// </summary>
void SwapChain::ClearScreen(){
	ID3D11DeviceContext* const context = this->dependentDevice->GetContext();

	// 各ビューの情報を削除
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };	// 透明で初期化
	context->ClearRenderTargetView(this->renderTargetView, clearColor);
	context->ClearDepthStencilView(this->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &(this->renderTargetView), this->depthStencilView);
}

/// <summary>
/// バッファを入れ替えて表示
/// </summary>
void SwapChain::Present(){
	// 画面に同期させる
	HRESULT hr = this->swapChain->Present(1, 0);

	// 切断、またはドライバのアップグレードによりデバイスが削除された場合
	// デバイスリソースを再生成する
	if((hr == DXGI_ERROR_DEVICE_REMOVED) || (hr == DXGI_ERROR_DEVICE_RESET)){
#if __IS_DEBUG
		OutputDebugStringA("Device lost on Present()\n");
#endif
		this->dependentDevice->Lost();

		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Lost device.");
	}
	else if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't present swap chain.");
	}
}


/// <summary>
/// スワップチェーンの作成
/// </summary>
/// <param name="_factory">ファクトリ</param>
/// <param name="_device">デバイス</param>
/// <param name="_windowHandle">出力ウィンドウのハンドル</param>
void SwapChain::CreateSwapChain(
	IDXGIFactory* const _factory,
	ID3D11Device* const _device,
	HWND__* const _windowHandle
){
	// 作成されているものを削除
	this->ReleaseRenderTargetView();
	this->ReleaseRenderTargetTexture();
	this->ReleaseSwapChain();

	// 設定
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

	// 作成
	HRESULT hr = _factory->CreateSwapChain(
		_device,
		&swapChainDesc,
		&(this->swapChain)
	);
	if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create swap chain.");
	}
}

/// <summary>
/// ファクトリの取得
/// </summary>
/// <param name="_factory">ファクトリ格納先</param>
/// <param name="_device">デバイス</param>
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
/// デフォルトサンプル設定に変更
/// </summary>
void SwapChain::ChangeDefaultSampleDesc(){
	this->sampleDescCount = 1U;
	this->sampleDescQuality = 0U;
	this->depthStencilViewDimention = D3D11_DSV_DIMENSION_TEXTURE2D;
}

/// <summary>
/// マルチサンプル・アンチエイリアシング設定に変更
/// </summary>
/// <param name="_device">デバイス</param>
void SwapChain::ChangeMultisampleAntiAliasing(ID3D11Device* const _device){
	// 最高品質を探す
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
	// できなければログを残す
	::Ishikawa::Common::Debug::Log("Couldn't change multisample anti-aliasing.\n");
}

/// <summary>
/// レンダーターゲットテクスチャの作成
/// </summary>
void SwapChain::CreateRenderTargetTexture(){
	this->ReleaseRenderTargetTexture();

	// バックバッファのスワップチェーンからレンダーターゲットビューを作成する
	if(FAILED(this->swapChain->GetBuffer(0, IID_PPV_ARGS(&this->renderTargetTexture)))){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't get render target texture of swap chain.");
	}
}

/// <summary>
/// レンダーターゲットビューの作成
/// </summary>
/// <param name="_device">デバイス</param>
void SwapChain::CreateRenderTargetView(ID3D11Device* const _device){
	this->ReleaseRenderTargetView();

	// テクスチャからレンダーターゲットビューを作成
	HRESULT hr = _device->CreateRenderTargetView(
		this->renderTargetTexture,
		nullptr,
		&(this->renderTargetView)
	);
	if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create render target view.");
	}
}

/// <summary>
/// デプスステンシルテクスチャの作成
/// </summary>
/// <param name="_device">デバイス</param>
void SwapChain::CreateDepthStencilTexture(ID3D11Device* const _device){
	this->ReleaseDepthStencilTexture();

	// 設定
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

	// 作成
	HRESULT hr = _device->CreateTexture2D(
		&depthStencilDesc,
		nullptr,
		&(this->depthStencilTexture)
	);
	if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create depth stencil texture.");
	}
}

/// <summary>
/// デプスステンシルビューの作成
/// </summary>
/// <param name="_device">デバイス</param>
void SwapChain::CreateDepthStencilView(ID3D11Device* const _device){
	this->ReleaseDepthStencilView();

	// 設定
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = this->depthStencilFormat;
	depthStencilViewDesc.ViewDimension = this->depthStencilViewDimention;
	depthStencilViewDesc.Flags = 0U;
	depthStencilViewDesc.Texture2D.MipSlice = 0U;

	// テクスチャからデプスステンシルビューを作成
	HRESULT hr = _device->CreateDepthStencilView(
		this->depthStencilTexture,
		&depthStencilViewDesc,
		&(this->depthStencilView)
	);
	if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create depth stencil view.");
	}
}


/// <summary>
/// スワップチェーンの開放
/// </summary>
void SwapChain::ReleaseSwapChain(){
	if(this->swapChain != nullptr){
		this->swapChain->Release();
		this->swapChain = nullptr;
	}
}

/// <summary>
/// レンダーターゲットテクスチャの開放
/// </summary>
void SwapChain::ReleaseRenderTargetTexture(){
	if(this->renderTargetTexture != nullptr){
		this->renderTargetTexture->Release();
		this->renderTargetTexture = nullptr;
	}
}

/// <summary>
/// レンダーターゲットビューの開放
/// </summary>
void SwapChain::ReleaseRenderTargetView(){
	if(this->renderTargetView != nullptr){
		this->renderTargetView->Release();
		this->renderTargetView = nullptr;
	}
}

/// <summary>
/// デプスステンシルテクスチャの開放
/// </summary>
void SwapChain::ReleaseDepthStencilTexture(){
	if(this->depthStencilTexture != nullptr){
		this->depthStencilTexture->Release();
		this->depthStencilTexture = nullptr;
	}
}

/// <summary>
/// デプスステンシルビューの開放
/// </summary>
void SwapChain::ReleaseDepthStencilView(){
	if(this->depthStencilView != nullptr){
		this->depthStencilView->Release();
		this->depthStencilView = nullptr;
	}
}
