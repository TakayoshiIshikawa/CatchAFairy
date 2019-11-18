//=============================================================================
//	ITShaderResourceInterface.h
//
//	����V�F�[�_���\�[�X�C���^�[�t�F�[�X�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_SHADER_RESOURCE_INTERFACE_H__
#define __IT_SHADER_RESOURCE_INTERFACE_H__

struct ID3D11ShaderResourceView;

namespace Ishikawa{
	namespace DirectX{
		namespace Texture{
			/// <summary>
			/// ����V�F�[�_���\�[�X�C���^�[�t�F�[�X
			/// </summary>
			class ShaderResourceInterface{
			public:
				/// <summary>�f�t�H���g�R���X�g���N�^</summary>
				ShaderResourceInterface() = default;
				/// <summary>�f�X�g���N�^</summary>
				virtual ~ShaderResourceInterface() = default;

			public:
				// �V�F�[�_���\�[�X�r���[�̎擾
				virtual ID3D11ShaderResourceView* GetShaderResourceView() const = 0;
			};
		}
	}
}

#endif
