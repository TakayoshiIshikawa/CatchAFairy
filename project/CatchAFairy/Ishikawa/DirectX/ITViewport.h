//=============================================================================
//	ITViewport.h
//
//	����r���[�|�[�g�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_VIEWPORT_H__
#define __IT_VIEWPORT_H__

struct D3D11_VIEWPORT;


namespace Ishikawa{
	namespace DirectX{
		/// <summary>
		/// ����r���[�|�[�g
		/// </summary>
		struct Viewport{
		public:
			/// <summary>�����X���W</summary>
			float leftTopX;
			/// <summary>�����Y���W</summary>
			float leftTopY;
			/// <summary>��</summary>
			float width;
			/// <summary>����</summary>
			float height;
			/// <summary>�ŏ��[�x</summary>
			float minDepth;
			/// <summary>�ő�[�x</summary>
			float maxDepth;


		public:
			/// <summary>�f�t�H���g�R���X�g���N�^</summary>
			Viewport() :
				leftTopX(0.0f), leftTopY(0.0f),
				width(0.0f), height(0.0f),
				minDepth(0.0f), maxDepth(0.0f)
			{}
			/// <summary>�t���R���X�g���N�^</summary>
			/// <param name="_leftTopX">�����X���W</param>
			/// <param name="_leftTopY">�����Y���W</param>
			/// <param name="_width">��</param>
			/// <param name="_height">����</param>
			/// <param name="_minDepth">�ŏ��[�x</param>
			/// <param name="_maxDepth">�ő�[�x</param>
			Viewport(
				float _leftTopX, float _leftTopY,
				float _width, float _height,
				float _minDepth, float _maxDepth
			) :
				leftTopX(_leftTopX), leftTopY(_leftTopY),
				width(_width), height(_height),
				minDepth(_minDepth), maxDepth(_maxDepth)
			{}
			/// <summary>�R�s�[�R���X�g���N�^</summary>
			/// <param name="_original">�I���W�i���f�[�^</param>
			Viewport(const Viewport& _original) :
				leftTopX(_original.leftTopX), leftTopY(_original.leftTopY),
				width(_original.width), height(_original.height),
				minDepth(_original.minDepth), maxDepth(_original.maxDepth)
			{}
			/// <summary>�f�X�g���N�^</summary>
			~Viewport(){}

		public:
			/// <summary>�R�s�[�R���X�g���N�^</summary>
			/// <param name="_original">�I���W�i���f�[�^</param>
			/// <returns>���g�̃f�[�^</returns>
			Viewport& operator=(const Viewport& _original){
				this->leftTopX	= _original.leftTopX;
				this->leftTopY	= _original.leftTopY;
				this->width		= _original.width;
				this->height	= _original.height;
				this->minDepth	= _original.minDepth;
				this->maxDepth	= _original.maxDepth;
				return (*this);
			}
		};

		// ����r���[�|�[�g����D3D11�r���[�|�[�g�ɕϊ�
		D3D11_VIEWPORT ConvertViewportToD3D11Viewport(const Viewport& _viewport);
	}
}

#endif
