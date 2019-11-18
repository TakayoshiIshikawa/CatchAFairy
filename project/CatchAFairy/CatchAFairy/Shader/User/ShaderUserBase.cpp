//=============================================================================
//	ShaderUserBase.cpp
//
//	�V�F�[�_���[�U���̃\�[�X�t�@�C��
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
/// �t�@�C���̃R�[�h�Ƃ��̒����̎擾
/// </summary>
/// <param name="_code">�R�[�h�i�[��[���p�I�������烁�������폜���邱��]</param>
/// <param name="_length">�R�[�h�̒����i�[��</param>
/// <param name="_filepath">�t�@�C���p�X</param>
void Base::GetFileCodeAndLength(
	char** const _code,
	unsigned long* const _length,
	const char* const _filepath
){
	if(_code == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_code is NULL.");
	}
	if(_length == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_length is NULL.");
	}
	if(_filepath == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_filepath is NULL.");
	}

	// �t�@�C���T�C�Y�擾
	{
		struct stat statBuffer;

		if(stat(_filepath, &statBuffer) != 0){
			// �ُ�I��
			throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't get file stat.");
		}

		(*_length) = static_cast<unsigned long>(statBuffer.st_size);
	}

	// �R�[�h�ۑ��p�������̊m��
	(*_code) = new char[(*_length) + 1]{0};
	if((*_code) == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't new code memory.");
	}

#pragma warning(disable:4996)
	FILE* const file = fopen(_filepath, "rb");
#pragma warning(default:4996)
	if(file == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_filepath is NULL.");
	}

	// �R�[�h�̎擾
#pragma warning(disable:4996)
	fread(*_code, sizeof(char), *_length, file);
#pragma warning(default:4996)

	fclose(file);
}


/// <summary>
/// �f�t�H���g�R���X�g���N�^
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
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Base::~Base(){
	this->ReleaseAll();
}


/// <summary>
/// �f�o�C�X�̎擾
/// </summary>
/// <returns>�f�o�C�X</returns>
ID3D11Device* Base::GetDevice() const{
	// ����f�o�C�X�̎擾
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetDependentITDevice();
	if(itDevice == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("itDevice is NULL.");
	}

	// �f�o�C�X�̎擾
	ID3D11Device* const device = itDevice->GetDevice();
	if(device == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("device is NULL.");
	}

	return device;
}

/// <summary>
/// �R���e�L�X�g�̎擾
/// </summary>
/// <returns>�R���e�L�X�g</returns>
ID3D11DeviceContext* Base::GetContext() const{
	// �ˑ����Ă��鎩��f�o�C�X�̎擾
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetDependentITDevice();
	if(itDevice == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("itDevice is NULL.");
	}

	// �R���e�L�X�g�̎擾
	ID3D11DeviceContext* const context = itDevice->GetContext();
	if(context == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("context is NULL.");
	}

	return context;
}


/// <summary>
/// �f�o�C�X�ɓǂݍ��܂ꂽ
/// </summary>
void Base::OnDeviceLoaded(){
	// �쐬
	this->Create();
}

/// <summary>
/// �f�o�C�X���������ꂽ
/// </summary>
void Base::OnDeviceRestored(){
	// �쐬
	this->Create();
}

/// <summary>
/// �f�o�C�X���j�����ꂽ
/// </summary>
void Base::OnDeviceLost(){
	// ���\�[�X�̊J��
	this->ReleaseAll();
}


/// <summary>
/// ������
/// </summary>
void Base::Initialize(){
	// ���̏�����
	this->InitializeITDeviceDependentResource();
}

/// <summary>
/// �C���f�b�N�X�o�b�t�@�쐬
/// [���p���I�����o�b�t�@�͊J�����Ă�������]
/// </summary>
/// <param name="_indexArray">�C���f�b�N�X�z��</param>
/// <param name="_indexCount">�C���f�b�N�X��</param>
/// <returns> �C���f�b�N�X�o�b�t�@</returns>
ID3D11Buffer* Base::CreateIndexBuffer(
	const unsigned long* const _indexArray,
	unsigned int _indexCount
) const {
	if(_indexArray == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_indexArray is NULL.");
	}
	if(_indexCount == 0U){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_indexCount is zero.");
	}

	ID3D11Device* const device = this->GetDevice();
	ID3D11Buffer* indexBuffer = nullptr;
	// �o�b�t�@�ݒ�
	D3D11_BUFFER_DESC indexBufferDesc;
	memset(&indexBufferDesc, 0, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = _indexCount * sizeof(unsigned int);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	// ���\�[�X�f�[�^
	D3D11_SUBRESOURCE_DATA indexSubresource;
	memset(&indexSubresource, 0, sizeof(indexSubresource));
	indexSubresource.pSysMem = _indexArray;

	// �C���f�b�N�X�o�b�t�@�쐬
	HRESULT hr = device->CreateBuffer(&indexBufferDesc, &indexSubresource, &indexBuffer);
	if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create index buffer.");
	}

	return indexBuffer;
}

