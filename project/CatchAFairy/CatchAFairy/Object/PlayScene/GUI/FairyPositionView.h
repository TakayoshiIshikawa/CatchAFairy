//=============================================================================
//	FairyPositionView.h
//
//	�d���ʒu�\���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __FAIRY_POSITION_VIEW_H__
#define __FAIRY_POSITION_VIEW_H__

#include "GUIObjectInterface.h"
#include "Ishikawa/Math/ITFloatN.h"

namespace CatchAFairy{
	namespace Object{
		namespace PlayScene{
			class EnemyFairies;
			class Player;

			namespace GUI{
				/// <summary>
				/// �d���ʒu�\��
				/// </summary>
				class FairyPositionView final : public GUIInterface{
				private:
					// �\���F
					static const ::Ishikawa::Math::Float3 COLOR;
					// �`��e�N�X�`���R�[�f�B�l�[�g(����ƉE��)
					static const ::Ishikawa::Math::Float4 SHAPE_TEXTURE_COORDINATE;
					// �\���e�N�X�`���R�[�f�B�l�[�g(����ƉE��)
					static const ::Ishikawa::Math::Float4 VIEW_TEXTURE_COORDINATE;
					// �傫��
					static const ::Ishikawa::Math::Float2 SIZE;

				private:
					/// <summary>GUI�}�l�[�W��</summary>
					::CatchAFairy::Object::PlayScene::GUI::GUIManager* manager;
					/// <summary>���p���_</summary>
					::CatchAFairy::Shader::User::PlayGUI::VSInput* useVertex;
					/// <summary>�G�d��</summary>
					const ::CatchAFairy::Object::PlayScene::EnemyFairies* enemyFairies;
					/// <summary>�v���C��</summary>
					const ::CatchAFairy::Object::PlayScene::Player* player;


				public:
					// �f�t�H���g�R���X�g���N�^
					FairyPositionView();
					// �f�X�g���N�^
					virtual ~FairyPositionView() override final;


				public:
					// �}�l�[�W���̐ݒ�
					virtual void SetManager(
						::CatchAFairy::Object::PlayScene::GUI::GUIManager* const _manager
					) override final;
					/// <summary>�d���̐ݒ�</summary>
					/// <param name="_enemyFairies">�G�d��</param>
					void SetFairy(const ::CatchAFairy::Object::PlayScene::EnemyFairies* const _enemyFairies){
						this->enemyFairies = _enemyFairies;
					}
					/// <summary>�v���C���̐ݒ�</summary>
					/// <param name="_player">�v���C��</param>
					void SetPlayer(const ::CatchAFairy::Object::PlayScene::Player* const _player){
						this->player = _player;
					}

				protected:
					// ���_�̐ݒ�
					virtual void SetVertices(
						::CatchAFairy::Shader::User::PlayGUI::VSInput* const _vertices,
						unsigned int _count
					) override final;

				public:
					// �X�V
					virtual void Update(float _elapsedTime) override final;
				};
			}
		}
	}
}

#endif
