//=============================================================================
//	SceneLogo.h
//
//	ロゴシーンのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SCENE_LOGO_H__
#define __SCENE_LOGO_H__

struct ID3D11Buffer;

#include "SceneBase.h"
#include "Ishikawa/DirectX/Texture/ITTexture2D.h"
#include "Ishikawa/DirectX/Texture/ITFileTexture.h"
#include "CatchAFairy/Shader/User/LogoShaderUser.h"

namespace Ishikawa{
	namespace DirectX{
		namespace Device{
			class Device;
		}
	}
}


namespace CatchAFairy{
	namespace Scene{
		/// <summary>
		/// ロゴシーン
		/// </summary>
		class Logo final : public Base{
		private:
			enum class UpdateFunctionType;


		private:
			// 出現時間
			static const float APPEARING_TIME;
			// 表示時間
			static const float PRINT_TIME;
			// 隠蔽時間
			static const float HIDING_TIME;
			// インデックス数
			static const unsigned int INDEX_COUNT;


		private:
			/// <summary>出力レンダーターゲットテクスチャ</summary>
			::Ishikawa::DirectX::Texture::Texture2D outputRenderTargetTexture;
			/// <summary>ロゴシェーダユーザ</summary>
			::CatchAFairy::Shader::User::Logo logoShaderUser;
			/// <summary>ロゴ画像</summary>
			::Ishikawa::DirectX::Texture::FileTexture logoTexture;
			/// <summary>表示色[0.0:黒 ～ 1.0:白]</summary>
			float viewColor;
			/// <summary>現在の更新処理の種類</summary>
			UpdateFunctionType currentUpdateType;
			/// <summary>タイマー</summary>
			float timer;
			/// <summary>頂点バッファ</summary>
			ID3D11Buffer* vertexBuffer;
			/// <summary>インデックスバッファ</summary>
			ID3D11Buffer* indexBuffer;


		public:
			// デフォルトコンストラクタ
			Logo();
			// デストラクタ
			virtual ~Logo() override final;

		public:
			/// <summary>テクスチャサイズの取得</summary>
			/// <param name="_width">幅の格納先</param>
			/// <param name="_height">高さの格納先</param>
			virtual void GetTextureSize(unsigned int* const _width, unsigned int* const _height) const override final{
				this->outputRenderTargetTexture.GetTextureSize(_width, _height);
			}
			/// <summary>レンダーターゲットビューの取得</summary>
			/// <returns>レンダーターゲットビュー</returns>
			virtual ID3D11RenderTargetView* GetRenderTargetView() const override final {
				return this->outputRenderTargetTexture.GetRenderTargetView();
			}
			/// <summary>シェーダリソースビューの取得</summary>
			/// <returns>シェーダリソースビュー</returns>
			virtual ID3D11ShaderResourceView* GetShaderResourceView() const override final {
				return this->outputRenderTargetTexture.GetShaderResourceView();
			}

		private:
			// ゲームの依存している自作デバイスの取得
			::Ishikawa::DirectX::Device::Device* GetGameDependentITDevice() const;

		public:
			// ゲームのデバイスが読み込まれた
			virtual void OnLoadedGameDevice() override final;
			// ゲームのデバイスが復元された
			virtual void OnRestoredGameDevice() override final;
			// ゲームのデバイスが破棄された
			virtual void OnLostGameDevice() override final;

		public:
			// サイズ変更
			virtual void ChangeSceneSize(unsigned int _width, unsigned int _height) override final;

			// 初期化
			virtual void Initialize() override final;

			// 開始
			virtual void Start() override final;
			// 更新
			virtual void Update(float _elapsedTime) override final;
			// 描画
			virtual void Render() override final;
			// 終了
			virtual void End() override final;

		private:
			// 出力テクスチャ作成
			void CreateOutputTextures();
			// 頂点バッファ作成
			void CreateVertexBuffer();
			// インデックスバッファ作成
			void CreateIndexBuffer();
			// 頂点バッファ開放
			void ReleaseVertexBuffer();
			// インデックスバッファ開放
			void ReleaseIndexBuffer();

			// ロゴ出現
			void AppearLogo(float _elapsedTime);
			// ロゴ表示
			void PrintLogo(float _elapsedTime);
			// ロゴ隠蔽
			void HideLogo(float _elapsedTime);
		};
	}
}

#endif
