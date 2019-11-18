//=============================================================================
//	OperationMethodView.h
//
//	������@�\���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __OPERATION_METHOD_VIEW_H__
#define __OPERATION_METHOD_VIEW_H__

#include "GUIObjectInterface.h"
#include "Ishikawa/Math/ITFloatN.h"

namespace CatchAFairy{
	namespace Object{
		namespace PlayScene{
			namespace GUI{
				/// <summary>
				/// ������@�\��
				/// </summary>
				class OperationMethodView final : public GUIInterface{
				private:
					// �ʒu
					static const ::Ishikawa::Math::Float2 POSITION;
					// �\���F
					static const ::Ishikawa::Math::Float3 COLOR;
					// �`��e�N�X�`���R�[�f�B�l�[�g(����ƉE��)
					static const ::Ishikawa::Math::Float4 SHAPE_TEXTURE_COORDINATE;
					// �\���e�N�X�`���R�[�f�B�l�[�g(����ƉE��)
					static const ::Ishikawa::Math::Float4 VIEW_TEXTURE_COORDINATE;
					// �傫��
					static const ::Ishikawa::Math::Float2 SIZE;
					// �\���܂ł̎���(��ԏ��߂̂�)
					static const float FIRST_CAST_TIME_VIEWING;
					// �\���܂ł̎���
					static const float CAST_TIME_VIEWING;
					// ���S�ɕ\�������܂ł̎���
					static const float START_VIEWING_TIME;
					// �\�����̊Ԋu
					static const float VIEWING_INTERVAL;
					// ���S�ɏ�����܂ł̎���
					static const float END_VIEWING_TIME;
					// �\������Ƃ��̎ア�A���t�@�l
					static const float WEAK_ALPHA;

				private:
					/// <summary>GUI�}�l�[�W��</summary>
					::CatchAFairy::Object::PlayScene::GUI::GUIManager* manager;
					/// <summary>���p���_</summary>
					::CatchAFairy::Shader::User::PlayGUI::VSInput* useVertex;
					/// <summary>�\���܂ł̎���</summary>
					float castTime;
					/// <summary>�\���p�̒l [0.0�`1.0:�ڍs��� 1.0�`:���[�v]</summary>
					float viewValue;


				public:
					// �f�t�H���g�R���X�g���N�^
					OperationMethodView();
					// �f�X�g���N�^
					virtual ~OperationMethodView() override final;


				public:
					// �}�l�[�W���̐ݒ�
					virtual void SetManager(
						::CatchAFairy::Object::PlayScene::GUI::GUIManager* const _manager
					) override final;

				protected:
					// ���_�̐ݒ�
					virtual void SetVertices(
						::CatchAFairy::Shader::User::PlayGUI::VSInput* const _vertices,
						unsigned int _count
					) override final;

				public:
					/// <summary>�v���C�����s������</summary>
					void OnActPlayer(){
						this->castTime = CAST_TIME_VIEWING;
					}

				public:
					// �X�V
					virtual void Update(float _elapsedTime) override final;
				};
			}
		}
	}
}

#endif
