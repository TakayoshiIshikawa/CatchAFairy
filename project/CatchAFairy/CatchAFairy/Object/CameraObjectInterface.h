//=============================================================================
//	CameraObjectInterface.h
//
//	�J�����I�u�W�F�N�g�C���^�[�t�F�[�X�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __CAMERA_OBJECT_INTERFACE_H__
#define __CAMERA_OBJECT_INTERFACE_H__

struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;

namespace Ishikawa{
	namespace Math{
		struct Matrix;
	}
	namespace DirectX{
		namespace Device{
			class Device;
		}
	}
}

namespace CatchAFairy{
	namespace Object{
		/// <summary>
		/// �J�����I�u�W�F�N�g�C���^�[�t�F�[�X
		/// </summary>
		class CameraInterface{
		public:
			/// <summary>�f�t�H���g�R���X�g���N�^</summary>
			CameraInterface(){}
			/// <summary>�f�X�g���N�^</summary>
			virtual ~CameraInterface(){}

		public:
			// ���[���h�ϊ��s��̎擾
			virtual const ::Ishikawa::Math::Matrix& GetWorld() const = 0;
			// �r���[�ϊ��s��̎擾
			virtual const ::Ishikawa::Math::Matrix& GetView() const = 0;
			// �v���W�F�N�V�����ϊ��s��̎擾
			virtual const ::Ishikawa::Math::Matrix& GetProjection() const = 0;
			// �ŏI�V�F�[�_���\�[�X�r���[�̎擾
			virtual ID3D11ShaderResourceView* GetLastShaderResourceView() const = 0;

		public:
			// �Q�[���̃f�o�C�X�Ɉˑ����郊�\�[�X�̒ǉ�
			virtual void AddITDeviceDependentResources(::Ishikawa::DirectX::Device::Device* const _itDevice) = 0;
			// �f�o�C�X�ˑ����\�[�X�쐬
			virtual void CreateDeviceDependentResources() = 0;
			// �f�o�C�X�ˑ����\�[�X�J��
			virtual void ReleaseDeviceDependentResources() = 0;
		};
	}
}

#endif
