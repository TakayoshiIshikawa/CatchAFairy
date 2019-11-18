//=============================================================================
//	ShaderUserBase.cpp
//
//	シェーダユーザ基底のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ShaderUserBase.h"
#include <stdio.h>
#include <sys/stat.h>
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
#include "Ishikawa/Common/SafePointer.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"

using namespace CatchAFairy::Shader::User;


/// <summary>
/// ファイルのコードとその長さの取得
/// </summary>
/// <param name="_code">コード格納先[利用終了したらメモリを削除すること]</param>
/// <param name="_length">コードの長さ格納先</param>
/// <param name="_filepath">ファイルパス</param>
void Base::GetFileCodeAndLength(
	char** const _code,
	unsigned long* const _length,
	const char* const _filepath
){
	if(_code == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_code is NULL.");
	}
	if(_length == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_length is NULL.");
	}
	if(_filepath == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_filepath is NULL.");
	}

	// ファイルサイズ取得
	{
		struct stat statBuffer;

		if(stat(_filepath, &statBuffer) != 0){
			// 異常終了
			throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't get file stat.");
		}

		(*_length) = static_cast<unsigned long>(statBuffer.st_size);
	}

	// コード保存用メモリの確保
	(*_code) = new char[(*_length) + 1]{0};
	if((*_code) == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't new code memory.");
	}

#pragma warning(disable:4996)
	FILE* const file = fopen(_filepath, "rb");
#pragma warning(default:4996)
	if(file == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_filepath is NULL.");
	}

	// コードの取得
#pragma warning(disable:4996)
	fread(*_code, sizeof(char), *_length, file);
#pragma warning(default:4996)

	fclose(file);
}


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
Base::Base() :
	::Ishikawa::DirectX::Device::DependentResource::Base(),
	inputLayout(nullptr),
	vertexShader(nullptr),
	geometryShader(nullptr),
	pixelShader(nullptr),
	samplerState(nullptr),
	blendState(nullptr),
	depthStencilState(nullptr),
	rasterizerState(nullptr)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Base::~Base(){
	this->ReleaseAll();
}


/// <summary>
/// デバイスの取得
/// </summary>
/// <returns>デバイス</returns>
ID3D11Device* Base::GetDevice() const{
	// 自作デバイスの取得
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetDependentITDevice();
	if(itDevice == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("itDevice is NULL.");
	}

	// デバイスの取得
	ID3D11Device* const device = itDevice->GetDevice();
	if(device == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("device is NULL.");
	}

	return device;
}

/// <summary>
/// コンテキストの取得
/// </summary>
/// <returns>コンテキスト</returns>
ID3D11DeviceContext* Base::GetContext() const{
	// 依存している自作デバイスの取得
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetDependentITDevice();
	if(itDevice == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("itDevice is NULL.");
	}

	// コンテキストの取得
	ID3D11DeviceContext* const context = itDevice->GetContext();
	if(context == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("context is NULL.");
	}

	return context;
}


/// <summary>
/// デバイスに読み込まれた
/// </summary>
void Base::OnDeviceLoaded(){
	// 作成
	this->Create();
}

/// <summary>
/// デバイスが復元された
/// </summary>
void Base::OnDeviceRestored(){
	// 作成
	this->Create();
}

/// <summary>
/// デバイスが破棄された
/// </summary>
void Base::OnDeviceLost(){
	// リソースの開放
	this->ReleaseAll();
}


/// <summary>
/// 初期化
/// </summary>
void Base::Initialize(){
	// 基底の初期化
	this->InitializeITDeviceDependentResource();
}

/// <summary>
/// インデックスバッファ作成
/// [利用し終えたバッファは開放してください]
/// </summary>
/// <param name="_indexArray">インデックス配列</param>
/// <param name="_indexCount">インデックス数</param>
/// <returns> インデックスバッファ</returns>
ID3D11Buffer* Base::CreateIndexBuffer(
	const unsigned long* const _indexArray,
	unsigned int _indexCount
) const {
	if(_indexArray == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_indexArray is NULL.");
	}
	if(_indexCount == 0U){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_indexCount is zero.");
	}

	ID3D11Device* const device = this->GetDevice();
	ID3D11Buffer* indexBuffer = nullptr;
	// バッファ設定
	D3D11_BUFFER_DESC indexBufferDesc;
	memset(&indexBufferDesc, 0, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = _indexCount * sizeof(unsigned int);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	// リソースデータ
	D3D11_SUBRESOURCE_DATA indexSubresource;
	memset(&indexSubresource, 0, sizeof(indexSubresource));
	indexSubresource.pSysMem = _indexArray;

	// インデックスバッファ作成
	HRESULT hr = device->CreateBuffer(&indexBufferDesc, &indexSubresource, &indexBuffer);
	if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create index buffer.");
	}

	return indexBuffer;
}

/// <summary>
/// バッファ開放
/// </summary>
/// <param name="_buffer">バッファ</param>
void Base::ReleaseBuffer(ID3D11Buffer* const _buffer) const {
	if(_buffer != nullptr) _buffer->Release();
}

