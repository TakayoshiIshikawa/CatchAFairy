//=============================================================================
//	DirectionalLight.h
//
//	平行ライトのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __DIRECTIONAL_LIGHT_H__
#define __DIRECTIONAL_LIGHT_H__

#include "CatchAFairy/Object/CameraObjectInterface.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "Ishikawa/Math/ITMatrix.h"
#include "Ishikawa/DirectX/Texture/ITTexture2D.h"
#include "CatchAFairy/Shader/User/DirectionalLightShaderUser.h"


namespace CatchAFairy{
	namespace Object{
		class RenderInterface;
		class RenderSameInterface;


		namespace PlayScene{
			/// <summary>
			/// 平行ライト
			/// </summary>
			class DirectionalLight final : public ::CatchAFairy::Object::CameraInterface{
			private:
				// テクスチャサイズ
				static const unsigned int TEXTURE_SIZE;
				// 注視点からの距離
				static const float DISTANCE;
				// Y軸の回転角[方角を決定]
				static const float ROTATE_Y;
				// X軸の回転角[高さを決定]
				static const float ROTATE_X;
				// 表示サイズ
				static const float VIEW_SIZE;
				// ニアクリップ
				static const float NEAR_CLIP;
				// ファークリップ
				static const float FAR_CLIP;
				// デフォルト色
				static const ::Ishikawa::Math::Float4 DEFAULT_COLOR;

			private:
				/// <summary>レンダーターゲット画像</summary>
				::Ishikawa::DirectX::Texture::Texture2D renderTargetTexture;
				/// <summary>デプスステンシル画像</summary>
				::Ishikawa::DirectX::Texture::Texture2D depthStencilTexture;
				/// <summary>平行ライトシェーダユーザ</summary>
				::CatchAFairy::Shader::User::DirectionalLight directionalLightShaderUser;

				/// <summary>ローカル変換行列</summary>
				::Ishikawa::Math::Matrix local;
				/// <summary>ワールド変換行列</summary>
				::Ishikawa::Math::Matrix world;
				/// <summary>ビュー変換行列</summary>
				::Ishikawa::Math::Matrix view;
				/// <summary>プロジェクション変換行列</summary>
				::Ishikawa::Math::Matrix projection;
				/// <summary>色</summary>
				::Ishikawa::Math::Float4 color;


			public:
				// デフォルトコンストラクタ
				DirectionalLight();
				// デストラクタ
				virtual ~DirectionalLight() override final;

			public:
				/// <summary>ワールド変換行列の取得</summary>
				/// <returns>ワールド変換行列</returns>
				virtual const ::Ishikawa::Math::Matrix& GetWorld() const override final{
					return this->world;
				}
				/// <summary>ビュー変換行列の取得</summary>
				/// <returns>ビュー変換行列</returns>
				virtual const ::Ishikawa::Math::Matrix& GetView() const override final{
					return this->view;
				}
				/// <summary>プロジェクション変換行列の取得</summary>
				/// <returns>プロジェクション変換行列</returns>
				virtual const ::Ishikawa::Math::Matrix& GetProjection() const override final{
					return this->projection;
				}
				/// <summary>方向の取得</summary>
				/// <returns>方向</returns>
				const ::Ishikawa::Math::Float4& GetDirection() const { return this->local.row3; }
				/// <summary>色の取得</summary>
				/// <returns>色</returns>
				const ::Ishikawa::Math::Float4& GetColor() const { return this->color; }
				/// <summary>最終シェーダリソースビューの取得</summary>
				/// <returns>最終シェーダリソースビュー</returns>
				virtual ID3D11ShaderResourceView* GetLastShaderResourceView() const override final{
					return this->renderTargetTexture.GetShaderResourceView();
				}

			public:
				/// <summary>注視点の設定</summary>
				/// <param name="_position">注視点座標</param>
				void SetLookPosition(const ::Ishikawa::Math::Float3& _position){
					this->SetLookPosition(_position.x, _position.y, _position.z);
				}
				// 注視点の設定
				void SetLookPosition(float _x, float _y, float _z);
				/// <summary>色の設定</summary>
				/// <param name="_color">色</param>
				void SetColor(const ::Ishikawa::Math::Float4& _color){ this->color = _color; }
				/// <summary>色の設定</summary>
				/// <param name="_red">赤成分</param>
				/// <param name="_green">緑成分</param>
				/// <param name="_blue">青成分</param>
				/// <param name="_alpha">非透明成分</param>
				void SetColor(float _red, float _green, float _blue, float _alpha){
					this->SetColor(::Ishikawa::Math::Float4(_red, _green, _blue, _alpha));
				}
				/// <summary>色の設定</summary>
				/// <param name="_red">赤成分</param>
				/// <param name="_green">緑成分</param>
				/// <param name="_blue">青成分</param>
				void SetColor(float _red, float _green, float _blue){
					this->SetColor(::Ishikawa::Math::Float4(_red, _green, _blue, 1.0f));
				}

			public:
				// ゲームのデバイスに依存するリソースの追加
				virtual void AddITDeviceDependentResources(::Ishikawa::DirectX::Device::Device* const _itDevice) override final;
				// デバイス依存リソース作成
				virtual void CreateDeviceDependentResources() override final;
				// デバイス依存リソース開放
				virtual void ReleaseDeviceDependentResources() override final;

				// 初期化
				void Initialize();

				// 平行ライトシェーダ描画前処理
				void DirectionalLightShaderDrawBefore();
				// 平行ライトシェーダでオブジェクトを描画
				void RenderObjectToDirectionalLightShader(
					const ::CatchAFairy::Object::RenderInterface& _renderObject
				);
				// 平行ライトシェーダで複数オブジェクトを描画
				void RenderSameObjectToDirectionalLightShader(
					const ::CatchAFairy::Object::RenderSameInterface& _renderObjects
				);
				// 平行ライトシェーダ描画後処理
				void DirectionalLightShaderDrawAfter();

			private:
				// テクスチャ作成
				void CreateTextures();
			};
		}
	}
}

#endif
