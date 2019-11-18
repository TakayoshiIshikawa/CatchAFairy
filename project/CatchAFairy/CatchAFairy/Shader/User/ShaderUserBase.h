//=============================================================================
//	ShaderUserBase.h
//
//	�V�F�[�_���[�U���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SHADER_USER_BASE_H__
#define __SHADER_USER_BASE_H__

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11GeometryShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;
struct ID3D11SamplerState;
struct ID3D11BlendState;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct D3D11_INPUT_ELEMENT_DESC;
#include "Ishikawa/DirectX/Device/DependentResource/ITDeviceDependentResourceBase.h"


namespace CatchAFairy{
	namespace Shader{
		namespace User{
			/// <summary>
			/// �V�F�[�_���[�U���
			/// </summary>
			class Base : public ::Ishikawa::DirectX::Device::DependentResource::Base{
			private:
				/// <summary>���̓��C�A�E�g</summary>
				ID3D11InputLayout* inputLayout;
				/// <summary>���_�V�F�[�_</summary>
				ID3D11VertexShader* vertexShader;
				/// <summary>�W�I���g���V�F�[�_</summary>
				ID3D11GeometryShader* geometryShader;
				/// <summary>�s�N�Z���V�F�[�_</summary>
				ID3D11PixelShader* pixelShader;
				/// <summary>�T���v���X�e�[�g</summary>
				ID3D11SamplerState* samplerState;
				/// <summary>�u�����h�X�e�[�g</summary>
				ID3D11BlendState* blendState;
				/// <summary>�f�v�X�X�e���V���X�e�[�g</summary>
				ID3D11DepthStencilState* depthStencilState;
				/// <summary>���X�^���C�U�X�e�[�g</summary>
				ID3D11RasterizerState* rasterizerState;


			private:
				// �t�@�C���̃R�[�h�Ƃ��̒����̎擾
				static void GetFileCodeAndLength(
					char** const _code,
					unsigned long* const _length,
					const char* const _filepath
				);


			public:
				// �f�t�H���g�R���X�g���N�^
				Base();
				// �f�X�g���N�^
				virtual ~Base() override;

			public:
				/// <summary>���̓��C�A�E�g�̎擾</summary>
				/// <returns>���̓��C�A�E�g</returns>
				virtual ID3D11InputLayout* GetInputLayout() const final { return this->inputLayout; }
				/// <summary>���_�V�F�[�_�̎擾</summary>
				/// <returns>���_�V�F�[�_</returns>
				virtual ID3D11VertexShader* GetVertexShader() const final { return this->vertexShader; }
				/// <summary>�W�I���g���V�F�[�_�̎擾</summary>
				/// <returns>�W�I���g���V�F�[�_</returns>
				virtual ID3D11GeometryShader* GetGeometryShader() const final { return this->geometryShader; }
				/// <summary>�s�N�Z���V�F�[�_�̎擾</summary>
				/// <returns>�s�N�Z���V�F�[�_</returns>
				virtual ID3D11PixelShader* GetPixelShader() const final { return this->pixelShader; }
				/// <summary>�T���v���X�e�[�g�̎擾</summary>
				/// <returns>�T���v���X�e�[�g</returns>
				virtual ID3D11SamplerState* GetSamplerState() const final { return this->samplerState; }
				/// <summary>�u�����h�X�e�[�g�̎擾</summary>
				/// <returns>�u�����h�X�e�[�g</returns>
				virtual ID3D11BlendState* GetBlendState() const final { return this->blendState; }
				/// <summary>�f�v�X�X�e���V���X�e�[�g�̎擾</summary>
				/// <returns>�f�v�X�X�e���V���X�e�[�g</returns>
				virtual ID3D11DepthStencilState* GetDepthStencilState() const final { return this->depthStencilState; }
				/// <summary>���X�^���C�U�X�e�[�g�̎擾</summary>
				/// <returns>���X�^���C�U�X�e�[�g</returns>
				virtual ID3D11RasterizerState* GetRasterizerState() const final { return this->rasterizerState; }

			protected:
				// ���̓��C�A�E�g���̎擾
				virtual unsigned int GetInputLayoutCount() const = 0;
				// ���̓��C�A�E�g�ݒ�̎擾
				virtual const D3D11_INPUT_ELEMENT_DESC* GetInputLayoutDesc() const = 0;
				// ���_�V�F�[�_�t�@�C���p�X�̎擾
				virtual const char* GetVertexShaderFilepath() const = 0;
				// �W�I���g���V�F�[�_�t�@�C���p�X�̎擾
				virtual const char* GetGeometryShaderFilepath() const = 0;
				// �s�N�Z���V�F�[�_�t�@�C���p�X�̎擾
				virtual const char* GetPixelShaderFilepath() const = 0;

