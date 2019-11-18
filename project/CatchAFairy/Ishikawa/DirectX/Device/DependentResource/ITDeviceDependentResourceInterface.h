//=============================================================================
//	ITDeviceDependentResourceInterface.h
//
//	����f�o�C�X�ˑ����\�[�X�C���^�[�t�F�[�X�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_DEVICE_DEPENDENT_RESOURCE_INTERFACE_H__
#define __IT_DEVICE_DEPENDENT_RESOURCE_INTERFACE_H__

namespace Ishikawa{
	namespace DirectX{
		namespace Device{
			class Device;


			namespace DependentResource{
				/// <summary>
				/// ����f�o�C�X�ˑ����\�[�X�C���^�[�t�F�[�X
				/// </summary>
				class Interface{
				public:
					/// <summary>�f�t�H���g�R���X�g���N�^</summary>
					Interface(){}
					/// <summary>�f�X�g���N�^</summary>
					virtual ~Interface(){}

				public:
					// ����f�o�C�X�ˑ����\�[�X���X�g�̒[��?
					virtual bool IsITDeviceDependentResourceListEdge() const = 0;
					// �ˑ����Ă��鎩��f�o�C�X�̎擾
					virtual Device* GetDependentITDevice() const = 0;
					// ���̎���f�o�C�X�ˑ����\�[�X�̎擾
					virtual Interface* GetNextITDeviceDependentResource() const = 0;
					// �O�̎���f�o�C�X�ˑ����\�[�X�̎擾
					virtual Interface* GetBeforeITDeviceDependentResource() const = 0;

				public:
					// �ˑ����鎩��f�o�C�X�̐ݒ�
					virtual void SetDependentITDevice(Device* const _device) = 0;
					// ���̎���f�o�C�X�ˑ����\�[�X�̐ݒ�
					virtual void SetNextITDeviceDependentResource(Interface* const _next) = 0;
					// �O�̎���f�o�C�X�ˑ����\�[�X�̐ݒ�
					virtual void SetBeforeITDeviceDependentResource(Interface* const _before) = 0;

				public:
					// �f�o�C�X�ɓǂݍ��܂ꂽ
					virtual void OnDeviceLoaded() = 0;
					// �f�o�C�X���������ꂽ
					virtual void OnDeviceRestored() = 0;
					// �f�o�C�X���j�����ꂽ
					virtual void OnDeviceLost() = 0;

				public:
					// ���X�g����O���
					virtual void RemoveITDeviceDependentResourceList() = 0;

				protected:
					// ����f�o�C�X�ˑ����\�[�X�̏�����
					virtual void InitializeITDeviceDependentResource() = 0;
				};
			}
		}
	}
}

#endif
