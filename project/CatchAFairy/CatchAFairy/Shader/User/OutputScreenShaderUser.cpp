//=============================================================================
//	OutputScreenShaderUser.cpp
//
//	��ʏo�̓V�F�[�_���[�U�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "OutputScreenShaderUser.h"
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
const D3D11_INPUT_ELEMENT_DESC OutputScreen::INPUT_LAYOUT_DESC[INPUT_LAYOUT_COUNT] = {
	{ "SV_POSITION",	0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
/// <summary>���_�V�F�[�_�t�@�C���p�X</summary>
const char* const OutputScreen::VERTEX_SHADER_FILEPATH = "Resources\\Shaders\\OutputScreenVertexShader.cso";
/// <summary>�W�I���g���V�F�[�_�t�@�C���p�X</summary>
const char* const OutputScreen::GEOMETRY_SHADER_FILEPATH = "Resources\\Shaders\\OutputScreenGeometryShader.cso";
/// <summary>�s�N�Z���V�F�[�_�t�@�C���p�X</summary>
const char* const OutputScreen::PIXEL_SHADER_FILEPATH = "Resources\\Shaders\\OutputScreenPixelShader.cso";


/// <summary>
/// ���_�o�b�t�@�쐬
/// [���p���I�����o�b�t�@�͊J�����Ă�������]
/// </summary>
/// <param name="_vertexArray">���_�z��</param>
/// <param name="_vertexCount">���_��</param>
/// <returns>���_�o�b�t�@</returns>
ID3D11Buffer* OutputScreen::CreateVertexBuffer(const VSInput* const _vertexArray, unsigned int _vertexCount){
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
void OutputScreen::CreateSamplerState(ID3D11Device* const _device){
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
void OutputScreen::CreateBlendState(ID3D11Device* const _device){
	/*
	// �����_�[�^�[�Q�b�g�u�����h�ݒ�
	D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc[8];
	memset(&renderTargetBlendDesc, 0, sizeof(renderTargetBlendDesc));
	// 0�Ԗڂ̐ݒ�[o.rgb = s.rgb*s.a + d.rgb*(1-s.a); o.a = d.a + (1-d.a)*s.a]
	renderTargetBlendDesc[0].BlendEnable	= TRUE;
	renderTargetBlendDesc[0].BlendOp		= D3D11_BLEND_OP_ADD;
	renderTargetBlendDesc[0].BlendOpAlpha	= D3D11_BLEND_OP_ADD;
	renderTargetBlendDesc[0].SrcBlend		= D3D11_BLEND_SRC_ALPHA;
	renderTargetBlendDesc[0].SrcBlendAlpha	= D3D11_BLEND_INV_DEST_ALPHA;
	renderTargetBlendDesc[0].DestBlend		= D3D11_BLEND_INV_SRC_ALPHA;
	renderTargetBlendDesc[0].DestBlendAlpha	= D3D11_BLEND_ONE;
	renderTargetBlendDesc[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	// 1�`7�Ԃ͗��p���Ȃ�
	renderTargetBlendDesc[1].BlendEnable = FALSE;
	renderTargetBlendDesc[2].BlendEnable = FALSE;
	renderTargetBlendDesc[3].BlendEnable = FALSE;
	renderTargetBlendDesc[4].BlendEnable = FALSE;
	renderTargetBlendDesc[5].BlendEnable = FALSE;
	renderTargetBlendDesc[6].BlendEnable = FALSE;
	renderTargetBlendDesc[7].BlendEnable = FALSE;

	// �u�����h�X�e�[�g�ݒ�
	D3D11_BLEND_DESC blendDesc;
	memset(&blendDesc, 0, sizeof(blendDesc));
	blendDesc.RenderTarget[0] = renderTargetBlendDesc[0];
	blendDesc.RenderTarget[1] = renderTargetBlendDesc[1];
	blendDesc.RenderTarget[2] = renderTargetBlendDesc[2];
	blendDesc.RenderTarget[3] = renderTargetBlendDesc[3];
	blendDesc.RenderTarget[4] = renderTargetBlendDesc[4];
	blendDesc.RenderTarget[5] = renderTargetBlendDesc[5];
	blendDesc.RenderTarget[6] = renderTargetBlendDesc[6];
	blendDesc.RenderTarget[7] = renderTargetBlendDesc[7];
	blendDesc.AlphaToCoverageEnable	 = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	*/
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
void OutputScreen::CreateDepthStencilState(ID3D11Device* const _device){
	/*
	D3D11_DEPTH_STENCILOP_DESC depthStencilopDesc[2];
	memset(&depthStencilopDesc, 0, sizeof(depthStencilopDesc));
	// �t�����g
	depthStencilopDesc[0].StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;
	depthStencilopDesc[0].StencilFunc			= D3D11_COMPARISON_ALWAYS;
	depthStencilopDesc[0].StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilopDesc[0].StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	// �o�b�N�ݒ�
	depthStencilopDesc[1].StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;
	depthStencilopDesc[1].StencilFunc			= D3D11_COMPARISON_ALWAYS;
	depthStencilopDesc[1].StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilopDesc[1].StencilPassOp			= D3D11_STENCIL_OP_KEEP;

	// �f�v�X�X�e���V���X�e�[�g�ݒ�
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	memset(&depthStencilDesc, 0, sizeof(depthStencilDesc));
//	depthStencilDesc.FrontFace			= depthStencilopDesc[0];
//	depthStencilDesc.BackFace			= depthStencilopDesc[1];
	depthStencilDesc.DepthEnable		= TRUE;
	depthStencilDesc.DepthFunc			= D3D11_COMPARISON_LESS;
	depthStencilDesc.DepthWriteMask		= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.StencilEnable		= FALSE;
	depthStencilDesc.StencilReadMask	= D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask	= D3D11_DEFAULT_STENCIL_WRITE_MASK;
	*/

	CD3D11_DEFAULT defaultState;
	CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(defaultState);
	depthStencilDesc.DepthEnable = FALSE;

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
void OutputScreen::CreateRasterizerState(ID3D11Device* const _device){
	/*
	// �ݒ�
	D3D11_RASTERIZER_DESC rasterizerDesc;
	memset(&rasterizerDesc, 0, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode					= D3D11_FILL_SOLID;
	rasterizerDesc.CullMode					= D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise	= FALSE;
	rasterizerDesc.DepthBias				= 0;
	rasterizerDesc.SlopeScaledDepthBias		= 0.0f;
	rasterizerDesc.DepthBiasClamp			= 0.0f;
	rasterizerDesc.DepthClipEnable			= TRUE;
	rasterizerDesc.ScissorEnable			= FALSE;
	rasterizerDesc.MultisampleEnable		= FALSE;
	rasterizerDesc.AntialiasedLineEnable	= FALSE;
	*/

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
void OutputScreen::DrawBefore(
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
}

/// <summary>
/// �`��
/// </summary>
/// <param name="_inputResource">���̓��\�[�X</param>
void OutputScreen::Draw(const RenderUseResource& _inputResource){
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

	// �`��
	context->DrawIndexed(_inputResource.indexCount, 0U, 0);

	// �V�F�[�_���\�[�X���Z�b�g
	ID3D11ShaderResourceView* const nullSrvs[pixelShaderResourceCount] = {nullptr};
	context->PSSetShaderResources(0U, pixelShaderResourceCount, nullSrvs);
}

/// <summary>
/// �`��㏈��
/// </summary>
void OutputScreen::DrawAfter(){
	ID3D11DeviceContext* const context = this->GetContext();

	// �V�F�[�_���Z�b�g
	context->VSSetShader(nullptr, nullptr, 0U);
	context->GSSetShader(nullptr, nullptr, 0U);
	context->PSSetShader(nullptr, nullptr, 0U);
	// ���̓��C�A�E�g���Z�b�g
	context->IASetInputLayout(nullptr);

	// �o�̓��\�[�X���Z�b�g
	context->OMSetRenderTargets(0U, nullptr, nullptr);
}
