//=============================================================================
//	GUIManager.h
//
//	GUI�}�l�[�W���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __GUI_MANAGER_H__
#define __GUI_MANAGER_H__

#include "Ishikawa/Math/ITFloatN.h"
#include "Ishikawa/DirectX/Texture/ITTexture2D.h"
#include "Ishikawa/DirectX/Texture/ITFileTexture.h"
#include "CatchAFairy/Shader/User/PlayGUIShaderUser.h"
#include "CatchAFairy/Data/GUIVerticesSetting.h"

namespace CatchAFairy{
	namespace Object{
		namespace PlayScene{
			namespace GUI{
				/// <summary>
				/// GUI�}�l�[�W��
				/// </summary>
				class GUIManager final {
				public:
					// �o�̓T�C�Y
					static const ::Ishikawa::Math::Float2 OUTPUT_SIZE;

				private:
					// GUI�C���f�b�N�X��
					static const unsigned int GUI_INDEX_COUNT;
					/// <summary>GUI���_��</summary>
					static const unsigned int GUI_VERTEX_COUNT = ::CatchAFairy::Data::GUIVerticesSetting::ALL_VERTEX_COUNT;
					// �`��e�N�X�`���t�@�C���p�X
					static const wchar_t* const SHAPE_TEXTURE_FILEPATH;
					// �\���e�N�X�`���t�@�C���p�X
					static const wchar_t* const VIEW_TEXTURE_FILEPATH;


				private:
					/// <summary>�o�̓^�[�Q�b�g�e�N�X�`��</summary>
					::Ishikawa::DirectX::Texture::Texture2D* outputTargetTexture;
					/// <summary>GUI�V�F�[�_���[�U</summary>
					::CatchAFairy::Shader::User::PlayGUI guiShaderUser;
					/// <summary>GUI���_�o�b�t�@</summary>
					ID3D11Buffer* guiVertexBuffer;
					/// <summary>GUI�C���f�b�N�X�o�b�t�@</summary>
					ID3D11Buffer* guiIndexBuffer;
					/// <summary>GUI�C���f�b�N�X��</summary>
					unsigned int guiIndexCount;
					/// <summary>GUI���_�z��</summary>
					::CatchAFairy::Shader::User::PlayGUI::VSInput guiVertexArray[GUI_VERTEX_COUNT];

					/// <summary>�`��e�N�X�`��</summary>
					::Ishikawa::DirectX::Texture::FileTexture shapeTexture;
					/// <summary>�\���e�N�X�`��</summary>
					::Ishikawa::DirectX::Texture::FileTexture viewTexture;


				public:
					// �f�t�H���g�R���X�g���N�^
					GUIManager();
					// �f�X�g���N�^
					~GUIManager();

				public:
					// GUI���_�̎擾
					::CatchAFairy::Shader::User::PlayGUI::VSInput* GetGUIVertices(
						unsigned int _startIndex,
						unsigned int _getCount
					);

				public:
					/// <summary>�o�̓^�[�Q�b�g�e�N�X�`���̐ݒ�</summary>
					/// <param name="_outputTargetTexture">�o�̓^�[�Q�b�g�e�N�X�`��</param>
					void SetOutputTargetTexture(::Ishikawa::DirectX::Texture::Texture2D* const _outputTargetTexture){
						this->outputTargetTexture = _outputTargetTexture;
					}

				public:
					// ������
					void Initialize();
					// �`��
					void Render();

					// �Q�[���̃f�o�C�X�Ɉˑ����郊�\�[�X�̒ǉ�
					void AddITDeviceDependentResources(::Ishikawa::DirectX::Device::Device* const _itDevice);
					// �f�o�C�X�ˑ����\�[�X�쐬
					void CreateDeviceDependentResources();
					// �f�o�C�X�ˑ����\�[�X�J��
					void ReleaseDeviceDependentResources();

				private:
					// �`��O
					void DrawBefore();
					// �`��
					void Draw();
					// �`���
					void DrawAfter();

					// GUI���_�o�b�t�@�̍쐬
					void CreateGUIVertexBuffer();
					// GUI���_�o�b�t�@�̊J��
					void ReleaseGUIVertexBuffer();
					// GUI�C���f�b�N�X�o�b�t�@�̍쐬
					void CreateGUIIndexBuffer();
					// GUI�C���f�b�N�X�o�b�t�@�̊J��
					void ReleaseGUIIndexBuffer();
				};
			}
		}
	}
}

#endif
