//=============================================================================
//	GUIManager.h
//
//	GUIマネージャのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __GUI_MANAGER_H__
#define __GUI_MANAGER_H__

#include "Ishikawa/Math/ITFloatN.h"
#include "Ishikawa/DirectX/Texture/ITTexture2D.h"
#include "Ishikawa/DirectX/Texture/ITFileTexture.h"
#include "CatchAFairy/Shader/User/PlayGUIShaderUser.h"
#include "CatchAFairy/Data/GUIVerticesSetting.h"

namespace CatchAFairy{
	namespace Object{
		namespace PlayScene{
			namespace GUI{
				/// <summary>
				/// GUIマネージャ
				/// </summary>
				class GUIManager final {
				public:
					// 出力サイズ
					static const ::Ishikawa::Math::Float2 OUTPUT_SIZE;

				private:
					// GUIインデックス数
					static const unsigned int GUI_INDEX_COUNT;
					/// <summary>GUI頂点数</summary>
					static const unsigned int GUI_VERTEX_COUNT = ::CatchAFairy::Data::GUIVerticesSetting::ALL_VERTEX_COUNT;
					// 形状テクスチャファイルパス
					static const wchar_t* const SHAPE_TEXTURE_FILEPATH;
					// 表示テクスチャファイルパス
					static const wchar_t* const VIEW_TEXTURE_FILEPATH;


				private:
					/// <summary>出力ターゲットテクスチャ</summary>
					::Ishikawa::DirectX::Texture::Texture2D* outputTargetTexture;
					/// <summary>GUIシェーダユーザ</summary>
					::CatchAFairy::Shader::User::PlayGUI guiShaderUser;
					/// <summary>GUI頂点バッファ</summary>
					ID3D11Buffer* guiVertexBuffer;
					/// <summary>GUIインデックスバッファ</summary>
					ID3D11Buffer* guiIndexBuffer;
					/// <summary>GUIインデックス数</summary>
					unsigned int guiIndexCount;
					/// <summary>GUI頂点配列</summary>
					::CatchAFairy::Shader::User::PlayGUI::VSInput guiVertexArray[GUI_VERTEX_COUNT];

					/// <summary>形状テクスチャ</summary>
					::Ishikawa::DirectX::Texture::FileTexture shapeTexture;
					/// <summary>表示テクスチャ</summary>
					::Ishikawa::DirectX::Texture::FileTexture viewTexture;


				public:
					// デフォルトコンストラクタ
					GUIManager();
					// デストラクタ
					~GUIManager();

				public:
					// GUI頂点の取得
					::CatchAFairy::Shader::User::PlayGUI::VSInput* GetGUIVertices(
						unsigned int _startIndex,
						unsigned int _getCount
					);

				public:
					/// <summary>出力ターゲットテクスチャの設定</summary>
					/// <param name="_outputTargetTexture">出力ターゲットテクスチャ</param>
					void SetOutputTargetTexture(::Ishikawa::DirectX::Texture::Texture2D* const _outputTargetTexture){
						this->outputTargetTexture = _outputTargetTexture;
					}

				public:
					// 初期化
					void Initialize();
					// 描画
					void Render();

					// ゲームのデバイスに依存するリソースの追加
					void AddITDeviceDependentResources(::Ishikawa::DirectX::Device::Device* const _itDevice);
					// デバイス依存リソース作成
					void CreateDeviceDependentResources();
					// デバイス依存リソース開放
					void ReleaseDeviceDependentResources();

				private:
					// 描画前
					void DrawBefore();
					// 描画
					void Draw();
					// 描画後
					void DrawAfter();

					// GUI頂点バッファの作成
					void CreateGUIVertexBuffer();
					// GUI頂点バッファの開放
					void ReleaseGUIVertexBuffer();
					// GUIインデックスバッファの作成
					void CreateGUIIndexBuffer();
					// GUIインデックスバッファの開放
					void ReleaseGUIIndexBuffer();
				};
			}
		}
	}
}

#endif
