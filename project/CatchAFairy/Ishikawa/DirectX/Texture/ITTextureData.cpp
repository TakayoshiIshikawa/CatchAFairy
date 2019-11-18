//=============================================================================
//	ITTextureData.cpp
//
//	自作テクスチャデータのソースファイル
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
/// テクスチャバインド可能フラグをD3D11バインドフラグに対応するuint値に変換
/// </summary>
/// <param name="_bindableflag">テクスチャバインド可能フラグ</param>
/// <returns>D3D11バインドフラグに対応するuint値</returns>
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
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't convert bindable flag.[_bindableFlag is unknown]");
	}
}


/// /// <summary>
/// テクスチャバインドフラグをD3D11バインドフラグに変換
/// </summary>
/// <param name="_bindflag">テクスチャバインドフラグ</param>
/// <returns>D3D11バインドフラグ</returns>
D3D11_BIND_FLAG Texture::ConvertBindFlagToD3D11BindFlag(BindFlag _bindflag){
	switch(_bindflag){
	case BindFlag::NotBind:
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't convert bind flag.[_bindFlag is NotBind]");

	case BindFlag::ShaderResource:
		return D3D11_BIND_SHADER_RESOURCE;

	case BindFlag::RenderTarget:
		return D3D11_BIND_RENDER_TARGET;

	case BindFlag::DepthStencil:
		return D3D11_BIND_DEPTH_STENCIL;

	default:
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't convert bind flag.[_bindFlag is unknown]");
	}
}

/// <summary>
/// テクスチャフォーマットをDXGIフォーマットに変換
/// </summary>
/// <param name="_format">テクスチャフォーマット</param>
/// <returns>DXGIフォーマット</returns>
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
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't convert format.[_format is unknown]");
	}
}

/// <summary>
/// テクスチャディメンションをD3D11シェーダリソースビューディメンションに変換
/// </summary>
/// <param name="_dimention">テクスチャディメンション</param>
/// <returns>D3D11シェーダリソースビューディメンション</returns>
D3D_SRV_DIMENSION Texture::ConvertDimentionToD3D11SRVDimention(Dimention _dimention){
	switch(_dimention){
	case Dimention::Texture2D:
		return D3D_SRV_DIMENSION_TEXTURE2D;

	case Dimention::MaltisampleTexture2D:
		return D3D_SRV_DIMENSION_TEXTURE2DMS;

	default:
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't convert format.[_dimention is unknown]");
	}
}

/// <summary>
/// テクスチャディメンションをD3D11レンダーターゲットビューディメンションに変換
/// </summary>
/// <param name="_dimention">テクスチャディメンション</param>
/// <returns>D3D11レンダーターゲットビューディメンション</returns>
D3D11_RTV_DIMENSION Texture::ConvertDimentionToD3D11RTVDimention(Dimention _dimention){
	switch(_dimention){
	case Dimention::Texture2D:
		return D3D11_RTV_DIMENSION_TEXTURE2D;

	case Dimention::MaltisampleTexture2D:
		return D3D11_RTV_DIMENSION_TEXTURE2DMS;

	default:
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't convert format.[_dimention is unknown]");
	}
}

/// <summary>
/// テクスチャディメンションをD3D11デプスステンシルビューディメンションに変換
/// </summary>
/// <param name="_dimention">テクスチャディメンション</param>
/// <returns>D3D11デプスステンシルビューディメンション</returns>
D3D11_DSV_DIMENSION Texture::ConvertDimentionToD3D11DSVDimention(Dimention _dimention){
	switch(_dimention){
	case Dimention::Texture2D:
		return D3D11_DSV_DIMENSION_TEXTURE2D;

	case Dimention::MaltisampleTexture2D:
		return D3D11_DSV_DIMENSION_TEXTURE2DMS;

	default:
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't convert format.[_dimention is unknown]");
	}
}


/// <summary>
/// デフォルトサンプル設定データの取得
/// </summary>
/// <param name="_sampleDescCount">サンプル設定カウント格納先</param>
/// <param name="_sampleDescQuality">サンプル設定クォリティ格納先</param>
/// <param name="_dimention">ディメンション格納先</param>
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
/// マルチサンプル・アンチエイリアシング設定データの取得
/// </summary>
/// <param name="_sampleDescCount">サンプル設定カウント格納先</param>
/// <param name="_sampleDescQuality">サンプル設定クォリティ格納先</param>
/// <param name="_dimention">ディメンション格納先</param>
/// <param name="_device">デバイス</param>
void Texture::GetMultisampleAntiAliasingDescData(
	unsigned int* const _sampleDescCount,
	unsigned int* const _sampleDescQuality,
	Dimention* const _dimention,
	ID3D11Device* const _device
){
	// 最高品質を探す
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
	// できなければログを残す
	::Ishikawa::Common::Debug::Log("Couldn't get multisample anti-aliasing desc data.\n");
}
