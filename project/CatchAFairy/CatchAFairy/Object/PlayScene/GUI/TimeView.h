//=============================================================================
//	TimeView.h
//
//	���ԕ\���̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __TIME_VIEW_H__
#define __TIME_VIEW_H__

#include "GUIObjectInterface.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "CatchAFairy/Data/GUIVerticesSetting.h"

namespace CatchAFairy{
	namespace Scene{
		class Play;
	}

	namespace Object{
		namespace PlayScene{
			namespace GUI{
				/// <summary>
				/// ���ԕ\��
				/// </summary>
				class TimeView final : public GUIInterface{
				private:
					/// <summary>�\������</summary>
					static const unsigned int VIEW_DIGIT = ::CatchAFairy::Data::GUIVerticesSetting::TIME_VIEW_INDEX_COUNT;
					// �ő�J�E���g
					static const unsigned int MAX_COUNT;
					// �\���ʒu�z��
					static const ::Ishikawa::Math::Float2 POSITION[VIEW_DIGIT];
					// �\���F
					static const ::Ishikawa::Math::Float3 COLOR;
					// �傫��
					static const ::Ishikawa::Math::Float2 SIZE;

				private:
					/// <summary>�v���C�V�[��</summary>
					::CatchAFairy::Scene::Play* scenePlay;
					/// <summary>GUI�}�l�[�W��</summary>
					::CatchAFairy::Object::PlayScene::GUI::GUIManager* manager;
					/// <summary>���p���_</summary>
					::CatchAFairy::Shader::User::PlayGUI::VSInput* useVertices;
					/// <summary>�o�ߎ���</summary>
					float currentTime;


				public:
					// �f�t�H���g�R���X�g���N�^
					TimeView();
					// �f�X�g���N�^
					virtual ~TimeView() override final;

				public:
					/// <summary>�v���C�V�[���̐ݒ�</summary>
					/// <param name="_scenePlay">�v���C�V�[��</param>
					void SetScenePlay(::CatchAFairy::Scene::Play* const _scenePlay){
						this->scenePlay = _scenePlay;
					}
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
					// �X�V
					virtual void Update(float _elapsedTime) override final;
				};
			}
		}
	}
}

#endif
