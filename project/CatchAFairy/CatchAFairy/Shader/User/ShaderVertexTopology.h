//=============================================================================
//	ShaderVertexTopology.h
//
//	�V�F�[�_���_�g�|���W�[�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SHADER_VERTEX_TOPOLOGY_H__
#define __SHADER_VERTEX_TOPOLOGY_H__

#pragma warning(disable:4471)
enum D3D_PRIMITIVE_TOPOLOGY;
#pragma warning(default:4471)


namespace CatchAFairy{
	namespace Shader{
		namespace User{
			/// <summary>
			/// �V�F�[�_���_�g�|���W�[
			/// </summary>
			enum class VertexTopology : int{
				/// <summary>�|�C���g���X�g</summary>
				PointList,
				/// <summary>�������X�g</summary>
				LineList,
				/// <summary>�����X�g���b�v</summary>
				LineStrip,
				/// <summary>�O�p�`���X�g</summary>
				TriangleList,
				/// <summary>�O�p�`�X�g���b�v</summary>
				TriangleStrip
			};

			// �V�F�[�_���_�g�|���W�[��D3D�v���~�e�B�u�g�|���W�[�ɕϊ�
			D3D_PRIMITIVE_TOPOLOGY ConvertVertexTopologyToD3DPrimitiveTopology(VertexTopology _topology);
		}
	}
}

#endif
