//=============================================================================
//	Trees.h
//
//	木々のヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __TREES_H__
#define __TREES_H__

#include "CatchAFairy/Object/UpdateObjectInterface.h"
#include "CatchAFairy/Object/RenderSameObjectInterface.h"
#include "Ishikawa/DirectX/Device/DependentResource/ITDeviceDependentResourceBase.h"
#include "Ishikawa/Math/ITMatrix.h"
#include "Ishikawa/DirectX/Texture/ITFileTexture.h"
#include "CatchAFairy/Data/TreesOffset.h"

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
			/// 木々[移動範囲内とその近辺]
			/// </summary>
			class Trees final :
				public ::CatchAFairy::Object::UpdateInterface,
				public ::CatchAFairy::Object::RenderSameInterface,
				public ::Ishikawa::DirectX::Device::DependentResource::Base
			{
			private:
				/// <summary>ワールド変換行列初期化用データ</summary>
				struct WorldOffset{
				public:
					/// <summary>X座標</summary>
					float positionX;
					/// <summary>Y座標</summary>
					float positionY;
					/// <summary>Z座標</summary>
					float positionZ;
					/// <summary>Y軸回転量</summary>
					float angle;
					/// <summary>幅拡大率</summary>
					float scaleOfWidth;
					/// <summary>高さ拡大率</summary>
					float scaleOfHeight;

				public:
					// 行列に変換
					void ToMatrix(::Ishikawa::Math::Matrix* const _matrix) const;
				};

			private:
				// 自作モデルデータファイルパス
				static const char* const IT_MODEL_DATA_FILEPATH;
				// 拡散反射テクスチャファイルパス
				static const wchar_t* const DIFFUSE_TEXTURE_FILEPATH;
				// 法線テクスチャファイルパス
				static const wchar_t* const NORMAL_TEXTURE_FILEPATH;
				// オフセットデータファイルパス
				static const char* const OFFSET_DATA_FILEPATH;
				// 判定軸長さ
				static const float COLLISION_AXIS_HEIGHT;
				// 判定半径
				static const float COLLISION_RADIUS;
				// 最大振幅
				static const ::Ishikawa::Math::Float3 MAX_AMPLITUDE;
				// 最小揺れ速度
				static const float MIN_SWING_SPEED;
				// 最大揺れ速度
				static const float MAX_SWING_SPEED;

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
				/// <summary>振幅配列</summary>
				::Ishikawa::Math::Float3* amplitudeArray;
				/// <summary>揺れ速度配列</summary>
				::Ishikawa::Math::Float3* swingSpeedArray;
				/// <summary>揺れ時間配列</summary>
				::Ishikawa::Math::Float3* swingTimeArray;
				/// <summary>オフセットデータ</summary>
				::CatchAFairy::Data::TreesOffset treesOffset;
				/// <summary>ワールド変換行列配列</summary>
				::Ishikawa::Math::Matrix* worldArray;
				/// <summary>拡散反射テクスチャ</summary>
				::Ishikawa::DirectX::Texture::FileTexture diffuseTexture;
				/// <summary>法線テクスチャ</summary>
				::Ishikawa::DirectX::Texture::FileTexture normalTexture;


			public:
				// デフォルトコンストラクタ
				Trees();
				// デストラクタ
				virtual ~Trees() override final;

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
				// ワールド変換行列数の取得
				virtual unsigned int GetWorldCount() const override final;
				// ワールド変換行列配列の取得
				virtual const ::Ishikawa::Math::Matrix* GetWorldArray() const override final;

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
				// 更新
				virtual void Update(float _elapsedTime) override final;
				// 頂点バッファとインデックスバッファ作成
				void CreateVertexBufferAndIndexBuffer(
					const ::CatchAFairy::Shader::User::PlayCameraEyeFirst* const _useShader
				);
				// 頂点バッファとインデックスバッファ開放
				void ReleaseVertexBufferAndIndexBuffer();

				// ベクトルとの判定
				::Ishikawa::Math::Float3 CollideVector(
					const ::Ishikawa::Math::Float3& _startPosition,
					const ::Ishikawa::Math::Float3& _vector,
					const ::Ishikawa::Math::Matrix& _matrixVectorToWorld,
					float _reflectionRate
				) const;

			private:
				// 自作オブジェクトデータ読み込み
				void LoadItObjectData();
				// 自作オブジェクトデータ開放
				void ReleaseItObjectData();

				// ワールド変換行列配列の初期化
				void InitializeWorldArray();
				// ワールド変換行列配列の更新
				void UpdateWorldArray(float _elapsedTime);
				// ワールド変換行列配列の終了
				void FinalizeWorldArray();
			};
		}
	}
}

#endif
