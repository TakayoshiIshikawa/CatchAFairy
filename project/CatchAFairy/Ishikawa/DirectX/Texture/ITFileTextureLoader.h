//=============================================================================
//	ITFileTextureLoader.h
//
//	自作ファイルテクスチャローダのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#ifndef __IT_FILE_TEXTURE_LOADER_H__
#define __IT_FILE_TEXTURE_LOADER_H__

struct ID3D11Device;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
#include "Ishikawa/Common/PatternBase/SingletonBase.h"


namespace Ishikawa{
	namespace DirectX{
		namespace Texture{
			/// <summary>
			/// 自作ファイルテクスチャローダ
			/// </summary>
			class FileTextureLoader final : public ::Ishikawa::Common::SingletonBase{
			public:
				// GetInstance<FileTextureLoader>()でアクセスするためフレンド化
				friend class Ishikawa::Common::SingletonManager;


			private:
				// シングルトンインスタンス
				static FileTextureLoader* instance;


			private:
				// デフォルトコンストラクタ
				FileTextureLoader();
				// デストラクタ
				~FileTextureLoader();
			private:
				/// <summary>コピーコンストラクタ[削除]</summary>
				FileTextureLoader(const FileTextureLoader&) = delete;
				/// <summary>代入演算子[削除]</summary>
				FileTextureLoader& operator=(const FileTextureLoader&) = delete;

			public:
				// 読み込み
				void Load(
					ID3D11Device* const _device,
					const wchar_t* const _filepath,
					ID3D11Texture2D** _texture,
					ID3D11ShaderResourceView** _shaderResourceView
				);
			};
		}
	}
}

#endif
