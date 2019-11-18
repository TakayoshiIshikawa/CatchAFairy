//=============================================================================
//	ITRenderTargetInterface.h
//
//	自作レンダーターゲットインターフェースのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_RENDER_TARGET_INTERFACE_H__
#define __IT_RENDER_TARGET_INTERFACE_H__

struct ID3D11RenderTargetView;

namespace Ishikawa{
	namespace DirectX{
		namespace Texture{
			/// <summary>
			/// 自作レンダーターゲットインターフェース
			/// </summary>
			class RenderTargetInterface{
			public:
				/// <summary>デフォルトコンストラクタ</summary>
				RenderTargetInterface() = default;
				/// <summary>デストラクタ</summary>
				virtual ~RenderTargetInterface() = default;

			public:
				// レンダーターゲットビューの取得
				virtual ID3D11RenderTargetView* GetRenderTargetView() const = 0;
			};
		}
	}
}

#endif
