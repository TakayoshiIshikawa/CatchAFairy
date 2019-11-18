//=============================================================================
//	ITTexture2D.h
//
//	自作2Dテクスチャのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_TEXTURE_2D_H__
#define __IT_TEXTURE_2D_H__

struct ID3D11Device;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;
#pragma warning(disable:4471)
enum D3D11_BIND_FLAG;
enum DXGI_FORMAT;
#pragma warning(default:4471)

#include "Ishikawa/DirectX/Device/DependentResource/ITDeviceDependentResourceBase.h"
#include "ITTexture2DInterface.h"


namespace Ishikawa{
	namespace DirectX{
		namespace Texture{
			enum class BindableFlag : int;
			enum class BindFlag : int;
			enum class Format : int;
			enum class Dimention : int;


			/// <summary>
			/// 自作2Dテクスチャ
			/// </summary>
			class Texture2D final :
				public ::Ishikawa::DirectX::Device::DependentResource::Base,
				public Texture2DInterface
			{
			private:
				/// <summary>バインド可能フラグ</summary>
				BindableFlag bindableFlag;
				/// <summary>現在のバインドフラグ</summary>
				BindFlag currentBindFlag;
				/// <summary>幅</summary>
				unsigned int width;
				/// <summary>高さ</summary>
				unsigned int height;
				/// <summary>フォーマット</summary>
				Format format;
				/// <summary>CPUアクセスフラグ</summary>
				unsigned int cpuAccessFlag;
				/// <summary>サンプル設定カウント</summary>
				unsigned int sampleDescCount;
				/// <summary>サンプル設定クォリティ</summary>
				unsigned int sampleDescQuality;
				/// <summary>ビューディメンション</summary>
				Dimention dimention;
				/// <summary>テクスチャ</summary>
				ID3D11Texture2D* texture;
				/// <summary>シェーダリソースビュー</summary>
				ID3D11ShaderResourceView* shaderResourceView;
				/// <summary>レンダーターゲットビュー</summary>
				ID3D11RenderTargetView* renderTargetView;
				/// <summary>デプスステンシルビュー</summary>
				ID3D11DepthStencilView* depthStencilView;


			public:
				// デフォルトコンストラクタ
				Texture2D();
				// デストラクタ
				virtual ~Texture2D() override final;

			public:
				/// <summary>テクスチャサイズの取得</summary>
				/// <param name="_width">幅の格納先</param>
				/// <param name="_height">高さの格納先</param>
				virtual void GetTextureSize(unsigned int* const _width, unsigned int* const _height) const override final{
					if(_width != nullptr) (*_width) = this->width;
					if(_height != nullptr) (*_height) = this->height;
				}
				// シェーダリソースビューの取得
				ID3D11ShaderResourceView* GetShaderResourceView() const;
				// レンダーターゲットビューの取得
				ID3D11RenderTargetView* GetRenderTargetView() const;
				// デプスステンシルビューの取得
				ID3D11DepthStencilView* GetDepthStencilView() const;

			private:
				// バインド可能か?
				bool IsBindable(BindFlag _bindFlag) const;
				// デバイスの取得
				ID3D11Device* GetDevice() const;

			public:
				// デバイスに読み込まれた
				virtual void OnDeviceLoaded() override final;
				// デバイスが復元された
				virtual void OnDeviceRestored() override final;
				// デバイスが破棄された
				virtual void OnDeviceLost() override final;

			public:
				// 初期化
				void Initialize();
				// 作成
				void Create(
					BindableFlag _bindableFlag,
					unsigned int _width,
					unsigned int _height,
					Format _format,
					unsigned int _cpuAccessFlag,
					unsigned int _sampleDescCount,
					unsigned int _sampleDescQuality,
					Dimention _dimention
				);
				// サイズ変更
				void ChangeSize(unsigned int _width, unsigned int _height);

			private:
				// テクスチャの作成
				void CreateTexture();
				// シェーダリソースビューの作成
				void CreateShaderResourceView();
				// レンダーターゲットビューの作成
				void CreateRenderTargetView();
				// デプスステンシルビューの作成
				void CreateDepthStencilView();

				// 全リソースの開放
				void ReleaseAll();
				// テクスチャの開放
				void ReleaseTexture();
				// 現在利用中のビューを開放
				void ReleaseCurrentView();
				// シェーダリソースビューの開放
				void ReleaseShaderResourceView();
				// レンダーターゲットビューの開放
				void ReleaseRenderTargetView();
				// デプスステンシルビューの開放
				void ReleaseDepthStencilView();
			};
		}
	}
}

#endif
