//=============================================================================
//	ITDevice.h
//
//	����f�o�C�X�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_DEVICE_H__
#define __IT_DEVICE_H__

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct IDXGIAdapter1;
#pragma warning(disable:4471)
enum D3D_FEATURE_LEVEL;
#pragma warning(default:4471)

#include "DependentResource/ITDeviceDependentResourceListEdge.h"


namespace Ishikawa{
	namespace DirectX{
		namespace Device{
			/// <summary>
			/// ����f�o�C�X
			/// </summary>
			class Device final{
			private:
				/// <summary>�f�o�C�X</summary>
				ID3D11Device* device;
				/// <summary>�t���[�`�����x��</summary>
				D3D_FEATURE_LEVEL featureLevel;
				/// <summary>�R���e�L�X�g</summary>
				ID3D11DeviceContext* context;
				/// <summary>���̃f�o�C�X�Ɉˑ����Ă��郊�\�[�X���X�g�̒[</summary>
				DependentResource::ListEdge deviceDependentResourceListEdge;


			public:
				// �f�t�H���g�R���X�g���N�^
				Device();
				// �f�X�g���N�^
				~Device();

			public:
				/// <summary>�f�o�C�X�̎擾</summary>
				/// <returns>�f�o�C�X</returns>
				ID3D11Device* GetDevice() const { return this->device; }
				/// <summary>�R���e�L�X�g�̎擾</summary>
				/// <returns>�R���e�L�X�g</returns>
				ID3D11DeviceContext* GetContext() const { return this->context; }

			public:
				// ������
				void Initialize();
				// �쐬
				void Create();
				// ����
				void Restore();
				// ���X�g
				void Lost();

				// �ˑ����\�[�X�̒ǉ�[1����]
				void AddDependentResource(DependentResource::Interface* const _dependentResource);
				// �ˑ����\�[�X�̒ǉ�[�z���X�g����]
				// �ǉ�������͌����X�g�͋�ɂȂ�܂�
				void AddDependentResourceList(DependentResource::ListEdge* const _dependentResourceListEdge);

			private:
				// �f�o�C�X�̐���
				void CreateDevice(IDXGIAdapter1* const _adapter1);
				// �A�_�v�^�̎擾
				void GetAdapter(IDXGIAdapter1** const _adapter1);

				// �f�o�C�X�̊J��
				void ReleaseDevice();
				// �R���e�L�X�g�̊J��
				void ReleaseContext();
			};
		}
	}
}

#endif
