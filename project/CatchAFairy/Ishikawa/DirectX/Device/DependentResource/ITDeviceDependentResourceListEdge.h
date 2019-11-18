//=============================================================================
//	ITDeviceDependentResourceListEdge.h
//
//	����f�o�C�X�ˑ����\�[�X�̃��X�g�[�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_DEVICE_DEPENDENT_RESOURCE_LIST_EDGE_H__
#define __IT_DEVICE_DEPENDENT_RESOURCE_LIST_EDGE_H__

#include "ITDeviceDependentResourceInterface.h"


namespace Ishikawa{
	namespace DirectX{
		namespace Device{
			namespace DependentResource{
				/// <summary>
				/// ����f�o�C�X�ˑ����\�[�X�̃��X�g�[
				/// </summary>
				class ListEdge final : public Interface{
				private:
					/// <summary>���̎���f�o�C�X�ˑ����\�[�X</summary>
					Interface* nextITDeviceDependentResource;
					/// <summary>�O�̎���f�o�C�X�ˑ����\�[�X</summary>
					Interface* beforeITDeviceDependentResource;


				public:
					// �f�t�H���g�R���X�g���N�^
					ListEdge();
					// �f�X�g���N�^
					virtual ~ListEdge() override final;

				public:
					/// <summary>����f�o�C�X�ˑ����\�[�X���X�g�̒[��?</summary>
					/// <returns>���true</returns>
					virtual bool IsITDeviceDependentResourceListEdge() const override final { return true; }
					/// <summary>�ˑ����Ă��鎩��f�o�C�X�̎擾[�ł��Ȃ�]</summary>
					/// <returns>���NULL</returns>
					virtual Device* GetDependentITDevice() const override final { return nullptr; }
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
					/// <summary>�ˑ����鎩��f�o�C�X�̐ݒ�[�ł��Ȃ�]</summary>
					virtual void SetDependentITDevice(Device* const) override final{}
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
					/// <summary>�f�o�C�X�ɓǂݍ��܂ꂽ</summary>
					virtual void OnDeviceLoaded() override final{}
					/// <summary>�f�o�C�X���������ꂽ</summary>
					virtual void OnDeviceRestored() override final{}
					/// <summary>�f�o�C�X���j�����ꂽ</summary>
					virtual void OnDeviceLost() override final{}

				public:
					/// <summary>���X�g����O���[�O��Ȃ�]</summary>
					virtual void RemoveITDeviceDependentResourceList() override final{}

					// ������
					void Initialize();

				protected:
					// ����f�o�C�X�ˑ����\�[�X�̏�����
					virtual void InitializeITDeviceDependentResource() override final;
				};
			}
		}
	}
}

#endif
