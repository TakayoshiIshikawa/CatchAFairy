//=============================================================================
//	Numbers.cpp
//
//	数値表示のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "Numbers.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"

using namespace ::CatchAFairy::Object::PlayScene::GUI;


/// <summary>形状テクスチャコーディネート配列(左上と右下)</summary>
const ::Ishikawa::Math::Float4 Numbers::SHAPE_TEXTURE_COORDINATE[10] = {
	::Ishikawa::Math::Float4(0.01f/32.0f, 0.0f, 1.0f/32.0f, 0.99f/32.0f),
	::Ishikawa::Math::Float4(1.0f/32.0f, 0.0f, 2.0f/32.0f, 1.0f/32.0f),
	::Ishikawa::Math::Float4(2.0f/32.0f, 0.0f, 3.0f/32.0f, 1.0f/32.0f),
	::Ishikawa::Math::Float4(3.0f/32.0f, 0.0f, 4.0f/32.0f, 1.0f/32.0f),
	::Ishikawa::Math::Float4(4.0f/32.0f, 0.0f, 5.0f/32.0f, 1.0f/32.0f),
	::Ishikawa::Math::Float4(5.0f/32.0f, 0.0f, 6.0f/32.0f, 1.0f/32.0f),
	::Ishikawa::Math::Float4(6.0f/32.0f, 0.0f, 7.0f/32.0f, 1.0f/32.0f),
	::Ishikawa::Math::Float4(7.0f/32.0f, 0.0f, 8.0f/32.0f, 1.0f/32.0f),
	::Ishikawa::Math::Float4(8.0f/32.0f, 0.0f, 9.0f/32.0f, 1.0f/32.0f),
	::Ishikawa::Math::Float4(9.0f/32.0f, 0.0f,10.0f/32.0f, 1.0f/32.0f)
};
/// <summary>表示テクスチャコーディネート(左上と右下)</summary>
const ::Ishikawa::Math::Float4 Numbers::VIEW_TEXTURE_COORDINATE(0.1f/32.0f, 0.1f/32.0f, 0.9f/32.0f, 0.9f/32.0f);


/// <summary>
/// 形状テクスチャコーディネートの取得
/// </summary>
/// <param name="_number">数字 [0～9]</param>
/// <returns>形状テクスチャコーディネート</returns>
const ::Ishikawa::Math::Float4& Numbers::GetShapeTextureCoordinate(int _number){
	if((_number < 0) || (_number > 9)){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_number was out of range.");
	}

	return Numbers::SHAPE_TEXTURE_COORDINATE[_number];
}

/// <summary>
/// 表示テクスチャコーディネートの取得
/// </summary>
/// <returns>表示テクスチャコーディネート</returns>
const ::Ishikawa::Math::Float4& Numbers::GetViewTextureCoordinate(){
	return Numbers::VIEW_TEXTURE_COORDINATE;
}
