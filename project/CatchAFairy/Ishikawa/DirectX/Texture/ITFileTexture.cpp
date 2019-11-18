//=============================================================================
//	ITFileTexture.cpp
//
//	����t�@�C���e�N�X�`���̃\�[�X�t�@�C��
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
/// �f�t�H���g�R���X�g���N�^
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
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
FileTexture::~FileTexture(){
	this->ReleaseShaderResourceView();

	if(this->filepath != nullptr){
		delete[] this->filepath;
		this->filepath = nullptr;
	}
}


/// <summary>
/// �f�o�C�X�ɓǂݍ��܂ꂽ
/// </summary>
void FileTexture::OnDeviceLoaded(){
	if(this->filepath != nullptr){
		this->Load(this->filepath);
	}
}

/// <summary>
/// �f�o�C�X���������ꂽ
/// </summary>
void FileTexture::OnDeviceRestored(){
	if(this->filepath != nullptr){
		this->Load(this->filepath);
	}
}

/// <summary>
/// �f�o�C�X���j�����ꂽ
/// </summary>
void FileTexture::OnDeviceLost(){
	this->ReleaseShaderResourceView();
}


/// <summary>
/// ������
/// </summary>
void FileTexture::Initialize(){
	// ���̏�����
	this->InitializeITDeviceDependentResource();
}


/// <summary>
/// �ǂݍ���
/// </summary>
/// <param name="_filepath">�t�@�C���p�X</param>
void FileTexture::Load(const wchar_t* const _filepath){
	if(_filepath == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_filepath is NULL.");
	}

	// �t�@�C���p�X�X�V
	this->ChangeFilepath(_filepath);


	// �f�o�C�X�̎擾
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


	// �摜��񏉊���
	this->ReleaseShaderResourceView();
	this->width = 0U;
	this->height = 0U;

	// �V�F�[�_���\�[�X�r���[�쐬
	ID3D11Resource* resource = nullptr;
	HRESULT hr = ::DirectX::CreateWICTextureFromFile(device, _filepath, &resource, &(this->shaderResourceView));
	if(FAILED(hr)){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't load file texture.");
	}

	// �摜�T�C�Y�擾
	ID3D11Texture2D* const texture = static_cast<ID3D11Texture2D*>(resource);
	D3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);
	this->width	 = textureDesc.Width;
	this->height = textureDesc.Height;

	// ���\�[�X�J��
	resource->Release();
}


/// <summary>
/// �t�@�C���p�X�̕ύX
/// </summary>
/// <param name="_filepath">�t�@�C���p�X</param>
void FileTexture::ChangeFilepath(const wchar_t* const _filepath){
	if(this->filepath != nullptr){
		delete[] this->filepath;
		this->filepath = nullptr;
	}

	unsigned int length = wcslen(_filepath);
	this->filepath = new wchar_t[length+1]{0};
	if(this->filepath == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't secure new memory.");
	}

#pragma warning(disable:4996)
	wcscpy(this->filepath, _filepath);
#pragma warning(default:4996)
}

/// <summary>
/// �V�F�[�_���\�[�X�r���[�̊J��
/// </summary>
void FileTexture::ReleaseShaderResourceView(){
	if(this->shaderResourceView != nullptr){
		this->shaderResourceView->Release();
		this->shaderResourceView = nullptr;
	}
}
