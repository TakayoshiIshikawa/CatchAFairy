//=============================================================================
//	ITDepthStancilInterface.h
//
//	����f�v�X�X�e���V���C���^�[�t�F�[�X�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_DEPTH_STENCIL_INTERFACE_H__
#define __IT_DEPTH_STENCIL_INTERFACE_H__

struct ID3D11DepthStencilView;

namespace Ishikawa{
	namespace DirectX{
		namespace Texture{
			/// <summary>
			/// ����f�v�X�X�e���V���C���^�[�t�F�[�X
			/// </summary>
			class DepthStencilInterface{
			public:
				/// <summary>�f�t�H���g�R���X�g���N�^</summary>
				DepthStencilInterface() = default;
				/// <summary>�f�X�g���N�^</summary>
				virtual ~DepthStencilInterface() = default;

			public:
				// �f�v�X�X�e���V���r���[�̎擾
				virtual ID3D11DepthStencilView* GetDepthStencilView() const = 0;
			};
		}
	}
}

#endif
