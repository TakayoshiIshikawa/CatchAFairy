//=============================================================================
//	ITTextureData.h
//
//	����e�N�X�`���f�[�^�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_TEXTURE_DATA_H__
#define __IT_TEXTURE_DATA_H__

struct ID3D11Device;
#pragma warning(disable:4471)
enum D3D11_BIND_FLAG;
enum DXGI_FORMAT;
enum D3D_SRV_DIMENSION;
enum D3D11_RTV_DIMENSION;
enum D3D11_DSV_DIMENSION;
#pragma warning(default:4471)

#include "Ishikawa/DirectX/Device/DependentResource/ITDeviceDependentResourceBase.h"


namespace Ishikawa{
	namespace DirectX{
		namespace Texture{
			/// <summary>
			/// �e�N�X�`���o�C���h�\�t���O
			/// </summary>
			enum class BindableFlag : int{
				/// <summary>�V�F�[�_���\�[�X�̂�</summary>
				ShaderResource,
				/// <summary>�����_�[�^�[�Q�b�g�̂�</summary>
				RenderTarget,
				/// <summary>�f�v�X�X�e���V���̂�</summary>
				DepthStencil,
				/// <summary>�V�F�[�_���\�[�X�ƃ����_�[�^�[�Q�b�g</summary>
				SR_RT,
				/// <summary>�V�F�[�_���\�[�X�ƃf�v�X�X�e���V��</summary>
				SR_DS
			};

			// �e�N�X�`���o�C���h�\�t���O��D3D11�o�C���h�t���O�ɑΉ�����uint�l�ɕϊ�
			unsigned int ConvertBindableFlagToUInt(BindableFlag _bindableflag);


			/// <summary>
			/// �e�N�X�`���o�C���h�t���O
			/// </summary>
			enum class BindFlag : int{
				/// <summary>�o�C���h�Ȃ�</summary>
				NotBind = 0x0000,
				/// <summary>�V�F�[�_���\�[�X</summary>
				ShaderResource = 0x0001,
				/// <summary>�����_�[�^�[�Q�b�g</summary>
				RenderTarget = 0x0002,
				/// <summary>�f�v�X�X�e���V��</summary>
				DepthStencil = 0x0004
			};

			// �e�N�X�`���o�C���h�t���O��D3D11�o�C���h�t���O�ɕϊ�
			D3D11_BIND_FLAG ConvertBindFlagToD3D11BindFlag(BindFlag _bindflag);


			/// <summary>
			/// �e�N�X�`���t�H�[�}�b�g
			/// </summary>
			enum class Format : int{
				/// <summary>�s���ȃt�H�[�}�b�g</summary>
				Unknown,
				/// <summary>R8G8B8A8�����Ȃ�����</summary>
				R8G8B8A8UNorm,
				/// <summary>R10G10B10A2�����Ȃ�����</summary>
				R10G10B10A2UNorm,
				/// <summary>R32G32B32A32����</summary>
				R32G32B32A32Float,
				/// <summary>R32����</summary>
				R32Float,
				/// <summary>D24�����Ȃ����� S8�����Ȃ�����</summary>
				D24UNormS8UInt
			};

			// �e�N�X�`���t�H�[�}�b�g��DXGI�t�H�[�}�b�g�ɕϊ�
			DXGI_FORMAT ConvertFormatToDxgiFormat(Format _format);


			/// <summary>
			/// �e�N�X�`���f�B�����V����
			/// </summary>
			enum class Dimention : int{
				/// <summary>2D�e�N�X�`��</summary>
				Texture2D,
				/// <summary>�}���`�T���v��2D�e�N�X�`��</summary>
				MaltisampleTexture2D
			};

			// �e�N�X�`���f�B�����V������D3D�V�F�[�_���\�[�X�r���[�f�B�����V�����ɕϊ�
			D3D_SRV_DIMENSION ConvertDimentionToD3D11SRVDimention(Dimention _dimention);
			// �e�N�X�`���f�B�����V������D3D11�����_�[�^�[�Q�b�g�r���[�f�B�����V�����ɕϊ�
			D3D11_RTV_DIMENSION ConvertDimentionToD3D11RTVDimention(Dimention _dimention);
			// �e�N�X�`���f�B�����V������D3D11�f�v�X�X�e���V���r���[�f�B�����V�����ɕϊ�
			D3D11_DSV_DIMENSION ConvertDimentionToD3D11DSVDimention(Dimention _dimention);


			// �f�t�H���g�T���v���ݒ�f�[�^�̎擾
			void GetDefaultSampleDescData(
				unsigned int* const _sampleDescCount,
				unsigned int* const _sampleDescQuality,
				Dimention* const _dimention
			);
			// �}���`�T���v���E�A���`�G�C���A�V���O�ݒ�f�[�^�̎擾
			void GetMultisampleAntiAliasingDescData(
				unsigned int* const _sampleDescCount,
				unsigned int* const _sampleDescQuality,
				Dimention* const _dimention,
				ID3D11Device* const _device
			);
		}
	}
}

#endif
