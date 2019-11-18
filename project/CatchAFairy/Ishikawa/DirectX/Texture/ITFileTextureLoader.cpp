//=============================================================================
//	ITFileTextureLoader.cpp
//
//	����t�@�C���e�N�X�`�����[�_�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITFileTextureLoader.h"
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#include <windows.h>
#pragma warning(default:4820)
#pragma warning(default:4668)
#include <combaseapi.h>
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#pragma warning(disable:4917)
#include <wincodec.h>
#pragma warning(default:4917)
#pragma warning(default:4820)
#pragma warning(default:4668)
#pragma warning(disable:4626)
#include <wrl/client.h>
#pragma warning(default:4626)
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Common/PatternBase/SingletonManager.h"

using namespace Ishikawa::DirectX::Texture;


/// <summary>�V���O���g���C���X�^���X</summary>
FileTextureLoader* FileTextureLoader::instance = nullptr;


/// <summary>
/// �C���X�^���X�̎擾[����t�@�C���e�N�X�`�����[�_�[]
/// </summary>
/// <returns>����t�@�C���e�N�X�`�����[�_�[�̃C���X�^���X</returns>
template<>
FileTextureLoader& Ishikawa::Common::SingletonManager::GetInstance<FileTextureLoader>(){
	if(FileTextureLoader::instance == nullptr){
		FileTextureLoader::instance = new FileTextureLoader();
	}
	return (*(FileTextureLoader::instance));
}

/// <summary>
/// WIC�C���[�W�t�@�N�g���̎擾
/// </summary>
/// <returns>WIC�C���[�W�t�@�N�g��</returns>
static IWICImagingFactory* GetWICImagingFactory(){
	static Microsoft::WRL::ComPtr<IWICImagingFactory> wicImagingFactory(nullptr);

	if(wicImagingFactory.Get() == nullptr){
		HRESULT hr = CoCreateInstance(
			CLSID_WICImagingFactory1,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(wicImagingFactory.ReleaseAndGetAddressOf())
		);
		if(FAILED(hr)){
			// �ُ�I��
			throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create WIC imaging factory.");
		}
	}

	return wicImagingFactory.Get();
}


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
FileTextureLoader::FileTextureLoader(){
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
FileTextureLoader::~FileTextureLoader(){
	// �������Ȃ�
}


/// <summary>
/// �ǂݍ���
/// </summary>
/// <param name="_device">�f�o�C�X</param>
/// <param name="_filepath">�t�@�C���p�X</param>
/// <param name="_texture">�e�N�X�`���i�[��</param>
/// <param name="_shaderResourceView">�V�F�[�_���\�[�X�r���[�i�[��</param>
void FileTextureLoader::Load(
	ID3D11Device* const _device,
	const wchar_t* const _filepath,
	ID3D11Texture2D** _texture,
	ID3D11ShaderResourceView** _shaderResourceView
){
	NOT_USING(_device);
	NOT_USING(_filepath);
	NOT_USING(_texture);
	NOT_USING(_shaderResourceView);

	IWICImagingFactory* const wicFactory = GetWICImagingFactory();
	NOT_USING(wicFactory);

	//TODO:�����҂�
}
