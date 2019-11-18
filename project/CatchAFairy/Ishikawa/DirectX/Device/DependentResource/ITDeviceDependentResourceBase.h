//=============================================================================
//	ITDeviceDependentResourceBase.h
//
//	自作デバイス依存リソースの基底のヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_DEVICE_DEPENDENT_RESOURCE_BASE_H__
#define __IT_DEVICE_DEPENDENT_RESOURCE_BASE_H__

#include "ITDeviceDependentResourceInterface.h"


namespace Ishikawa{
	namespace DirectX{
		namespace Device{
			namespace DependentResource{
				/// <summary>
				/// 自作デバイス依存リソースの基底
				/// </summary>
				class Base : public Interface{
				private:
					/// <summary>依存している自作デバイス</summary>
					Device* dependentDevice;
					/// <summary>次の自作デバイス依存リソース</summary>
					Interface* nextITDeviceDependentResource;
					/// <summary>前の自作デバイス依存リソース</summary>
					Interface* beforeITDeviceDependentResource;


				public:
					// デフォルトコンストラクタ
					Base();
					// デストラクタ
					virtual ~Base() override;

				public:
					/// <summary>自作デバイス依存リソースリストの端か?</summary>
					/// <returns>常にtrue</returns>
					virtual bool IsITDeviceDependentResourceListEdge() const override final { return true; }
					/// <summary>依存している自作デバイスの取得</summary>
					/// <returns>依存している自作デバイス</returns>
					virtual Device* GetDependentITDevice() const override final {
						return this->dependentDevice;
					}
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
					/// <summary>依存する自作デバイスの設定</summary>
					/// <param name="_device">デバイス</param>
					virtual void SetDependentITDevice(Device* const _device) override final {
						this->dependentDevice = _device;
					}
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
					// リストから外れる
					virtual void RemoveITDeviceDependentResourceList() override final;

				protected:
					// 自作デバイス依存リソースの初期化
					virtual void InitializeITDeviceDependentResource() override final;
				};
			}
		}
	}
}

#endif
