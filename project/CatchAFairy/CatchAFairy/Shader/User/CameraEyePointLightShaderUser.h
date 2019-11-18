//=============================================================================
//	CameraEyePointLightShaderUser.h
//
//	�J�������_�|�C���g���C�g�V�F�[�_���[�U�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __CAMERA_EYE_POINT_LIGHT_SHADER_USER_H__
#define __CAMERA_EYE_POINT_LIGHT_SHADER_USER_H__

struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;
#include "ShaderUserBase.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "Ishikawa/Math/ITMatrix.h"
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
			/// �J�������_�|�C���g���C�g�V�F�[�_���[�U
			/// </summary>
			class CameraEyePointLight final : public Base{
			public:
				/// <summary>
				/// ���_�V�F�[�_����
				/// </summary>
				struct VSInput{
					/// <summary>�ʒu</summary>
					::Ishikawa::Math::Float3 position;
					/// <summary>�F</summary>
					::Ishikawa::Math::Float4 color;
					/// <summary>���a�Ɩ��邳</summary>
					::Ishikawa::Math::Float2 radiusAndLumen;
				};
				/// <summary>
				/// �萔�o�b�t�@
				/// </summary>
				struct ConstantBuffer{
					/// <summary>���[�J����Ԃ���v���W�F�N�V������Ԃɕϊ�����s��</summary>
					::Ishikawa::Math::Matrix localToProjectionMatrix;
					/// <summary>���[�J����Ԃ���r���[��Ԃɕϊ�����s��</summary>
					::Ishikawa::Math::Matrix localToViewMatrix;
					/// <summary>�v���W�F�N�V�����ϊ��s��</summary>
					::Ishikawa::Math::Matrix projectionMatrix;
					/// <summary>�v���W�F�N�V������Ԃ̋t�s��</summary>
					::Ishikawa::Math::Matrix invertProjectionMatrix;
				};
				/// <summary>
				/// �`�旘�p���\�[�X
				/// </summary>
				struct RenderUseResource{
					/// <summary>�萔�o�b�t�@</summary>
					ConstantBuffer constantBuffer;
					/// <summary>���_�o�b�t�@</summary>
					ID3D11Buffer* vertexBuffer;
					/// <summary>�C���f�b�N�X�z��</summary>
					ID3D11Buffer* indexBuffer;
					/// <summary>�C���f�b�N�X��</summary>
					unsigned int indexCount;
					/// <summary>�V�F�[�_���\�[�X��</summary>
					static const unsigned int SHADER_RESOURCE_COUNT = 1U;
#pragma warning(disable:4201)
					union{
						struct{
							/// <summary>�v���W�F�N�V�������W�V�F�[�_���\�[�X�r���[</summary>
							ID3D11ShaderResourceView* positionResourceView;
						};
						/// <summary>�V�F�[�_���\�[�X�z��</summary>
						ID3D11ShaderResourceView* shaderResources[SHADER_RESOURCE_COUNT];
					};
#pragma warning(default:4201)
				};
				/// <summary>
				/// �o�̓��\�[�X
				/// </summary>
				struct OutputResource{
					/// <summary>�����_�[�^�[�Q�b�g��</summary>
					static const unsigned int RENDER_TARGET_COUNT = 1U;
#pragma warning(disable:4201)
					union{
						struct{
							/// <summary>�|�C���g���C�g�����_�[�^�[�Q�b�g�r���[</summary>
							ID3D11RenderTargetView* pointLightRenderTargetView;
						};
						/// <summary>�����_�[�^�[�Q�b�g�z��</summary>
						ID3D11RenderTargetView* renderTargets[RENDER_TARGET_COUNT];
					};
#pragma warning(default:4201)
					/// <summary>�J�������_1�ԃV�F�[�_�̃f�v�X�X�e���V���r���[</summary>
					ID3D11DepthStencilView* firstDepthStencilView;
					/// <summary>�r���[�|�[�g</summary>
					::Ishikawa::DirectX::Viewport* viewport;
				};


			private:
				/// <summary>���̓��C�A�E�g��</summary>
				static const unsigned int INPUT_LAYOUT_COUNT = 3U;
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
				/// <summary>���̓g�|���W�[</summary>
				VertexTopology topology;


			public:
				// �f�t�H���g�R���X�g���N�^
				CameraEyePointLight();
				// �f�X�g���N�^
				virtual ~CameraEyePointLight() override final;

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
				// �f�o�C�X�ɓǂݍ��܂ꂽ
				virtual void OnDeviceLoaded() override final;

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
				void DrawBefore(const OutputResource& _outputResource, const float (&_color)[4]);
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
