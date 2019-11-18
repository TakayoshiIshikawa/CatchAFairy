//=============================================================================
//	ITTextureData.h
//
//	自作テクスチャデータのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_TEXTURE_DATA_H__
#define __IT_TEXTURE_DATA_H__

struct ID3D11Device;
#pragma warning(disable:4471)
enum D3D11_BIND_FLAG;
enum DXGI_FORMAT;
enum D3D_SRV_DIMENSION;
enum D3D11_RTV_DIMENSION;
enum D3D11_DSV_DIMENSION;
#pragma warning(default:4471)

#include "Ishikawa/DirectX/Device/DependentResource/ITDeviceDependentResourceBase.h"


namespace Ishikawa{
	namespace DirectX{
		namespace Texture{
			/// <summary>
			/// テクスチャバインド可能フラグ
			/// </summary>
			enum class BindableFlag : int{
				/// <summary>シェーダリソースのみ</summary>
				ShaderResource,
				/// <summary>レンダーターゲットのみ</summary>
				RenderTarget,
				/// <summary>デプスステンシルのみ</summary>
				DepthStencil,
				/// <summary>シェーダリソースとレンダーターゲット</summary>
				SR_RT,
				/// <summary>シェーダリソースとデプスステンシル</summary>
				SR_DS
			};

			// テクスチャバインド可能フラグをD3D11バインドフラグに対応するuint値に変換
			unsigned int ConvertBindableFlagToUInt(BindableFlag _bindableflag);


			/// <summary>
			/// テクスチャバインドフラグ
			/// </summary>
			enum class BindFlag : int{
				/// <summary>バインドなし</summary>
				NotBind = 0x0000,
				/// <summary>シェーダリソース</summary>
				ShaderResource = 0x0001,
				/// <summary>レンダーターゲット</summary>
				RenderTarget = 0x0002,
				/// <summary>デプスステンシル</summary>
				DepthStencil = 0x0004
			};

			// テクスチャバインドフラグをD3D11バインドフラグに変換
			D3D11_BIND_FLAG ConvertBindFlagToD3D11BindFlag(BindFlag _bindflag);


			/// <summary>
			/// テクスチャフォーマット
			/// </summary>
			enum class Format : int{
				/// <summary>不明なフォーマット</summary>
				Unknown,
				/// <summary>R8G8B8A8符号なし整数</summary>
				R8G8B8A8UNorm,
				/// <summary>R10G10B10A2符号なし整数</summary>
				R10G10B10A2UNorm,
				/// <summary>R32G32B32A32実数</summary>
				R32G32B32A32Float,
				/// <summary>R32実数</summary>
				R32Float,
				/// <summary>D24符号なし整数 S8符号なし整数</summary>
				D24UNormS8UInt
			};

			// テクスチャフォーマットをDXGIフォーマットに変換
			DXGI_FORMAT ConvertFormatToDxgiFormat(Format _format);


			/// <summary>
			/// テクスチャディメンション
			/// </summary>
			enum class Dimention : int{
				/// <summary>2Dテクスチャ</summary>
				Texture2D,
				/// <summary>マルチサンプル2Dテクスチャ</summary>
				MaltisampleTexture2D
			};

			// テクスチャディメンションをD3Dシェーダリソースビューディメンションに変換
			D3D_SRV_DIMENSION ConvertDimentionToD3D11SRVDimention(Dimention _dimention);
			// テクスチャディメンションをD3D11レンダーターゲットビューディメンションに変換
			D3D11_RTV_DIMENSION ConvertDimentionToD3D11RTVDimention(Dimention _dimention);
			// テクスチャディメンションをD3D11デプスステンシルビューディメンションに変換
			D3D11_DSV_DIMENSION ConvertDimentionToD3D11DSVDimention(Dimention _dimention);


			// デフォルトサンプル設定データの取得
			void GetDefaultSampleDescData(
				unsigned int* const _sampleDescCount,
				unsigned int* const _sampleDescQuality,
				Dimention* const _dimention
			);
			// マルチサンプル・アンチエイリアシング設定データの取得
			void GetMultisampleAntiAliasingDescData(
				unsigned int* const _sampleDescCount,
				unsigned int* const _sampleDescQuality,
				Dimention* const _dimention,
				ID3D11Device* const _device
			);
		}
	}
}

#endif
