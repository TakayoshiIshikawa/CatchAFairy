//=============================================================================
//	TreesOffset.h
//
//	木々初期データのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __TREES_OFFSET_H__
#define __TREES_OFFSET_H__

namespace Ishikawa{
	namespace Math{
		struct Matrix;
	}
}

namespace CatchAFairy{
	namespace Data{
		/// <summary>
		/// 木々初期データ
		/// </summary>
		class TreesOffset final{
		private:
			// ワールド変換行列初期化用データ
			struct WorldOffset;

		private:
			/// <summary>要素数</summary>
			unsigned int count;
			/// <summary>ワールド変換行列初期化用データ配列</summary>
			WorldOffset* worldOffsetArray;
			/// <summary>ワールド変換行列配列</summary>
			::Ishikawa::Math::Matrix* worldArray;

		public:
			// デフォルトコンストラクタ
			TreesOffset();
			// デストラクタ
			~TreesOffset();

		public:
			/// <summary>要素数の取得</summary>
			/// <returns>要素数</returns>
			unsigned int GetCount() const { return this->count; }
			/// <summary>ワールド変換行列配列の取得</summary>
			/// <returns>ワールド変換行列配列</returns>
			const ::Ishikawa::Math::Matrix* GetWorldArray() const { return this->worldArray; }

		public:
			// 読み込み
			void Load(const char* const _csvFilepath);
			// データ破棄
			void Clear();
		};
	}
}

#endif
