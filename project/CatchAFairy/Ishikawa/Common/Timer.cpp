//=============================================================================
//	Timer.cpp
//
//	タイマーのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "Timer.h"
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#include <windows.h>
#pragma warning(default:4820)
#pragma warning(default:4668)
#include "Exception/ITExceptions.h"

using namespace ::Ishikawa::Common;

/// <summary>デフォルトタイマー</summary>
Timer Timer::DEFAULT_TIMER;


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
Timer::Timer() :
	queryPerformanceFrequency(0LL),
	startTicks(0LL),
	lastTicks(0LL),
	elapsedTicks(0LL),
	totalTicks(0LL),
	elapsedSeconds(0.0f),
	totalSeconds(0.0f)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Timer::~Timer(){
	// 何もしない
}


/// <summary>
/// 現在のカウンタ値を取得
/// </summary>
/// <returns>現在のカウンタ値</returns>
inline long long Timer::GetCurrentTicks() const {
	LARGE_INTEGER qpc;
	if(!QueryPerformanceCounter(&qpc)){
		// 異常終了
		throw Exception::FunctionFailed("Couldn't get query performance count.");
	}
	return qpc.QuadPart;
}


/// <summary>
/// 初期化
/// </summary>
void Timer::Initialize(){
	// 1秒当たりのカウント数を取得する
	LARGE_INTEGER qpf;
	if(!QueryPerformanceFrequency(&qpf)){
		// 異常終了
		throw Exception::FunctionFailed("Couldn't get query performance frequency.");
	}
	this->queryPerformanceFrequency = qpf.QuadPart;

	this->Reset();
}

/// <summary>
/// 更新
/// </summary>
void Timer::Update(){
	long long currentTicks = this->GetCurrentTicks();

	this->elapsedTicks = currentTicks - this->lastTicks;
	this->totalTicks = currentTicks - this->startTicks;
	this->elapsedSeconds = this->TicksToSeconds(this->elapsedTicks);
	this->totalSeconds = this->TicksToSeconds(this->totalTicks);

	this->lastTicks = currentTicks;
}

/// <summary>
/// リセット
/// </summary>
void Timer::Reset(){
	this->startTicks = this->GetCurrentTicks();
	this->lastTicks = this->startTicks;
	this->elapsedTicks = 0LL;
	this->totalTicks = 0LL;
	this->elapsedSeconds = 0.0f;
	this->totalSeconds = 0.0f;
}


/// <summary>
/// カウンタ値を秒数に変換
/// </summary>
/// <param name="_ticks">カウンタ値</param>
/// <returns>対応する秒数</returns>
inline float Timer::TicksToSeconds(long long _ticks) const {
	if(this->queryPerformanceFrequency == 0ULL){
		// 異常終了
		throw Exception::FunctionFailed("Query performance frequency is ZERO. Prease initialization.");
	}

	return static_cast<float>(static_cast<double>(_ticks) / this->queryPerformanceFrequency);
}
