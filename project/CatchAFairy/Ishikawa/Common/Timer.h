//=============================================================================
//	Timer.h
//
//	タイマーのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __TIMER_H__
#define __TIMER_H__

namespace Ishikawa{
	namespace Common{
		/// <summary>
		/// タイマー
		/// </summary>
		class Timer final{
		public:
			// デフォルトタイマー
			static Timer DEFAULT_TIMER;

		private:
			/// <summary>1秒当たりのカウント数</summary>
			long long queryPerformanceFrequency;
			/// <summary>開始カウンタ値</summary>
			long long startTicks;
			/// <summary>最終カウンタ値</summary>
			long long lastTicks;
			/// <summary>カウンタ値差分</summary>
			long long elapsedTicks;
			/// <summary>カウンタ値合計</summary>
			long long totalTicks;
			/// <summary>差分秒数</summary>
			float elapsedSeconds;
			/// <summary>合計秒数</summary>
			float totalSeconds;


		public:
			// デフォルトコンストラクタ
			Timer();
			// デストラクタ
			~Timer();

		public:
			/// <summary>カウンタ値差分を取得</summary>
			/// <returns>カウンタ値差分</returns>
			long long GetElapsedTicks() const { return this->elapsedTicks; }
			/// <summary>差分秒数を取得</summary>
			/// <returns>差分秒数</returns>
			float GetElapsedSeconds() const { return this->elapsedSeconds; }
			/// <summary>カウンタ値合計を取得</summary>
			/// <returns>カウンタ値合計</returns>
			long long GetTotalTicks() const { return this->totalTicks; }
			/// <summary>合計秒数を取得</summary>
			/// <returns>合計秒数</returns>
			float GetTotalSeconds() const { return this->totalSeconds; }

		private:
			// 現在のカウンタ値を取得
			inline long long GetCurrentTicks() const;

		public:
			// 初期化
			void Initialize();
			// 更新
			void Update();
			// リセット
			void Reset();

		private:
			// カウンタ値を秒数に変換
			inline float TicksToSeconds(long long _ticks) const;
		};
	}
}

#endif
