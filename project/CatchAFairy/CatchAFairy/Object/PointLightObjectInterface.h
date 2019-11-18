//=============================================================================
//	PointLightObjectInterface.h
//
//	�|�C���g���C�g�I�u�W�F�N�g�C���^�[�t�F�[�X�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __POINT_LIGHT_OBJECT_INTERFACE_H__
#define __POINT_LIGHT_OBJECT_INTERFACE_H__

struct ID3D11Buffer;

namespace CatchAFairy{
	namespace Shader{
		namespace User{
			class CameraEyeReflectionLight;
		}
	}

	namespace Object{
		/// <summary>
		/// �|�C���g���C�g�I�u�W�F�N�g�C���^�[�t�F�[�X
		/// </summary>
		class PointLightInterface{
		public:
			/// <summary>�f�t�H���g�R���X�g���N�^</summary>
			PointLightInterface(){}
			/// <summary>�f�X�g���N�^</summary>
			virtual ~PointLightInterface(){}

		public:
			// ���_�o�b�t�@�̎擾
			virtual ID3D11Buffer* GetVertexBuffer() const = 0;
			// �C���f�b�N�X�o�b�t�@�̎擾
			virtual ID3D11Buffer* GetIndexBuffer() const = 0;
			// �C���f�b�N�X���̎擾
			virtual unsigned int GetIndexCount() const = 0;

		public:
			// ���ˌ��V�F�[�_���[�U�̐ݒ�
			virtual void SetReflectionLightShaderUser(
				::CatchAFairy::Shader::User::CameraEyeReflectionLight* const _reflectionLightShaderUser
			) = 0;
		};
	}
}

#endif