/// <summary>
/// 作成
/// </summary>
void Base::Create(){
	// デバイスの取得
	ID3D11Device* const device = this->GetDevice();

	this->ReleaseVertexShader();
	this->CreateVertexShaderAndInputLayout(device);

	this->ReleaseGeometryShader();
	this->CreateGeometryShader(device);

	this->ReleasePixelShader();
	this->CreatePixelShader(device);

	this->ReleaseSamplerState();
	this->CreateSamplerState(device);

	this->ReleaseBlendState();
	this->CreateBlendState(device);

	this->ReleaseDepthStencilState();
	this->CreateDepthStencilState(device);

	this->ReleaseRasterizerState();
	this->CreateRasterizerState(device);
}

/// <summary>
/// 頂点シェーダと入力レイアウトの作成
/// </summary>
/// <param name="_device">デバイス</param>
void Base::CreateVertexShaderAndInputLayout(ID3D11Device* const _device){
	// シェーダコード取得
	::Ishikawa::Common::SafePointer<char> codeBuffer;
	unsigned long codeLength = 0UL;
	{
		char* code = nullptr;
		Base::GetFileCodeAndLength(&code, &codeLength, this->GetVertexShaderFilepath());
		codeBuffer.Reset(code);
	}

	// 頂点シェーダ作成
	HRESULT hr = _device->CreateVertexShader(codeBuffer.Get(), codeLength, nullptr, &(this->vertexShader));
	if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create vertex shader.");
	}

	// 入力レイアウト作成
	hr = _device->CreateInputLayout(
		this->GetInputLayoutDesc(),
		this->GetInputLayoutCount(),
		codeBuffer.Get(), codeLength,
		&(this->inputLayout)
	);
	if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create input layout.");
	}
}

/// <summary>
/// ジオメトリシェーダの作成
/// </summary>
/// <param name="_device">デバイス</param>
void Base::CreateGeometryShader(ID3D11Device* const _device){
	// シェーダコード取得
	::Ishikawa::Common::SafePointer<char> codeBuffer;
	unsigned long codeLength = 0UL;
	{
		char* code = nullptr;
		Base::GetFileCodeAndLength(&code, &codeLength, this->GetGeometryShaderFilepath());
		codeBuffer.Reset(code);
	}

	// ジオメトリシェーダ作成
	HRESULT hr = _device->CreateGeometryShader(codeBuffer.Get(), codeLength, nullptr, &(this->geometryShader));
	if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create geometry shader.");
	}
}

/// <summary>
/// ピクセルシェーダの作成
/// </summary>
/// <param name="_device">デバイス</param>
void Base::CreatePixelShader(ID3D11Device* const _device){
	// シェーダコード取得
	::Ishikawa::Common::SafePointer<char> codeBuffer;
	unsigned long codeLength = 0UL;
	{
		char* code = nullptr;
		Base::GetFileCodeAndLength(&code, &codeLength, this->GetPixelShaderFilepath());
		codeBuffer.Reset(code);
	}

	// ピクセルシェーダ作成
	HRESULT hr = _device->CreatePixelShader(codeBuffer.Get(), codeLength, nullptr, &(this->pixelShader));
	if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create pixel shader.");
	}
}


/// <summary>
/// 全てのリソースの開放
/// </summary>
void Base::ReleaseAll(){
	this->ReleaseRasterizerState();
	this->ReleaseDepthStencilState();
	this->ReleaseBlendState();
	this->ReleaseSamplerState();
	this->ReleasePixelShader();
	this->ReleaseGeometryShader();
	this->ReleaseInputLayout();
	this->ReleaseVertexShader();
}

/// <summary>
/// 入力レイアウトの開放
/// </summary>
void Base::ReleaseInputLayout(){
	if(this->inputLayout != nullptr){
		this->inputLayout->Release();
		this->inputLayout = nullptr;
	}
}

/// <summary>
/// 頂点シェーダの開放
/// </summary>
void Base::ReleaseVertexShader(){
	if(this->vertexShader != nullptr){
		this->vertexShader->Release();
		this->vertexShader = nullptr;
	}
}

/// <summary>
/// ジオメトリシェーダの開放
/// </summary>
void Base::ReleaseGeometryShader(){
	if(this->geometryShader != nullptr){
		this->geometryShader->Release();
		this->geometryShader = nullptr;
	}
}

/// <summary>
/// ピクセルシェーダの開放
/// </summary>
void Base::ReleasePixelShader(){
	if(this->pixelShader != nullptr){
		this->pixelShader->Release();
		this->pixelShader = nullptr;
	}
}

/// <summary>
/// サンプラステートの開放
/// </summary>
void Base::ReleaseSamplerState(){
	if(this->samplerState != nullptr){
		this->samplerState->Release();
		this->samplerState = nullptr;
	}
}

/// <summary>
/// ブレンドステートの開放
/// </summary>
void Base::ReleaseBlendState(){
	if(this->blendState != nullptr){
		this->blendState->Release();
		this->blendState = nullptr;
	}
}

/// <summary>
/// デプスステンシルステートの開放
/// </summary>
void Base::ReleaseDepthStencilState(){
	if(this->depthStencilState != nullptr){
		this->depthStencilState->Release();
		this->depthStencilState = nullptr;
	}
}

/// <summary>
/// ラスタライザステートの開放
/// </summary>
void Base::ReleaseRasterizerState(){
	if(this->rasterizerState != nullptr){
		this->rasterizerState->Release();
		this->rasterizerState = nullptr;
	}
}
