//=============================================================================
//	UpdateObjectInterface.h
//
//	更新オブジェクトインターフェースのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __UPDATE_OBJECT_INTERFACE_H__
#define __UPDATE_OBJECT_INTERFACE_H__

namespace CatchAFairy{
	namespace Object{
		/// <summary>
		/// 更新オブジェクトインターフェース
		/// </summary>
		class UpdateInterface{
		public:
			/// <summary>デフォルトコンストラクタ</summary>
			UpdateInterface(){}
			/// <summary>デストラクタ</summary>
			virtual ~UpdateInterface(){}

		public:
			// 更新
			virtual void Update(float _elapsedTime) = 0;
		};
	}
}

#endif
