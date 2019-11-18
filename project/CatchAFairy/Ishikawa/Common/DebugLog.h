//=============================================================================
//	DebugLog.h
//
//	�f�o�b�O���O�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __DEBUG_LOG_H__
#define __DEBUG_LOG_H__

namespace Ishikawa{
	namespace Common{
		namespace Debug{
			// ���O���c��
			void Log(const char* const _log);

			/// <summary>
			/// �֐����O
			/// </summary>
			class FunctionLog final{
			private:
				/// <summary>�֐���</summary>
				const char* const functionName;


			public:
				// �R���X�g���N�^
				FunctionLog(const char* const _functionName);
				// �f�X�g���N�^
				~FunctionLog();
			private:
				/// <summary>�R�s�[�R���X�g���N�^[�폜]</summary>
				FunctionLog(const FunctionLog&) = delete;
				/// <summary>������Z�q[�폜]</summary>
				FunctionLog& operator=(const FunctionLog&) = delete;
			};
		}
	}
}

#endif
