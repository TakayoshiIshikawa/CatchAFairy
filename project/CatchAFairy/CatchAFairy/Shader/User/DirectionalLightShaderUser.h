//=============================================================================
//	DirectionalLightShaderUser.h
//
//	平行ライトシェーダユーザのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __DIRECTIONAL_LIGHT_SHADER_USER_H__
#define __DIRECTIONAL_LIGHT_SHADER_USER_H__

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
			/// 平行ライトシェーダユーザ
			/// </summary>
			class DirectionalLight final : public Base{
			public:
				/// <summary>
				/// 頂点シェーダ入力
				/// </summary>
				struct VSInput{
					/// <summary>位置</summary>
					::Ishikawa::Math::Float3 position;
					/// <summary>法線</summary>
					::Ishikawa::Math::Float3 normal;
					/// <summary>テクスチャコーディネート</summary>
					::Ishikawa::Math::Float2 textureCoordinate;
				};
				/// <summary>
				/// 定数バッファ
				/// </summary>
				struct ConstantBuffer{
					/// <summary>ローカル空間からプロジェクション空間に変換する行列</summary>
					::Ishikawa::Math::Matrix modelToProjectionMatrix;
				};
				/// <summary>
				/// 描画利用リソース
				/// </summary>
				struct RenderUseResource{
					/// <summary>描画判定中心位置</summary>
					::Ishikawa::Math::Float3 checkCenterPosition;
					/// <summary>描画判定二乗半径</summary>
					float checkSquareRadius;
					/// <summary>ローカル空間からプロジェクション空間に変換する判定用行列</summary>
					::Ishikawa::Math::Matrix checkModelToProjectionMatrix;
					/// <summary>定数バッファ</summary>
					ConstantBuffer constantBuffer;
					/// <summary>入力トポロジー</summary>
					VertexTopology topology;
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
							/// <summary>拡散反射シェーダリソースビュー</summary>
							ID3D11ShaderResourceView* diffuseShaderResourceView;
						};
						/// <summary>シェーダリソース配列</summary>
						ID3D11ShaderResourceView* shaderResources[SHADER_RESOURCE_COUNT];
					};
#pragma warning(default:4201)
				};
				/// <summary>
				/// 複数描画利用リソース
				/// </summary>
				struct MultipleRenderUseResource{
					/// <summary>描画判定中心位置</summary>
					::Ishikawa::Math::Float3 checkCenterPosition;
					/// <summary>描画判定二乗半径</summary>
					float checkSquareRadius;
					/// <summary>ワールド変換行列数</summary>
					unsigned int worldMatrixCount;
					/// <summary>ワールド変換行列配列</summary>
					const ::Ishikawa::Math::Matrix* worldMatrixArray;
					/// <summary>ビュー変換行列</summary>
					::Ishikawa::Math::Matrix viewMatrix;
					/// <summary>プロジェクション変換行列</summary>
					::Ishikawa::Math::Matrix projectionMatrix;
					/// <summary>入力トポロジー</summary>
					VertexTopology topology;
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
							/// <summary>拡散反射シェーダリソースビュー</summary>
							ID3D11ShaderResourceView* diffuseShaderResourceView;
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
							/// <summary>座標レンダーターゲットビュー</summary>
							ID3D11RenderTargetView* positionRenderTargetView;
						};
						/// <summary>レンダーターゲット配列</summary>
						ID3D11RenderTargetView* renderTargets[RENDER_TARGET_COUNT];
					};
#pragma warning(default:4201)
					/// <summary>デプスステンシルビュー</summary>
					ID3D11DepthStencilView* depthStencilView;
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


			public:
				// デフォルトコンストラクタ
				DirectionalLight();
				// デストラクタ
				virtual ~DirectionalLight() override final;

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
				// 頂点バッファ作成
				// [利用し終えたバッファは開放してください]
				ID3D11Buffer* CreateVertexBuffer(const VSInput* const _vertexArray, unsigned int _vertexCount) const;
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
				// 複数描画
				void DrawMultiple(const MultipleRenderUseResource& _inputResource);
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
