//=============================================================================
//	DirectionalLight.h
//
//	���s���C�g�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __DIRECTIONAL_LIGHT_H__
#define __DIRECTIONAL_LIGHT_H__

#include "CatchAFairy/Object/CameraObjectInterface.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "Ishikawa/Math/ITMatrix.h"
#include "Ishikawa/DirectX/Texture/ITTexture2D.h"
#include "CatchAFairy/Shader/User/DirectionalLightShaderUser.h"


namespace CatchAFairy{
	namespace Object{
		class RenderInterface;
		class RenderSameInterface;


		namespace PlayScene{
			/// <summary>
			/// ���s���C�g
			/// </summary>
			class DirectionalLight final : public ::CatchAFairy::Object::CameraInterface{
			private:
				// �e�N�X�`���T�C�Y
				static const unsigned int TEXTURE_SIZE;
				// �����_����̋���
				static const float DISTANCE;
				// Y���̉�]�p[���p������]
				static const float ROTATE_Y;
				// X���̉�]�p[����������]
				static const float ROTATE_X;
				// �\���T�C�Y
				static const float VIEW_SIZE;
				// �j�A�N���b�v
				static const float NEAR_CLIP;
				// �t�@�[�N���b�v
				static const float FAR_CLIP;
				// �f�t�H���g�F
				static const ::Ishikawa::Math::Float4 DEFAULT_COLOR;

			private:
				/// <summary>�����_�[�^�[�Q�b�g�摜</summary>
				::Ishikawa::DirectX::Texture::Texture2D renderTargetTexture;
				/// <summary>�f�v�X�X�e���V���摜</summary>
				::Ishikawa::DirectX::Texture::Texture2D depthStencilTexture;
				/// <summary>���s���C�g�V�F�[�_���[�U</summary>
				::CatchAFairy::Shader::User::DirectionalLight directionalLightShaderUser;

				/// <summary>���[�J���ϊ��s��</summary>
				::Ishikawa::Math::Matrix local;
				/// <summary>���[���h�ϊ��s��</summary>
				::Ishikawa::Math::Matrix world;
				/// <summary>�r���[�ϊ��s��</summary>
				::Ishikawa::Math::Matrix view;
				/// <summary>�v���W�F�N�V�����ϊ��s��</summary>
				::Ishikawa::Math::Matrix projection;
				/// <summary>�F</summary>
				::Ishikawa::Math::Float4 color;


			public:
				// �f�t�H���g�R���X�g���N�^
				DirectionalLight();
				// �f�X�g���N�^
				virtual ~DirectionalLight() override final;

			public:
				/// <summary>���[���h�ϊ��s��̎擾</summary>
				/// <returns>���[���h�ϊ��s��</returns>
				virtual const ::Ishikawa::Math::Matrix& GetWorld() const override final{
					return this->world;
				}
				/// <summary>�r���[�ϊ��s��̎擾</summary>
				/// <returns>�r���[�ϊ��s��</returns>
				virtual const ::Ishikawa::Math::Matrix& GetView() const override final{
					return this->view;
				}
				/// <summary>�v���W�F�N�V�����ϊ��s��̎擾</summary>
				/// <returns>�v���W�F�N�V�����ϊ��s��</returns>
				virtual const ::Ishikawa::Math::Matrix& GetProjection() const override final{
					return this->projection;
				}
				/// <summary>�����̎擾</summary>
				/// <returns>����</returns>
				const ::Ishikawa::Math::Float4& GetDirection() const { return this->local.row3; }
				/// <summary>�F�̎擾</summary>
				/// <returns>�F</returns>
				const ::Ishikawa::Math::Float4& GetColor() const { return this->color; }
				/// <summary>�ŏI�V�F�[�_���\�[�X�r���[�̎擾</summary>
				/// <returns>�ŏI�V�F�[�_���\�[�X�r���[</returns>
				virtual ID3D11ShaderResourceView* GetLastShaderResourceView() const override final{
					return this->renderTargetTexture.GetShaderResourceView();
				}

			public:
				/// <summary>�����_�̐ݒ�</summary>
				/// <param name="_position">�����_���W</param>
				void SetLookPosition(const ::Ishikawa::Math::Float3& _position){
					this->SetLookPosition(_position.x, _position.y, _position.z);
				}
				// �����_�̐ݒ�
				void SetLookPosition(float _x, float _y, float _z);
				/// <summary>�F�̐ݒ�</summary>
				/// <param name="_color">�F</param>
				void SetColor(const ::Ishikawa::Math::Float4& _color){ this->color = _color; }
				/// <summary>�F�̐ݒ�</summary>
				/// <param name="_red">�Ԑ���</param>
				/// <param name="_green">�ΐ���</param>
				/// <param name="_blue">����</param>
				/// <param name="_alpha">�񓧖�����</param>
				void SetColor(float _red, float _green, float _blue, float _alpha){
					this->SetColor(::Ishikawa::Math::Float4(_red, _green, _blue, _alpha));
				}
				/// <summary>�F�̐ݒ�</summary>
				/// <param name="_red">�Ԑ���</param>
				/// <param name="_green">�ΐ���</param>
				/// <param name="_blue">����</param>
				void SetColor(float _red, float _green, float _blue){
					this->SetColor(::Ishikawa::Math::Float4(_red, _green, _blue, 1.0f));
				}

			public:
				// �Q�[���̃f�o�C�X�Ɉˑ����郊�\�[�X�̒ǉ�
				virtual void AddITDeviceDependentResources(::Ishikawa::DirectX::Device::Device* const _itDevice) override final;
				// �f�o�C�X�ˑ����\�[�X�쐬
				virtual void CreateDeviceDependentResources() override final;
				// �f�o�C�X�ˑ����\�[�X�J��
				virtual void ReleaseDeviceDependentResources() override final;

				// ������
				void Initialize();

				// ���s���C�g�V�F�[�_�`��O����
				void DirectionalLightShaderDrawBefore();
				// ���s���C�g�V�F�[�_�ŃI�u�W�F�N�g��`��
				void RenderObjectToDirectionalLightShader(
					const ::CatchAFairy::Object::RenderInterface& _renderObject
				);
				// ���s���C�g�V�F�[�_�ŕ����I�u�W�F�N�g��`��
				void RenderSameObjectToDirectionalLightShader(
					const ::CatchAFairy::Object::RenderSameInterface& _renderObjects
				);
				// ���s���C�g�V�F�[�_�`��㏈��
				void DirectionalLightShaderDrawAfter();

			private:
				// �e�N�X�`���쐬
				void CreateTextures();
			};
		}
	}
}

#endif
