//=============================================================================
//	RenderSameObjectInterface.h
//
//	�����`��I�u�W�F�N�g�C���^�[�t�F�[�X�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __RENDER_SAME_OBJECT_INTERFACE_H__
#define __RENDER_SAME_OBJECT_INTERFACE_H__

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

namespace Ishikawa{
	namespace Math{
		struct Float3;
		struct Matrix;
	}
}

namespace CatchAFairy{
	namespace Shader{
		namespace User{
			enum class VertexTopology : int;
		}
	}


	namespace Object{
		/// <summary>
		/// �����`��I�u�W�F�N�g�C���^�[�t�F�[�X
		/// </summary>
		class RenderSameInterface{
		public:
			/// <summary>�f�t�H���g�R���X�g���N�^</summary>
			RenderSameInterface(){}
			/// <summary>�f�X�g���N�^</summary>
			virtual ~RenderSameInterface(){}

		public:
			// �g�|���W�[�^�C�v�̎擾
			virtual ::CatchAFairy::Shader::User::VertexTopology GetTopology() const = 0;
			// ���_�o�b�t�@�̎擾
			virtual ID3D11Buffer* GetVertexBuffer() const = 0;
			// �C���f�b�N�X�o�b�t�@�̎擾
			virtual ID3D11Buffer* GetIndexBuffer() const = 0;
			// �C���f�b�N�X���̎擾
			virtual unsigned int GetIndexCount() const = 0;
			// �O�ډ~���S���W�̎擾
			virtual const ::Ishikawa::Math::Float3& GetCircumscribedCircleCenterPosition() const = 0;
			// �O�ډ~���a���̎擾
			virtual float GetSquareCircumscribedCircleRadius() const = 0;
			// �g�U���˃V�F�[�_���\�[�X�r���[�̎擾
			virtual ID3D11ShaderResourceView* GetDiffuseShaderResourceView() const = 0;
			// �@���V�F�[�_���\�[�X�r���[�̎擾
			virtual ID3D11ShaderResourceView* GetNormalShaderResourceView() const = 0;
			// ���[���h�ϊ��s�񐔂̎擾
			virtual unsigned int GetWorldCount() const = 0;
			// ���[���h�ϊ��s��z��̎擾
			virtual const ::Ishikawa::Math::Matrix* GetWorldArray() const = 0;
		};
	}
}

#endif
