//=============================================================================
//	PlayCameraEyeFirstShaderUser.cpp
//
//	プレイカメラ視点1番シェーダユーザのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "PlayCameraEyeFirstShaderUser.h"
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
#include "Ishikawa/DirectX/ITViewport.h"
#include "ShaderVertexTopology.h"

using namespace CatchAFairy::Shader::User;


/// <summary>入力レイアウト設定</summary>
const D3D11_INPUT_ELEMENT_DESC PlayCameraEyeFirst::INPUT_LAYOUT_DESC[INPUT_LAYOUT_COUNT] = {
	{ "SV_POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
/// <summary>頂点シェーダファイルパス</summary>
const char* const PlayCameraEyeFirst::VERTEX_SHADER_FILEPATH = "Resources\\Shaders\\PlayCameraEyeFirstVertexShader.cso";
/// <summary>ジオメトリシェーダファイルパス</summary>
const char* const PlayCameraEyeFirst::GEOMETRY_SHADER_FILEPATH = "Resources\\Shaders\\PlayCameraEyeFirstGeometryShader.cso";
/// <summary>ピクセルシェーダファイルパス</summary>
const char* const PlayCameraEyeFirst::PIXEL_SHADER_FILEPATH = "Resources\\Shaders\\PlayCameraEyeFirstPixelShader.cso";


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
PlayCameraEyeFirst::PlayCameraEyeFirst() :
	constantBufferArray{nullptr}
{
	// 何もしない
}
/// <summary>
/// デストラクタ
/// </summary>
PlayCameraEyeFirst::~PlayCameraEyeFirst(){
	this->ReleaseConstantBufferArray();
}


/// <summary>
/// 頂点バッファ作成
/// [利用し終えたバッファは開放してください]
/// </summary>
/// <param name="_vertexArray">頂点配列</param>
/// <param name="_vertexCount">頂点数</param>
/// <returns>頂点バッファ</returns>
ID3D11Buffer* PlayCameraEyeFirst::CreateVertexBuffer(const VSInput* const _vertexArray, unsigned int _vertexCount) const {
	if(_vertexArray == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_vertexArray is NULL.");
	}
	if(_vertexCount == 0U){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_vertexCount is zero.");
	}
	
	ID3D11Device* const device = this->GetDevice();
	ID3D11Buffer* vertexBuffer = nullptr;
	// バッファ設定
	D3D11_BUFFER_DESC vertexBufferDesc;
	memset(&vertexBufferDesc, 0, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = _vertexCount * sizeof(VSInput);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	// リソースデータ
	D3D11_SUBRESOURCE_DATA vertexSubresource;
	memset(&vertexSubresource, 0, sizeof(vertexSubresource));
	vertexSubresource.pSysMem = _vertexArray;

	// 頂点バッファ作成
	HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexSubresource, &vertexBuffer);
	if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create vertex buffer.");
	}

	return vertexBuffer;
}

/// <summary>
/// サンプラステートの作成
/// </summary>
/// <param name="_device">デバイス</param>
void PlayCameraEyeFirst::CreateSamplerState(ID3D11Device* const _device){
	// 設定
	D3D11_SAMPLER_DESC samplerDesc;
	memset(&samplerDesc, 0, sizeof(samplerDesc));
	samplerDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD			= 0;
	samplerDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	// 生成
	ID3D11SamplerState* sampler = nullptr;
	HRESULT hr = _device->CreateSamplerState(&samplerDesc, &(sampler));
	if(FAILED(hr)){
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create sampler state.");
	}
	this->SetSamplerState(sampler);
}

/// <summary>
/// ブレンドステートの作成
/// </summary>
/// <param name="_device">デバイス</param>
void PlayCameraEyeFirst::CreateBlendState(ID3D11Device* const _device){
	// ブレンドステート設定
	CD3D11_DEFAULT defaultState;
	CD3D11_BLEND_DESC blendDesc(defaultState);

	// 生成
	ID3D11BlendState* blend = nullptr;
	HRESULT hr = _device->CreateBlendState(&blendDesc, &(blend));
	if(FAILED(hr)){
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create blend state.");
	}
	this->SetBlendState(blend);
}

/// <summary>
/// デプスステンシルステートの作成
/// </summary>
/// <param name="_device">デバイス</param>
void PlayCameraEyeFirst::CreateDepthStencilState(ID3D11Device* const _device){
	CD3D11_DEFAULT defaultState;
	CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(defaultState);

	// 生成
	ID3D11DepthStencilState* depthStencil = nullptr;
	HRESULT hr = _device->CreateDepthStencilState(&depthStencilDesc, &(depthStencil));
	if(FAILED(hr)){
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create depth stencil state.");
	}
	this->SetDepthStencilState(depthStencil);
}

/// <summary>
/// ラスタライザステートの作成
/// </summary>
/// <param name="_device">デバイス</param>
void PlayCameraEyeFirst::CreateRasterizerState(ID3D11Device* const _device){
	CD3D11_DEFAULT defaultState;
	CD3D11_RASTERIZER_DESC rasterizerDesc(defaultState);
	rasterizerDesc.CullMode = D3D11_CULL_BACK;

	// 生成
	ID3D11RasterizerState* rasterizer = nullptr;
	HRESULT hr = _device->CreateRasterizerState(&rasterizerDesc, &(rasterizer));
	if(FAILED(hr)){
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create sampler state.");
	}
	this->SetRasterizerState(rasterizer);
}


/// <summary>
/// 描画前処理
/// </summary>
/// <param name="_outputResource">出力リソース</param>
/// <param name="_color">初期化色</param>
void PlayCameraEyeFirst::DrawBefore(const OutputResource& _outputResource, const float (&_color)[4]){
	ID3D11DeviceContext* const context = this->GetContext();

	// 出力リソースの初期化
	context->ClearRenderTargetView(_outputResource.diffuseRenderTargetView, _color);
	const float normalClearColor[4] = { 0.5f, 0.5f, 0.5f, 0.0f };
	context->ClearRenderTargetView(_outputResource.normalRenderTargetView, normalClearColor);
	const float positionClearColor[4] = { 0.5f, 0.5f, 1.0f, 0.0f };
	context->ClearRenderTargetView(_outputResource.positionRenderTargetView, positionClearColor);
	context->ClearDepthStencilView(_outputResource.depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// 出力設定
	context->OMSetRenderTargets(
		OutputResource::RENDER_TARGET_COUNT,
		_outputResource.renderTargets,
		_outputResource.depthStencilView
	);

	// シェーダ設定
	context->VSSetShader(this->GetVertexShader(), nullptr, 0U);
	context->GSSetShader(this->GetGeometryShader(), nullptr, 0U);
	context->PSSetShader(this->GetPixelShader(), nullptr, 0U);
	// 入力レイアウト設定
	context->IASetInputLayout(this->GetInputLayout());
	// サンプラ設定
	const unsigned int samplerCount = 1U;
	ID3D11SamplerState* const samplers[samplerCount] = {
		this->GetSamplerState()
	};
	context->PSSetSamplers(0U, samplerCount, samplers);
	// ブレンド設定
	context->OMSetBlendState(this->GetBlendState(), nullptr, 0xffffffff);
	// デプスステンシル設定
	context->OMSetDepthStencilState(this->GetDepthStencilState(), 0U);
	// ラスタライザ設定
	context->RSSetState(this->GetRasterizerState());

	// ビューポート設定
	const unsigned int viewportCount = 1U;
	const D3D11_VIEWPORT viewports[viewportCount] = {
		::Ishikawa::DirectX::ConvertViewportToD3D11Viewport(*(_outputResource.viewport))
	};
	context->RSSetViewports(viewportCount, viewports);

	// 定数バッファ作成
	this->CreateConstantBufferArray();

	// 定数バッファ設定
	context->VSSetConstantBuffers(0U, CONSTANT_BUFFER_COUNT, this->constantBufferArray);
	context->GSSetConstantBuffers(0U, CONSTANT_BUFFER_COUNT, this->constantBufferArray);
	context->PSSetConstantBuffers(0U, CONSTANT_BUFFER_COUNT, this->constantBufferArray);
}

/// <summary>
/// 描画
/// </summary>
/// <param name="_inputResource">入力リソース</param>
void PlayCameraEyeFirst::Draw(const RenderUseResource& _inputResource){
	// 描画するかチェック
	{
		::Ishikawa::Math::Float4 projectionCheckCenter = 
			::Ishikawa::Math::Float4(_inputResource.checkCenterPosition, 1.0f) *
			_inputResource.checkModelToProjectionMatrix;
		projectionCheckCenter *= (1.0f / projectionCheckCenter.w);

		::Ishikawa::Math::Float4 mostNearProjectionPosition;
		mostNearProjectionPosition.x = (projectionCheckCenter.x < -1.0f) ? (-1.0f) :
										((projectionCheckCenter.x > 1.0f) ? (1.0f) : (projectionCheckCenter.x));
		mostNearProjectionPosition.y = (projectionCheckCenter.y < -1.0f) ? (-1.0f) :
										((projectionCheckCenter.y > 1.0f) ? (1.0f) : (projectionCheckCenter.y));
		mostNearProjectionPosition.z = (projectionCheckCenter.z < 0.0f) ? (0.0f) :
										((projectionCheckCenter.z > 1.0f) ? (1.0f) : (projectionCheckCenter.z));
		mostNearProjectionPosition.w = 1.0f;
		::Ishikawa::Math::Float4 mostNearPosition = mostNearProjectionPosition * _inputResource.checkModelToProjectionMatrix.Invert();
		mostNearPosition *= (1.0f / mostNearPosition.w);

		::Ishikawa::Math::Float3 delta(
			_inputResource.checkCenterPosition.x - mostNearPosition.x,
			_inputResource.checkCenterPosition.y - mostNearPosition.y,
			_inputResource.checkCenterPosition.z - mostNearPosition.z
		);

		if(delta.LengthSquared() > _inputResource.checkSquareRadius){
			// 描画しない
			return;
		}
	}

	ID3D11DeviceContext* const context = this->GetContext();

	// シェーダリソース設定
	context->PSSetShaderResources(
		0U,
		RenderUseResource::SHADER_RESOURCE_COUNT,
		_inputResource.shaderResources
	);

	// 頂点バッファ設定
	ID3D11Buffer* vertexBuffers[1] = { _inputResource.vertexBuffer };
	unsigned int vertexStride[1] = { sizeof(VSInput) };
	unsigned int vertexOffset[1] = { 0U };
	context->IASetVertexBuffers(0U, 1U, vertexBuffers, vertexStride, vertexOffset);

	// インデックスバッファ設定
	context->IASetIndexBuffer(_inputResource.indexBuffer, DXGI_FORMAT_R32_UINT, 0U);

	// プリミティブトポロジー設定
	context->IASetPrimitiveTopology(ConvertVertexTopologyToD3DPrimitiveTopology(_inputResource.topology));

	// 定数バッファ更新
	context->UpdateSubresource(this->constantBufferArray[0], 0U, nullptr, &(_inputResource.constantBuffer), 0U, 0U);

	// 描画
	context->DrawIndexed(_inputResource.indexCount, 0U, 0);

	// シェーダリソースリセット
	ID3D11ShaderResourceView* const nullSrvs[RenderUseResource::SHADER_RESOURCE_COUNT] = {nullptr};
	context->PSSetShaderResources(0U, RenderUseResource::SHADER_RESOURCE_COUNT, nullSrvs);
}

/// <summary>
/// 複数描画
/// </summary>
/// <param name="_inputResource">入力リソース</param>
void PlayCameraEyeFirst::DrawMultiple(const MultipleRenderUseResource& _inputResource){
	ID3D11DeviceContext* const context = this->GetContext();

	// シェーダリソース設定
	context->PSSetShaderResources(
		0U,
		RenderUseResource::SHADER_RESOURCE_COUNT,
		_inputResource.shaderResources
	);

	// 頂点バッファ設定
	ID3D11Buffer* vertexBuffers[1] = { _inputResource.vertexBuffer };
	unsigned int vertexStride[1] = { sizeof(VSInput) };
	unsigned int vertexOffset[1] = { 0U };
	context->IASetVertexBuffers(0U, 1U, vertexBuffers, vertexStride, vertexOffset);

	// インデックスバッファ設定
	context->IASetIndexBuffer(_inputResource.indexBuffer, DXGI_FORMAT_R32_UINT, 0U);

	// プリミティブトポロジー設定
	context->IASetPrimitiveTopology(ConvertVertexTopologyToD3DPrimitiveTopology(_inputResource.topology));

	// 定数バッファ設定
	ConstantBuffer constantBuffer;
	constantBuffer.modelToProjectionMatrix = ::Ishikawa::Math::Matrix::IDENTITY;
	::Ishikawa::Math::Matrix viewProjection = _inputResource.viewMatrix * _inputResource.projectionMatrix;

	for(unsigned int i=0U; i<(_inputResource.worldMatrixCount); ++i){
		// 描画するかチェック
		{
			::Ishikawa::Math::Matrix checkModelToProjectionMatrix = _inputResource.worldMatrixArray[i] * viewProjection;

			::Ishikawa::Math::Float4 projectionCheckCenter = 
				::Ishikawa::Math::Float4(_inputResource.checkCenterPosition, 1.0f) * checkModelToProjectionMatrix;
			projectionCheckCenter *= (1.0f / projectionCheckCenter.w);

			::Ishikawa::Math::Float4 mostNearProjectionPosition;
			mostNearProjectionPosition.x = (projectionCheckCenter.x < -1.0f) ? (-1.0f) :
				((projectionCheckCenter.x > 1.0f) ? (1.0f) : (projectionCheckCenter.x));
			mostNearProjectionPosition.y = (projectionCheckCenter.y < -1.0f) ? (-1.0f) :
				((projectionCheckCenter.y > 1.0f) ? (1.0f) : (projectionCheckCenter.y));
			mostNearProjectionPosition.z = (projectionCheckCenter.z < 0.0f) ? (0.0f) :
				((projectionCheckCenter.z > 1.0f) ? (1.0f) : (projectionCheckCenter.z));
			mostNearProjectionPosition.w = 1.0f;
			::Ishikawa::Math::Float4 mostNearPosition = mostNearProjectionPosition * checkModelToProjectionMatrix.Invert();
			mostNearPosition *= (1.0f / mostNearPosition.w);

			::Ishikawa::Math::Float3 delta(
				_inputResource.checkCenterPosition.x - mostNearPosition.x,
				_inputResource.checkCenterPosition.y - mostNearPosition.y,
				_inputResource.checkCenterPosition.z - mostNearPosition.z
			);

			if(delta.LengthSquared() > _inputResource.checkSquareRadius){
				// 描画せずに次へ
				continue;
			}
		}

		// 変換行列変更
		constantBuffer.modelToProjectionMatrix = (_inputResource.worldMatrixArray[i] * viewProjection).Transpose();

		// 定数バッファ更新
		context->UpdateSubresource(this->constantBufferArray[0], 0U, nullptr, &constantBuffer, 0U, 0U);

		// 描画
		context->DrawIndexed(_inputResource.indexCount, 0U, 0);
	}

	// シェーダリソースリセット
	ID3D11ShaderResourceView* const nullSrvs[RenderUseResource::SHADER_RESOURCE_COUNT] = {nullptr};
	context->PSSetShaderResources(0U, RenderUseResource::SHADER_RESOURCE_COUNT, nullSrvs);
}

/// <summary>
/// 描画後処理
/// </summary>
void PlayCameraEyeFirst::DrawAfter(){
	ID3D11DeviceContext* const context = this->GetContext();

	// 定数バッファ開放
	this->ReleaseConstantBufferArray();

	// シェーダリセット
	context->VSSetShader(nullptr, nullptr, 0U);
	context->GSSetShader(nullptr, nullptr, 0U);
	context->PSSetShader(nullptr, nullptr, 0U);
	// 入力レイアウトリセット
	context->IASetInputLayout(nullptr);

	// 出力リソースリセット
	ID3D11RenderTargetView* nullRtvs[OutputResource::RENDER_TARGET_COUNT]{nullptr};
	context->OMSetRenderTargets(OutputResource::RENDER_TARGET_COUNT, nullRtvs, nullptr);
}


/// <summary>
/// 定数バッファ配列作成
/// </summary>
void PlayCameraEyeFirst::CreateConstantBufferArray(){
	ID3D11Device* const device = this->GetDevice();
	ID3D11Buffer* constantBuffer = nullptr;
	// バッファ設定
	D3D11_BUFFER_DESC constantBufferDesc;
	memset(&constantBufferDesc, 0, sizeof(constantBufferDesc));
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;

	// 定数バッファ作成
	HRESULT hr = device->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);
	if(FAILED(hr)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create constant buffer.");
	}


	// 定数バッファ格納
	this->constantBufferArray[0] = constantBuffer;
}

/// <summary>
/// 定数バッファ配列開放
/// </summary>
void PlayCameraEyeFirst::ReleaseConstantBufferArray(){
	// 一つずつ開放
	for(unsigned int i=0U; i<CONSTANT_BUFFER_COUNT; ++i){
		this->ReleaseBuffer(this->constantBufferArray[i]);
		this->constantBufferArray[i] = nullptr;
	}
}
