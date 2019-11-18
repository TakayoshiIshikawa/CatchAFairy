//=============================================================================
//	ITViewport.cpp
//
//	����r���[�|�[�g�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITViewport.h"
#pragma warning(disable:4061)
#pragma warning(disable:4365)
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#pragma warning(disable:4917)
#include <d3d11_1.h>
#pragma warning(default:4917)
#pragma warning(default:4820)
#pragma warning(default:4668)
#pragma warning(default:4365)
#pragma warning(default:4061)


/// <summary>
/// ����r���[�|�[�g����D3D11�r���[�|�[�g�ɕϊ�
/// </summary>
/// <param name="_viewport">����r���[�|�[�g</param>
/// <returns>D3D11�r���[�|�[�g</returns>
D3D11_VIEWPORT Ishikawa::DirectX::ConvertViewportToD3D11Viewport(const Viewport& _viewport){
	D3D11_VIEWPORT viewport;

	viewport.TopLeftX	= _viewport.leftTopX;
	viewport.TopLeftY	= _viewport.leftTopY;
	viewport.Width		= _viewport.width;
	viewport.Height		= _viewport.height;
	viewport.MinDepth	= _viewport.minDepth;
	viewport.MaxDepth	= _viewport.maxDepth;

	return viewport;
}
