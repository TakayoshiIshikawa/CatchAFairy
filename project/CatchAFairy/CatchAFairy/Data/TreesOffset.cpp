//=============================================================================
//	TreesOffset.cpp
//
//	木々初期データのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "TreesOffset.h"
#include <stdio.h>
#include <math.h>
#include <sys/stat.h>
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Common/SafePointer.h"
#include "Ishikawa/Math/ITMatrix.h"

using namespace ::CatchAFairy::Data;


/// <summary>
/// ワールド変換行列初期化用データ
/// </summary>
struct TreesOffset::WorldOffset{
private:
	/// <summary>X座標</summary>
	float positionX;
	/// <summary>Y座標</summary>
	float positionY;
	/// <summary>Z座標</summary>
	float positionZ;
	/// <summary>Y軸回転量</summary>
	float angle;
	/// <summary>幅拡大率</summary>
	float scaleOfWidth;
	/// <summary>高さ拡大率</summary>
	float scaleOfHeight;

public:
	// デフォルトコンストラクタ
	WorldOffset();

public:
	// 読み込み
	void Load(const char* const _data);
	// 行列に変換
	void ToMatrix(::Ishikawa::Math::Matrix* const _matrix) const;
};

/// <summary>
/// デフォルトコンストラクタ
/// </summary>
TreesOffset::WorldOffset::WorldOffset() :
	positionX(0.0f),
	positionY(0.0f),
	positionZ(0.0f),
	angle(0.0f),
	scaleOfWidth(0.00001f),
	scaleOfHeight(0.00001f)
{
	// 何もしない
}

/// <summary>
/// 読み込み
/// </summary>
/// <param name="_data">読み込むデータ</param>
void TreesOffset::WorldOffset::Load(const char* const _data){
	if(_data == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_data is NULL.");
	}

#pragma warning(disable:4996)
	sscanf(_data, "%f,%f,%f,%f,%f,%f",
		&(this->positionX),
		&(this->positionY),
		&(this->positionZ),
		&(this->angle),
		&(this->scaleOfWidth),
		&(this->scaleOfHeight)
	);
#pragma warning(default:4996)
}

/// <summary>
/// 行列に変換
/// </summary>
/// <param name="_matrix">保存する行列</param>
void TreesOffset::WorldOffset::ToMatrix(::Ishikawa::Math::Matrix* const _matrix) const {
	if(_matrix == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_matrix is NULL.");
	}

	// 設定
	(*_matrix) = ::Ishikawa::Math::Matrix::IDENTITY;
	float sinValue = sinf(this->angle);
	float cosValue = cosf(this->angle);
	_matrix->_11 = this->scaleOfWidth * cosValue;
	_matrix->_13 = this->scaleOfWidth * sinValue;
	_matrix->_22 = this->scaleOfHeight;
	_matrix->_31 = this->scaleOfWidth * (-sinValue);
	_matrix->_33 = this->scaleOfWidth * cosValue;
	_matrix->_41 = this->positionX;
	_matrix->_42 = this->positionY;
	_matrix->_43 = this->positionZ;
}



/// <summary>
/// デフォルトコンストラクタ
/// </summary>
TreesOffset::TreesOffset() :
	count(0U),
	worldOffsetArray(nullptr),
	worldArray(nullptr)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
TreesOffset::~TreesOffset(){
	this->Clear();
}


/// <summary>
/// 読み込み
/// </summary>
/// <param name="_csvFilepath">読み込むCSVファイルへのパス</param>
void TreesOffset::Load(const char* const _csvFilepath){
	if(_csvFilepath == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_csvFilepath is NULL.");
	}

	struct stat fileStat;
	if(stat(_csvFilepath, &fileStat) != 0){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't get stat of _csvFilepath.");
	}
	
	const long FILE_SIZE = fileStat.st_size;
	::Ishikawa::Common::SafePointer<char> fileDataWrap(new char[static_cast<unsigned int>(FILE_SIZE+1L)]{0});
	{
#pragma warning(disable:4996)
		FILE* const file = fopen(_csvFilepath, "rb");
#pragma warning(default:4996)

		fread(fileDataWrap.Get(), sizeof(char), static_cast<size_t>(FILE_SIZE), file);

		fclose(file);
	}

	const char* const fileData = fileDataWrap.Get();

	// 要素数読み込み
#pragma warning(disable:4996)
	sscanf(fileData, "%u,", &(this->count));
#pragma warning(default:4996)

	// ワールド変換行列初期化用データ配列生成
	if(this->worldOffsetArray != nullptr) delete[] this->worldOffsetArray;
	this->worldOffsetArray = new WorldOffset[this->count];

	// ワールド変換行列初期化用データ読み込み
	const char* readPoint = fileData;
	for(unsigned int i=0U; i<this->count; ++i){
		while(((*readPoint)!='\n') && ((*readPoint)!='\0')){ ++readPoint; }
		++readPoint;

		this->worldOffsetArray[i].Load(readPoint);
	}

	// ワールド変換行列配列生成
	if(this->worldArray != nullptr) delete[] this->worldArray;
	this->worldArray = new ::Ishikawa::Math::Matrix[this->count];
	for(unsigned int i=0U; i<this->count; ++i){
		this->worldOffsetArray[i].ToMatrix(&(this->worldArray[i]));
	}
}

/// <summary>
/// データ破棄
/// </summary>
void TreesOffset::Clear(){
	// ワールド変換行列配列削除
	if(this->worldArray != nullptr){
		delete[] this->worldArray;
		this->worldArray = nullptr;
	}
	// ワールド変換行列初期化用データ配列削除
	if(this->worldOffsetArray != nullptr){
		delete[] this->worldOffsetArray;
		this->worldOffsetArray = nullptr;
	}
	// 要素数を0にする
	this->count = 0U;
}
