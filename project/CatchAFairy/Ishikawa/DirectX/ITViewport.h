//=============================================================================
//	ITViewport.h
//
//	自作ビューポートのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_VIEWPORT_H__
#define __IT_VIEWPORT_H__

struct D3D11_VIEWPORT;


namespace Ishikawa{
	namespace DirectX{
		/// <summary>
		/// 自作ビューポート
		/// </summary>
		struct Viewport{
		public:
			/// <summary>左上のX座標</summary>
			float leftTopX;
			/// <summary>左上のY座標</summary>
			float leftTopY;
			/// <summary>幅</summary>
			float width;
			/// <summary>高さ</summary>
			float height;
			/// <summary>最小深度</summary>
			float minDepth;
			/// <summary>最大深度</summary>
			float maxDepth;


		public:
			/// <summary>デフォルトコンストラクタ</summary>
			Viewport() :
				leftTopX(0.0f), leftTopY(0.0f),
				width(0.0f), height(0.0f),
				minDepth(0.0f), maxDepth(0.0f)
			{}
			/// <summary>フルコンストラクタ</summary>
			/// <param name="_leftTopX">左上のX座標</param>
			/// <param name="_leftTopY">左上のY座標</param>
			/// <param name="_width">幅</param>
			/// <param name="_height">高さ</param>
			/// <param name="_minDepth">最小深度</param>
			/// <param name="_maxDepth">最大深度</param>
			Viewport(
				float _leftTopX, float _leftTopY,
				float _width, float _height,
				float _minDepth, float _maxDepth
			) :
				leftTopX(_leftTopX), leftTopY(_leftTopY),
				width(_width), height(_height),
				minDepth(_minDepth), maxDepth(_maxDepth)
			{}
			/// <summary>コピーコンストラクタ</summary>
			/// <param name="_original">オリジナルデータ</param>
			Viewport(const Viewport& _original) :
				leftTopX(_original.leftTopX), leftTopY(_original.leftTopY),
				width(_original.width), height(_original.height),
				minDepth(_original.minDepth), maxDepth(_original.maxDepth)
			{}
			/// <summary>デストラクタ</summary>
			~Viewport(){}

		public:
			/// <summary>コピーコンストラクタ</summary>
			/// <param name="_original">オリジナルデータ</param>
			/// <returns>自身のデータ</returns>
			Viewport& operator=(const Viewport& _original){
				this->leftTopX	= _original.leftTopX;
				this->leftTopY	= _original.leftTopY;
				this->width		= _original.width;
				this->height	= _original.height;
				this->minDepth	= _original.minDepth;
				this->maxDepth	= _original.maxDepth;
				return (*this);
			}
		};

		// 自作ビューポートからD3D11ビューポートに変換
		D3D11_VIEWPORT ConvertViewportToD3D11Viewport(const Viewport& _viewport);
	}
}

#endif
