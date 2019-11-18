//=============================================================================
//	ITSwapChain.h
//
//	����f�o�C�X�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_SWAP_CHAIN_H__
#define __IT_SWAP_CHAIN_H__

struct HWND__;
struct ID3D11Device;
struct IDXGISwapChain;
struct IDXGIFactory;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
#pragma warning(disable:4471)
enum DXGI_FORMAT;
enum D3D11_DSV_DIMENSION;
#pragma warning(default:4471)


namespace Ishikawa{
	namespace Window{
		class Window;
	}
	namespace DirectX{
		namespace Device{
			class Device;
		}


		/// <summary>
		/// ����X���b�v�`�F�[��
		/// </summary>
		class SwapChain final{
		private:
			/// <summary>�T���v���ݒ�J�E���g</summary>
			unsigned int sampleDescCount;
			/// <summary>�T���v���ݒ�N�H���e�B</summary>
			unsigned int sampleDescQuality;
			/// <summary>�X���b�v�`�F�[��</summary>
			IDXGISwapChain* swapChain;
			/// <summary>�o�b�t�@�J�E���g</summary>
			unsigned int bufferCount;
			/// <summary>�o�b�t�@�t�H�[�}�b�g</summary>
			DXGI_FORMAT bufferFormat;
			/// <summary>��</summary>
			unsigned int width;
			/// <summary>����</summary>
			unsigned int height;
			/// <summary>�����_�[�^�[�Q�b�g�e�N�X�`��</summary>
			ID3D11Texture2D* renderTargetTexture;
			/// <summary>�����_�[�^�[�Q�b�g�r���[</summary>
			ID3D11RenderTargetView* renderTargetView;
			/// <summary>�f�v�X�X�e���V���t�H�[�}�b�g</summary>
			DXGI_FORMAT depthStencilFormat;
			/// <summary>�f�v�X�X�e���V���r���[�f�B�����V����</summary>
			D3D11_DSV_DIMENSION depthStencilViewDimention;
			/// <summary>�f�v�X�X�e���V���e�N�X�`��</summary>
			ID3D11Texture2D* depthStencilTexture;
			/// <summary>�f�v�X�X�e���V���r���[</summary>
			ID3D11DepthStencilView* depthStencilView;
			/// <summary>�ˑ��f�o�C�X</summary>
			Device::Device* dependentDevice;


		public:
			// �f�t�H���g�R���X�g���N�^
			SwapChain();
			// �f�X�g���N�^
			~SwapChain();

		public:
			/// <summary>�X���b�v�`�F�[���̎擾</summary>
			/// <returns>�X���b�v�`�F�[��</returns>
			IDXGISwapChain* GetSwapChain() const { return this->swapChain; }
			/// <summary>�����_�[�^�[�Q�b�g�r���[�̎擾</summary>
			/// <returns>�����_�[�^�[�Q�b�g�r���[</returns>
			ID3D11RenderTargetView* GetRenderTargetView() const { return this->renderTargetView; }
			/// <summary>�f�v�X�X�e���V���r���[�̎擾</summary>
			/// <returns>�f�v�X�X�e���V���r���[</returns>
			ID3D11DepthStencilView* GetDepthStencilView() const { return this->depthStencilView; }
			/// <summary>�T�C�Y�̎擾</summary>
			/// <param name="_width">���̊i�[��</param>
			/// <param name="_height">�����̊i�[��</param>
			void GetSize(unsigned int* const _width, unsigned int* const _height) const {
				if(_width != nullptr) (*_width) = this->width;
				if(_height != nullptr) (*_height) = this->height;
			}

		public:
			// �쐬
			void Create(Device::Device* const _device, ::Ishikawa::Window::Window* const _window);
			// �T�C�Y�ύX
			void ChangeSize(unsigned int _width, unsigned int _height);

			// ��ʂ̃N���A
			void ClearScreen();
			// �o�b�t�@�����ւ��ĕ\��
			void Present();

		private:
			// �X���b�v�`�F�[���̍쐬
			void CreateSwapChain(
				IDXGIFactory* const _factory,
				ID3D11Device* const _device,
				HWND__* const _windowHandle
			);
			// �t�@�N�g���̎擾
			void GetFactory(IDXGIFactory** const _factory, ID3D11Device* const _device);
			// �f�t�H���g�T���v���ݒ�ɕύX
			void ChangeDefaultSampleDesc();
			// �}���`�T���v���E�A���`�G�C���A�V���O�ݒ�ɕύX
			void ChangeMultisampleAntiAliasing(ID3D11Device* const _device);
			// �����_�[�^�[�Q�b�g�e�N�X�`���̍쐬
			void CreateRenderTargetTexture();
			// �����_�[�^�[�Q�b�g�r���[�̍쐬
			void CreateRenderTargetView(ID3D11Device* const _device);
			// �f�v�X�X�e���V���e�N�X�`���̍쐬
			void CreateDepthStencilTexture(ID3D11Device* const _device);
			// �f�v�X�X�e���V���r���[�̍쐬
			void CreateDepthStencilView(ID3D11Device* const _device);

			// �X���b�v�`�F�[���̊J��
			void ReleaseSwapChain();
			// �����_�[�^�[�Q�b�g�e�N�X�`���̊J��
			void ReleaseRenderTargetTexture();
			// �����_�[�^�[�Q�b�g�r���[�̊J��
			void ReleaseRenderTargetView();
			// �f�v�X�X�e���V���e�N�X�`���̊J��
			void ReleaseDepthStencilTexture();
			// �f�v�X�X�e���V���r���[�̊J��
			void ReleaseDepthStencilView();
		};
	}
}

#endif
