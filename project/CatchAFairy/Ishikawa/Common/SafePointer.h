//=============================================================================
//	SafePointer.h
//
//	セーフポインタのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SAFEPOINTER_H__
#define __SAFEPOINTER_H__

namespace Ishikawa{
	namespace Common{
		/// <summary>
		/// セーフポインタ
		/// </summary>
		template<typename DataType>
		class SafePointer{
		private:
			/// <summary>データ</summary>
			DataType* data;


		public:
			/// <summary>デフォルトコンストラクタ</summary>
			SafePointer():data(nullptr){}
			/// <summary>コンストラクタ</summary>
			/// <param name="_data">データ</param>
			SafePointer(DataType* const _data):data(_data){}
			/// <summary>デストラクタ</summary>
			~SafePointer(){ if(this->data != nullptr) delete this->data; }
		private:
			/// <summary>コピーコンストラクタ[削除]</summary>
			SafePointer(const SafePointer<DataType>&) = delete;
			/// <summary>代入演算子[削除]</summary>
			SafePointer<DataType>& operator=(const SafePointer<DataType>&) = delete;

		public:
			/// <summary>ポインタ参照</summary>
			/// <returns>管理データ</returns>
			DataType& operator*(){ return (*(this->data)); }
			/// <summary>アロー演算子</summary>
			/// <returns>管理データ</returns>
			DataType& operator->(){ return (*(this->data)); }
			/// <summary>bool演算子</summary>
			/// <returns>true:データ有 false:データ無</returns>
			operator bool() const { return (this->data != nullptr); }
			/// <summary>not演算子</summary>
			/// <returns>true:データ無 false:データ有</returns>
			bool operator!() const { return (this->data == nullptr); }

		public:
			/// <summary>データの取得</summary>
			/// <returns>データ</returns>
			DataType* Get() const { return this->data; }
			/// <summary>データのリセット</summary>
			/// <param name="_data">データ</param>
			void Reset(DataType* const _data = nullptr){
				if(this->data != nullptr) delete this->data;
				this->data = _data;
			}
			/// <summary>データの移動</summary>
			/// <returns>データ</returns>
			DataType* Move(){
				DataType* const returnData = this->data;
				this->data = nullptr;
				return returnData;
			}
			/// <summary>データが空かどうか</summary>
			/// <returns>true:データ無 false:データ有</returns>
			bool Empty() const { return (this->data == nullptr); }
		};
	}
}

#endif
