//=============================================================================
//	ITSwapChain.h
//
//	自作デバイスのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_SWAP_CHAIN_H__
#define __IT_SWAP_CHAIN_H__

struct HWND__;
struct ID3D11Device;
struct IDXGISwapChain;
struct IDXGIFactory;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
#pragma warning(disable:4471)
enum DXGI_FORMAT;
enum D3D11_DSV_DIMENSION;
#pragma warning(default:4471)


namespace Ishikawa{
	namespace Window{
		class Window;
	}
	namespace DirectX{
		namespace Device{
			class Device;
		}


		/// <summary>
		/// 自作スワップチェーン
		/// </summary>
		class SwapChain final{
		private:
			/// <summary>サンプル設定カウント</summary>
			unsigned int sampleDescCount;
			/// <summary>サンプル設定クォリティ</summary>
			unsigned int sampleDescQuality;
			/// <summary>スワップチェーン</summary>
			IDXGISwapChain* swapChain;
			/// <summary>バッファカウント</summary>
			unsigned int bufferCount;
			/// <summary>バッファフォーマット</summary>
			DXGI_FORMAT bufferFormat;
			/// <summary>幅</summary>
			unsigned int width;
			/// <summary>高さ</summary>
			unsigned int height;
			/// <summary>レンダーターゲットテクスチャ</summary>
			ID3D11Texture2D* renderTargetTexture;
			/// <summary>レンダーターゲットビュー</summary>
			ID3D11RenderTargetView* renderTargetView;
			/// <summary>デプスステンシルフォーマット</summary>
			DXGI_FORMAT depthStencilFormat;
			/// <summary>デプスステンシルビューディメンション</summary>
			D3D11_DSV_DIMENSION depthStencilViewDimention;
			/// <summary>デプスステンシルテクスチャ</summary>
			ID3D11Texture2D* depthStencilTexture;
			/// <summary>デプスステンシルビュー</summary>
			ID3D11DepthStencilView* depthStencilView;
			/// <summary>依存デバイス</summary>
			Device::Device* dependentDevice;


		public:
			// デフォルトコンストラクタ
			SwapChain();
			// デストラクタ
			~SwapChain();

		public:
			/// <summary>スワップチェーンの取得</summary>
			/// <returns>スワップチェーン</returns>
			IDXGISwapChain* GetSwapChain() const { return this->swapChain; }
			/// <summary>レンダーターゲットビューの取得</summary>
			/// <returns>レンダーターゲットビュー</returns>
			ID3D11RenderTargetView* GetRenderTargetView() const { return this->renderTargetView; }
			/// <summary>デプスステンシルビューの取得</summary>
			/// <returns>デプスステンシルビュー</returns>
			ID3D11DepthStencilView* GetDepthStencilView() const { return this->depthStencilView; }
			/// <summary>サイズの取得</summary>
			/// <param name="_width">幅の格納先</param>
			/// <param name="_height">高さの格納先</param>
			void GetSize(unsigned int* const _width, unsigned int* const _height) const {
				if(_width != nullptr) (*_width) = this->width;
				if(_height != nullptr) (*_height) = this->height;
			}

		public:
			// 作成
			void Create(Device::Device* const _device, ::Ishikawa::Window::Window* const _window);
			// サイズ変更
			void ChangeSize(unsigned int _width, unsigned int _height);

			// 画面のクリア
			void ClearScreen();
			// バッファを入れ替えて表示
			void Present();

		private:
			// スワップチェーンの作成
			void CreateSwapChain(
				IDXGIFactory* const _factory,
				ID3D11Device* const _device,
				HWND__* const _windowHandle
			);
			// ファクトリの取得
			void GetFactory(IDXGIFactory** const _factory, ID3D11Device* const _device);
			// デフォルトサンプル設定に変更
			void ChangeDefaultSampleDesc();
			// マルチサンプル・アンチエイリアシング設定に変更
			void ChangeMultisampleAntiAliasing(ID3D11Device* const _device);
			// レンダーターゲットテクスチャの作成
			void CreateRenderTargetTexture();
			// レンダーターゲットビューの作成
			void CreateRenderTargetView(ID3D11Device* const _device);
			// デプスステンシルテクスチャの作成
			void CreateDepthStencilTexture(ID3D11Device* const _device);
			// デプスステンシルビューの作成
			void CreateDepthStencilView(ID3D11Device* const _device);

			// スワップチェーンの開放
			void ReleaseSwapChain();
			// レンダーターゲットテクスチャの開放
			void ReleaseRenderTargetTexture();
			// レンダーターゲットビューの開放
			void ReleaseRenderTargetView();
			// デプスステンシルテクスチャの開放
			void ReleaseDepthStencilTexture();
			// デプスステンシルビューの開放
			void ReleaseDepthStencilView();
		};
	}
}

#endif
