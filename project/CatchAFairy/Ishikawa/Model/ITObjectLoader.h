//=============================================================================
//	ITObjectLoader.h
//
//	自作オブジェクトファイルローダのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#ifndef __IT_OBJECT_FILE_LOADER_H__
#define __IT_OBJECT_FILE_LOADER_H__

struct _iobuf;
#include "Ishikawa/Math/ITFloatN.h"


namespace Ishikawa{
	namespace Model{
		namespace ItObject{
			/// <summary>
			/// 自作オブジェクト頂点データ
			/// </summary>
			struct VertexData{
				/// <summary>座標</summary>
				::Ishikawa::Math::Float3 position;
				/// <summary>法線</summary>
				::Ishikawa::Math::Float3 normal;
				/// <summary>色</summary>
				::Ishikawa::Math::Float4 color;
				/// <summary>テクスチャコーディネート</summary>
				::Ishikawa::Math::Float2 textureCoordinate;
			};

			/// <summary>
			/// 自作オブジェクトファイルローダ
			/// </summary>
			class FileLoader{
			public:
				// デフォルトコンストラクタ
				FileLoader();
				// デストラクタ
				~FileLoader();

			public:
				// 読み込み
				void Load(
					VertexData** const _vertices,
					unsigned int* const _vertexCount,
					unsigned long** const _indices,
					unsigned int* const _indexCount,
					::Ishikawa::Math::Float3* const _circumscribedCircleCenterPosition,
					float* const _squareCircumscribedCircleRadius,
					const char* const _filepath
				) const;
				// 読み込み
				void Load(
					VertexData** const _vertices,
					unsigned int* const _vertexCount,
					unsigned long** const _indices,
					unsigned int* const _indexCount,
					::Ishikawa::Math::Float3* const _circumscribedCircleCenterPosition,
					float* const _squareCircumscribedCircleRadius,
					const wchar_t* const _filepath
				) const;

			public:
				// 読み込み
				void Load(
					VertexData** const _vertices,
					unsigned int* const _vertexCount,
					unsigned long** const _indices,
					unsigned int* const _indexCount,
					::Ishikawa::Math::Float3* const _circumscribedCircleCenterPosition,
					float* const _squareCircumscribedCircleRadius,
					_iobuf* const _file
				) const;
			};
		}
	}
}

#endif
