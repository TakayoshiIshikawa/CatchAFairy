//=============================================================================
//	ITTexture2D.cpp
//
//	自作2Dテクスチャのソースファイル
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
/// デフォルトコンストラクタ
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
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Texture2D::~Texture2D(){
	// リソースの開放
	this->ReleaseAll();
}


/// <summary>
/// シェーダリソースビューの取得
/// </summary>
/// <returns>シェーダリソースビュー</returns>
ID3D11ShaderResourceView* Texture2D::GetShaderResourceView() const {
	// バインド可能かを調べる
	if(!(this->IsBindable(BindFlag::ShaderResource))){
		// バインドできない
		throw ::Ishikawa::Common::Exception::FunctionFailed("Can't bind texture to shader resource view.");
	}
	// バインド先切替え
	if(this->currentBindFlag != BindFlag::ShaderResource){
		Texture2D* const _this = const_cast<Texture2D*>(this);
		_this->ReleaseCurrentView();
		_this->CreateShaderResourceView();
		_this->currentBindFlag = BindFlag::ShaderResource;
	}

	return this->shaderResourceView;
}

/// <summary>
/// レンダーターゲットビューの取得
/// </summary>
/// <returns>レンダーターゲットビュー</returns>
ID3D11RenderTargetView* Texture2D::GetRenderTargetView() const {
	// バインド可能かを調べる
	if(!(this->IsBindable(BindFlag::RenderTarget))){
		// バインドできない
		throw ::Ishikawa::Common::Exception::FunctionFailed("Can't bind texture to render target view.");
	}
	// バインド先切替え
	if(this->currentBindFlag != BindFlag::RenderTarget){
		Texture2D* const _this = const_cast<Texture2D*>(this);
		_this->ReleaseCurrentView();
		_this->CreateRenderTargetView();
		_this->currentBindFlag = BindFlag::RenderTarget;
	}
	return this->renderTargetView;
}

/// <summary>
/// デプスステンシルビューの取得
/// </summary>
/// <returns>デプスステンシルビュー</returns>
ID3D11DepthStencilView* Texture2D::GetDepthStencilView() const {
	// バインド可能かを調べる
	if(!(this->IsBindable(BindFlag::DepthStencil))){
		// バインドできない
		throw ::Ishikawa::Common::Exception::FunctionFailed("Can't bind texture to depth stencil view.");
	}
	// バインド先切替え
	if(this->currentBindFlag != BindFlag::DepthStencil){
		Texture2D* const _this = const_cast<Texture2D*>(this);
		_this->ReleaseCurrentView();
		_this->CreateDepthStencilView();
		_this->currentBindFlag = BindFlag::DepthStencil;
	}

	return this->depthStencilView;
}

/// <summary>
/// バインド可能か?
/// </summary>
/// <param name="_bindFlag">バインドフラグ</param>
/// <returns>true:可能 false:不可能</returns>
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
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_bindFlag is unknown.");
	}
}

/// <summary>
/// デバイスの取得
/// </summary>
/// <returns>デバイス</returns>
ID3D11Device* Texture2D::GetDevice() const{
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetDependentITDevice();
	if(itDevice == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("itDevice is NULL.");
	}
	ID3D11Device* const device = itDevice->GetDevice();
	if(device == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("device is NULL.");
	}

	return device;
}


/// <summary>
/// 初期化
/// </summary>
void Texture2D::Initialize(){
	// メンバの初期化
	this->InitializeITDeviceDependentResource();
}


/// <summary>
/// デバイスに読み込まれた
/// </summary>
void Texture2D::OnDeviceLoaded(){
	// 何もしない
}

/// <summary>
/// デバイスが復元された
/// </summary>
void Texture2D::OnDeviceRestored(){
	// 何もしない
}

/// <summary>
/// デバイスが破棄された
/// </summary>
void Texture2D::OnDeviceLost(){
	// リソースの開放
	this->ReleaseAll();
}


/// <summary>
/// 作成
/// </summary>
/// <param name="_bindableFlag">バインド可能フラグ</param>
/// <param name="_width">幅</param>
/// <param name="_height">高さ</param>
/// <param name="_format">フォーマット</param>
/// <param name="_cpuAccessFlag">CPUアクセスフラグ</param>
/// <param name="_sampleDescCount">サンプル設定カウント</param>
/// <param name="_sampleDescQuality">サンプル設定クォリティ</param>
/// <param name="_dimention">ディメンション</param>
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
	// リソースの開放
	this->ReleaseAll();

	// メンバ変更
	this->bindableFlag = _bindableFlag;
	this->width = _width;
	this->height = _height;
	this->format = _format;
	this->cpuAccessFlag = _cpuAccessFlag;
	this->sampleDescCount = _sampleDescCount;
	this->sampleDescQuality = _sampleDescQuality;
	this->dimention = _dimention;

	// テクスチャ作成
	this->CreateTexture();
}

