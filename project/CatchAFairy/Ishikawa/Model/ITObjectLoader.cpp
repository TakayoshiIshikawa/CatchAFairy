//=============================================================================
//	ITObjectLoader.cpp
//
//	����I�u�W�F�N�g�t�@�C�����[�_�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITObjectLoader.h"
#include <stdio.h>
#include "Ishikawa/Common/Exception/ITExceptions.h"

using namespace Ishikawa::Model::ItObject;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
FileLoader::FileLoader(){
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
FileLoader::~FileLoader(){
	// �������Ȃ�
}


/// <summary>
/// �ǂݍ���
/// </summary>
/// <param name="_vertices">���_�z��i�[��</param>
/// <param name="_vertexCount">���_���i�[��</param>
/// <param name="_indices">�C���f�b�N�X�z��i�[��</param>
/// <param name="_indexCount">�C���f�b�N�X���i�[��</param>
/// <param name="_circumscribedCircleCenterPosition">�O�ډ~���S���W�i�[��</param>
/// <param name="_squareCircumscribedCircleRadius">�O�ډ~���a���[��</param>
/// <param name="_filepath">�ǂݍ��݃t�@�C���p�X</param>
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
		// �ُ�I��
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
/// �ǂݍ���
/// </summary>
/// <param name="_vertices">���_�z��i�[��</param>
/// <param name="_vertexCount">���_���i�[��</param>
/// <param name="_indices">�C���f�b�N�X�z��i�[��</param>
/// <param name="_indexCount">�C���f�b�N�X���i�[��</param>
/// <param name="_circumscribedCircleCenterPosition">�O�ډ~���S���W�i�[��</param>
/// <param name="_squareCircumscribedCircleRadius">�O�ډ~���a���[��</param>
/// <param name="_filepath">�ǂݍ��݃t�@�C���p�X</param>
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
		// �ُ�I��
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
/// �ǂݍ���
/// </summary>
/// <param name="_vertices">���_�z��i�[��</param>
/// <param name="_vertexCount">���_���i�[��</param>
/// <param name="_indices">�C���f�b�N�X�z��i�[��</param>
/// <param name="_indexCount">�C���f�b�N�X���i�[��</param>
/// <param name="_circumscribedCircleCenterPosition">�O�ډ~���S���W�i�[��</param>
/// <param name="_squareCircumscribedCircleRadius">�O�ډ~���a���[��</param>
/// <param name="_file">�ǂݍ��݃t�@�C��</param>
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
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_vertices is NULL.");
	}
	if(_vertexCount == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_vertexCount is NULL.");
	}
	if(_indices == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_indices is NULL.");
	}
	if(_indexCount == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_indexCount is NULL.");
	}
	if(_circumscribedCircleCenterPosition == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_circumscribedCircleCenterPosition is NULL.");
	}
	if(_squareCircumscribedCircleRadius == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_squareCircumscribedCircleRadius is NULL.");
	}
	if(_file == nullptr){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't open ITObject file.");
	}


	// ���_���ǂݎ��
	(*_vertices) = nullptr;
	fread(_vertexCount, sizeof(unsigned long), 1U, _file);
	if((*_vertexCount) > 0){
		// ���_�z��쐬
		(*_vertices) = new VertexData[(*_vertexCount)];
		// ���_�ǂݎ��
		fread((*_vertices), sizeof(VertexData), (*_vertexCount), _file);

		// �O�ڂ���AABB�����߂�
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

	// �C���f�b�N�X���ǂݎ��
	(*_indices) = nullptr;
	fread(_indexCount, sizeof(unsigned long), 1U, _file);
	if((*_indexCount) > 0){
		// �C���f�b�N�X�z��쐬
		(*_indices) = new unsigned long[(*_indexCount)];
		// �C���f�b�N�X�ǂݎ��
		fread((*_indices), sizeof(unsigned long), (*_indexCount), _file);
	}


	fclose(_file);
}
