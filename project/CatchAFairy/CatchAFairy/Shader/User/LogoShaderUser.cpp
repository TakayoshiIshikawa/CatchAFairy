//=============================================================================
//	LogoShaderUser.cpp
//
//	���S�V�F�[�_���[�U�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "LogoShaderUser.h"
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


/// <summary>���̓��C�A�E�g�ݒ�</summary>
const D3D11_INPUT_ELEMENT_DESC Logo::INPUT_LAYOUT_DESC[INPUT_LAYOUT_COUNT] = {
	{ "SV_POSITION",	0, DXGI_FORMAT_R32G32_FLOAT,	0, 0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
/// <summary>���_�V�F�[�_�t�@�C���p�X</summary>
const char* const Logo::VERTEX_SHADER_FILEPATH = "Resources\\Shaders\\LogoVertexShader.cso";
/// <summary>�W�I���g���V�F�[�_�t�@�C���p�X</summary>
const char* const Logo::GEOMETRY_SHADER_FILEPATH = "Resources\\Shaders\\LogoGeometryShader.cso";
/// <summary>�s�N�Z���V�F�[�_�t�@�C���p�X</summary>
const char* const Logo::PIXEL_SHADER_FILEPATH = "Resources\\Shaders\\LogoPixelShader.cso";


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Logo::Logo() :
	constantBufferArray{nullptr}
{
	// �������Ȃ�
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
Logo::~Logo(){
	this->ReleaseConstantBufferArray();
}


/// <summary>
/// ���_�o�b�t�@�쐬
/// [���p���I�����o�b�t�@�͊J�����Ă�������]
/// </summary>
/// <param name="_vertexArray">���_�z��</param>
/// <param name="_vertexCount">���_��</param>
/// <returns>���_�o�b�t�@</returns>
ID3D11Buffer* Logo::CreateVertexBuffer(const VSInput* const _vertexArray, unsigned int _vertexCount){
	if(_vertexArray == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_vertexArray is NULL.");
	}
	if(_vertexCount == 0U){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_vertexCount is zero.");
	}
	
	ID3D11Device* const device = this->GetDevice();
	ID3D11Buffer* vertexBuffer = nullptr;
	// �o�b�t�@�ݒ�
	D3D11_BUFFER_DESC vertexBufferDesc;
	memset(&vertexBufferDesc, 0, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = _vertexCount * sizeof(VSInput);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	// ���\�[�X�f�[�^
	D3D11_SUBRESOURCE_DATA vertexSubresource;
	memset(&vertexSubresource, 0, sizeof(vertexSubresource));
	vertexSubresource.pSysMem = _vertexArray;

	// ���_�o�b�t�@�쐬
	HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexSubresource, &vertexBuffer);
	if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create vertex buffer.");
	}

	return vertexBuffer;
}

/// <summary>
/// �T���v���X�e�[�g�̍쐬
/// </summary>
/// <param name="_device">�f�o�C�X</param>
void Logo::CreateSamplerState(ID3D11Device* const _device){
	// �ݒ�
	D3D11_SAMPLER_DESC samplerDesc;
	memset(&samplerDesc, 0, sizeof(samplerDesc));
	samplerDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD			= 0;
	samplerDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	// ����
	ID3D11SamplerState* sampler = nullptr;
	HRESULT hr = _device->CreateSamplerState(&samplerDesc, &(sampler));
	if(FAILED(hr)){
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create sampler state.");
	}
	this->SetSamplerState(sampler);
}

/// <summary>
/// �u�����h�X�e�[�g�̍쐬
/// </summary>
/// <param name="_device">�f�o�C�X</param>
void Logo::CreateBlendState(ID3D11Device* const _device){
	CD3D11_DEFAULT defaultState;
	CD3D11_BLEND_DESC blendDesc(defaultState);

	// ����
	ID3D11BlendState* blend = nullptr;
	HRESULT hr = _device->CreateBlendState(&blendDesc, &(blend));
	if(FAILED(hr)){
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create blend state.");
	}
	this->SetBlendState(blend);
}

/// <summary>
/// �f�v�X�X�e���V���X�e�[�g�̍쐬
/// </summary>
/// <param name="_device">�f�o�C�X</param>
void Logo::CreateDepthStencilState(ID3D11Device* const _device){
	CD3D11_DEFAULT defaultState;
	CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(defaultState);
	depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	// ����
	ID3D11DepthStencilState* depthStencil = nullptr;
	HRESULT hr = _device->CreateDepthStencilState(&depthStencilDesc, &(depthStencil));
	if(FAILED(hr)){
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create depth stencil state.");
	}
	this->SetDepthStencilState(depthStencil);
}

/// <summary>
/// ���X�^���C�U�X�e�[�g�̍쐬
/// </summary>
/// <param name="_device">�f�o�C�X</param>
void Logo::CreateRasterizerState(ID3D11Device* const _device){
	CD3D11_DEFAULT defaultState;
	CD3D11_RASTERIZER_DESC rasterizerDesc(defaultState);
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;

	// ����
	ID3D11RasterizerState* rasterizer = nullptr;
	HRESULT hr = _device->CreateRasterizerState(&rasterizerDesc, &(rasterizer));
	if(FAILED(hr)){
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create sampler state.");
	}
	this->SetRasterizerState(rasterizer);
}


/// <summary>
/// �`��O����
/// </summary>
/// <param name="_outputResource">�o�̓��\�[�X</param>
/// <param name="_color">�������F</param>
void Logo::DrawBefore(
	const OutputResource& _outputResource,
	const ::Ishikawa::Math::Float4* const _color
){
	ID3D11DeviceContext* const context = this->GetContext();

	// �o�̓��\�[�X�̏�����
	if(_color != nullptr){
		context->ClearRenderTargetView(_outputResource.renderTargetView, _color->v);
	}

	// �o�͐ݒ�
	context->OMSetRenderTargets(1U, &(_outputResource.renderTargetView), nullptr);

	// �V�F�[�_�ݒ�
	context->VSSetShader(this->GetVertexShader(), nullptr, 0U);
	context->GSSetShader(this->GetGeometryShader(), nullptr, 0U);
	context->PSSetShader(this->GetPixelShader(), nullptr, 0U);
	// ���̓��C�A�E�g�ݒ�
	context->IASetInputLayout(this->GetInputLayout());
	// �T���v���ݒ�
	const unsigned int samplerCount = 1U;
	ID3D11SamplerState* const samplers[samplerCount] = {
		this->GetSamplerState()
	};
	context->PSSetSamplers(0U, samplerCount, samplers);
	// �u�����h�ݒ�
	context->OMSetBlendState(this->GetBlendState(), nullptr, 0xffffffff);
	// �f�v�X�X�e���V���ݒ�
	context->OMSetDepthStencilState(this->GetDepthStencilState(), 0U);
	// ���X�^���C�U�ݒ�
	context->RSSetState(this->GetRasterizerState());

	// �r���[�|�[�g�ݒ�
	const unsigned int viewportCount = 1U;
	const D3D11_VIEWPORT viewports[viewportCount] = {
		::Ishikawa::DirectX::ConvertViewportToD3D11Viewport(*(_outputResource.viewport))
	};
	context->RSSetViewports(viewportCount, viewports);

	// �萔�o�b�t�@�쐬
	this->CreateConstantBufferArray();

	// �萔�o�b�t�@�ݒ�
	context->VSSetConstantBuffers(0U, CONSTANT_BUFFER_COUNT, this->constantBufferArray);
	context->GSSetConstantBuffers(0U, CONSTANT_BUFFER_COUNT, this->constantBufferArray);
	context->PSSetConstantBuffers(0U, CONSTANT_BUFFER_COUNT, this->constantBufferArray);
}

/// <summary>
/// �`��
/// </summary>
/// <param name="_inputResource">���̓��\�[�X</param>
void Logo::Draw(const RenderUseResource& _inputResource){
	ID3D11DeviceContext* const context = this->GetContext();

	// �V�F�[�_���\�[�X�ݒ�
	const unsigned int pixelShaderResourceCount = 1U;
	ID3D11ShaderResourceView* const pixelShaderResources[pixelShaderResourceCount] = {
		_inputResource.shaderResourceView
	};
	context->PSSetShaderResources(0U, pixelShaderResourceCount, pixelShaderResources);

	// ���_�o�b�t�@�ݒ�
	ID3D11Buffer* vertexBuffers[1] = { _inputResource.vertexBuffer };
	unsigned int vertexStride[1] = { sizeof(VSInput) };
	unsigned int vertexOffset[1] = { 0U };
	context->IASetVertexBuffers(0U, 1U, vertexBuffers, vertexStride, vertexOffset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	context->IASetIndexBuffer(_inputResource.indexBuffer, DXGI_FORMAT_R32_UINT, 0U);

	// �v���~�e�B�u�g�|���W�[�ݒ�
	context->IASetPrimitiveTopology(ConvertVertexTopologyToD3DPrimitiveTopology(_inputResource.topology));

	// �萔�o�b�t�@�X�V
	context->UpdateSubresource(this->constantBufferArray[0], 0U, nullptr, &(_inputResource.constantBuffer), 0U, 0U);

	// �`��
	context->DrawIndexed(_inputResource.indexCount, 0U, 0);

	// �V�F�[�_���\�[�X���Z�b�g
	ID3D11ShaderResourceView* const nullSrvs[pixelShaderResourceCount] = {nullptr};
	context->PSSetShaderResources(0U, pixelShaderResourceCount, nullSrvs);
}

/// <summary>
/// �`��㏈��
/// </summary>
void Logo::DrawAfter(){
	ID3D11DeviceContext* const context = this->GetContext();

	// �萔�o�b�t�@�J��
	this->ReleaseConstantBufferArray();

	// �V�F�[�_���Z�b�g
	context->VSSetShader(nullptr, nullptr, 0U);
	context->GSSetShader(nullptr, nullptr, 0U);
	context->PSSetShader(nullptr, nullptr, 0U);
	// ���̓��C�A�E�g���Z�b�g
	context->IASetInputLayout(nullptr);

	// �o�̓��\�[�X���Z�b�g
	context->OMSetRenderTargets(0U, nullptr, nullptr);
}


/// <summary>
/// �萔�o�b�t�@�z��쐬
/// </summary>
void Logo::CreateConstantBufferArray(){
	ID3D11Device* const device = this->GetDevice();
	ID3D11Buffer* constantBuffer = nullptr;
	// �o�b�t�@�ݒ�
	D3D11_BUFFER_DESC constantBufferDesc;
	memset(&constantBufferDesc, 0, sizeof(constantBufferDesc));
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;

	// �萔�o�b�t�@�쐬
	HRESULT hr = device->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);
	if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create constant buffer.");
	}


	// �萔�o�b�t�@�i�[
	this->constantBufferArray[0] = constantBuffer;
}

/// <summary>
/// �萔�o�b�t�@�z��J��
/// </summary>
void Logo::ReleaseConstantBufferArray(){
	// ����J��
	for(unsigned int i=0U; i<CONSTANT_BUFFER_COUNT; ++i){
		this->ReleaseBuffer(this->constantBufferArray[i]);
		this->constantBufferArray[i] = nullptr;
	}
}
