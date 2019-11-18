//=============================================================================
//	Numbers.h
//
//	���l���̃w�b�_�t�@�C��
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
				/// ���l���
				/// </summary>
				class Numbers final{
				private:
					// �`��e�N�X�`���R�[�f�B�l�[�g�z��(����ƉE��)
					static const ::Ishikawa::Math::Float4 SHAPE_TEXTURE_COORDINATE[10];
					// �\���e�N�X�`���R�[�f�B�l�[�g(����ƉE��)
					static const ::Ishikawa::Math::Float4 VIEW_TEXTURE_COORDINATE;


				public:
					// �`��e�N�X�`���R�[�f�B�l�[�g�̎擾
					static const ::Ishikawa::Math::Float4& GetShapeTextureCoordinate(int _number);
					// �\���e�N�X�`���R�[�f�B�l�[�g�̎擾
					static const ::Ishikawa::Math::Float4& GetViewTextureCoordinate();

				private:
					/// <summary>�f�t�H���g�R���X�g���N�^ [�폜]</summary>
					Numbers() = delete;
					/// <summary>�R�s�[�R���X�g���N�^ [�폜]</summary>
					Numbers(const Numbers&) = delete;
					/// <summary>�f�X�g���N�^ [�폜]</summary>
					~Numbers() = delete;
					/// <summary>������Z�q [�폜]</summary>
					Numbers& operator=(const Numbers&) = delete;
				};
			}
		}
	}
}

#endif
