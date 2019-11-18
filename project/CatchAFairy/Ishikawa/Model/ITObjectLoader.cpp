//=============================================================================
//	ITObjectLoader.cpp
//
//	自作オブジェクトファイルローダのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITObjectLoader.h"
#include <stdio.h>
#include "Ishikawa/Common/Exception/ITExceptions.h"

using namespace Ishikawa::Model::ItObject;


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
FileLoader::FileLoader(){
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
FileLoader::~FileLoader(){
	// 何もしない
}


/// <summary>
/// 読み込み
/// </summary>
/// <param name="_vertices">頂点配列格納先</param>
/// <param name="_vertexCount">頂点数格納先</param>
/// <param name="_indices">インデックス配列格納先</param>
/// <param name="_indexCount">インデックス数格納先</param>
/// <param name="_circumscribedCircleCenterPosition">外接円中心座標格納先</param>
/// <param name="_squareCircumscribedCircleRadius">外接円半径二乗納先</param>
/// <param name="_filepath">読み込みファイルパス</param>
void FileLoader::Load(
	VertexData** const _vertices,
	unsigned int* const _vertexCount,
	unsigned long** const _indices,
	unsigned int* const _indexCount,
	::Ishikawa::Math::Float3* const _circumscribedCircleCenterPosition,
	float* const _squareCircumscribedCircleRadius,
	const char* const _filepath
) const {
	if(_filepath == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_filepath is NULL.");
	}

#pragma warning(disable:4996)
	this->Load(
		_vertices, _vertexCount, _indices, _indexCount,
		_circumscribedCircleCenterPosition, _squareCircumscribedCircleRadius,
		fopen(_filepath, "rb")
	);
#pragma warning(default:4996)
}

/// <summary>
/// 読み込み
/// </summary>
/// <param name="_vertices">頂点配列格納先</param>
/// <param name="_vertexCount">頂点数格納先</param>
/// <param name="_indices">インデックス配列格納先</param>
/// <param name="_indexCount">インデックス数格納先</param>
/// <param name="_circumscribedCircleCenterPosition">外接円中心座標格納先</param>
/// <param name="_squareCircumscribedCircleRadius">外接円半径二乗納先</param>
/// <param name="_filepath">読み込みファイルパス</param>
void FileLoader::Load(
	VertexData** const _vertices,
	unsigned int* const _vertexCount,
	unsigned long** const _indices,
	unsigned int* const _indexCount,
	::Ishikawa::Math::Float3* const _circumscribedCircleCenterPosition,
	float* const _squareCircumscribedCircleRadius,
	const wchar_t* const _filepath
) const {
	if(_filepath == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_filepath is NULL.");
	}

#pragma warning(disable:4996)
	this->Load(
		_vertices, _vertexCount, _indices, _indexCount,
		_circumscribedCircleCenterPosition, _squareCircumscribedCircleRadius,
		_wfopen(_filepath, L"rb")
	);
#pragma warning(default:4996)
}

/// <summary>
/// 読み込み
/// </summary>
/// <param name="_vertices">頂点配列格納先</param>
/// <param name="_vertexCount">頂点数格納先</param>
/// <param name="_indices">インデックス配列格納先</param>
/// <param name="_indexCount">インデックス数格納先</param>
/// <param name="_circumscribedCircleCenterPosition">外接円中心座標格納先</param>
/// <param name="_squareCircumscribedCircleRadius">外接円半径二乗納先</param>
/// <param name="_file">読み込みファイル</param>
void FileLoader::Load(
	VertexData** const _vertices,
	unsigned int* const _vertexCount,
	unsigned long** const _indices,
	unsigned int* const _indexCount,
	::Ishikawa::Math::Float3* const _circumscribedCircleCenterPosition,
	float* const _squareCircumscribedCircleRadius,
	_iobuf* const _file
) const {
	if(_vertices == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_vertices is NULL.");
	}
	if(_vertexCount == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_vertexCount is NULL.");
	}
	if(_indices == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_indices is NULL.");
	}
	if(_indexCount == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_indexCount is NULL.");
	}
	if(_circumscribedCircleCenterPosition == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_circumscribedCircleCenterPosition is NULL.");
	}
	if(_squareCircumscribedCircleRadius == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_squareCircumscribedCircleRadius is NULL.");
	}
	if(_file == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't open ITObject file.");
	}


	// 頂点数読み取り
	(*_vertices) = nullptr;
	fread(_vertexCount, sizeof(unsigned long), 1U, _file);
	if((*_vertexCount) > 0){
		// 頂点配列作成
		(*_vertices) = new VertexData[(*_vertexCount)];
		// 頂点読み取り
		fread((*_vertices), sizeof(VertexData), (*_vertexCount), _file);

		// 外接するAABBを求める
		::Ishikawa::Math::Float3 min, max;
		min = max = (*_vertices)[0UL].position;
		for(unsigned long i=1UL; i<(*_vertexCount); ++i){
			const ::Ishikawa::Math::Float3& pos = (*_vertices)[i].position;
			if(pos.x < min.x) min.x = pos.x;
			if(pos.y < min.y) min.y = pos.y;
			if(pos.z < min.z) min.z = pos.z;
			if(pos.x > max.x) max.x = pos.x;
			if(pos.y > max.y) max.y = pos.y;
			if(pos.z > max.z) max.z = pos.z;
		}

		(*_circumscribedCircleCenterPosition) = (min + max) * 0.5f;
		(*_squareCircumscribedCircleRadius) = (max - min).LengthSquared();
	}

	// インデックス数読み取り
	(*_indices) = nullptr;
	fread(_indexCount, sizeof(unsigned long), 1U, _file);
	if((*_indexCount) > 0){
		// インデックス配列作成
		(*_indices) = new unsigned long[(*_indexCount)];
		// インデックス読み取り
		fread((*_indices), sizeof(unsigned long), (*_indexCount), _file);
	}


	fclose(_file);
}
