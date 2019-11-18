//=============================================================================
//	CameraObjectInterface.h
//
//	カメラオブジェクトインターフェースのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __CAMERA_OBJECT_INTERFACE_H__
#define __CAMERA_OBJECT_INTERFACE_H__

struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;

namespace Ishikawa{
	namespace Math{
		struct Matrix;
	}
	namespace DirectX{
		namespace Device{
			class Device;
		}
	}
}

namespace CatchAFairy{
	namespace Object{
		/// <summary>
		/// カメラオブジェクトインターフェース
		/// </summary>
		class CameraInterface{
		public:
			/// <summary>デフォルトコンストラクタ</summary>
			CameraInterface(){}
			/// <summary>デストラクタ</summary>
			virtual ~CameraInterface(){}

		public:
			// ワールド変換行列の取得
			virtual const ::Ishikawa::Math::Matrix& GetWorld() const = 0;
			// ビュー変換行列の取得
			virtual const ::Ishikawa::Math::Matrix& GetView() const = 0;
			// プロジェクション変換行列の取得
			virtual const ::Ishikawa::Math::Matrix& GetProjection() const = 0;
			// 最終シェーダリソースビューの取得
			virtual ID3D11ShaderResourceView* GetLastShaderResourceView() const = 0;

		public:
			// ゲームのデバイスに依存するリソースの追加
			virtual void AddITDeviceDependentResources(::Ishikawa::DirectX::Device::Device* const _itDevice) = 0;
			// デバイス依存リソース作成
			virtual void CreateDeviceDependentResources() = 0;
			// デバイス依存リソース開放
			virtual void ReleaseDeviceDependentResources() = 0;
		};
	}
}

#endif
