//=============================================================================
//	PointLightObjectInterface.h
//
//	ポイントライトオブジェクトインターフェースのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __POINT_LIGHT_OBJECT_INTERFACE_H__
#define __POINT_LIGHT_OBJECT_INTERFACE_H__

struct ID3D11Buffer;

namespace CatchAFairy{
	namespace Shader{
		namespace User{
			class CameraEyeReflectionLight;
		}
	}

	namespace Object{
		/// <summary>
		/// ポイントライトオブジェクトインターフェース
		/// </summary>
		class PointLightInterface{
		public:
			/// <summary>デフォルトコンストラクタ</summary>
			PointLightInterface(){}
			/// <summary>デストラクタ</summary>
			virtual ~PointLightInterface(){}

		public:
			// 頂点バッファの取得
			virtual ID3D11Buffer* GetVertexBuffer() const = 0;
			// インデックスバッファの取得
			virtual ID3D11Buffer* GetIndexBuffer() const = 0;
			// インデックス数の取得
			virtual unsigned int GetIndexCount() const = 0;

		public:
			// 反射光シェーダユーザの設定
			virtual void SetReflectionLightShaderUser(
				::CatchAFairy::Shader::User::CameraEyeReflectionLight* const _reflectionLightShaderUser
			) = 0;
		};
	}
}

#endif
