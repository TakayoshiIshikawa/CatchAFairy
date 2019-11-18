//=============================================================================
//	PointLightsSetting.h
//
//	�|�C���g���C�g�ݒ�f�[�^�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __POINT_LIGHTS_SETTING_H__
#define __POINT_LIGHTS_SETTING_H__

namespace CatchAFairy{
	namespace Data{
		/// <summary>
		/// �|�C���g���C�g�ݒ�f�[�^
		/// </summary>
		namespace PointLightsSetting{
			/// <summary>�d��1�̖̂{�̂ɂ����C�g�̐�</summary>
			static const unsigned int ONE_FAIRY_BODY_LIGHT_COUNT = 2U;
			/// <summary>�d��1�̂���o��}�i���C�g�̐�</summary>
			static const unsigned int ONE_FAIRY_MANA_LIGHT_COUNT = 8U;
			/// <summary>�d��1�̂�����̃��C�g�̑���</summary>
			static const unsigned int ONE_FAIRY_LIGHT_ALL_COUNT = ONE_FAIRY_BODY_LIGHT_COUNT + ONE_FAIRY_MANA_LIGHT_COUNT;
			/// <summary>�v���C���d���̐�</summary>
			static const unsigned int PLAYER_FAIRY_COUNT = 1U;
			/// <summary>�G�d���̐�</summary>
			static const unsigned int ENEMY_FAIRY_COUNT = 5U;
			/// <summary>�d���̑���</summary>
			static const unsigned int FAIRY_COUNT = PLAYER_FAIRY_COUNT + ENEMY_FAIRY_COUNT;

			/// <summary>���C�g�̍ő吔</summary>
			static const unsigned int MAX_COUNT = FAIRY_COUNT * ONE_FAIRY_LIGHT_ALL_COUNT;
		}
	}
}

#endif
