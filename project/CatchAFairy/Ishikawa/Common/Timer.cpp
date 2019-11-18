//=============================================================================
//	Timer.cpp
//
//	�^�C�}�[�̃\�[�X�t�@�C��
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

/// <summary>�f�t�H���g�^�C�}�[</summary>
Timer Timer::DEFAULT_TIMER;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
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
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Timer::~Timer(){
	// �������Ȃ�
}


/// <summary>
/// ���݂̃J�E���^�l���擾
/// </summary>
/// <returns>���݂̃J�E���^�l</returns>
inline long long Timer::GetCurrentTicks() const {
	LARGE_INTEGER qpc;
	if(!QueryPerformanceCounter(&qpc)){
		// �ُ�I��
		throw Exception::FunctionFailed("Couldn't get query performance count.");
	}
	return qpc.QuadPart;
}


/// <summary>
/// ������
/// </summary>
void Timer::Initialize(){
	// 1�b������̃J�E���g�����擾����
	LARGE_INTEGER qpf;
	if(!QueryPerformanceFrequency(&qpf)){
		// �ُ�I��
		throw Exception::FunctionFailed("Couldn't get query performance frequency.");
	}
	this->queryPerformanceFrequency = qpf.QuadPart;

	this->Reset();
}

/// <summary>
/// �X�V
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
/// ���Z�b�g
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
/// �J�E���^�l��b���ɕϊ�
/// </summary>
/// <param name="_ticks">�J�E���^�l</param>
/// <returns>�Ή�����b��</returns>
inline float Timer::TicksToSeconds(long long _ticks) const {
	if(this->queryPerformanceFrequency == 0ULL){
		// �ُ�I��
		throw Exception::FunctionFailed("Query performance frequency is ZERO. Prease initialization.");
	}

	return static_cast<float>(static_cast<double>(_ticks) / this->queryPerformanceFrequency);
}
