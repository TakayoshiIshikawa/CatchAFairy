//=============================================================================
//	SceneManager.h
//
//	シーンマネージャのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "SceneListEdge.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "CatchAFairy/Shader/User/OutputScreenShaderUser.h"

namespace CatchAFairy{
	class Game;

	namespace Scene{
		class Interface;
		class Base;


		/// <summary>
		/// シーンマネージャ
		/// </summary>
		class Manager final{
		public:
			// シーン出力テクスチャサイズ
			static const unsigned int SCENE_OUTPUT_TEXTURE_SIZE;
			// シーン出力テクスチャの描画幅
			static const unsigned int SCENE_OUTPUT_TEXTURE_WIDTH;
			// シーン出力テクスチャの描画高さ
			static const unsigned int SCENE_OUTPUT_TEXTURE_HEIGHT;
			// シーン出力テクスチャのアスペクト比
			static const float SCENE_OUTPUT_TEXTURE_ASPECT;
			// 余白の色
			static const ::Ishikawa::Math::Float4 MARGIN_COLOR;

		private:
			/// <summary>ゲーム</summary>
			Game* game;
			/// <summary>シーンリストの端</summary>
			ListEdge sceneListEdge;
			/// <summary>画面出力シェーダユーザ</summary>
			::CatchAFairy::Shader::User::OutputScreen outputScreenShaderUser;
			/// <summary>シーン変更したフレームか?</summary>
			int isChangedSceneFrame;

		public:
			// デフォルトコンストラクタ
			Manager();
			// デストラクタ
			~Manager();

		public:
			/// <summary>ゲームの取得</summary>
			/// <returns>ゲーム</returns>
			Game* GetGame() const { return this->game; }

		private:
			// ゲームが依存している自作デバイスの取得
			::Ishikawa::DirectX::Device::Device* GetGameDependentITDevice() const;
			// 現在のシーンを取得
			Interface* GetCurrentScene() const;


		public:
			/// <summary>ゲームの設定</summary>
			/// <param name="_game">ゲーム</param>
			void SetGame(Game* const _game){ this->game = _game; }

		public:
			// ゲームのデバイスが読み込まれた
			void OnLoadedGameDevice();
			// ゲームのデバイスが復元された
			void OnRestoredGameDevice();
			// ゲームのデバイスが破棄された
			void OnLostGameDevice();

		public:
			// 初期化
			void Initialize();
			// 更新
			void Update(float _elapsedTime);
			// 描画
			void Render();
			// 画面へ描画
			void RenderToScreen(
				ID3D11RenderTargetView* const _renderTargetView,
				ID3D11DepthStencilView* const _depthStencilView,
				unsigned int _width,
				unsigned int _height
			);
			// 終了
			void Finalize();

			// プッシュ
			void PushScene(Base* const _scene);
			// ポップ
			void PopScene();
			// チェンジ
			void ChangeScene(Base* const _scene);
			// 全削除
			void ClearSceneList();
		};
	}
}

#endif
