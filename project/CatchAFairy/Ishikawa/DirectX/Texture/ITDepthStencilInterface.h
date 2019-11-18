//=============================================================================
//	ITDepthStancilInterface.h
//
//	自作デプスステンシルインターフェースのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_DEPTH_STENCIL_INTERFACE_H__
#define __IT_DEPTH_STENCIL_INTERFACE_H__

struct ID3D11DepthStencilView;

namespace Ishikawa{
	namespace DirectX{
		namespace Texture{
			/// <summary>
			/// 自作デプスステンシルインターフェース
			/// </summary>
			class DepthStencilInterface{
			public:
				/// <summary>デフォルトコンストラクタ</summary>
				DepthStencilInterface() = default;
				/// <summary>デストラクタ</summary>
				virtual ~DepthStencilInterface() = default;

			public:
				// デプスステンシルビューの取得
				virtual ID3D11DepthStencilView* GetDepthStencilView() const = 0;
			};
		}
	}
}

#endif
