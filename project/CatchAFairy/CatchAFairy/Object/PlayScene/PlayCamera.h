//=============================================================================
//	PlayCamera.h
//
//	プレイカメラのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __PLAY_CAMERA_H__
#define __PLAY_CAMERA_H__

#include "CatchAFairy/Object/UpdateObjectInterface.h"
#include "CatchAFairy/Object/CameraObjectInterface.h"
#include "Ishikawa/Math/ITMatrix.h"
#include "Ishikawa/DirectX/Texture/ITTexture2D.h"
#include "CatchAFairy/Shader/User/PlayCameraEyeFirstShaderUser.h"
#include "CatchAFairy/Shader/User/PlayCameraEyeLastShaderUser.h"
#include "CatchAFairy/Shader/User/CameraEyeShadowShaderUser.h"
#include "CatchAFairy/Shader/User/CameraEyeReflectionLightShaderUser.h"
#include "CatchAFairy/Shader/User/CameraEyePointLightShaderUser.h"


namespace CatchAFairy{
	namespace Object{
		class RenderInterface;
		class RenderSameInterface;
		class PointLightInterface;

		namespace PlayScene{
			class DirectionalLight;


			/// <summary>
			/// プレイカメラ
			/// </summary>
			class PlayCamera final :
				public ::CatchAFairy::Object::UpdateInterface,
				public ::CatchAFairy::Object::CameraInterface
			{
			private:
				// 最終インデックス数
				static const unsigned int LAST_INDEX_COUNT;
				// 追従比率
				static const float FOLLOW_RATE;
				// 視野角
				static const float VIEW_ANGLE;
				// アスペクト比
				static const float ASPECT_RATE;
				// ニアクリップ
				static const float NEAR_CLIP;
				// ファークリップ
				static const float FAR_CLIP;
				// クリップ中央
				static const float CLIP_CENTER;

			private:
				/// <summary>拡散反射レンダーターゲットテクスチャ</summary>
				::Ishikawa::DirectX::Texture::Texture2D diffuseRenderTargetTexture;
				/// <summary>法線レンダーターゲットテクスチャ</summary>
				::Ishikawa::DirectX::Texture::Texture2D normalRenderTargetTexture;
				/// <summary>座標レンダーターゲットテクスチャ</summary>
				::Ishikawa::DirectX::Texture::Texture2D positionRenderTargetTexture;
				/// <summary>1番デプスステンシルテクスチャ</summary>
				::Ishikawa::DirectX::Texture::Texture2D firstDepthStencilTexture;
				/// <summary>1番シェーダユーザ</summary>
				::CatchAFairy::Shader::User::PlayCameraEyeFirst firstShaderUser;

				/// <summary>出力ターゲットテクスチャ</summary>
				::Ishikawa::DirectX::Texture::Texture2D* outputTargetTexture;
				/// <summary>最終シェーダユーザ</summary>
				::CatchAFairy::Shader::User::PlayCameraEyeLast lastShaderUser;
				/// <summary>最終頂点バッファ</summary>
				ID3D11Buffer* lastVertexBuffer;
				/// <summary>最終インデックスバッファ</summary>
				ID3D11Buffer* lastIndexBuffer;

				/// <summary>影レンダーターゲットテクスチャ</summary>
				::Ishikawa::DirectX::Texture::Texture2D shadowRenderTargetTexture;
				/// <summary>影シェーダユーザ</summary>
				::CatchAFairy::Shader::User::CameraEyeShadow shadowShaderUser;

				/// <summary>反射光レンダーターゲットテクスチャ</summary>
				::Ishikawa::DirectX::Texture::Texture2D reflectionLightRenderTargetTexture;
				/// <summary>反射光デプスステンシルテクスチャ</summary>
				::Ishikawa::DirectX::Texture::Texture2D reflectionLightDepthStencilTexture;
				/// <summary>反射光シェーダユーザ</summary>
				::CatchAFairy::Shader::User::CameraEyeReflectionLight reflectionLightShaderUser;

				/// <summary>ポイントライトレンダーターゲットテクスチャ</summary>
				::Ishikawa::DirectX::Texture::Texture2D pointLightRenderTargetTexture;
				/// <summary>ポイントライトシェーダユーザ</summary>
				::CatchAFairy::Shader::User::CameraEyePointLight pointLightShaderUser;

				/// <summary>ローカル行列</summary>
				::Ishikawa::Math::Matrix local;
				/// <summary>ワールド行列</summary>
				::Ishikawa::Math::Matrix world;
				/// <summary>ビュー行列</summary>
				::Ishikawa::Math::Matrix view;
				/// <summary>目標ビュー行列</summary>
				::Ishikawa::Math::Matrix targetView;
				/// <summary>プロジェクション行列</summary>
				::Ishikawa::Math::Matrix projection;
				/// <summary>平行ライト</summary>
				DirectionalLight* directionalLight;


			public:
				// デフォルトコンストラクタ
				PlayCamera();
				// デストラクタ
				virtual ~PlayCamera() override final;

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
				/// <summary>最終シェーダリソースビューの取得</summary>
				/// <returns>最終シェーダリソースビュー</returns>
				virtual ID3D11ShaderResourceView* GetLastShaderResourceView() const override final{
					return (this->outputTargetTexture == nullptr) ? (nullptr) :
						this->outputTargetTexture->GetShaderResourceView();
				}
				/// <summary>1番シェーダユーザの取得</summary>
				/// <returns>1番シェーダユーザ</returns>
				::CatchAFairy::Shader::User::PlayCameraEyeFirst* GetFirstShaderUser(){
					return (&(this->firstShaderUser));
				}
				/// <summary>反射光シェーダユーザの取得</summary>
				/// <returns>反射光シェーダユーザ</returns>
				::CatchAFairy::Shader::User::CameraEyeReflectionLight* GetReflectionLightShaderUser(){
					return (&(this->reflectionLightShaderUser));
				}

			public:
				// 平行ライトの設定
				void SetDirectionalLight(DirectionalLight* const _directionalLight);
				/// <summary>出力ターゲットテクスチャの設定</summary>
				/// <param name="_outputTargetTexture">出力ターゲットテクスチャ</param>
				void SetOutputTargetTexture(::Ishikawa::DirectX::Texture::Texture2D* const _outputTargetTexture){
					this->outputTargetTexture = _outputTargetTexture;
				}

			public:
				// 追従オブジェクトの移動
				void OnMoveFollowingTheObject(const ::Ishikawa::Math::Matrix& _followMatrix);

			public:
				// ゲームのデバイスに依存するリソースの追加
				virtual void AddITDeviceDependentResources(::Ishikawa::DirectX::Device::Device* const _itDevice) override final;
				// デバイス依存リソース作成
				virtual void CreateDeviceDependentResources() override final;
				// デバイス依存リソース開放
				virtual void ReleaseDeviceDependentResources() override final;

				// 初期化
				void Initialize();
				// 更新
				virtual void Update(float _elapsedTime) override final;

				// 1番シェーダの描画前処理
				void DrawBeforeOfFirstShader();
				// 1番シェーダにオブジェクトを描画
				void RenderObjectToFirstShader(
					const ::CatchAFairy::Object::RenderInterface& _renderObject
				);
				// 1番シェーダに複数オブジェクトを描画
				void RenderSameObjectToFirstShader(
					const ::CatchAFairy::Object::RenderSameInterface& _renderObjects
				);
				// 1番シェーダの描画後処理
				void DrawAfterOfFirstShader();
				// 影シェーダ描画
				void RenderOfShadowShader();
				// 反射光シェーダの描画前処理
				void DrawBeforeOfReflectionLightShader();
				// 反射光シェーダ描画
				void RenderOfReflectionLightShader(const ::CatchAFairy::Object::PointLightInterface& _pointLightObject);
				// 反射光シェーダの描画後処理
				void DrawAfterOfReflectionLightShader();
				// ポイントライトシェーダの描画前処理
				void DrawBeforeOfPointLightShader();
				// ポイントライトシェーダ描画
				void RenderOfPointLightShader(const ::CatchAFairy::Object::PointLightInterface& _pointLightObject);
				// ポイントライトシェーダの描画後処理
				void DrawAfterOfPointLightShader();
				// 最終シェーダ描画
				void RenderOfLastShader();

			private:
				// 1番テクスチャ作成
				void CreateFirstTextures();
				// 影テクスチャ作成
				void CreateShadowTextures();
				// 反射光テクスチャ作成
				void CreateReflectionLightTextures();
				// ポイントライトテクスチャ作成
				void CreatePointLightTextures();

				// 最終頂点バッファ作成
				void CreateLastVertexBuffer();
				// 最終インデックスバッファ作成
				void CreateLastIndexBuffer();
				// 最終頂点バッファ開放
				void ReleaseLastVertexBuffer();
				// 最終インデックスバッファ開放
				void ReleaseLastIndexBuffer();

				// 1番リソース初期化
				void InitializeFirstResources();
				// 最終リソース初期化
				void InitializeLastResources();
				// 影リソース初期化
				void InitializeShadowResources();
				// 反射光リソース初期化
				void InitializeReflectionLightResources();
				// ポイントライトリソース初期化
				void InitializePointLightResources();
			};
		}
	}
}

#endif
