//=============================================================================
//	TreesOffset.cpp
//
//	�؁X�����f�[�^�̃\�[�X�t�@�C��
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
/// ���[���h�ϊ��s�񏉊����p�f�[�^
/// </summary>
struct TreesOffset::WorldOffset{
private:
	/// <summary>X���W</summary>
	float positionX;
	/// <summary>Y���W</summary>
	float positionY;
	/// <summary>Z���W</summary>
	float positionZ;
	/// <summary>Y����]��</summary>
	float angle;
	/// <summary>���g�嗦</summary>
	float scaleOfWidth;
	/// <summary>�����g�嗦</summary>
	float scaleOfHeight;

public:
	// �f�t�H���g�R���X�g���N�^
	WorldOffset();

public:
	// �ǂݍ���
	void Load(const char* const _data);
	// �s��ɕϊ�
	void ToMatrix(::Ishikawa::Math::Matrix* const _matrix) const;
};

/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
TreesOffset::WorldOffset::WorldOffset() :
	positionX(0.0f),
	positionY(0.0f),
	positionZ(0.0f),
	angle(0.0f),
	scaleOfWidth(0.00001f),
	scaleOfHeight(0.00001f)
{
	// �������Ȃ�
}

/// <summary>
/// �ǂݍ���
/// </summary>
/// <param name="_data">�ǂݍ��ރf�[�^</param>
void TreesOffset::WorldOffset::Load(const char* const _data){
	if(_data == nullptr){
		// �ُ�I��
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
/// �s��ɕϊ�
/// </summary>
/// <param name="_matrix">�ۑ�����s��</param>
void TreesOffset::WorldOffset::ToMatrix(::Ishikawa::Math::Matrix* const _matrix) const {
	if(_matrix == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_matrix is NULL.");
	}

	// �ݒ�
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
/// �f�t�H���g�R���X�g���N�^
/// </summary>
TreesOffset::TreesOffset() :
	count(0U),
	worldOffsetArray(nullptr),
	worldArray(nullptr)
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TreesOffset::~TreesOffset(){
	this->Clear();
}


/// <summary>
/// �ǂݍ���
/// </summary>
/// <param name="_csvFilepath">�ǂݍ���CSV�t�@�C���ւ̃p�X</param>
void TreesOffset::Load(const char* const _csvFilepath){
	if(_csvFilepath == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_csvFilepath is NULL.");
	}

	struct stat fileStat;
	if(stat(_csvFilepath, &fileStat) != 0){
		// �ُ�I��
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

	// �v�f���ǂݍ���
#pragma warning(disable:4996)
	sscanf(fileData, "%u,", &(this->count));
#pragma warning(default:4996)

	// ���[���h�ϊ��s�񏉊����p�f�[�^�z�񐶐�
	if(this->worldOffsetArray != nullptr) delete[] this->worldOffsetArray;
	this->worldOffsetArray = new WorldOffset[this->count];

	// ���[���h�ϊ��s�񏉊����p�f�[�^�ǂݍ���
	const char* readPoint = fileData;
	for(unsigned int i=0U; i<this->count; ++i){
		while(((*readPoint)!='\n') && ((*readPoint)!='\0')){ ++readPoint; }
		++readPoint;

		this->worldOffsetArray[i].Load(readPoint);
	}

	// ���[���h�ϊ��s��z�񐶐�
	if(this->worldArray != nullptr) delete[] this->worldArray;
	this->worldArray = new ::Ishikawa::Math::Matrix[this->count];
	for(unsigned int i=0U; i<this->count; ++i){
		this->worldOffsetArray[i].ToMatrix(&(this->worldArray[i]));
	}
}

/// <summary>
/// �f�[�^�j��
/// </summary>
void TreesOffset::Clear(){
	// ���[���h�ϊ��s��z��폜
	if(this->worldArray != nullptr){
		delete[] this->worldArray;
		this->worldArray = nullptr;
	}
	// ���[���h�ϊ��s�񏉊����p�f�[�^�z��폜
	if(this->worldOffsetArray != nullptr){
		delete[] this->worldOffsetArray;
		this->worldOffsetArray = nullptr;
	}
	// �v�f����0�ɂ���
	this->count = 0U;
}
