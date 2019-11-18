//=============================================================================
//	ITObjectLoader.h
//
//	����I�u�W�F�N�g�t�@�C�����[�_�̃w�b�_�t�@�C��
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
			/// ����I�u�W�F�N�g���_�f�[�^
			/// </summary>
			struct VertexData{
				/// <summary>���W</summary>
				::Ishikawa::Math::Float3 position;
				/// <summary>�@��</summary>
				::Ishikawa::Math::Float3 normal;
				/// <summary>�F</summary>
				::Ishikawa::Math::Float4 color;
				/// <summary>�e�N�X�`���R�[�f�B�l�[�g</summary>
				::Ishikawa::Math::Float2 textureCoordinate;
			};

			/// <summary>
			/// ����I�u�W�F�N�g�t�@�C�����[�_
			/// </summary>
			class FileLoader{
			public:
				// �f�t�H���g�R���X�g���N�^
				FileLoader();
				// �f�X�g���N�^
				~FileLoader();

			public:
				// �ǂݍ���
				void Load(
					VertexData** const _vertices,
					unsigned int* const _vertexCount,
					unsigned long** const _indices,
					unsigned int* const _indexCount,
					::Ishikawa::Math::Float3* const _circumscribedCircleCenterPosition,
					float* const _squareCircumscribedCircleRadius,
					const char* const _filepath
				) const;
				// �ǂݍ���
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
				// �ǂݍ���
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
