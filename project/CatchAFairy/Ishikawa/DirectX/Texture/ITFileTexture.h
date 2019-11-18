//=============================================================================
//	ITFileTexture.h
//
//	����t�@�C���e�N�X�`���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#ifndef __IT_FILE_TEXTURE_H__
#define __IT_FILE_TEXTURE_H__

#include "Ishikawa/DirectX/Device/DependentResource/ITDeviceDependentResourceBase.h"
#include "ITTexture2DInterface.h"
#include "ITShaderResourceInterface.h"


namespace Ishikawa{
	namespace DirectX{
		namespace Texture{
			/// <summary>
			/// ����t�@�C���e�N�X�`��
			/// </summary>
			class FileTexture final :
				public ::Ishikawa::DirectX::Device::DependentResource::Base,
				public Texture2DInterface,
				public ShaderResourceInterface
			{
			private:
				/// <summary>�t�@�C���p�X</summary>
				wchar_t* filepath;
				/// <summary>�V�F�[�_���\�[�X�r���[</summary>
				ID3D11ShaderResourceView* shaderResourceView;
				/// <summary>��</summary>
				unsigned int width;
				/// <summary>����</summary>
				unsigned int height;


			public:
				// �f�t�H���g�R���X�g���N�^
				FileTexture();
				// �f�X�g���N�^
				virtual ~FileTexture() override final;

			public:
				/// <summary>�e�N�X�`���T�C�Y�̎擾</summary>
				/// <param name="_width">���̊i�[��</param>
				/// <param name="_height">�����̊i�[��</param>
				virtual void GetTextureSize(unsigned int* const _width, unsigned int* const _height) const override final{
					if(_width != nullptr) (*_width) = this->width;
					if(_height != nullptr) (*_height) = this->height;
				}
				/// <summary>�V�F�[�_���\�[�X�r���[�̎擾</summary>
				/// <returns>�V�F�[�_���\�[�X�r���[</returns>
				virtual ID3D11ShaderResourceView* GetShaderResourceView() const override final{
					return this->shaderResourceView;
				}

			public:
				// �f�o�C�X�ɓǂݍ��܂ꂽ
				virtual void OnDeviceLoaded() override final;
				// �f�o�C�X���������ꂽ
				virtual void OnDeviceRestored() override final;
				// �f�o�C�X���j�����ꂽ
				virtual void OnDeviceLost() override final;

			public:
				// ������
				void Initialize();
				// �ǂݍ���
				void Load(const wchar_t* const _filepath);

			private:
				// �t�@�C���p�X�̕ύX
				void ChangeFilepath(const wchar_t* const _filepath);
				// �V�F�[�_���\�[�X�r���[�̊J��
				void ReleaseShaderResourceView();
			};
		}
	}
}

#endif
