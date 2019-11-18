//=============================================================================
//	ITDeviceDependentResourceBase.h
//
//	����f�o�C�X�ˑ����\�[�X�̊��̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_DEVICE_DEPENDENT_RESOURCE_BASE_H__
#define __IT_DEVICE_DEPENDENT_RESOURCE_BASE_H__

#include "ITDeviceDependentResourceInterface.h"


namespace Ishikawa{
	namespace DirectX{
		namespace Device{
			namespace DependentResource{
				/// <summary>
				/// ����f�o�C�X�ˑ����\�[�X�̊��
				/// </summary>
				class Base : public Interface{
				private:
					/// <summary>�ˑ����Ă��鎩��f�o�C�X</summary>
					Device* dependentDevice;
					/// <summary>���̎���f�o�C�X�ˑ����\�[�X</summary>
					Interface* nextITDeviceDependentResource;
					/// <summary>�O�̎���f�o�C�X�ˑ����\�[�X</summary>
					Interface* beforeITDeviceDependentResource;


				public:
					// �f�t�H���g�R���X�g���N�^
					Base();
					// �f�X�g���N�^
					virtual ~Base() override;

				public:
					/// <summary>����f�o�C�X�ˑ����\�[�X���X�g�̒[��?</summary>
					/// <returns>���true</returns>
					virtual bool IsITDeviceDependentResourceListEdge() const override final { return true; }
					/// <summary>�ˑ����Ă��鎩��f�o�C�X�̎擾</summary>
					/// <returns>�ˑ����Ă��鎩��f�o�C�X</returns>
					virtual Device* GetDependentITDevice() const override final {
						return this->dependentDevice;
					}
					/// <summary>���̎���f�o�C�X�ˑ����\�[�X�̎擾</summary>
					/// <returns>���̎���f�o�C�X�ˑ����\�[�X</returns>
					virtual Interface* GetNextITDeviceDependentResource() const override final {
						return this->nextITDeviceDependentResource;
					}
					/// <summary>�O�̎���f�o�C�X�ˑ����\�[�X�̎擾</summary>
					/// <returns>�O�̎���f�o�C�X�ˑ����\�[�X</returns>
					virtual Interface* GetBeforeITDeviceDependentResource() const override final {
						return this->beforeITDeviceDependentResource;
					}

				public:
					/// <summary>�ˑ����鎩��f�o�C�X�̐ݒ�</summary>
					/// <param name="_device">�f�o�C�X</param>
					virtual void SetDependentITDevice(Device* const _device) override final {
						this->dependentDevice = _device;
					}
					/// <summary>���̎���f�o�C�X�ˑ����\�[�X�̐ݒ�</summary>
					/// <param name="_next">���̎���f�o�C�X�ˑ����\�[�X</param>
					virtual void SetNextITDeviceDependentResource(Interface* const _next) override final {
						this->nextITDeviceDependentResource = _next;
					}
					/// <summary>�O�̎���f�o�C�X�ˑ����\�[�X�̐ݒ�</summary>
					/// <param name="_before">�O�̎���f�o�C�X�ˑ����\�[�X</param>
					virtual void SetBeforeITDeviceDependentResource(Interface* const _before) override final {
						this->beforeITDeviceDependentResource = _before;
					}

				public:
					// ���X�g����O���
					virtual void RemoveITDeviceDependentResourceList() override final;

				protected:
					// ����f�o�C�X�ˑ����\�[�X�̏�����
					virtual void InitializeITDeviceDependentResource() override final;
				};
			}
		}
	}
}

#endif
