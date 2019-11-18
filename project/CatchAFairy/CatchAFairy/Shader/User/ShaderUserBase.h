//=============================================================================
//	ShaderUserBase.h
//
//	シェーダユーザ基底のヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SHADER_USER_BASE_H__
#define __SHADER_USER_BASE_H__

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11GeometryShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;
struct ID3D11SamplerState;
struct ID3D11BlendState;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct D3D11_INPUT_ELEMENT_DESC;
#include "Ishikawa/DirectX/Device/DependentResource/ITDeviceDependentResourceBase.h"


namespace CatchAFairy{
	namespace Shader{
		namespace User{
			/// <summary>
			/// シェーダユーザ基底
			/// </summary>
			class Base : public ::Ishikawa::DirectX::Device::DependentResource::Base{
			private:
				/// <summary>入力レイアウト</summary>
				ID3D11InputLayout* inputLayout;
				/// <summary>頂点シェーダ</summary>
				ID3D11VertexShader* vertexShader;
				/// <summary>ジオメトリシェーダ</summary>
				ID3D11GeometryShader* geometryShader;
				/// <summary>ピクセルシェーダ</summary>
				ID3D11PixelShader* pixelShader;
				/// <summary>サンプラステート</summary>
				ID3D11SamplerState* samplerState;
				/// <summary>ブレンドステート</summary>
				ID3D11BlendState* blendState;
				/// <summary>デプスステンシルステート</summary>
				ID3D11DepthStencilState* depthStencilState;
				/// <summary>ラスタライザステート</summary>
				ID3D11RasterizerState* rasterizerState;


			private:
				// ファイルのコードとその長さの取得
				static void GetFileCodeAndLength(
					char** const _code,
					unsigned long* const _length,
					const char* const _filepath
				);


			public:
				// デフォルトコンストラクタ
				Base();
				// デストラクタ
				virtual ~Base() override;

			public:
				/// <summary>入力レイアウトの取得</summary>
				/// <returns>入力レイアウト</returns>
				virtual ID3D11InputLayout* GetInputLayout() const final { return this->inputLayout; }
				/// <summary>頂点シェーダの取得</summary>
				/// <returns>頂点シェーダ</returns>
				virtual ID3D11VertexShader* GetVertexShader() const final { return this->vertexShader; }
				/// <summary>ジオメトリシェーダの取得</summary>
				/// <returns>ジオメトリシェーダ</returns>
				virtual ID3D11GeometryShader* GetGeometryShader() const final { return this->geometryShader; }
				/// <summary>ピクセルシェーダの取得</summary>
				/// <returns>ピクセルシェーダ</returns>
				virtual ID3D11PixelShader* GetPixelShader() const final { return this->pixelShader; }
				/// <summary>サンプラステートの取得</summary>
				/// <returns>サンプラステート</returns>
				virtual ID3D11SamplerState* GetSamplerState() const final { return this->samplerState; }
				/// <summary>ブレンドステートの取得</summary>
				/// <returns>ブレンドステート</returns>
				virtual ID3D11BlendState* GetBlendState() const final { return this->blendState; }
				/// <summary>デプスステンシルステートの取得</summary>
				/// <returns>デプスステンシルステート</returns>
				virtual ID3D11DepthStencilState* GetDepthStencilState() const final { return this->depthStencilState; }
				/// <summary>ラスタライザステートの取得</summary>
				/// <returns>ラスタライザステート</returns>
				virtual ID3D11RasterizerState* GetRasterizerState() const final { return this->rasterizerState; }

			protected:
				// 入力レイアウト数の取得
				virtual unsigned int GetInputLayoutCount() const = 0;
				// 入力レイアウト設定の取得
				virtual const D3D11_INPUT_ELEMENT_DESC* GetInputLayoutDesc() const = 0;
				// 頂点シェーダファイルパスの取得
				virtual const char* GetVertexShaderFilepath() const = 0;
				// ジオメトリシェーダファイルパスの取得
				virtual const char* GetGeometryShaderFilepath() const = 0;
				// ピクセルシェーダファイルパスの取得
				virtual const char* GetPixelShaderFilepath() const = 0;

				// デバイスの取得
				virtual ID3D11Device* GetDevice() const final;
				// コンテキストの取得
				virtual ID3D11DeviceContext* GetContext() const final;

			protected:
				/// <summary>サンプラステートの設定</summary>
				/// <param name="_samplerState">サンプラステート</param>
				virtual void SetSamplerState(ID3D11SamplerState* const _samplerState) final{
					this->samplerState = _samplerState;
				}
				/// <summary>ブレンドステートの設定</summary>
				/// <param name="_blendState">ブレンドステート</param>
				virtual void SetBlendState(ID3D11BlendState* const _blendState) final{
					this->blendState = _blendState;
				}
				/// <summary>デプスステンシルステートの設定</summary>
				/// <param name="_depthStencilState">デプスステンシルステート</param>
				virtual void SetDepthStencilState(ID3D11DepthStencilState* const _depthStencilState) final{
					this->depthStencilState = _depthStencilState;
				}
				/// <summary>ラスタライザステートの設定</summary>
				/// <param name="_rasterizerState">ラスタライザステート</param>
				virtual void SetRasterizerState(ID3D11RasterizerState* const _rasterizerState) final{
					this->rasterizerState = _rasterizerState;
				}

			public:
				// デバイスに読み込まれた
				virtual void OnDeviceLoaded() override;
				// デバイスが復元された
				virtual void OnDeviceRestored() override;
				// デバイスが破棄された
				virtual void OnDeviceLost() override;

			public:
				// 初期化
				virtual void Initialize() final;
				// サンプラステートの作成
				virtual void CreateSamplerState(ID3D11Device* const _device) = 0;
				// ブレンドステートの作成
				virtual void CreateBlendState(ID3D11Device* const _device) = 0;
				// デプスステンシルステートの作成
				virtual void CreateDepthStencilState(ID3D11Device* const _device) = 0;
				// ラスタライザステートの作成
				virtual void CreateRasterizerState(ID3D11Device* const _device) = 0;
				// インデックスバッファ作成
				// [利用し終えたバッファは開放してください]
				virtual ID3D11Buffer* CreateIndexBuffer(
					const unsigned long* const _indexArray,
					unsigned int _indexCount
				) const final;
				// バッファ開放
				virtual void ReleaseBuffer(ID3D11Buffer* const _buffer) const final;
				// 作成
				virtual void Create() final;

			private:
				// 頂点シェーダと入力レイアウトの作成
				virtual void CreateVertexShaderAndInputLayout(ID3D11Device* const _device) final;
				// ジオメトリシェーダの作成
				virtual void CreateGeometryShader(ID3D11Device* const _device) final;
				// ピクセルシェーダの作成
				virtual void CreatePixelShader(ID3D11Device* const _device) final;

				// 全てのリソースの開放
				virtual void ReleaseAll() final;
				// 入力レイアウトの開放
				virtual void ReleaseInputLayout() final;
				// 頂点シェーダの開放
				virtual void ReleaseVertexShader() final;
				// ジオメトリシェーダの開放
				virtual void ReleaseGeometryShader() final;
				// ピクセルシェーダの開放
				virtual void ReleasePixelShader() final;
				// サンプラステートの開放
				virtual void ReleaseSamplerState() final;
				// ブレンドステートの開放
				virtual void ReleaseBlendState() final;
				// デプスステンシルステートの開放
				virtual void ReleaseDepthStencilState() final;
				// ラスタライザステートの開放
				virtual void ReleaseRasterizerState() final;
			};
		}
	}
}

#endif
