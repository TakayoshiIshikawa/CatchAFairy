//=============================================================================
//	SceneResult.h
//
//	リザルトシーンのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SCENE_RESULT_H__
#define __SCENE_RESULT_H__

#include "SceneBase.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "Ishikawa/DirectX/Texture/ITTexture2D.h"
#include "Ishikawa/DirectX/Texture/ITFileTexture.h"
#include "CatchAFairy/Shader/User/TitleOutputShaderUser.h"
#include "CatchAFairy/Data/EnemyFairyType.h"

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
		/// リザルトシーン
		/// </summary>
		class Result final : public Base{
		private:
			// 背景インデックス数
			static const unsigned int BACKGROUND_INDEX_COUNT;

			// 数字のテクスチャコーディネート [(x,y):左上 (z,w):右下]
			static const ::Ishikawa::Math::Float4 NUMBER_TEXTURECOORDINATE[11];
			/// <summary>各妖精数の表示桁数</summary>
			static const unsigned int FAIRY_COUNT_DIGIT = 2U;
			/// <summary>各妖精スコアの表示桁数</summary>
			static const unsigned int FAIRY_SCORE_DIGIT = 5U;
			/// <summary>合計スコアの表示桁数</summary>
			static const unsigned int TOTAL_SCORE_DIGIT = 5U;
			// 各妖精のスコア
			static const unsigned int FAIRY_SCORE[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)];

			// 妖精カウント文字サイズ
			static const ::Ishikawa::Math::Float2 FAIRY_COUNT_NUMBER_SIZE;
			// スコア文字サイズ
			static const ::Ishikawa::Math::Float2 SCORE_NUMBER_SIZE;
			// 妖精カウント座標
			static const ::Ishikawa::Math::Float2 FAIRY_COUNT_POSITION[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)];
			// 妖精スコア座標
			static const ::Ishikawa::Math::Float2 FAIRY_SCORE_POSITION[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)];
			// 合計スコア座標
			static const ::Ishikawa::Math::Float2 TOTAL_SCORE_POSITION;

			/// <summary>各妖精数の数字の先頭インデックス</summary>
			static const unsigned int FAIRY_COUNT_NUMBER_FARST_INDEX = 0U;
			/// <summary>各妖精スコアの数字の先頭インデックス</summary>
			static const unsigned int FAIRY_SCORE_NUMBER_FARST_INDEX =
				FAIRY_COUNT_NUMBER_FARST_INDEX +
				FAIRY_COUNT_DIGIT*static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count);
			/// <summary>合計スコアの数字の先頭インデックス</summary>
			static const unsigned int TOTAL_SCORE_NUMBER_FARST_INDEX =
				FAIRY_SCORE_NUMBER_FARST_INDEX +
				FAIRY_SCORE_DIGIT*static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count);
			/// <summary>表示される数字の合計</summary>
			static const unsigned int TOTAL_VIEW_NUMBER_COUNT =
				TOTAL_SCORE_NUMBER_FARST_INDEX + TOTAL_SCORE_DIGIT;
			// 数字を表示するための頂点の合計
			static const unsigned int TOTAL_VIEW_NUMBER_VERTEX_COUNT;
			// 数字を表示するためのインデックスの合計
			static const unsigned int TOTAL_VIEW_NUMBER_INDEX_COUNT;


		private:
			/// <summary>出力レンダーターゲットテクスチャ</summary>
			::Ishikawa::DirectX::Texture::Texture2D outputRenderTargetTexture;
			/// <summary>タイトル出力シェーダユーザ (使いまわし)</summary>
			::CatchAFairy::Shader::User::TitleOutput titleOutputShaderUser;
			/// <summary>背景画像</summary>
			::Ishikawa::DirectX::Texture::FileTexture backgroundTexture;
			/// <summary>背景頂点バッファ</summary>
			ID3D11Buffer* backgroundVertexBuffer;
			/// <summary>背景インデックスバッファ</summary>
			ID3D11Buffer* backgroundIndexBuffer;
			/// <summary>数字画像</summary>
			::Ishikawa::DirectX::Texture::FileTexture numbersTexture;
			/// <summary>数字頂点バッファ</summary>
			ID3D11Buffer* numberVertexBuffer;
			/// <summary>数字インデックスバッファ</summary>
			ID3D11Buffer* numberIndexBuffer;
			/// <summary>種類ごとの捕獲妖精数</summary>
			unsigned int catchedFairyCount[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)];

			/// <summary>BGM音ID</summary>
			unsigned int bgmSoundId;

		public:
			// デフォルトコンストラクタ
			Result();
			// デストラクタ
			virtual ~Result() override final;

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
			// 数字の文字からテクスチャコーディネートを取得する
			const ::Ishikawa::Math::Float4& GetNumberTexcoord(const char _numberCharacter);

		public:
			/// <summary>捕獲妖精数の設定</summary>
			/// <param name="_type">妖精の種類</param>
			/// <param name="_count">捕獲数</param>
			void SetCatchedFairyCount(CatchAFairy::Data::EnemyFairyType _type, unsigned int _count){
				if(_type == CatchAFairy::Data::EnemyFairyType::Count) return;
				this->catchedFairyCount[static_cast<unsigned int>(_type)] = _count;
			}

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

			// 数字頂点バッファ作成
			void CreateNumberVertexBuffer();
			// 数字インデックスバッファ作成
			void CreateNumberIndexBuffer();
			// 数字頂点バッファ開放
			void ReleaseNumberVertexBuffer();
			// 数字インデックスバッファ開放
			void ReleaseNumberIndexBuffer();

			// 出力に描画
			void RenderToOutput();
		};
	}
}

#endif
