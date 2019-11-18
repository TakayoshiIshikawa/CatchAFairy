//=============================================================================
//	Numbers.h
//
//	数値情報のヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __NUMBERS_H__
#define __NUMBERS_H__

#include "Ishikawa/Math/ITFloatN.h"

namespace CatchAFairy{
	namespace Object{
		namespace PlayScene{
			namespace GUI{
				/// <summary>
				/// 数値情報
				/// </summary>
				class Numbers final{
				private:
					// 形状テクスチャコーディネート配列(左上と右下)
					static const ::Ishikawa::Math::Float4 SHAPE_TEXTURE_COORDINATE[10];
					// 表示テクスチャコーディネート(左上と右下)
					static const ::Ishikawa::Math::Float4 VIEW_TEXTURE_COORDINATE;


				public:
					// 形状テクスチャコーディネートの取得
					static const ::Ishikawa::Math::Float4& GetShapeTextureCoordinate(int _number);
					// 表示テクスチャコーディネートの取得
					static const ::Ishikawa::Math::Float4& GetViewTextureCoordinate();

				private:
					/// <summary>デフォルトコンストラクタ [削除]</summary>
					Numbers() = delete;
					/// <summary>コピーコンストラクタ [削除]</summary>
					Numbers(const Numbers&) = delete;
					/// <summary>デストラクタ [削除]</summary>
					~Numbers() = delete;
					/// <summary>代入演算子 [削除]</summary>
					Numbers& operator=(const Numbers&) = delete;
				};
			}
		}
	}
}

#endif
