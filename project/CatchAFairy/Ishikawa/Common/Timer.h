//=============================================================================
//	Timer.h
//
//	�^�C�}�[�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __TIMER_H__
#define __TIMER_H__

namespace Ishikawa{
	namespace Common{
		/// <summary>
		/// �^�C�}�[
		/// </summary>
		class Timer final{
		public:
			// �f�t�H���g�^�C�}�[
			static Timer DEFAULT_TIMER;

		private:
			/// <summary>1�b������̃J�E���g��</summary>
			long long queryPerformanceFrequency;
			/// <summary>�J�n�J�E���^�l</summary>
			long long startTicks;
			/// <summary>�ŏI�J�E���^�l</summary>
			long long lastTicks;
			/// <summary>�J�E���^�l����</summary>
			long long elapsedTicks;
			/// <summary>�J�E���^�l���v</summary>
			long long totalTicks;
			/// <summary>�����b��</summary>
			float elapsedSeconds;
			/// <summary>���v�b��</summary>
			float totalSeconds;


		public:
			// �f�t�H���g�R���X�g���N�^
			Timer();
			// �f�X�g���N�^
			~Timer();

		public:
			/// <summary>�J�E���^�l�������擾</summary>
			/// <returns>�J�E���^�l����</returns>
			long long GetElapsedTicks() const { return this->elapsedTicks; }
			/// <summary>�����b�����擾</summary>
			/// <returns>�����b��</returns>
			float GetElapsedSeconds() const { return this->elapsedSeconds; }
			/// <summary>�J�E���^�l���v���擾</summary>
			/// <returns>�J�E���^�l���v</returns>
			long long GetTotalTicks() const { return this->totalTicks; }
			/// <summary>���v�b�����擾</summary>
			/// <returns>���v�b��</returns>
			float GetTotalSeconds() const { return this->totalSeconds; }

		private:
			// ���݂̃J�E���^�l���擾
			inline long long GetCurrentTicks() const;

		public:
			// ������
			void Initialize();
			// �X�V
			void Update();
			// ���Z�b�g
			void Reset();

		private:
			// �J�E���^�l��b���ɕϊ�
			inline float TicksToSeconds(long long _ticks) const;
		};
	}
}

#endif
