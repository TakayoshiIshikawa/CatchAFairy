//=============================================================================
//	ITFileTexture.cpp
//
//	自作ファイルテクスチャのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITFileTexture.h"
#include <stdlib.h>
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
#include "DirectXTK/Inc/WICTextureLoader.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"

using namespace Ishikawa::DirectX::Texture;


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
FileTexture::FileTexture() :
	::Ishikawa::DirectX::Device::DependentResource::Base(),
	Texture2DInterface(),
	ShaderResourceInterface(),
	filepath(nullptr),
	shaderResourceView(nullptr),
	width(0U),
	height(0U)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
FileTexture::~FileTexture(){
	this->ReleaseShaderResourceView();

	if(this->filepath != nullptr){
		delete[] this->filepath;
		this->filepath = nullptr;
	}
}


/// <summary>
/// デバイスに読み込まれた
/// </summary>
void FileTexture::OnDeviceLoaded(){
	if(this->filepath != nullptr){
		this->Load(this->filepath);
	}
}

/// <summary>
/// デバイスが復元された
/// </summary>
void FileTexture::OnDeviceRestored(){
	if(this->filepath != nullptr){
		this->Load(this->filepath);
	}
}

/// <summary>
/// デバイスが破棄された
/// </summary>
void FileTexture::OnDeviceLost(){
	this->ReleaseShaderResourceView();
}


/// <summary>
/// 初期化
/// </summary>
void FileTexture::Initialize(){
	// 基底の初期化
	this->InitializeITDeviceDependentResource();
}


/// <summary>
/// 読み込み
/// </summary>
/// <param name="_filepath">ファイルパス</param>
void FileTexture::Load(const wchar_t* const _filepath){
	if(_filepath == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_filepath is NULL.");
	}

	// ファイルパス更新
	this->ChangeFilepath(_filepath);


	// デバイスの取得
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


	// 画像情報初期化
	this->ReleaseShaderResourceView();
	this->width = 0U;
	this->height = 0U;

	// シェーダリソースビュー作成
	ID3D11Resource* resource = nullptr;
	HRESULT hr = ::DirectX::CreateWICTextureFromFile(device, _filepath, &resource, &(this->shaderResourceView));
	if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't load file texture.");
	}

	// 画像サイズ取得
	ID3D11Texture2D* const texture = static_cast<ID3D11Texture2D*>(resource);
	D3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);
	this->width	 = textureDesc.Width;
	this->height = textureDesc.Height;

	// リソース開放
	resource->Release();
}


/// <summary>
/// ファイルパスの変更
/// </summary>
/// <param name="_filepath">ファイルパス</param>
void FileTexture::ChangeFilepath(const wchar_t* const _filepath){
	if(this->filepath != nullptr){
		delete[] this->filepath;
		this->filepath = nullptr;
	}

	unsigned int length = wcslen(_filepath);
	this->filepath = new wchar_t[length+1]{0};
	if(this->filepath == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't secure new memory.");
	}

#pragma warning(disable:4996)
	wcscpy(this->filepath, _filepath);
#pragma warning(default:4996)
}

/// <summary>
/// シェーダリソースビューの開放
/// </summary>
void FileTexture::ReleaseShaderResourceView(){
	if(this->shaderResourceView != nullptr){
		this->shaderResourceView->Release();
		this->shaderResourceView = nullptr;
	}
}
