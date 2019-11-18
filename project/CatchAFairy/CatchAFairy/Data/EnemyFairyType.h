//=============================================================================
//	EnemyFairyType.h
//
//	�G�d����ނ̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __ENEMY_FAIRY_TYPE_H__
#define __ENEMY_FAIRY_TYPE_H__

namespace CatchAFairy{
	namespace Data{
		/// <summary>
		/// �G�d�����
		/// </summary>
		enum class EnemyFairyType : unsigned int {
			/// <summary>�m�[�}��</summary>
			Normal,
			/// <summary>���A</summary>
			Rare,
			/// <summary>�`��</summary>
			Legend,

			/// <summary>��</summary>
			Count
		};
	}
}

#endif
