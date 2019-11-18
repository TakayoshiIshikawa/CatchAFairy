//=============================================================================
//	ITFileTextureLoader.cpp
//
//	自作ファイルテクスチャローダのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITFileTextureLoader.h"
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#include <windows.h>
#pragma warning(default:4820)
#pragma warning(default:4668)
#include <combaseapi.h>
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#pragma warning(disable:4917)
#include <wincodec.h>
#pragma warning(default:4917)
#pragma warning(default:4820)
#pragma warning(default:4668)
#pragma warning(disable:4626)
#include <wrl/client.h>
#pragma warning(default:4626)
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Common/PatternBase/SingletonManager.h"

using namespace Ishikawa::DirectX::Texture;


/// <summary>シングルトンインスタンス</summary>
FileTextureLoader* FileTextureLoader::instance = nullptr;


/// <summary>
/// インスタンスの取得[自作ファイルテクスチャローダー]
/// </summary>
/// <returns>自作ファイルテクスチャローダーのインスタンス</returns>
template<>
FileTextureLoader& Ishikawa::Common::SingletonManager::GetInstance<FileTextureLoader>(){
	if(FileTextureLoader::instance == nullptr){
		FileTextureLoader::instance = new FileTextureLoader();
	}
	return (*(FileTextureLoader::instance));
}

/// <summary>
/// WICイメージファクトリの取得
/// </summary>
/// <returns>WICイメージファクトリ</returns>
static IWICImagingFactory* GetWICImagingFactory(){
	static Microsoft::WRL::ComPtr<IWICImagingFactory> wicImagingFactory(nullptr);

	if(wicImagingFactory.Get() == nullptr){
		HRESULT hr = CoCreateInstance(
			CLSID_WICImagingFactory1,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(wicImagingFactory.ReleaseAndGetAddressOf())
		);
		if(FAILED(hr)){
			// 異常終了
			throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't create WIC imaging factory.");
		}
	}

	return wicImagingFactory.Get();
}


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
FileTextureLoader::FileTextureLoader(){
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
FileTextureLoader::~FileTextureLoader(){
	// 何もしない
}


/// <summary>
/// 読み込み
/// </summary>
/// <param name="_device">デバイス</param>
/// <param name="_filepath">ファイルパス</param>
/// <param name="_texture">テクスチャ格納先</param>
/// <param name="_shaderResourceView">シェーダリソースビュー格納先</param>
void FileTextureLoader::Load(
	ID3D11Device* const _device,
	const wchar_t* const _filepath,
	ID3D11Texture2D** _texture,
	ID3D11ShaderResourceView** _shaderResourceView
){
	NOT_USING(_device);
	NOT_USING(_filepath);
	NOT_USING(_texture);
	NOT_USING(_shaderResourceView);

	IWICImagingFactory* const wicFactory = GetWICImagingFactory();
	NOT_USING(wicFactory);

	//TODO:実装待ち
}
