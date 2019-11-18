//=============================================================================
//	GUIVerticesSetting.h
//
//	GUI���_�ݒ�f�[�^�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __GUI_VERTICES_SETTING_H__
#define __GUI_VERTICES_SETTING_H__

#include "PointLightsSetting.h"

namespace CatchAFairy{
	namespace Data{
		/// <summary>
		/// GUI���_�ݒ�f�[�^
		/// </summary>
		namespace GUIVerticesSetting{
			/// <summary>�d���ʒu�\���p�C���f�b�N�X�̐擪</summary>
			const unsigned int FAIRY_START_POSITION_VIEW_INDEX = 0U;
			/// <summary>�d���ʒu�\���p�C���f�b�N�X��</summary>
			const unsigned int FAIRY_POSITION_VIEW_INDEX_COUNT = PointLightsSetting::ENEMY_FAIRY_COUNT;
			/// <summary>���ԕ\���p�C���f�b�N�X�̐擪</summary>
			const unsigned int TIME_VIEW_START_INDEX = FAIRY_POSITION_VIEW_INDEX_COUNT;
			/// <summary>���ԕ\���p�C���f�b�N�X�� (�\������)</summary>
			const unsigned int TIME_VIEW_INDEX_COUNT = 3U;
			/// <summary>������@�\���p�C���f�b�N�X</summary>
			const unsigned int OPERATION_METHOD_VIEW_INDEX = TIME_VIEW_START_INDEX + TIME_VIEW_INDEX_COUNT;
			/// <summary>GUI�S���_��</summary>
			const unsigned int ALL_VERTEX_COUNT = OPERATION_METHOD_VIEW_INDEX + 1U;
		}
	}
}

#endif
