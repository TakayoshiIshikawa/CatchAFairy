//=============================================================================
//	SceneInterface.h
//
//	シーンインターフェースのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SCENE_INTERFACE_H__
#define __SCENE_INTERFACE_H__

#include "Ishikawa/DirectX/Texture/ITTexture2DInterface.h"
#include "Ishikawa/DirectX/Texture/ITRenderTargetInterface.h"
#include "Ishikawa/DirectX/Texture/ITShaderResourceInterface.h"


namespace CatchAFairy{
	namespace Scene{
		class Manager;


		/// <summary>
		/// シーンインターフェース
		/// </summary>
		class Interface :
			public ::Ishikawa::DirectX::Texture::Texture2DInterface,
			public ::Ishikawa::DirectX::Texture::RenderTargetInterface,
			public ::Ishikawa::DirectX::Texture::ShaderResourceInterface
		{
		public:
			/// <summary>デフォルトコンストラクタ</summary>
			Interface(){}
			/// <summary>デストラクタ</summary>
			virtual ~Interface(){}

		public:
			// シーンリストの端か?
			virtual bool IsSceneListEdge() const = 0;
			// シーンマネージャの取得
			virtual Manager* GetSceneManager() const = 0;
			// 次のシーンの取得
			virtual Interface* GetNextScene() const = 0;
			// 前のシーンの取得
			virtual Interface* GetBeforeScene() const = 0;
			// 更新するか?
			virtual bool IsUpdateScene() const = 0;
			// 描画するか?
			virtual bool IsRenderScene() const = 0;

		public:
			// シーンマネージャの設定
			virtual void SetSceneManager(Manager* const _manager) = 0;
			// 次のシーンの設定
			virtual void SetNextScene(Interface* const _next) = 0;
			// 前のシーンの設定
			virtual void SetBeforeScene(Interface* const _before) = 0;
			// 更新フラグの設定
			virtual void SetUpdateFlag(bool _flag) = 0;
			// 描画フラグの設定
			virtual void SetRenderFlag(bool _flag) = 0;

		public:
			// ゲームのデバイスが読み込まれた
			virtual void OnLoadedGameDevice() = 0;
			// ゲームのデバイスが復元された
			virtual void OnRestoredGameDevice() = 0;
			// ゲームのデバイスが破棄された
			virtual void OnLostGameDevice() = 0;

		public:
			// リストから外れる
			virtual void RemoveSceneList() = 0;
			// サイズ変更
			virtual void ChangeSceneSize(unsigned int _width, unsigned int _height) = 0;

			// 開始
			virtual void Start() = 0;
			// 更新
			virtual void Update(float _elapsedTime) = 0;
			// 描画
			virtual void Render() = 0;
			// 終了
			virtual void End() = 0;

		protected:
			// シーンの初期化
			virtual void InitializeScene() = 0;
		};
	}
}

#endif
