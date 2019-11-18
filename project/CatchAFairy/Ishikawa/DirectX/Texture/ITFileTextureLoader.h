//=============================================================================
//	ITFileTextureLoader.h
//
//	����t�@�C���e�N�X�`�����[�_�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#ifndef __IT_FILE_TEXTURE_LOADER_H__
#define __IT_FILE_TEXTURE_LOADER_H__

struct ID3D11Device;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
#include "Ishikawa/Common/PatternBase/SingletonBase.h"


namespace Ishikawa{
	namespace DirectX{
		namespace Texture{
			/// <summary>
			/// ����t�@�C���e�N�X�`�����[�_
			/// </summary>
			class FileTextureLoader final : public ::Ishikawa::Common::SingletonBase{
			public:
				// GetInstance<FileTextureLoader>()�ŃA�N�Z�X���邽�߃t�����h��
				friend class Ishikawa::Common::SingletonManager;


			private:
				// �V���O���g���C���X�^���X
				static FileTextureLoader* instance;


			private:
				// �f�t�H���g�R���X�g���N�^
				FileTextureLoader();
				// �f�X�g���N�^
				~FileTextureLoader();
			private:
				/// <summary>�R�s�[�R���X�g���N�^[�폜]</summary>
				FileTextureLoader(const FileTextureLoader&) = delete;
				/// <summary>������Z�q[�폜]</summary>
				FileTextureLoader& operator=(const FileTextureLoader&) = delete;

			public:
				// �ǂݍ���
				void Load(
					ID3D11Device* const _device,
					const wchar_t* const _filepath,
					ID3D11Texture2D** _texture,
					ID3D11ShaderResourceView** _shaderResourceView
				);
			};
		}
	}
}

#endif
