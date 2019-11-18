//=============================================================================
//	ITTexture2DInterface.h
//
//	自作2Dテクスチャインターフェースのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_TEXTURE_2D_INTERFACE_H__
#define __IT_TEXTURE_2D_INTERFACE_H__

namespace Ishikawa{
	namespace DirectX{
		namespace Texture{
			/// <summary>
			/// 自作2Dテクスチャインターフェース
			/// </summary>
			class Texture2DInterface{
			public:
				/// <summary>デフォルトコンストラクタ</summary>
				Texture2DInterface() = default;
				/// <summary>デストラクタ</summary>
				virtual ~Texture2DInterface() = default;

			public:
				// テクスチャサイズの取得
				virtual void GetTextureSize(unsigned int* const _width, unsigned int* const _height) const = 0;
			};
		}
	}
}

#endif
