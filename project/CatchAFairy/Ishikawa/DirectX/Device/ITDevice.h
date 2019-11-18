//=============================================================================
//	ITDevice.h
//
//	自作デバイスのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_DEVICE_H__
#define __IT_DEVICE_H__

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct IDXGIAdapter1;
#pragma warning(disable:4471)
enum D3D_FEATURE_LEVEL;
#pragma warning(default:4471)

#include "DependentResource/ITDeviceDependentResourceListEdge.h"


namespace Ishikawa{
	namespace DirectX{
		namespace Device{
			/// <summary>
			/// 自作デバイス
			/// </summary>
			class Device final{
			private:
				/// <summary>デバイス</summary>
				ID3D11Device* device;
				/// <summary>フューチャレベル</summary>
				D3D_FEATURE_LEVEL featureLevel;
				/// <summary>コンテキスト</summary>
				ID3D11DeviceContext* context;
				/// <summary>このデバイスに依存しているリソースリストの端</summary>
				DependentResource::ListEdge deviceDependentResourceListEdge;


			public:
				// デフォルトコンストラクタ
				Device();
				// デストラクタ
				~Device();

			public:
				/// <summary>デバイスの取得</summary>
				/// <returns>デバイス</returns>
				ID3D11Device* GetDevice() const { return this->device; }
				/// <summary>コンテキストの取得</summary>
				/// <returns>コンテキスト</returns>
				ID3D11DeviceContext* GetContext() const { return this->context; }

			public:
				// 初期化
				void Initialize();
				// 作成
				void Create();
				// 復元
				void Restore();
				// ロスト
				void Lost();

				// 依存リソースの追加[1つだけ]
				void AddDependentResource(DependentResource::Interface* const _dependentResource);
				// 依存リソースの追加[循環リストごと]
				// 追加した後は元リストは空になります
				void AddDependentResourceList(DependentResource::ListEdge* const _dependentResourceListEdge);

			private:
				// デバイスの生成
				void CreateDevice(IDXGIAdapter1* const _adapter1);
				// アダプタの取得
				void GetAdapter(IDXGIAdapter1** const _adapter1);

				// デバイスの開放
				void ReleaseDevice();
				// コンテキストの開放
				void ReleaseContext();
			};
		}
	}
}

#endif
