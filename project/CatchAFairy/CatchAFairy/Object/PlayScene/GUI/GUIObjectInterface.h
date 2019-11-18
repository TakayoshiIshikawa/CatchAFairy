//=============================================================================
//	GUIObjectInterface.h
//
//	GUI�I�u�W�F�N�g�C���^�[�t�F�[�X�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __GUI_OBJECT_INTERFACE_H__
#define __GUI_OBJECT_INTERFACE_H__

#include "CatchAFairy/Shader/User/PlayGUIShaderUser.h"

namespace CatchAFairy{
	namespace Object{
		namespace PlayScene{
			namespace GUI{
				class GUIManager;

				/// <summary>
				/// GUI�I�u�W�F�N�g�C���^�[�t�F�[�X
				/// </summary>
				class GUIInterface{
				public:
					/// <summary>�f�t�H���g�R���X�g���N�^</summary>
					GUIInterface(){}
					/// <summary>�f�X�g���N�^</summary>
					virtual ~GUIInterface(){}

				public:
					// �}�l�[�W���̐ݒ�
					virtual void SetManager(
						::CatchAFairy::Object::PlayScene::GUI::GUIManager* const _manager
					) = 0;

				protected:
					// ���_�̐ݒ�
					virtual void SetVertices(
						::CatchAFairy::Shader::User::PlayGUI::VSInput* const _vertices,
						unsigned int _count
					) = 0;

				public:
					// �X�V
					virtual void Update(float _elapsedTime) = 0;
				};
			}
		}
	}
}

#endif
