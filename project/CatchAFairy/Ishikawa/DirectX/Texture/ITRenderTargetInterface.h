//=============================================================================
//	ITRenderTargetInterface.h
//
//	���샌���_�[�^�[�Q�b�g�C���^�[�t�F�[�X�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_RENDER_TARGET_INTERFACE_H__
#define __IT_RENDER_TARGET_INTERFACE_H__

struct ID3D11RenderTargetView;

namespace Ishikawa{
	namespace DirectX{
		namespace Texture{
			/// <summary>
			/// ���샌���_�[�^�[�Q�b�g�C���^�[�t�F�[�X
			/// </summary>
			class RenderTargetInterface{
			public:
				/// <summary>�f�t�H���g�R���X�g���N�^</summary>
				RenderTargetInterface() = default;
				/// <summary>�f�X�g���N�^</summary>
				virtual ~RenderTargetInterface() = default;

			public:
				// �����_�[�^�[�Q�b�g�r���[�̎擾
				virtual ID3D11RenderTargetView* GetRenderTargetView() const = 0;
			};
		}
	}
}

#endif
