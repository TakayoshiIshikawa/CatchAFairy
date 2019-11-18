//=============================================================================
//	Player.h
//
//	�v���C���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "CatchAFairy/Object/UpdateObjectInterface.h"
#include "CatchAFairy/Object/RenderObjectInterface.h"
#include "Ishikawa/DirectX/Device/DependentResource/ITDeviceDependentResourceBase.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "Ishikawa/Math/ITMatrix.h"
#include "Ishikawa/DirectX/Texture/ITFileTexture.h"
#include "CatchAFairy/Data/EnemyFairyType.h"

namespace Ishikawa{
	namespace Model{
		namespace ItObject{
			struct VertexData;
		}
	}
	namespace Input{
		class Keyboard;
		enum class KeyCode : unsigned long;
	}
}

namespace CatchAFairy{
	namespace Shader{
		namespace User{
			class PlayCameraEyeFirst;
		}
	}
	namespace Scene{
		class Play;
	}

	namespace Object{
		namespace PlayScene{
			class PlayCamera;
			class PointLight;
			namespace GUI{
				class OperationMethodView;
			}

			/// <summary>
			/// �v���C��
			/// </summary>
			class Player final :
				public ::Ishikawa::DirectX::Device::DependentResource::Base,
				public ::CatchAFairy::Object::UpdateInterface,
				public ::CatchAFairy::Object::RenderInterface
			{
			private:
				// ���샂�f���f�[�^�t�@�C���p�X
				static const char* const IT_MODEL_DATA_FILEPATH;
				// �g�U���˃e�N�X�`���t�@�C���p�X
				static const wchar_t* const DIFFUSE_TEXTURE_FILEPATH;
				// �@���e�N�X�`���t�@�C���p�X
				static const wchar_t* const NORMAL_TEXTURE_FILEPATH;
				// �d��ID
				static const unsigned int FAIRY_ID;
				// �{�̃��C�g�̂���
				static const ::Ishikawa::Math::Float3 FAIRY_LIGHT_ANCHOR;
				// �{�̃��C�g�̐F
				static const ::Ishikawa::Math::Float4 FAIRY_LIGHT_COLOR;
				// �{�̃��C�g�̔��a
				static const float FAIRY_LIGHT_RADIUS;
				// �{�̃��C�g�̌���
				static const float FAIRY_LIGHT_LUMEN;
				// ��{�̃}�i���C�g�̐F
				static const ::Ishikawa::Math::Float4 DEFAULT_MANA_LIGHT_COLOR;
				// �e�d����߂܂����Ƃ��̃}�i���C�g�̐F
				static const ::Ishikawa::Math::Float4 CATCHED_FAIRY_MANA_LIGHT_COLOR[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)];
				// �}�i���C�g�̔��a
				static const float MANA_LIGHT_RADIUS;
				// �}�i���C�g�̌���
				static const float MANA_LIGHT_LUMEN;
				// �ŏ��̃}�i���C�g�����J�E���g�����䗦
				static const float MINIMUM_MANA_LIGHT_GENERATE_COUNT_ADDITIONAL_RATIO;
				// �ő�̃}�i���C�g�����J�E���g�����䗦
				static const float MAXIMUM_MANA_LIGHT_GENERATE_COUNT_ADDITIONAL_RATIO;
				// �J�n�ʒu
				static const ::Ishikawa::Math::Float3 START_POSITION;
				// �ړ����x
				static const float MOVE_SPEED;
				// ��]���x
				static const float ROTATE_SPEED;
				// �ߊl�Ԋu
				static const float CATCH_INTERVAL;

				// �d����߂܂�����Œፂ��
				static const float FAIRY_CATCHABLE_HEIGHT_MIN;
				// �d����߂܂�����ō�����
				static const float FAIRY_CATCHABLE_HEIGHT_MAX;
				// �d����߂܂����锼�a
				static const float FAIRY_CATCHABLE_RADIUS;
				// �d����߂܂������`���a
				static const float FAIRY_CATCHABLE_ANGLE_RADIUS;
				// �d����߂܂�����p�x�̃R�T�C���l
				static const float FAIRY_CATCHABLE_ANGLE_COS;

