//=============================================================================
//	ScenePlay.h
//
//	プレイシーンのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SCENE_PLAY_H__
#define __SCENE_PLAY_H__

#include "SceneBase.h"
#include "Ishikawa/DirectX/Texture/ITTexture2D.h"
#include "CatchAFairy/Object/PlayScene/DirectionalLight.h"
#include "CatchAFairy/Object/PlayScene/PlayCamera.h"
#include "CatchAFairy/Object/PlayScene/Ground.h"
#include "CatchAFairy/Object/PlayScene/Player.h"
#include "CatchAFairy/Object/PlayScene/EnemyFairies.h"
#include "CatchAFairy/Object/PlayScene/Trees.h"
#include "CatchAFairy/Object/PlayScene/PointLight.h"
#include "CatchAFairy/Object/PlayScene/GUI/GUIManager.h"
#include "CatchAFairy/Object/PlayScene/GUI/FairyPositionView.h"
#include "CatchAFairy/Object/PlayScene/GUI/TimeView.h"
#include "CatchAFairy/Object/PlayScene/GUI/OperationMethodView.h"

namespace Ishikawa{
	namespace Input{
		class Keyboard;
	}
	namespace DirectX{
		namespace Device{
			class Device;
		}
	}
	namespace Math{
		struct Float4;
		struct Matrix;
	}
}

namespace CatchAFairy{
	class Game;

	namespace Scene{
		/// <summary>
		/// プレイシーン
		/// </summary>
		class Play final : public Base{
		private:
			// 出力インデックス数
			static const unsigned int OUTPUT_INDEX_COUNT;
			// ステージサイズの半分
			static const float STAGE_SIZE_HALF;
			// ディレクショナルライトの色
			static const ::Ishikawa::Math::Float4 DIRECTIONAL_LIGHT_COLOR;


		private:
			/// <summary>出力レンダーターゲットテクスチャ</summary>
			::Ishikawa::DirectX::Texture::Texture2D outputRenderTargetTexture;

			/// <summary>平行ライト</summary>
			::CatchAFairy::Object::PlayScene::DirectionalLight directionalLight;
			/// <summary>プレイカメラ</summary>
			::CatchAFairy::Object::PlayScene::PlayCamera playCamera;
			/// <summary>地面</summary>
			::CatchAFairy::Object::PlayScene::Ground ground;
			/// <summary>プレイヤ</summary>
			::CatchAFairy::Object::PlayScene::Player player;
			/// <summary>敵妖精達</summary>
			::CatchAFairy::Object::PlayScene::EnemyFairies enemyFairies;
			/// <summary>木々</summary>
			::CatchAFairy::Object::PlayScene::Trees trees;
			/// <summary>ポイントライト</summary>
			::CatchAFairy::Object::PlayScene::PointLight pointLight;

			/// <summary>GUIマネージャ</summary>
			::CatchAFairy::Object::PlayScene::GUI::GUIManager guiManager;

			/// <summary>妖精位置表示</summary>
			::CatchAFairy::Object::PlayScene::GUI::FairyPositionView fairyPositionView;
			/// <summary>時間表示</summary>
			::CatchAFairy::Object::PlayScene::GUI::TimeView timeView;
			/// <summary>操作方法表示</summary>
			::CatchAFairy::Object::PlayScene::GUI::OperationMethodView operationMethodView;

			/// <summary>BGM音ID</summary>
			unsigned int bgmSoundId;
			/// <summary>終了フラグ</summary>
			int isEnd;


		public:
			// デフォルトコンストラクタ
			Play();
			// デストラクタ
			virtual ~Play() override final;

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
			/// <summary>敵妖精達の取得</summary>
			/// <returns>敵妖精達</returns>
			::CatchAFairy::Object::PlayScene::EnemyFairies& GetEnemyFairies(){
				return this->enemyFairies;
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
			// 手を振る音の再生
			void OnPlaySoundOfKaraburi() const;
			// 捕獲音の再生
			void OnPlaySoundOfCatched() const;

			/// <summary>終了する</summary>
			void OnEnd(){ this->isEnd = 1; }
			/// <summary>終了するか?</summary>
			/// <returns>true:する false:しない</returns>
			bool IsEnd() const { return (this->isEnd != 0); }

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

			// ステージと移動ベクトルの判定
			::Ishikawa::Math::Float3 CollideMoveVectorAtStage(
				const ::Ishikawa::Math::Float3& _startPosition,
				const ::Ishikawa::Math::Float3& _moveVector,
				const ::Ishikawa::Math::Matrix& _localToWorld,
				float _reflectionRate
			) const;

		private:
			// デバイス依存リソース作成
			void CreateDeviceDependentResources();
			// デバイス依存リソース開放
			void ReleaseDeviceDependentResources();

			// 出力テクスチャ作成
			void CreateOutputTextures();

			// 出力リソース初期化
			void InitializeOutputResources();
			// オブジェクトリソース初期化
			void InitializeObjectResources();

			// カメラ視点1番の描画
			void RenderOfCameraEyeFirst();
			// 平行ライトの描画
			void RenderOfDirectionalLight();
			// カメラ視点反射光の描画
			void RenderOfCameraEyeReflectionLight();
			// カメラ視点ポイントライトの描画
			void RenderOfCameraEyePointLight();
		};
	}
}

#endif
