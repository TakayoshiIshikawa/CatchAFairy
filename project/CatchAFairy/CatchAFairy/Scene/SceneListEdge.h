//=============================================================================
//	SceneListEdge.h
//
//	シーンリストの端のヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SCENE_LIST_EDGE_H__
#define __SCENE_LIST_EDGE_H__

#include "SceneInterface.h"


namespace CatchAFairy{
	namespace Scene{
		/// <summary>
		/// リストの端
		/// </summary>
		class ListEdge final : public Interface{
		protected:
			/// <summary>次のシーン</summary>
			Interface* nextScene;
			/// <summary>前のシーン</summary>
			Interface* beforeScene;


		public:
			// デフォルトコンストラクタ
			ListEdge();
			// デストラクタ
			virtual ~ListEdge() override final;

		public:
			/// <summary>シーンリストの端か?</summary>
			/// <returns>常にtrue</returns>
			virtual bool IsSceneListEdge() const override final { return true; }
			/// <summary>シーンマネージャの取得[出来ない]</summary>
			/// <returns>常にNULL</returns>
			virtual Manager* GetSceneManager() const override final { return nullptr; }
			/// <summary>次のシーンの取得</summary>
			/// <returns>次のシーン</returns>
			virtual Interface* GetNextScene() const override final { return this->nextScene; }
			/// <summary>前のシーンの取得</summary>
			/// <returns>前のシーン</returns>
			virtual Interface* GetBeforeScene() const override final { return this->beforeScene; }
			/// <summary>テクスチャサイズの取得[出来ない]</summary>
			/// <param name="_width">幅の格納先</param>
			/// <param name="_height">高さの格納先</param>
			virtual void GetTextureSize(unsigned int* const _width, unsigned int* const _height) const override final{
				NOT_USING(_width); NOT_USING(_height);
			}
			/// <summary>レンダーターゲットビューの取得[出来ない]</summary>
			/// <returns>常にNULL</returns>
			virtual ID3D11RenderTargetView* GetRenderTargetView() const override final { return nullptr; }
			/// <summary>シェーダリソースビューの取得[出来ない]</summary>
			/// <returns>常にNULL</returns>
			virtual ID3D11ShaderResourceView* GetShaderResourceView() const override final { return nullptr; }
			/// <summary>更新するか?</summary>
			/// <returns>常にfalse</returns>
			virtual bool IsUpdateScene() const override final { return false; }
			/// <summary>描画するか?</summary>
			/// <returns>常にfalse</returns>
			virtual bool IsRenderScene() const override final { return false; }

		public:
			/// <summary>シーンマネージャの設定[出来ない]</summary>
			virtual void SetSceneManager(Manager* const) override final{}
			/// <summary>次のシーンの設定</summary>
			/// <param name="_next">次のシーン</param>
			virtual void SetNextScene(Interface* const _next) override final { this->nextScene = _next; }
			/// <summary>前のシーンの設定</summary>
			/// <param name="_before">前のシーン</param>
			virtual void SetBeforeScene(Interface* const _before) override final { this->beforeScene = _before; }
			/// <summary>更新フラグの設定[出来ない]</summary>
			virtual void SetUpdateFlag(bool) override final{}
			/// <summary>描画フラグの設定[出来ない]</summary>
			virtual void SetRenderFlag(bool) override final{}

		public:
			/// <summary>ゲームのデバイスが読み込まれた</summary>
			virtual void OnLoadedGameDevice() override final{}
			/// <summary>ゲームのデバイスが復元された</summary>
			virtual void OnRestoredGameDevice() override final{}
			/// <summary>ゲームのデバイスが破棄された</summary>
			virtual void OnLostGameDevice() override final{}

		public:
			/// <summary>リストから外れる[外れない]</summary>
			virtual void RemoveSceneList() override final{}
			/// <summary>サイズ変更</summary>
			virtual void ChangeSceneSize(unsigned int, unsigned int) override final{}

			/// <summary>開始</summary>
			virtual void Start() override final{}
			/// <summary>更新</summary>
			virtual void Update(float) override final{}
			/// <summary>描画</summary>
			virtual void Render() override final{}
			/// <summary>終了</summary>
			virtual void End() override final{}

			// 初期化
			void Initialize();

		protected:
			// シーンの初期化
			virtual void InitializeScene() override final;
		};
	}
}

#endif
