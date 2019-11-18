//=============================================================================
//	SceneTitle.h
//
//	タイトルシーンのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

#include "SceneBase.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "Ishikawa/DirectX/Texture/ITTexture2D.h"
#include "Ishikawa/DirectX/Texture/ITFileTexture.h"
#include "CatchAFairy/Shader/User/TitleOutputShaderUser.h"

namespace Ishikawa{
	namespace Input{
		class Keyboard;
	}
	namespace DirectX{
		namespace Device{
			class Device;
		}
	}
}

namespace CatchAFairy{
	class Game;


	namespace Scene{
		/// <summary>
		/// タイトルシーン
		/// </summary>
		class Title final : public Base{
		private:
			// 背景インデックス数
			static const unsigned int BACKGROUND_INDEX_COUNT;
			// ボタンインデックス数
			static const unsigned int BUTTON_INDEX_COUNT;


		private:
			/// <summary>出力レンダーターゲットテクスチャ</summary>
			::Ishikawa::DirectX::Texture::Texture2D outputRenderTargetTexture;
			/// <summary>タイトル出力シェーダユーザ</summary>
			::CatchAFairy::Shader::User::TitleOutput titleOutputShaderUser;
			/// <summary>背景画像</summary>
			::Ishikawa::DirectX::Texture::FileTexture backgroundTexture;
			/// <summary>背景頂点バッファ</summary>
			ID3D11Buffer* backgroundVertexBuffer;
			/// <summary>背景インデックスバッファ</summary>
			ID3D11Buffer* backgroundIndexBuffer;
			/// <summary>ボタン画像</summary>
			::Ishikawa::DirectX::Texture::FileTexture buttonsTexture;
			/// <summary>プレイボタンの矩形情報 [(x,y):左上 (z,w):右下]</summary>
			::Ishikawa::Math::Float4 playButtonRect;
			/// <summary>ルールボタンの矩形情報 [(x,y):左上 (z,w):右下]</summary>
			::Ishikawa::Math::Float4 ruleButtonRect;
			/// <summary>終了ボタンの矩形情報 [(x,y):左上 (z,w):右下]</summary>
			::Ishikawa::Math::Float4 endButtonRect;
			/// <summary>プレイボタン頂点バッファ</summary>
			ID3D11Buffer* playButtonVertexBuffer;
			/// <summary>ルールボタン頂点バッファ</summary>
			ID3D11Buffer* ruleButtonVertexBuffer;
			/// <summary>終了ボタン頂点バッファ</summary>
			ID3D11Buffer* endButtonVertexBuffer;
			/// <summary>ボタンインデックスバッファ</summary>
			ID3D11Buffer* buttonIndexBuffer;

			/// <summary>BGM音ID</summary>
			unsigned int bgmSoundId;

		public:
			// デフォルトコンストラクタ
			Title();
			// デストラクタ
			virtual ~Title() override final;

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
			// ゲームの取得
			::CatchAFairy::Game* GetGame() const;
			// ゲームの依存している自作デバイスの取得
			::Ishikawa::DirectX::Device::Device* GetGameDependentITDevice() const;
			// キーボードの取得
			const ::Ishikawa::Input::Keyboard& GetKeyboard() const;

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
			// デバイス依存リソース作成
			void CreateDeviceDependentResources();
			// デバイス依存リソース開放
			void ReleaseDeviceDependentResources();

			// 出力テクスチャ作成
			void CreateOutputTextures();

			// 背景頂点バッファ作成
			void CreateBackgroundVertexBuffer();
			// 背景インデックスバッファ作成
			void CreateBackgroundIndexBuffer();
			// 背景頂点バッファ開放
			void ReleaseBackgroundVertexBuffer();
			// 背景インデックスバッファ開放
			void ReleaseBackgroundIndexBuffer();

			// プレイボタン頂点バッファ作成
			void CreatePlayButtonVertexBuffer();
			// ルールボタン頂点バッファ作成
			void CreateRuleButtonVertexBuffer();
			// プレイボタン頂点バッファ作成
			void CreateEndButtonVertexBuffer();
			// ボタンインデックスバッファ作成
			void CreateButtonIndexBuffer();
			// プレイボタン頂点バッファ開放
			void ReleasePlayButtonVertexBuffer();
			// ルールボタン頂点バッファ開放
			void ReleaseRuleButtonVertexBuffer();
			// プレイボタン頂点バッファ開放
			void ReleaseEndButtonVertexBuffer();
			// ボタンインデックスバッファ開放
			void ReleaseButtonIndexBuffer();

			// 出力に描画
			void RenderToOutput();
		};
	}
}

#endif
