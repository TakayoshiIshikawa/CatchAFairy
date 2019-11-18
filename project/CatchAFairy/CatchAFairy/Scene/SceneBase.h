//=============================================================================
//	SceneBase.h
//
//	シーン基底のヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SCENE_BASE_H__
#define __SCENE_BASE_H__

#include "SceneInterface.h"


namespace CatchAFairy{
	namespace Scene{
		/// <summary>
		/// シーン基底
		/// </summary>
		class Base : public Interface{
		private:
			// 更新フラグ
			static const unsigned long SCENE_UPDATE_FLAG;
			// 描画フラグ
			static const unsigned long SCENE_RENDER_FLAG;

		private:
			/// <summary>シーンマネージャ</summary>
			Manager* sceneManager;
			/// <summary>次のシーン</summary>
			Interface* nextScene;
			/// <summary>前のシーン</summary>
			Interface* beforeScene;
			/// <summary>シーンフラグ</summary>
			unsigned long sceneFlag;


		public:
			// デフォルトコンストラクタ
			Base();
			// デストラクタ
			virtual ~Base() override;

		public:
			/// <summary>シーンリストの端か?</summary>
			/// <returns>常にfalse</returns>
			virtual bool IsSceneListEdge() const override final { return false; }
			/// <summary>シーンマネージャの取得</summary>
			/// <returns>シーンマネージャ</returns>
			virtual Manager* GetSceneManager() const override final { return this->sceneManager; }
			/// <summary>次のシーンの取得</summary>
			/// <returns>次のシーン</returns>
			virtual Interface* GetNextScene() const override final { return this->nextScene; }
			/// <summary>前のシーンの取得</summary>
			/// <returns>前のシーン</returns>
			virtual Interface* GetBeforeScene() const override final { return this->beforeScene; }
			// 更新するか?
			virtual bool IsUpdateScene() const override final;
			// 描画するか?
			virtual bool IsRenderScene() const override final;

		public:
			/// <summary>シーンマネージャの設定</summary>
			/// <param name="_manager">シーンマネージャ</param>
			virtual void SetSceneManager(Manager* const _manager) override final {
				this->sceneManager = _manager;
			}
			/// <summary>次のシーンの設定</summary>
			/// <param name="_next">次のシーン</param>
			virtual void SetNextScene(Interface* const _next) override final { this->nextScene = _next; }
			/// <summary>前のシーンの設定</summary>
			/// <param name="_before">前のシーン</param>
			virtual void SetBeforeScene(Interface* const _before) override final { this->beforeScene = _before; }
			// 更新フラグの設定
			virtual void SetUpdateFlag(bool _flag) override final;
			// 描画フラグの設定
			virtual void SetRenderFlag(bool _flag) override final;

		public:
			// リストから外れる
			virtual void RemoveSceneList() override final;
			// 初期化
			virtual void Initialize() = 0;

		protected:
			// シーンの初期化
			virtual void InitializeScene() override final;
		};
	}
}

#endif
