//=============================================================================
//	GUIObjectInterface.h
//
//	GUIオブジェクトインターフェースのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __GUI_OBJECT_INTERFACE_H__
#define __GUI_OBJECT_INTERFACE_H__

#include "CatchAFairy/Shader/User/PlayGUIShaderUser.h"

namespace CatchAFairy{
	namespace Object{
		namespace PlayScene{
			namespace GUI{
				class GUIManager;

				/// <summary>
				/// GUIオブジェクトインターフェース
				/// </summary>
				class GUIInterface{
				public:
					/// <summary>デフォルトコンストラクタ</summary>
					GUIInterface(){}
					/// <summary>デストラクタ</summary>
					virtual ~GUIInterface(){}

				public:
					// マネージャの設定
					virtual void SetManager(
						::CatchAFairy::Object::PlayScene::GUI::GUIManager* const _manager
					) = 0;

				protected:
					// 頂点の設定
					virtual void SetVertices(
						::CatchAFairy::Shader::User::PlayGUI::VSInput* const _vertices,
						unsigned int _count
					) = 0;

				public:
					// 更新
					virtual void Update(float _elapsedTime) = 0;
				};
			}
		}
	}
}

#endif