				// �O�i�L�[
				static const ::Ishikawa::Input::KeyCode FRONT_MOVE_KEY;
				// ��i�L�[
				static const ::Ishikawa::Input::KeyCode BACK_MOVE_KEY;
				// �E�i�L�[
				static const ::Ishikawa::Input::KeyCode RIGHT_MOVE_KEY;
				// ���i�L�[
				static const ::Ishikawa::Input::KeyCode LEFT_MOVE_KEY;
				// ��i�L�[
				static const ::Ishikawa::Input::KeyCode UP_MOVE_KEY;
				// ���i�L�[
				static const ::Ishikawa::Input::KeyCode DOWN_MOVE_KEY;
				// �E����L�[
				static const ::Ishikawa::Input::KeyCode RIGHT_ROTATE_KEY;
				// ������L�[
				static const ::Ishikawa::Input::KeyCode LEFT_ROTATE_KEY;
				// �ߊl�L�[
				static const ::Ishikawa::Input::KeyCode CATCH_KEY;

			private:
				/// <summary>���p���Ă���V�F�[�_</summary>
				const ::CatchAFairy::Shader::User::PlayCameraEyeFirst* useShader;
				/// <summary>����I�u�W�F�N�g���_�z��</summary>
				::Ishikawa::Model::ItObject::VertexData* itObjectVertices;
				/// <summary>����I�u�W�F�N�g���_��</summary>
				unsigned int itObjectVertexCount;
				/// <summary>����I�u�W�F�N�g�C���f�b�N�X�z��</summary>
				unsigned long* itObjectIndices;
				/// <summary>����I�u�W�F�N�g�C���f�b�N�X��</summary>
				unsigned int itObjectIndexCount;
				/// <summary>�g�|���W�[�^�C�v</summary>
				::CatchAFairy::Shader::User::VertexTopology topology;
				/// <summary>���_�o�b�t�@</summary>
				ID3D11Buffer* vertexBuffer;
				/// <summary>�C���f�b�N�X�o�b�t�@</summary>
				ID3D11Buffer* indexBuffer;
				/// <summary>�C���f�b�N�X��</summary>
				unsigned int indexCount;
				/// <summary>�O�ډ~���S���W</summary>
				::Ishikawa::Math::Float3 circumscribedCircleCenterPosition;
				/// <summary>�O�ډ~���a���</summary>
				float squareCircumscribedCircleRadius;
				/// <summary>���[���h�s��</summary>
				::Ishikawa::Math::Matrix world;
				/// <summary>�g�U���˃e�N�X�`��</summary>
				::Ishikawa::DirectX::Texture::FileTexture diffuseTexture;
				/// <summary>�@���e�N�X�`��</summary>
				::Ishikawa::DirectX::Texture::FileTexture normalTexture;
				/// <summary>�v���C�J����</summary>
				PlayCamera* playCamera;
				/// <summary>�v���C�V�[��</summary>
				::CatchAFairy::Scene::Play* scenePlay;
				/// <summary>������@�\��</summary>
				GUI::OperationMethodView* operationMethodView;
				/// <summary>�ߊl����</summary>
				float catchTime;
				/// <summary>�|�C���g���C�g</summary>
				PointLight* pointLight;
				/// <summary>�}�i���C�g�����J�E���g�����䗦</summary>
				float manaLightGenerationCountAdditionalRatio;
				/// <summary>�}�i���C�g�����J�E���g</summary>
				float manaLightGenerationCount;
				/// <summary>�}�i���C�g�����F</summary>
				::Ishikawa::Math::Float4 manaLightGenerationColor;
				/// <summary>��ނ��Ƃ̕ߊl�d����</summary>
				unsigned int catchedFairyCount[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)];


			public:
				// �f�t�H���g�R���X�g���N�^
				Player();
				// �f�X�g���N�^
				virtual ~Player() override final;

