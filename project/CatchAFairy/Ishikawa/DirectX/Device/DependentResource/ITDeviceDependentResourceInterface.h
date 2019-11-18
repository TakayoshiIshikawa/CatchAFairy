//=============================================================================
//	ITDeviceDependentResourceInterface.h
//
//	自作デバイス依存リソースインターフェースのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_DEVICE_DEPENDENT_RESOURCE_INTERFACE_H__
#define __IT_DEVICE_DEPENDENT_RESOURCE_INTERFACE_H__

namespace Ishikawa{
	namespace DirectX{
		namespace Device{
			class Device;


			namespace DependentResource{
				/// <summary>
				/// 自作デバイス依存リソースインターフェース
				/// </summary>
				class Interface{
				public:
					/// <summary>デフォルトコンストラクタ</summary>
					Interface(){}
					/// <summary>デストラクタ</summary>
					virtual ~Interface(){}

				public:
					// 自作デバイス依存リソースリストの端か?
					virtual bool IsITDeviceDependentResourceListEdge() const = 0;
					// 依存している自作デバイスの取得
					virtual Device* GetDependentITDevice() const = 0;
					// 次の自作デバイス依存リソースの取得
					virtual Interface* GetNextITDeviceDependentResource() const = 0;
					// 前の自作デバイス依存リソースの取得
					virtual Interface* GetBeforeITDeviceDependentResource() const = 0;

				public:
					// 依存する自作デバイスの設定
					virtual void SetDependentITDevice(Device* const _device) = 0;
					// 次の自作デバイス依存リソースの設定
					virtual void SetNextITDeviceDependentResource(Interface* const _next) = 0;
					// 前の自作デバイス依存リソースの設定
					virtual void SetBeforeITDeviceDependentResource(Interface* const _before) = 0;

				public:
					// デバイスに読み込まれた
					virtual void OnDeviceLoaded() = 0;
					// デバイスが復元された
					virtual void OnDeviceRestored() = 0;
					// デバイスが破棄された
					virtual void OnDeviceLost() = 0;

				public:
					// リストから外れる
					virtual void RemoveITDeviceDependentResourceList() = 0;

				protected:
					// 自作デバイス依存リソースの初期化
					virtual void InitializeITDeviceDependentResource() = 0;
				};
			}
		}
	}
}

#endif