/// <summary>
/// �o�b�t�@�J��
/// </summary>
/// <param name="_buffer">�o�b�t�@</param>
void Base::ReleaseBuffer(ID3D11Buffer* const _buffer) const {
	if(_buffer != nullptr) _buffer->Release();
}

/// <summary>
/// �쐬
/// </summary>
void Base::Create(){
	// �f�o�C�X�̎擾
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
/// ���_�V�F�[�_�Ɠ��̓��C�A�E�g�̍쐬
/// </summary>
/// <param name="_device">�f�o�C�X</param>
void Base::CreateVertexShaderAndInputLayout(ID3D11Device* const _device){
	// �V�F�[�_�R�[�h�擾
	::Ishikawa::Common::SafePointer<char> codeBuffer;
	unsigned long codeLength = 0UL;
	{
		char* code = nullptr;
		Base::GetFileCodeAndLength(&code, &codeLength, this->GetVertexShaderFilepath());
		codeBuffer.Reset(code);
	}

	// ���_�V�F�[�_�쐬
	HRESULT hr = _device->CreateVertexShader(codeBuffer.Get(), codeLength, nullptr, &(this->vertexShader));
	if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create vertex shader.");
	}

	// ���̓��C�A�E�g�쐬
	hr = _device->CreateInputLayout(
		this->GetInputLayoutDesc(),
		this->GetInputLayoutCount(),
		codeBuffer.Get(), codeLength,
		&(this->inputLayout)
	);
	if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create input layout.");
	}
}

/// <summary>
/// �W�I���g���V�F�[�_�̍쐬
/// </summary>
/// <param name="_device">�f�o�C�X</param>
void Base::CreateGeometryShader(ID3D11Device* const _device){
	// �V�F�[�_�R�[�h�擾
	::Ishikawa::Common::SafePointer<char> codeBuffer;
	unsigned long codeLength = 0UL;
	{
		char* code = nullptr;
		Base::GetFileCodeAndLength(&code, &codeLength, this->GetGeometryShaderFilepath());
		codeBuffer.Reset(code);
	}

	// �W�I���g���V�F�[�_�쐬
	HRESULT hr = _device->CreateGeometryShader(codeBuffer.Get(), codeLength, nullptr, &(this->geometryShader));
	if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create geometry shader.");
	}
}

/// <summary>
/// �s�N�Z���V�F�[�_�̍쐬
/// </summary>
/// <param name="_device">�f�o�C�X</param>
void Base::CreatePixelShader(ID3D11Device* const _device){
	// �V�F�[�_�R�[�h�擾
	::Ishikawa::Common::SafePointer<char> codeBuffer;
	unsigned long codeLength = 0UL;
	{
		char* code = nullptr;
		Base::GetFileCodeAndLength(&code, &codeLength, this->GetPixelShaderFilepath());
		codeBuffer.Reset(code);
	}

	// �s�N�Z���V�F�[�_�쐬
	HRESULT hr = _device->CreatePixelShader(codeBuffer.Get(), codeLength, nullptr, &(this->pixelShader));
	if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create pixel shader.");
	}
}


/// <summary>
/// �S�Ẵ��\�[�X�̊J��
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
/// ���̓��C�A�E�g�̊J��
/// </summary>
void Base::ReleaseInputLayout(){
	if(this->inputLayout != nullptr){
		this->inputLayout->Release();
		this->inputLayout = nullptr;
	}
}

/// <summary>
/// ���_�V�F�[�_�̊J��
/// </summary>
void Base::ReleaseVertexShader(){
	if(this->vertexShader != nullptr){
		this->vertexShader->Release();
		this->vertexShader = nullptr;
	}
}

/// <summary>
/// �W�I���g���V�F�[�_�̊J��
/// </summary>
void Base::ReleaseGeometryShader(){
	if(this->geometryShader != nullptr){
		this->geometryShader->Release();
		this->geometryShader = nullptr;
	}
}

/// <summary>
/// �s�N�Z���V�F�[�_�̊J��
/// </summary>
void Base::ReleasePixelShader(){
	if(this->pixelShader != nullptr){
		this->pixelShader->Release();
		this->pixelShader = nullptr;
	}
}

/// <summary>
/// �T���v���X�e�[�g�̊J��
/// </summary>
void Base::ReleaseSamplerState(){
	if(this->samplerState != nullptr){
		this->samplerState->Release();
		this->samplerState = nullptr;
	}
}

/// <summary>
/// �u�����h�X�e�[�g�̊J��
/// </summary>
void Base::ReleaseBlendState(){
	if(this->blendState != nullptr){
		this->blendState->Release();
		this->blendState = nullptr;
	}
}

/// <summary>
/// �f�v�X�X�e���V���X�e�[�g�̊J��
/// </summary>
void Base::ReleaseDepthStencilState(){
	if(this->depthStencilState != nullptr){
		this->depthStencilState->Release();
		this->depthStencilState = nullptr;
	}
}

/// <summary>
/// ���X�^���C�U�X�e�[�g�̊J��
/// </summary>
void Base::ReleaseRasterizerState(){
	if(this->rasterizerState != nullptr){
		this->rasterizerState->Release();
		this->rasterizerState = nullptr;
	}
}
