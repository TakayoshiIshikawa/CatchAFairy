//=============================================================================
//	WinMain.h
//
//	�G���g���|�C���g�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __WINMAIN_H__
#define __WINMAIN_H__

struct HINSTANCE__;


namespace Main{
	// ���ۂ̎��s����
	// [���N���X�����friend���ʂ�Ȃ������̂Ŏd���Ȃ��ʂɂ���]
	int Execute(HINSTANCE__* hInstance, HINSTANCE__* hPrevInstance, char* lpCmdLine, int nCmdShow);
}

#endif