				// �f�o�C�X�̎擾
				virtual ID3D11Device* GetDevice() const final;
				// �R���e�L�X�g�̎擾
				virtual ID3D11DeviceContext* GetContext() const final;

			protected:
				/// <summary>�T���v���X�e�[�g�̐ݒ�</summary>
				/// <param name="_samplerState">�T���v���X�e�[�g</param>
				virtual void SetSamplerState(ID3D11SamplerState* const _samplerState) final{
					this->samplerState = _samplerState;
				}
				/// <summary>�u�����h�X�e�[�g�̐ݒ�</summary>
				/// <param name="_blendState">�u�����h�X�e�[�g</param>
				virtual void SetBlendState(ID3D11BlendState* const _blendState) final{
					this->blendState = _blendState;
				}
				/// <summary>�f�v�X�X�e���V���X�e�[�g�̐ݒ�</summary>
				/// <param name="_depthStencilState">�f�v�X�X�e���V���X�e�[�g</param>
				virtual void SetDepthStencilState(ID3D11DepthStencilState* const _depthStencilState) final{
					this->depthStencilState = _depthStencilState;
				}
				/// <summary>���X�^���C�U�X�e�[�g�̐ݒ�</summary>
				/// <param name="_rasterizerState">���X�^���C�U�X�e�[�g</param>
				virtual void SetRasterizerState(ID3D11RasterizerState* const _rasterizerState) final{
					this->rasterizerState = _rasterizerState;
				}

			public:
				// �f�o�C�X�ɓǂݍ��܂ꂽ
				virtual void OnDeviceLoaded() override;
				// �f�o�C�X���������ꂽ
				virtual void OnDeviceRestored() override;
				// �f�o�C�X���j�����ꂽ
				virtual void OnDeviceLost() override;

			public:
				// ������
				virtual void Initialize() final;
				// �T���v���X�e�[�g�̍쐬
				virtual void CreateSamplerState(ID3D11Device* const _device) = 0;
				// �u�����h�X�e�[�g�̍쐬
				virtual void CreateBlendState(ID3D11Device* const _device) = 0;
				// �f�v�X�X�e���V���X�e�[�g�̍쐬
				virtual void CreateDepthStencilState(ID3D11Device* const _device) = 0;
				// ���X�^���C�U�X�e�[�g�̍쐬
				virtual void CreateRasterizerState(ID3D11Device* const _device) = 0;
				// �C���f�b�N�X�o�b�t�@�쐬
				// [���p���I�����o�b�t�@�͊J�����Ă�������]
				virtual ID3D11Buffer* CreateIndexBuffer(
					const unsigned long* const _indexArray,
					unsigned int _indexCount
				) const final;
				// �o�b�t�@�J��
				virtual void ReleaseBuffer(ID3D11Buffer* const _buffer) const final;
				// �쐬
				virtual void Create() final;

			private:
				// ���_�V�F�[�_�Ɠ��̓��C�A�E�g�̍쐬
				virtual void CreateVertexShaderAndInputLayout(ID3D11Device* const _device) final;
				// �W�I���g���V�F�[�_�̍쐬
				virtual void CreateGeometryShader(ID3D11Device* const _device) final;
				// �s�N�Z���V�F�[�_�̍쐬
				virtual void CreatePixelShader(ID3D11Device* const _device) final;

				// �S�Ẵ��\�[�X�̊J��
				virtual void ReleaseAll() final;
				// ���̓��C�A�E�g�̊J��
				virtual void ReleaseInputLayout() final;
				// ���_�V�F�[�_�̊J��
				virtual void ReleaseVertexShader() final;
				// �W�I���g���V�F�[�_�̊J��
				virtual void ReleaseGeometryShader() final;
				// �s�N�Z���V�F�[�_�̊J��
				virtual void ReleasePixelShader() final;
				// �T���v���X�e�[�g�̊J��
				virtual void ReleaseSamplerState() final;
				// �u�����h�X�e�[�g�̊J��
				virtual void ReleaseBlendState() final;
				// �f�v�X�X�e���V���X�e�[�g�̊J��
				virtual void ReleaseDepthStencilState() final;
				// ���X�^���C�U�X�e�[�g�̊J��
				virtual void ReleaseRasterizerState() final;
			};
		}
	}
}

#endif