/// <summary>
/// サイズ変更
/// </summary>
/// <param name="_width">幅</param>
/// <param name="_height">高さ</param>
void Texture2D::ChangeSize(unsigned int _width, unsigned int _height){
	// リソースの開放
	this->ReleaseAll();

	// メンバ変更
	this->width = _width;
	this->height = _height;

	// テクスチャ作成
	this->CreateTexture();
}

/// <summary>
/// テクスチャの作成
/// </summary>
void Texture2D::CreateTexture(){
	// 設定
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
	// 作成
	HRESULT hr = device->CreateTexture2D(
		&textureDesc,
		nullptr,
		&(this->texture)
	);
	if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create depth stencil texture.");
	}
}

/// <summary>
/// シェーダリソースビューの作成
/// </summary>
void Texture2D::CreateShaderResourceView(){
	// 設定
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
	shaderResourceViewDesc.Format = ConvertFormatToDxgiFormat(this->format);
	shaderResourceViewDesc.ViewDimension = ConvertDimentionToD3D11SRVDimention(this->dimention);
	shaderResourceViewDesc.Texture2D.MipLevels = 1U;

	ID3D11Device* const device = this->GetDevice();
	// テクスチャからシェーダリソースビューを作成
	HRESULT hr = device->CreateShaderResourceView(
		this->texture,
		&shaderResourceViewDesc,
		&(this->shaderResourceView)
	);
	if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create shader resouce view.");
	}
}

/// <summary>
/// レンダーターゲットビューの作成
/// </summary>
void Texture2D::CreateRenderTargetView(){
	// 設定
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	renderTargetViewDesc.Format = ConvertFormatToDxgiFormat(this->format);
	renderTargetViewDesc.ViewDimension = ConvertDimentionToD3D11RTVDimention(this->dimention);
	renderTargetViewDesc.Texture2D.MipSlice = 0U;

	ID3D11Device* const device = this->GetDevice();
	// テクスチャからレンダーターゲットビューを作成
	HRESULT hr = device->CreateRenderTargetView(
		this->texture,
		&renderTargetViewDesc,
		&(this->renderTargetView)
	);
	if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create render target view.");
	}
}

/// <summary>
/// デプスステンシルビューの作成
/// </summary>
void Texture2D::CreateDepthStencilView(){
	// 設定
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = ConvertFormatToDxgiFormat(this->format);
	depthStencilViewDesc.ViewDimension = ConvertDimentionToD3D11DSVDimention(this->dimention);
	depthStencilViewDesc.Flags = 0U;
	depthStencilViewDesc.Texture2D.MipSlice = 0U;

	ID3D11Device* const device = this->GetDevice();
	// テクスチャからデプスステンシルビューを作成
	HRESULT hr = device->CreateDepthStencilView(
		this->texture,
		&depthStencilViewDesc,
		&(this->depthStencilView)
	);
	if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create depth stencil view.");
	}
}


/// <summary>
/// 全リソースの開放
/// </summary>
void Texture2D::ReleaseAll(){
	this->ReleaseDepthStencilView();
	this->ReleaseRenderTargetView();
	this->ReleaseShaderResourceView();
	this->ReleaseTexture();

	this->currentBindFlag = BindFlag::NotBind;
}

/// <summary>
/// テクスチャの開放
/// </summary>
void Texture2D::ReleaseTexture(){
	if(this->texture != nullptr){
		this->texture->Release();
		this->texture = nullptr;
	}
}

/// <summary>
/// 現在利用中のビューを開放
/// </summary>
void Texture2D::ReleaseCurrentView(){
	switch(this->currentBindFlag){
	case BindFlag::NotBind:
		// 何もしない
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
		// 変な値が入っているのでビューを全て開放
		this->ReleaseShaderResourceView();
		this->ReleaseRenderTargetView();
		this->ReleaseDepthStencilView();
		this->currentBindFlag = BindFlag::NotBind;
		break;
	}
}

/// <summary>
/// シェーダリソースビューの開放
/// </summary>
void Texture2D::ReleaseShaderResourceView(){
	if(this->shaderResourceView != nullptr){
		this->shaderResourceView->Release();
		this->shaderResourceView = nullptr;
	}
}

/// <summary>
/// レンダーターゲットビューの開放
/// </summary>
void Texture2D::ReleaseRenderTargetView(){
	if(this->renderTargetView != nullptr){
		this->renderTargetView->Release();
		this->renderTargetView = nullptr;
	}
}

/// <summary>
/// デプスステンシルビューの開放
/// </summary>
void Texture2D::ReleaseDepthStencilView(){
	if(this->depthStencilView != nullptr){
		this->depthStencilView->Release();
		this->depthStencilView = nullptr;
	}
}
