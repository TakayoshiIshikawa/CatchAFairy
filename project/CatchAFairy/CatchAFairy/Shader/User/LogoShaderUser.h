//=============================================================================
//	LogoShaderUser.h
//
//	���S�V�F�[�_���[�U�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __LOGO_SHADER_USER_H__
#define __LOGO_SHADER_USER_H__

struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;
#include "ShaderUserBase.h"
#include "Ishikawa/Math/ITFloatN.h"
namespace Ishikawa{
	namespace DirectX{
		struct Viewport;
	}
}

namespace CatchAFairy{
	namespace Shader{
		namespace User{
			enum class VertexTopology : int;


			/// <summary>
			/// ���S�V�F�[�_���[�U
			/// </summary>
			class Logo final : public Base{
			public:
				/// <summary>
				/// ���_�V�F�[�_����
				/// </summary>
				struct VSInput{
					/// <summary>�ʒu</summary>
					::Ishikawa::Math::Float2 position;
					/// <summary>�e�N�X�`���R�[�f�B�l�[�g</summary>
					::Ishikawa::Math::Float2 textureCoordinate;
				};
				/// <summary>
				/// �萔�o�b�t�@
				/// </summary>
				struct ConstantBuffer{
					/// <summary>�F�̔䗦</summary>
					::Ishikawa::Math::Float4 colorRate;
				};
				/// <summary>
				/// �`�旘�p���\�[�X
				/// </summary>
				struct RenderUseResource{
					/// <summary>�萔�o�b�t�@</summary>
					ConstantBuffer constantBuffer;
					/// <summary>���̓g�|���W�[</summary>
					VertexTopology topology;
					/// <summary>���_�o�b�t�@</summary>
					ID3D11Buffer* vertexBuffer;
					/// <summary>�C���f�b�N�X�z��</summary>
					ID3D11Buffer* indexBuffer;
					/// <summary>�C���f�b�N�X��</summary>
					unsigned int indexCount;
					/// <summary>�`�悷��V�F�[�_���\�[�X�r���[</summary>
					ID3D11ShaderResourceView* shaderResourceView;
				};
				/// <summary>
				/// �o�̓��\�[�X
				/// </summary>
				struct OutputResource{
					/// <summary>�����_�[�^�[�Q�b�g�r���[</summary>
					ID3D11RenderTargetView* renderTargetView;
					/// <summary>�r���[�|�[�g</summary>
					::Ishikawa::DirectX::Viewport* viewport;
				};


			private:
				/// <summary>���̓��C�A�E�g��</summary>
				static const unsigned int INPUT_LAYOUT_COUNT = 2U;
				// ���̓��C�A�E�g�ݒ�
				static const D3D11_INPUT_ELEMENT_DESC INPUT_LAYOUT_DESC[INPUT_LAYOUT_COUNT];
				// ���_�V�F�[�_�t�@�C���p�X
				static const char* const VERTEX_SHADER_FILEPATH;
				// �W�I���g���V�F�[�_�t�@�C���p�X
				static const char* const GEOMETRY_SHADER_FILEPATH;
				// �s�N�Z���V�F�[�_�t�@�C���p�X
				static const char* const PIXEL_SHADER_FILEPATH;
				/// <summary>�萔�o�b�t�@��</summary>
				static const unsigned int CONSTANT_BUFFER_COUNT = 1U;

			private:
				/// <summary>�萔�o�b�t�@�z��</summary>
				ID3D11Buffer* constantBufferArray[CONSTANT_BUFFER_COUNT];


			public:
				// �f�t�H���g�R���X�g���N�^
				Logo();
				// �f�X�g���N�^
				virtual ~Logo() override final;

			protected:
				/// <summary>���̓��C�A�E�g���̎擾</summary>
				/// <returns>���̓��C�A�E�g��</returns>
				virtual unsigned int GetInputLayoutCount() const override final{
					return INPUT_LAYOUT_COUNT;
				}
				/// <summary> ���̓��C�A�E�g�ݒ�̎擾</summary>
				/// <returns> ���̓��C�A�E�g�ݒ�</returns>
				virtual const D3D11_INPUT_ELEMENT_DESC* GetInputLayoutDesc() const override final{
					return INPUT_LAYOUT_DESC;
				}
				/// <summary>���_�V�F�[�_�t�@�C���p�X�̎擾</summary>
				/// <returns>���_�V�F�[�_�t�@�C���p�X</returns>
				virtual const char* GetVertexShaderFilepath() const override final{
					return VERTEX_SHADER_FILEPATH;
				}
				/// <summary>�W�I���g���V�F�[�_�t�@�C���p�X�̎擾</summary>
				/// <returns>�W�I���g���V�F�[�_�t�@�C���p�X</returns>
				virtual const char* GetGeometryShaderFilepath() const override final{
					return GEOMETRY_SHADER_FILEPATH;
				}
				/// <summary>�s�N�Z���V�F�[�_�t�@�C���p�X�̎擾</summary>
				/// <returns>�s�N�Z���V�F�[�_�t�@�C���p�X</returns>
				virtual const char* GetPixelShaderFilepath() const override final{
					return PIXEL_SHADER_FILEPATH;
				}

			public:
				// ���_�o�b�t�@�쐬
				// [���p���I�����o�b�t�@�͊J�����Ă�������]
				ID3D11Buffer* CreateVertexBuffer(const VSInput* const _vertexArray, unsigned int _vertexCount);
				// �T���v���X�e�[�g�̍쐬
				virtual void CreateSamplerState(ID3D11Device* const _device) override final;
				// �u�����h�X�e�[�g�̍쐬
				virtual void CreateBlendState(ID3D11Device* const _device) override final;
				// �f�v�X�X�e���V���X�e�[�g�̍쐬
				virtual void CreateDepthStencilState(ID3D11Device* const _device) override final;
				// ���X�^���C�U�X�e�[�g�̍쐬
				virtual void CreateRasterizerState(ID3D11Device* const _device) override final;

				// �`��O����
				void DrawBefore(
					const OutputResource& _outputResource,
					const ::Ishikawa::Math::Float4* const _color
				);
				// �`��
				void Draw(const RenderUseResource& _inputResource);
				// �`��㏈��
				void DrawAfter();

			private:
				// �萔�o�b�t�@�z��쐬
				void CreateConstantBufferArray();
				// �萔�o�b�t�@�z��J��
				void ReleaseConstantBufferArray();
			};
		}
	}
}

#endif
