//=============================================================================
//	ITDeviceDependentResourceListEdge.h
//
//	自作デバイス依存リソースのリスト端のヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_DEVICE_DEPENDENT_RESOURCE_LIST_EDGE_H__
#define __IT_DEVICE_DEPENDENT_RESOURCE_LIST_EDGE_H__

#include "ITDeviceDependentResourceInterface.h"


namespace Ishikawa{
	namespace DirectX{
		namespace Device{
			namespace DependentResource{
				/// <summary>
				/// 自作デバイス依存リソースのリスト端
				/// </summary>
				class ListEdge final : public Interface{
				private:
					/// <summary>次の自作デバイス依存リソース</summary>
					Interface* nextITDeviceDependentResource;
					/// <summary>前の自作デバイス依存リソース</summary>
					Interface* beforeITDeviceDependentResource;


				public:
					// デフォルトコンストラクタ
					ListEdge();
					// デストラクタ
					virtual ~ListEdge() override final;

				public:
					/// <summary>自作デバイス依存リソースリストの端か?</summary>
					/// <returns>常にtrue</returns>
					virtual bool IsITDeviceDependentResourceListEdge() const override final { return true; }
					/// <summary>依存している自作デバイスの取得[できない]</summary>
					/// <returns>常にNULL</returns>
					virtual Device* GetDependentITDevice() const override final { return nullptr; }
					/// <summary>次の自作デバイス依存リソースの取得</summary>
					/// <returns>次の自作デバイス依存リソース</returns>
					virtual Interface* GetNextITDeviceDependentResource() const override final {
						return this->nextITDeviceDependentResource;
					}
					/// <summary>前の自作デバイス依存リソースの取得</summary>
					/// <returns>前の自作デバイス依存リソース</returns>
					virtual Interface* GetBeforeITDeviceDependentResource() const override final {
						return this->beforeITDeviceDependentResource;
					}

				public:
					/// <summary>依存する自作デバイスの設定[できない]</summary>
					virtual void SetDependentITDevice(Device* const) override final{}
					/// <summary>次の自作デバイス依存リソースの設定</summary>
					/// <param name="_next">次の自作デバイス依存リソース</param>
					virtual void SetNextITDeviceDependentResource(Interface* const _next) override final {
						this->nextITDeviceDependentResource = _next;
					}
					/// <summary>前の自作デバイス依存リソースの設定</summary>
					/// <param name="_before">前の自作デバイス依存リソース</param>
					virtual void SetBeforeITDeviceDependentResource(Interface* const _before) override final {
						this->beforeITDeviceDependentResource = _before;
					}

				public:
					/// <summary>デバイスに読み込まれた</summary>
					virtual void OnDeviceLoaded() override final{}
					/// <summary>デバイスが復元された</summary>
					virtual void OnDeviceRestored() override final{}
					/// <summary>デバイスが破棄された</summary>
					virtual void OnDeviceLost() override final{}

				public:
					/// <summary>リストから外れる[外れない]</summary>
					virtual void RemoveITDeviceDependentResourceList() override final{}

					// 初期化
					void Initialize();

				protected:
					// 自作デバイス依存リソースの初期化
					virtual void InitializeITDeviceDependentResource() override final;
				};
			}
		}
	}
}

#endif
