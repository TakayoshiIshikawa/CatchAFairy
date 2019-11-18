//=============================================================================
//	PointLight.h
//
//	ポイントライトのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__

struct ID3D11Buffer;
#include "CatchAFairy/Object/UpdateObjectInterface.h"
#include "CatchAFairy/Object/PointLightObjectInterface.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "CatchAFairy/Shader/User/CameraEyeReflectionLightShaderUser.h"
#include "CatchAFairy/Object/PlayScene/ManaLight.h"
#include "CatchAFairy/Data/PointLightsSetting.h"

namespace CatchAFairy{
	namespace Object{
		namespace PlayScene{
			/// <summary>
			/// ポイントライト
			/// </summary>
			class PointLight final :
				public ::CatchAFairy::Object::UpdateInterface,
				public ::CatchAFairy::Object::PointLightInterface
			{
			private:
				/// <summary>妖精1体の本体につくライトの数</summary>
				static const unsigned int ONE_FAIRY_BODY_LIGHT_COUNT = ::CatchAFairy::Data::PointLightsSetting::ONE_FAIRY_BODY_LIGHT_COUNT;
				/// <summary>妖精1体から出るマナライトの数</summary>
				static const unsigned int ONE_FAIRY_MANA_LIGHT_COUNT = ::CatchAFairy::Data::PointLightsSetting::ONE_FAIRY_MANA_LIGHT_COUNT;
				/// <summary>妖精1体あたりのライトの総数</summary>
				static const unsigned int ONE_FAIRY_LIGHT_ALL_COUNT = ::CatchAFairy::Data::PointLightsSetting::ONE_FAIRY_LIGHT_ALL_COUNT;
				/// <summary>敵妖精の数</summary>
				static const unsigned int ENEMY_FAIRY_COUNT = ::CatchAFairy::Data::PointLightsSetting::ENEMY_FAIRY_COUNT;
				/// <summary>妖精の総数</summary>
				static const unsigned int FAIRY_COUNT = ::CatchAFairy::Data::PointLightsSetting::FAIRY_COUNT;

				/// <summary>ライトの最大数</summary>
				static const unsigned int MAX_COUNT = ::CatchAFairy::Data::PointLightsSetting::MAX_COUNT;

			private:
				/// <summary>頂点配列</summary>
				::CatchAFairy::Shader::User::CameraEyeReflectionLight::VSInput vertexArray[MAX_COUNT];
				/// <summary>頂点バッファ</summary>
				ID3D11Buffer* vertexBuffer;
				/// <summary>インデックスバッファ</summary>
				ID3D11Buffer* indexBuffer;
				/// <summary>インデックス数</summary>
				unsigned int indexCount;
				/// <summary>反射光シェーダユーザ</summary>
				::CatchAFairy::Shader::User::CameraEyeReflectionLight* reflectionLightShaderUser;
				/// <summary>マナライト色</summary>
				::Ishikawa::Math::Float4 manaLightColor[FAIRY_COUNT];
				/// <summary>マナライト半径と光度</summary>
				::Ishikawa::Math::Float2 manaLightRadiusAndLumen[FAIRY_COUNT];
				/// <summary>マナライト生成インデックス</summary>
				unsigned int manaLightCreateIndex[FAIRY_COUNT];
				/// <summary>マナライト配列</summary>
				ManaLight manaLightArray[FAIRY_COUNT][ONE_FAIRY_MANA_LIGHT_COUNT];


			public:
				// デフォルトコンストラクタ
				PointLight();
				// デストラクタ
				virtual ~PointLight() override final;

			public:
				/// <summary>頂点バッファの取得</summary>
				/// <returns>頂点バッファ</returns>
				virtual ID3D11Buffer* GetVertexBuffer() const override final { return this->vertexBuffer; }
				/// <summary>インデックスバッファの取得</summary>
				/// <returns>インデックスバッファ</returns>
				virtual ID3D11Buffer* GetIndexBuffer() const override final { return this->indexBuffer; }
				/// <summary>インデックス数の取得</summary>
				/// <returns>インデックス数</returns>
				virtual unsigned int GetIndexCount() const override final { return this->indexCount; }

			public:
				/// <summary>反射光シェーダユーザの設定</summary>
				/// <param name="_reflectionLightShaderUser">反射光シェーダユーザ</param>
				virtual void SetReflectionLightShaderUser(
					::CatchAFairy::Shader::User::CameraEyeReflectionLight* const _reflectionLightShaderUser
				) override final {
					this->reflectionLightShaderUser = _reflectionLightShaderUser;
				}

				// 妖精本体につくライトの座標の設定
				void SetFairyBodyLightPosition(unsigned int _id, float _x, float _y, float _z);
				// 妖精本体につくライトの色の設定
				void SetFairyBodyLightColor(unsigned int _id, float _red, float _green, float _blue, float _alpha);
				// 妖精本体につくライトの半径の設定
				void SetFairyBodyLightRadius(unsigned int _id, float _radius);
				// 妖精本体につくライトの光度の設定
				void SetFairyBodyLightLumen(unsigned int _id, float _lumen);
				// マナライト色の設定
				void SetFairyManaLightColor(unsigned int _id, float _red, float _green, float _blue, float _alpha);
				// マナライト半径の設定
				void SetFairyManaLightRadius(unsigned int _id, float _radius);
				// マナライト光度の設定
				void SetFairyManaLightLumen(unsigned int _id, float _lumen);

			public:
				// 初期化
				void Initialize();
				// 更新
				virtual void Update(float _elapsedTime) override final;

				// マナライト生成
				void GenerateFairyManaLight(unsigned int _id, float _x, float _y, float _z);

				// デバイス依存リソース作成
				void CreateDeviceDependentResources();
				// デバイス依存リソース開放
				void ReleaseDeviceDependentResources();

			private:
				// 頂点配列の初期化
				void InitializeVertexArray();

				// 頂点バッファ作成
				void CreateVertexBuffer();
				// インデックスバッファ作成
				void CreateIndexBuffer();
				// 頂点バッファ開放
				void ReleaseVertexBuffer();
				// インデックスバッファ開放
				void ReleaseIndexBuffer();
			};
		}
	}
}

#endif
