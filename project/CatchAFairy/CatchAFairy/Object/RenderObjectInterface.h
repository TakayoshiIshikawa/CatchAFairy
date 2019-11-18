//=============================================================================
//	RenderObjectInterface.h
//
//	描画オブジェクトインターフェースのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __RENDER_OBJECT_INTERFACE_H__
#define __RENDER_OBJECT_INTERFACE_H__

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

namespace Ishikawa{
	namespace Math{
		struct Float3;
		struct Matrix;
	}
}

namespace CatchAFairy{
	namespace Shader{
		namespace User{
			enum class VertexTopology : int;
		}
	}


	namespace Object{
		/// <summary>
		/// 描画オブジェクトインターフェース
		/// </summary>
		class RenderInterface{
		public:
			/// <summary>デフォルトコンストラクタ</summary>
			RenderInterface(){}
			/// <summary>デストラクタ</summary>
			virtual ~RenderInterface(){}

		public:
			// トポロジータイプの取得
			virtual ::CatchAFairy::Shader::User::VertexTopology GetTopology() const = 0;
			// 頂点バッファの取得
			virtual ID3D11Buffer* GetVertexBuffer() const = 0;
			// インデックスバッファの取得
			virtual ID3D11Buffer* GetIndexBuffer() const = 0;
			// インデックス数の取得
			virtual unsigned int GetIndexCount() const = 0;
			// 外接円中心座標の取得
			virtual const ::Ishikawa::Math::Float3& GetCircumscribedCircleCenterPosition() const = 0;
			// 外接円半径二乗の取得
			virtual float GetSquareCircumscribedCircleRadius() const = 0;
			// 拡散反射シェーダリソースビューの取得
			virtual ID3D11ShaderResourceView* GetDiffuseShaderResourceView() const = 0;
			// 法線シェーダリソースビューの取得
			virtual ID3D11ShaderResourceView* GetNormalShaderResourceView() const = 0;
			// ワールド変換行列の取得
			virtual const ::Ishikawa::Math::Matrix& GetWorld() const = 0;
		};
	}
}

#endif
