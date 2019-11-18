//=============================================================================
//	ITTextureData.cpp
//
//	����e�N�X�`���f�[�^�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITTextureData.h"
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

using namespace Ishikawa::DirectX;


/// <summary>
/// �e�N�X�`���o�C���h�\�t���O��D3D11�o�C���h�t���O�ɑΉ�����uint�l�ɕϊ�
/// </summary>
/// <param name="_bindableflag">�e�N�X�`���o�C���h�\�t���O</param>
/// <returns>D3D11�o�C���h�t���O�ɑΉ�����uint�l</returns>
unsigned int Texture::ConvertBindableFlagToUInt(BindableFlag _bindableflag){
	switch(_bindableflag){
	case BindableFlag::ShaderResource:
		return D3D11_BIND_SHADER_RESOURCE;

	case BindableFlag::RenderTarget:
		return D3D11_BIND_RENDER_TARGET;

	case BindableFlag::DepthStencil:
		return D3D11_BIND_DEPTH_STENCIL;

	case BindableFlag::SR_RT:
		return (D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

	case BindableFlag::SR_DS:
		return (D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL);

	default:
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't convert bindable flag.[_bindableFlag is unknown]");
	}
}


/// /// <summary>
/// �e�N�X�`���o�C���h�t���O��D3D11�o�C���h�t���O�ɕϊ�
/// </summary>
/// <param name="_bindflag">�e�N�X�`���o�C���h�t���O</param>
/// <returns>D3D11�o�C���h�t���O</returns>
D3D11_BIND_FLAG Texture::ConvertBindFlagToD3D11BindFlag(BindFlag _bindflag){
	switch(_bindflag){
	case BindFlag::NotBind:
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't convert bind flag.[_bindFlag is NotBind]");

	case BindFlag::ShaderResource:
		return D3D11_BIND_SHADER_RESOURCE;

	case BindFlag::RenderTarget:
		return D3D11_BIND_RENDER_TARGET;

	case BindFlag::DepthStencil:
		return D3D11_BIND_DEPTH_STENCIL;

	default:
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't convert bind flag.[_bindFlag is unknown]");
	}
}

/// <summary>
/// �e�N�X�`���t�H�[�}�b�g��DXGI�t�H�[�}�b�g�ɕϊ�
/// </summary>
/// <param name="_format">�e�N�X�`���t�H�[�}�b�g</param>
/// <returns>DXGI�t�H�[�}�b�g</returns>
DXGI_FORMAT Texture::ConvertFormatToDxgiFormat(Format _format){
	switch(_format){
	case Format::Unknown:
		return DXGI_FORMAT_UNKNOWN;

	case Format::R8G8B8A8UNorm:
		return DXGI_FORMAT_R8G8B8A8_UNORM;

	case Format::R10G10B10A2UNorm:
		return DXGI_FORMAT_R10G10B10A2_UNORM;

	case Format::R32G32B32A32Float:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;

	case Format::R32Float:
		return DXGI_FORMAT_R32_FLOAT;

	case Format::D24UNormS8UInt:
		return DXGI_FORMAT_D24_UNORM_S8_UINT;

	default:
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't convert format.[_format is unknown]");
	}
}

/// <summary>
/// �e�N�X�`���f�B�����V������D3D11�V�F�[�_���\�[�X�r���[�f�B�����V�����ɕϊ�
/// </summary>
/// <param name="_dimention">�e�N�X�`���f�B�����V����</param>
/// <returns>D3D11�V�F�[�_���\�[�X�r���[�f�B�����V����</returns>
D3D_SRV_DIMENSION Texture::ConvertDimentionToD3D11SRVDimention(Dimention _dimention){
	switch(_dimention){
	case Dimention::Texture2D:
		return D3D_SRV_DIMENSION_TEXTURE2D;

	case Dimention::MaltisampleTexture2D:
		return D3D_SRV_DIMENSION_TEXTURE2DMS;

	default:
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't convert format.[_dimention is unknown]");
	}
}

/// <summary>
/// �e�N�X�`���f�B�����V������D3D11�����_�[�^�[�Q�b�g�r���[�f�B�����V�����ɕϊ�
/// </summary>
/// <param name="_dimention">�e�N�X�`���f�B�����V����</param>
/// <returns>D3D11�����_�[�^�[�Q�b�g�r���[�f�B�����V����</returns>
D3D11_RTV_DIMENSION Texture::ConvertDimentionToD3D11RTVDimention(Dimention _dimention){
	switch(_dimention){
	case Dimention::Texture2D:
		return D3D11_RTV_DIMENSION_TEXTURE2D;

	case Dimention::MaltisampleTexture2D:
		return D3D11_RTV_DIMENSION_TEXTURE2DMS;

	default:
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't convert format.[_dimention is unknown]");
	}
}

/// <summary>
/// �e�N�X�`���f�B�����V������D3D11�f�v�X�X�e���V���r���[�f�B�����V�����ɕϊ�
/// </summary>
/// <param name="_dimention">�e�N�X�`���f�B�����V����</param>
/// <returns>D3D11�f�v�X�X�e���V���r���[�f�B�����V����</returns>
D3D11_DSV_DIMENSION Texture::ConvertDimentionToD3D11DSVDimention(Dimention _dimention){
	switch(_dimention){
	case Dimention::Texture2D:
		return D3D11_DSV_DIMENSION_TEXTURE2D;

	case Dimention::MaltisampleTexture2D:
		return D3D11_DSV_DIMENSION_TEXTURE2DMS;

	default:
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't convert format.[_dimention is unknown]");
	}
}


/// <summary>
/// �f�t�H���g�T���v���ݒ�f�[�^�̎擾
/// </summary>
/// <param name="_sampleDescCount">�T���v���ݒ�J�E���g�i�[��</param>
/// <param name="_sampleDescQuality">�T���v���ݒ�N�H���e�B�i�[��</param>
/// <param name="_dimention">�f�B�����V�����i�[��</param>
void Texture::GetDefaultSampleDescData(
	unsigned int* const _sampleDescCount,
	unsigned int* const _sampleDescQuality,
	Dimention* const _dimention
){
	if(_sampleDescCount != nullptr) (*_sampleDescCount) = 1U;
	if(_sampleDescQuality != nullptr) (*_sampleDescQuality) = 0U;
	if(_dimention != nullptr) (*_dimention) = Dimention::Texture2D;
}

/// <summary>
/// �}���`�T���v���E�A���`�G�C���A�V���O�ݒ�f�[�^�̎擾
/// </summary>
/// <param name="_sampleDescCount">�T���v���ݒ�J�E���g�i�[��</param>
/// <param name="_sampleDescQuality">�T���v���ݒ�N�H���e�B�i�[��</param>
/// <param name="_dimention">�f�B�����V�����i�[��</param>
/// <param name="_device">�f�o�C�X</param>
void Texture::GetMultisampleAntiAliasingDescData(
	unsigned int* const _sampleDescCount,
	unsigned int* const _sampleDescQuality,
	Dimention* const _dimention,
	ID3D11Device* const _device
){
	// �ō��i����T��
	for(unsigned int i=D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i>0U; --i){
		unsigned int quality = 0U;
		HRESULT hr = _device->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &quality);
		if(SUCCEEDED(hr)){
			if(quality > 0U){
				if(_sampleDescCount != nullptr) (*_sampleDescCount) = i;
				if(_sampleDescQuality != nullptr) (*_sampleDescQuality) = quality - 1U;
				if(_dimention != nullptr) (*_dimention) = Dimention::MaltisampleTexture2D;
				return;
			}
		}
	}
	// �ł��Ȃ���΃��O���c��
	::Ishikawa::Common::Debug::Log("Couldn't get multisample anti-aliasing desc data.\n");
}
