//=============================================================================
//	Ground.h
//
//	地面のヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __GROUND_H__
#define __GROUND_H__

#include "CatchAFairy/Object/RenderObjectInterface.h"
#include "Ishikawa/DirectX/Device/DependentResource/ITDeviceDependentResourceBase.h"
#include "Ishikawa/Math/ITMatrix.h"
#include "Ishikawa/DirectX/Texture/ITFileTexture.h"

namespace Ishikawa{
	namespace Model{
		namespace ItObject{
			struct VertexData;
		}
	}
}

namespace CatchAFairy{
	namespace Shader{
		namespace User{
			class PlayCameraEyeFirst;
		}
	}

	namespace Object{
		namespace PlayScene{
			/// <summary>
			/// 地面
			/// </summary>
			class Ground final :
				public ::CatchAFairy::Object::RenderInterface,
				public ::Ishikawa::DirectX::Device::DependentResource::Base
			{
			private:
				// 自作モデルデータファイルパス
				static const char* const IT_MODEL_DATA_FILEPATH;
				// 拡散反射テクスチャファイルパス
				static const wchar_t* const DIFFUSE_TEXTURE_FILEPATH;
				// 法線テクスチャファイルパス
				static const wchar_t* const NORMAL_TEXTURE_FILEPATH;

			private:
				/// <summary>利用しているシェーダ</summary>
				const ::CatchAFairy::Shader::User::PlayCameraEyeFirst* useShader;
				/// <summary>自作オブジェクト頂点配列</summary>
				::Ishikawa::Model::ItObject::VertexData* itObjectVertices;
				/// <summary>自作オブジェクト頂点数</summary>
				unsigned int itObjectVertexCount;
				/// <summary>自作オブジェクトインデックス配列</summary>
				unsigned long* itObjectIndices;
				/// <summary>自作オブジェクトインデックス数</summary>
				unsigned int itObjectIndexCount;
				/// <summary>トポロジータイプ</summary>
				::CatchAFairy::Shader::User::VertexTopology topology;
				/// <summary>頂点バッファ</summary>
				ID3D11Buffer* vertexBuffer;
				/// <summary>インデックスバッファ</summary>
				ID3D11Buffer* indexBuffer;
				/// <summary>インデックス数</summary>
				unsigned int indexCount;
				/// <summary>外接円中心座標</summary>
				::Ishikawa::Math::Float3 circumscribedCircleCenterPosition;
				/// <summary>外接円半径二乗</summary>
				float squareCircumscribedCircleRadius;
				/// <summary>ワールド行列</summary>
				::Ishikawa::Math::Matrix world;
				/// <summary>拡散反射テクスチャ</summary>
				::Ishikawa::DirectX::Texture::FileTexture diffuseTexture;
				/// <summary>法線テクスチャ</summary>
				::Ishikawa::DirectX::Texture::FileTexture normalTexture;


			public:
				// デフォルトコンストラクタ
				Ground();
				// デストラクタ
				virtual ~Ground() override final;

			public:
				// トポロジータイプの取得
				virtual ::CatchAFairy::Shader::User::VertexTopology GetTopology() const override final;
				// 頂点バッファの取得
				virtual ID3D11Buffer* GetVertexBuffer() const override final;
				// インデックスバッファの取得
				virtual ID3D11Buffer* GetIndexBuffer() const override final;
				// インデックス数の取得
				virtual unsigned int GetIndexCount() const override final;
				// 外接円中心座標の取得
				virtual const ::Ishikawa::Math::Float3& GetCircumscribedCircleCenterPosition() const override final;
				// 外接円半径二乗の取得
				virtual float GetSquareCircumscribedCircleRadius() const override final;
				// 拡散反射シェーダリソースビューの取得
				virtual ID3D11ShaderResourceView* GetDiffuseShaderResourceView() const override final;
				// 法線シェーダリソースビューの取得
				virtual ID3D11ShaderResourceView* GetNormalShaderResourceView() const override final;
				// ワールド変換行列の取得
				virtual const ::Ishikawa::Math::Matrix& GetWorld() const override final;

			public:
				// デバイスに読み込まれた
				virtual void OnDeviceLoaded() override final;
				// デバイスが復元された
				virtual void OnDeviceRestored() override final;
				// デバイスが破棄された
				virtual void OnDeviceLost() override final;

			public:
				// 初期化
				void Initialize();
				// 頂点バッファとインデックスバッファ作成
				void CreateVertexBufferAndIndexBuffer(
					const ::CatchAFairy::Shader::User::PlayCameraEyeFirst* const _useShader
				);
				// 頂点バッファとインデックスバッファ開放
				void ReleaseVertexBufferAndIndexBuffer();

			private:
				// 自作オブジェクトデータ読み込み
				void LoadItObjectData();
				// 自作オブジェクトデータ開放
				void ReleaseItObjectData();
			};
		}
	}
}

#endif
