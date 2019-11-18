//=============================================================================
//	ITFileTexture.h
//
//	自作ファイルテクスチャのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#ifndef __IT_FILE_TEXTURE_H__
#define __IT_FILE_TEXTURE_H__

#include "Ishikawa/DirectX/Device/DependentResource/ITDeviceDependentResourceBase.h"
#include "ITTexture2DInterface.h"
#include "ITShaderResourceInterface.h"


namespace Ishikawa{
	namespace DirectX{
		namespace Texture{
			/// <summary>
			/// 自作ファイルテクスチャ
			/// </summary>
			class FileTexture final :
				public ::Ishikawa::DirectX::Device::DependentResource::Base,
				public Texture2DInterface,
				public ShaderResourceInterface
			{
			private:
				/// <summary>ファイルパス</summary>
				wchar_t* filepath;
				/// <summary>シェーダリソースビュー</summary>
				ID3D11ShaderResourceView* shaderResourceView;
				/// <summary>幅</summary>
				unsigned int width;
				/// <summary>高さ</summary>
				unsigned int height;


			public:
				// デフォルトコンストラクタ
				FileTexture();
				// デストラクタ
				virtual ~FileTexture() override final;

			public:
				/// <summary>テクスチャサイズの取得</summary>
				/// <param name="_width">幅の格納先</param>
				/// <param name="_height">高さの格納先</param>
				virtual void GetTextureSize(unsigned int* const _width, unsigned int* const _height) const override final{
					if(_width != nullptr) (*_width) = this->width;
					if(_height != nullptr) (*_height) = this->height;
				}
				/// <summary>シェーダリソースビューの取得</summary>
				/// <returns>シェーダリソースビュー</returns>
				virtual ID3D11ShaderResourceView* GetShaderResourceView() const override final{
					return this->shaderResourceView;
				}

			public:
				// デバイスに読み込まれた
				virtual void OnDeviceLoaded() override final;
				// デバイスが復元された
				virtual void OnDeviceRestored() override final;
				// デバイスが破棄された
				virtual void OnDeviceLost() override final;

			public:
				// 初期化
				void Initialize();
				// 読み込み
				void Load(const wchar_t* const _filepath);

			private:
				// ファイルパスの変更
				void ChangeFilepath(const wchar_t* const _filepath);
				// シェーダリソースビューの開放
				void ReleaseShaderResourceView();
			};
		}
	}
}

#endif
