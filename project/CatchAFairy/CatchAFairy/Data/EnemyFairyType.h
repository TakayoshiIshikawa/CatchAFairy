//=============================================================================
//	EnemyFairyType.h
//
//	“G—d¸í—Ş‚Ìƒwƒbƒ_ƒtƒ@ƒCƒ‹
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __ENEMY_FAIRY_TYPE_H__
#define __ENEMY_FAIRY_TYPE_H__

namespace CatchAFairy{
	namespace Data{
		/// <summary>
		/// “G—d¸í—Ş
		/// </summary>
		enum class EnemyFairyType : unsigned int {
			/// <summary>ƒm[ƒ}ƒ‹</summary>
			Normal,
			/// <summary>ƒŒƒA</summary>
			Rare,
			/// <summary>“`à</summary>
			Legend,

			/// <summary>”</summary>
			Count
		};
	}
}

#endif
