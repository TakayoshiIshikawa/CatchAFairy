//=============================================================================
//	ITShaderResourceInterface.h
//
//	自作シェーダリソースインターフェースのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_SHADER_RESOURCE_INTERFACE_H__
#define __IT_SHADER_RESOURCE_INTERFACE_H__

struct ID3D11ShaderResourceView;

namespace Ishikawa{
	namespace DirectX{
		namespace Texture{
			/// <summary>
			/// 自作シェーダリソースインターフェース
			/// </summary>
			class ShaderResourceInterface{
			public:
				/// <summary>デフォルトコンストラクタ</summary>
				ShaderResourceInterface() = default;
				/// <summary>デストラクタ</summary>
				virtual ~ShaderResourceInterface() = default;

			public:
				// シェーダリソースビューの取得
				virtual ID3D11ShaderResourceView* GetShaderResourceView() const = 0;
			};
		}
	}
}

#endif