			public:
				// �g�|���W�[�^�C�v�̎擾
				virtual ::CatchAFairy::Shader::User::VertexTopology GetTopology() const override final;
				// ���_�o�b�t�@�̎擾
				virtual ID3D11Buffer* GetVertexBuffer() const override final;
				// �C���f�b�N�X�o�b�t�@�̎擾
				virtual ID3D11Buffer* GetIndexBuffer() const override final;
				// �C���f�b�N�X���̎擾
				virtual unsigned int GetIndexCount() const override final;
				// �O�ډ~���S���W�̎擾
				virtual const ::Ishikawa::Math::Float3& GetCircumscribedCircleCenterPosition() const override final;
				// �O�ډ~���a���̎擾
				virtual float GetSquareCircumscribedCircleRadius() const override final;
				// �g�U���˃V�F�[�_���\�[�X�r���[�̎擾
				virtual ID3D11ShaderResourceView* GetDiffuseShaderResourceView() const override final;
				// �@���V�F�[�_���\�[�X�r���[�̎擾
				virtual ID3D11ShaderResourceView* GetNormalShaderResourceView() const override final;
				// ���[���h�ϊ��s��̎擾
				virtual const ::Ishikawa::Math::Matrix& GetWorld() const override final;

				/// <summary>�d���ߊl���̎擾</summary>
				/// <param name="_type">�d���̎��</param>
				/// <returns>�ߊl��</returns>
				unsigned int GetCatchedFairyCount(CatchAFairy::Data::EnemyFairyType _type) const {
					if(_type == CatchAFairy::Data::EnemyFairyType::Count) return 0U;
					return this->catchedFairyCount[static_cast<unsigned int>(_type)];
				}

			private:
				// �L�[�{�[�h�̎擾
				const ::Ishikawa::Input::Keyboard& GetKeyboard() const;

			public:
				// �v���C�J�����̐ݒ�
				void SetPlayCamera(PlayCamera* const _playCamera);
				//�@�|�C���g���C�g�̐ݒ�
				void SetPointLight(PointLight* const _pointLight);
				/// <summary>�v���C�V�[���̐ݒ�</summary>
				/// <param name="_scenePlay">�v���C�V�[��</param>
				void SetScenePlay(::CatchAFairy::Scene::Play* const _scenePlay){
					this->scenePlay = _scenePlay;
				}
				/// <summary>������@�\���̐ݒ�</summary>
				/// <param name="_operationMethodView">������@�\��</param>
				void SetOperationMethodView(GUI::OperationMethodView* const _operationMethodView){
					this->operationMethodView = _operationMethodView;
				}

			private:
				// ���[���h�ϊ��s��̐ݒ�
				void SetWorld(const ::Ishikawa::Math::Matrix& _world);

			public:
				// �f�o�C�X�ɓǂݍ��܂ꂽ
				virtual void OnDeviceLoaded() override final;
				// �f�o�C�X���������ꂽ
				virtual void OnDeviceRestored() override final;
				// �f�o�C�X���j�����ꂽ
				virtual void OnDeviceLost() override final;

			private:
				// �d����߂܂���
				void OnCaughtAFairy(CatchAFairy::Data::EnemyFairyType _catchedFairyType);

			public:
				// ������
				void Initialize();
				// �X�V
				virtual void Update(float _elapsedTime) override final;

				// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�쐬
				void CreateVertexBufferAndIndexBuffer(
					const ::CatchAFairy::Shader::User::PlayCameraEyeFirst* const _useShader
				);
				// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�J��
				void ReleaseVertexBufferAndIndexBuffer();

			private:
				// ����I�u�W�F�N�g�f�[�^�ǂݍ���
				void LoadItObjectData();
				// ����I�u�W�F�N�g�f�[�^�J��
				void ReleaseItObjectData();

				// �ړ�
				void Move(float _elapsedTime);
				// ��]
				void Rotate(float _elapsedTime);
				// �߂܂���
				void Catch();

				// �|�C���g���C�g�̍X�V
				void UpdatePointLight(float _elapsedTime);

				// �d����߂܂��悤�Ƃ���
				void TryCatchingFairies();
			};
		}
	}
}

#endif
