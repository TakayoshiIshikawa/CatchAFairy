//=============================================================================
//	ManaLight.h
//
//	マナライトのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __MANA_LIGHT_H__
#define __MANA_LIGHT_H__

#include "CatchAFairy/Object/UpdateObjectInterface.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "CatchAFairy/Shader/User/CameraEyeReflectionLightShaderUser.h"

namespace CatchAFairy{
	namespace Object{
		namespace PlayScene{
			/// <summary>
			/// マナライト
			/// </summary>
			class ManaLight final : public ::CatchAFairy::Object::UpdateInterface{
			private:
				// 加速度
				static const ::Ishikawa::Math::Float3 ACCELERATION;
				// 左右方向の減速係数
				static const float DECELERATION_FACTOR_XZ;
				// 各方向の最大初期化速さ
				static const ::Ishikawa::Math::Float3 MAX_INITIALIZE_SPEED;

			private:
				/// <summary>対応するライト頂点</summary>
				::CatchAFairy::Shader::User::CameraEyeReflectionLight::VSInput* lightVertex;
				/// <summary>速度</summary>
				::Ishikawa::Math::Float3 velocity;


			public:
				// デフォルトコンストラクタ
				ManaLight();
				// デストラクタ
				virtual ~ManaLight() override final;

			public:
				/// <summary>対応するライト頂点の設定</summary>
				/// <param name="_lightVertex">対応するライト頂点</param>
				void ManaLight::SetLightVertex(
					::CatchAFairy::Shader::User::CameraEyeReflectionLight::VSInput* const _lightVertex
				){
					this->lightVertex = _lightVertex;
				}

			public:
				// 生成
				void Generate(
					const ::Ishikawa::Math::Float3& _position,
					const ::Ishikawa::Math::Float4& _color,
					const ::Ishikawa::Math::Float2& _radiusAndLumen
				);
				// 更新
				virtual void Update(float _elapsedTime) override final;

			private:
				// [-1.0f,1.0f)の乱数値を取得
				static inline float Random();
				// 速度をランダマイズ
				void RandomizeVelocity();
			};
		}
	}
}

#endif
