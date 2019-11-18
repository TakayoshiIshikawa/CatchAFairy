//=============================================================================
//	ITTexture2D.h
//
//	����2D�e�N�X�`���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_TEXTURE_2D_H__
#define __IT_TEXTURE_2D_H__

struct ID3D11Device;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;
#pragma warning(disable:4471)
enum D3D11_BIND_FLAG;
enum DXGI_FORMAT;
#pragma warning(default:4471)

#include "Ishikawa/DirectX/Device/DependentResource/ITDeviceDependentResourceBase.h"
#include "ITTexture2DInterface.h"


namespace Ishikawa{
	namespace DirectX{
		namespace Texture{
			enum class BindableFlag : int;
			enum class BindFlag : int;
			enum class Format : int;
			enum class Dimention : int;


			/// <summary>
			/// ����2D�e�N�X�`��
			/// </summary>
			class Texture2D final :
				public ::Ishikawa::DirectX::Device::DependentResource::Base,
				public Texture2DInterface
			{
			private:
				/// <summary>�o�C���h�\�t���O</summary>
				BindableFlag bindableFlag;
				/// <summary>���݂̃o�C���h�t���O</summary>
				BindFlag currentBindFlag;
				/// <summary>��</summary>
				unsigned int width;
				/// <summary>����</summary>
				unsigned int height;
				/// <summary>�t�H�[�}�b�g</summary>
				Format format;
				/// <summary>CPU�A�N�Z�X�t���O</summary>
				unsigned int cpuAccessFlag;
				/// <summary>�T���v���ݒ�J�E���g</summary>
				unsigned int sampleDescCount;
				/// <summary>�T���v���ݒ�N�H���e�B</summary>
				unsigned int sampleDescQuality;
				/// <summary>�r���[�f�B�����V����</summary>
				Dimention dimention;
				/// <summary>�e�N�X�`��</summary>
				ID3D11Texture2D* texture;
				/// <summary>�V�F�[�_���\�[�X�r���[</summary>
				ID3D11ShaderResourceView* shaderResourceView;
				/// <summary>�����_�[�^�[�Q�b�g�r���[</summary>
				ID3D11RenderTargetView* renderTargetView;
				/// <summary>�f�v�X�X�e���V���r���[</summary>
				ID3D11DepthStencilView* depthStencilView;


			public:
				// �f�t�H���g�R���X�g���N�^
				Texture2D();
				// �f�X�g���N�^
				virtual ~Texture2D() override final;

			public:
				/// <summary>�e�N�X�`���T�C�Y�̎擾</summary>
				/// <param name="_width">���̊i�[��</param>
				/// <param name="_height">�����̊i�[��</param>
				virtual void GetTextureSize(unsigned int* const _width, unsigned int* const _height) const override final{
					if(_width != nullptr) (*_width) = this->width;
					if(_height != nullptr) (*_height) = this->height;
				}
				// �V�F�[�_���\�[�X�r���[�̎擾
				ID3D11ShaderResourceView* GetShaderResourceView() const;
				// �����_�[�^�[�Q�b�g�r���[�̎擾
				ID3D11RenderTargetView* GetRenderTargetView() const;
				// �f�v�X�X�e���V���r���[�̎擾
				ID3D11DepthStencilView* GetDepthStencilView() const;

			private:
				// �o�C���h�\��?
				bool IsBindable(BindFlag _bindFlag) const;
				// �f�o�C�X�̎擾
				ID3D11Device* GetDevice() const;

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
				// �쐬
				void Create(
					BindableFlag _bindableFlag,
					unsigned int _width,
					unsigned int _height,
					Format _format,
					unsigned int _cpuAccessFlag,
					unsigned int _sampleDescCount,
					unsigned int _sampleDescQuality,
					Dimention _dimention
				);
				// �T�C�Y�ύX
				void ChangeSize(unsigned int _width, unsigned int _height);

			private:
				// �e�N�X�`���̍쐬
				void CreateTexture();
				// �V�F�[�_���\�[�X�r���[�̍쐬
				void CreateShaderResourceView();
				// �����_�[�^�[�Q�b�g�r���[�̍쐬
				void CreateRenderTargetView();
				// �f�v�X�X�e���V���r���[�̍쐬
				void CreateDepthStencilView();

				// �S���\�[�X�̊J��
				void ReleaseAll();
				// �e�N�X�`���̊J��
				void ReleaseTexture();
				// ���ݗ��p���̃r���[���J��
				void ReleaseCurrentView();
				// �V�F�[�_���\�[�X�r���[�̊J��
				void ReleaseShaderResourceView();
				// �����_�[�^�[�Q�b�g�r���[�̊J��
				void ReleaseRenderTargetView();
				// �f�v�X�X�e���V���r���[�̊J��
				void ReleaseDepthStencilView();
			};
		}
	}
}

#endif
