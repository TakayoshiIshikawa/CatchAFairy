//=============================================================================
//	CameraEyePointLightShaderUser.h
//
//	カメラ視点ポイントライトシェーダユーザのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __CAMERA_EYE_POINT_LIGHT_SHADER_USER_H__
#define __CAMERA_EYE_POINT_LIGHT_SHADER_USER_H__

struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;
#include "ShaderUserBase.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "Ishikawa/Math/ITMatrix.h"
namespace Ishikawa{
	namespace DirectX{
		struct Viewport;
	}
}

namespace CatchAFairy{
	namespace Shader{
		namespace User{
			enum class VertexTopology : int;


			/// <summary>
			/// カメラ視点ポイントライトシェーダユーザ
			/// </summary>
			class CameraEyePointLight final : public Base{
			public:
				/// <summary>
				/// 頂点シェーダ入力
				/// </summary>
				struct VSInput{
					/// <summary>位置</summary>
					::Ishikawa::Math::Float3 position;
					/// <summary>色</summary>
					::Ishikawa::Math::Float4 color;
					/// <summary>半径と明るさ</summary>
					::Ishikawa::Math::Float2 radiusAndLumen;
				};
				/// <summary>
				/// 定数バッファ
				/// </summary>
				struct ConstantBuffer{
					/// <summary>ローカル空間からプロジェクション空間に変換する行列</summary>
					::Ishikawa::Math::Matrix localToProjectionMatrix;
					/// <summary>ローカル空間からビュー空間に変換する行列</summary>
					::Ishikawa::Math::Matrix localToViewMatrix;
					/// <summary>プロジェクション変換行列</summary>
					::Ishikawa::Math::Matrix projectionMatrix;
					/// <summary>プロジェクション空間の逆行列</summary>
					::Ishikawa::Math::Matrix invertProjectionMatrix;
				};
				/// <summary>
				/// 描画利用リソース
				/// </summary>
				struct RenderUseResource{
					/// <summary>定数バッファ</summary>
					ConstantBuffer constantBuffer;
					/// <summary>頂点バッファ</summary>
					ID3D11Buffer* vertexBuffer;
					/// <summary>インデックス配列</summary>
					ID3D11Buffer* indexBuffer;
					/// <summary>インデックス数</summary>
					unsigned int indexCount;
					/// <summary>シェーダリソース数</summary>
					static const unsigned int SHADER_RESOURCE_COUNT = 1U;
#pragma warning(disable:4201)
					union{
						struct{
							/// <summary>プロジェクション座標シェーダリソースビュー</summary>
							ID3D11ShaderResourceView* positionResourceView;
						};
						/// <summary>シェーダリソース配列</summary>
						ID3D11ShaderResourceView* shaderResources[SHADER_RESOURCE_COUNT];
					};
#pragma warning(default:4201)
				};
				/// <summary>
				/// 出力リソース
				/// </summary>
				struct OutputResource{
					/// <summary>レンダーターゲット数</summary>
					static const unsigned int RENDER_TARGET_COUNT = 1U;
#pragma warning(disable:4201)
					union{
						struct{
							/// <summary>ポイントライトレンダーターゲットビュー</summary>
							ID3D11RenderTargetView* pointLightRenderTargetView;
						};
						/// <summary>レンダーターゲット配列</summary>
						ID3D11RenderTargetView* renderTargets[RENDER_TARGET_COUNT];
					};
#pragma warning(default:4201)
					/// <summary>カメラ視点1番シェーダのデプスステンシルビュー</summary>
					ID3D11DepthStencilView* firstDepthStencilView;
					/// <summary>ビューポート</summary>
					::Ishikawa::DirectX::Viewport* viewport;
				};


			private:
				/// <summary>入力レイアウト数</summary>
				static const unsigned int INPUT_LAYOUT_COUNT = 3U;
				// 入力レイアウト設定
				static const D3D11_INPUT_ELEMENT_DESC INPUT_LAYOUT_DESC[INPUT_LAYOUT_COUNT];
				// 頂点シェーダファイルパス
				static const char* const VERTEX_SHADER_FILEPATH;
				// ジオメトリシェーダファイルパス
				static const char* const GEOMETRY_SHADER_FILEPATH;
				// ピクセルシェーダファイルパス
				static const char* const PIXEL_SHADER_FILEPATH;
				/// <summary>定数バッファ数</summary>
				static const unsigned int CONSTANT_BUFFER_COUNT = 1U;

			private:
				/// <summary>定数バッファ配列</summary>
				ID3D11Buffer* constantBufferArray[CONSTANT_BUFFER_COUNT];
				/// <summary>入力トポロジー</summary>
				VertexTopology topology;


			public:
				// デフォルトコンストラクタ
				CameraEyePointLight();
				// デストラクタ
				virtual ~CameraEyePointLight() override final;

			protected:
				/// <summary>入力レイアウト数の取得</summary>
				/// <returns>入力レイアウト数</returns>
				virtual unsigned int GetInputLayoutCount() const override final{
					return INPUT_LAYOUT_COUNT;
				}
				/// <summary> 入力レイアウト設定の取得</summary>
				/// <returns> 入力レイアウト設定</returns>
				virtual const D3D11_INPUT_ELEMENT_DESC* GetInputLayoutDesc() const override final{
					return INPUT_LAYOUT_DESC;
				}
				/// <summary>頂点シェーダファイルパスの取得</summary>
				/// <returns>頂点シェーダファイルパス</returns>
				virtual const char* GetVertexShaderFilepath() const override final{
					return VERTEX_SHADER_FILEPATH;
				}
				/// <summary>ジオメトリシェーダファイルパスの取得</summary>
				/// <returns>ジオメトリシェーダファイルパス</returns>
				virtual const char* GetGeometryShaderFilepath() const override final{
					return GEOMETRY_SHADER_FILEPATH;
				}
				/// <summary>ピクセルシェーダファイルパスの取得</summary>
				/// <returns>ピクセルシェーダファイルパス</returns>
				virtual const char* GetPixelShaderFilepath() const override final{
					return PIXEL_SHADER_FILEPATH;
				}

			public:
				// デバイスに読み込まれた
				virtual void OnDeviceLoaded() override final;

			public:
				// 頂点バッファ作成
				// [利用し終えたバッファは開放してください]
				ID3D11Buffer* CreateVertexBuffer(const VSInput* const _vertexArray, unsigned int _vertexCount);
				// サンプラステートの作成
				virtual void CreateSamplerState(ID3D11Device* const _device) override final;
				// ブレンドステートの作成
				virtual void CreateBlendState(ID3D11Device* const _device) override final;
				// デプスステンシルステートの作成
				virtual void CreateDepthStencilState(ID3D11Device* const _device) override final;
				// ラスタライザステートの作成
				virtual void CreateRasterizerState(ID3D11Device* const _device) override final;

				// 描画前処理
				void DrawBefore(const OutputResource& _outputResource, const float (&_color)[4]);
				// 描画
				void Draw(const RenderUseResource& _inputResource);
				// 描画後処理
				void DrawAfter();

			private:
				// 定数バッファ配列作成
				void CreateConstantBufferArray();
				// 定数バッファ配列開放
				void ReleaseConstantBufferArray();
			};
		}
	}
}

#endif
