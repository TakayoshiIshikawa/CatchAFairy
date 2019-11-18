//=============================================================================
//	ITTexture2DInterface.h
//
//	����2D�e�N�X�`���C���^�[�t�F�[�X�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_TEXTURE_2D_INTERFACE_H__
#define __IT_TEXTURE_2D_INTERFACE_H__

namespace Ishikawa{
	namespace DirectX{
		namespace Texture{
			/// <summary>
			/// ����2D�e�N�X�`���C���^�[�t�F�[�X
			/// </summary>
			class Texture2DInterface{
			public:
				/// <summary>�f�t�H���g�R���X�g���N�^</summary>
				Texture2DInterface() = default;
				/// <summary>�f�X�g���N�^</summary>
				virtual ~Texture2DInterface() = default;

			public:
				// �e�N�X�`���T�C�Y�̎擾
				virtual void GetTextureSize(unsigned int* const _width, unsigned int* const _height) const = 0;
			};
		}
	}
}

#endif
