//=============================================================================
//	WinMain.cpp
//
//	�G���g���|�C���g�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "WinMain.h"
#pragma warning(disable:4820)
#include <time.h>
#pragma warning(default:4820)
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#include <windows.h>
#pragma warning(default:4820)
#pragma warning(default:4668)
#pragma warning(disable:4548)
#include <exception>
#pragma warning(default:4548)
#include "Ishikawa/Common/Random.h"
#include "Ishikawa/Common/PatternBase/SingletonManager.h"
#include "Ishikawa/Window/ITWindowManager.h"
#include "CatchAFairy/Game.h"


// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// �f�t�H���g�Ńf�B�X�N���[�g���i��D�悷��悤�Ƀn�C�u���b�h�O���t�B�b�N�V�X�e���Ɏw������
extern "C"{
	__declspec(dllexport) unsigned long int NvOptimusEnablement = 0x00000001UL;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

/// <summary>
/// �G���g���|�C���g
/// </summary>
/// <param name="hInstance">�C���X�^���X�n���h��</param>
/// <param name="hPrevInstance">Win16����Ɏg���Ă�������[���͏��NULL]</param>
/// <param name="lpCmdLine">�R�}���h���C������</param>
/// <param name="nCmdShow">�A�v���P�[�V�����̏����\�����@</param>
/// <returns>�Ō�Ɏ擾�������b�Z�[�W</returns>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
#if __IS_DEBUG
	::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	return ::Main::Execute(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

/// <summary>
/// ���ۂ̎��s����
/// [���N���X�����friend���ʂ�Ȃ������̂Ŏd���Ȃ��ʂɂ���]
/// </summary>
/// <param name="hInstance">�C���X�^���X�n���h��</param>
/// <param name="hPrevInstance">Win16����Ɏg���Ă�������[���͏��NULL]</param>
/// <param name="lpCmdLine">�R�}���h���C������</param>
/// <param name="nCmdShow">�A�v���P�[�V�����̏����\�����@</param>
/// <returns>�Ō�Ɏ擾�������b�Z�[�W</returns>
int Main::Execute(HINSTANCE__* hInstance, HINSTANCE__* hPrevInstance, char* lpCmdLine, int nCmdShow){
	NOT_USING(hPrevInstance);
	NOT_USING(lpCmdLine);

	int endMsg = 0;
	try{
		// ����������
		::Ishikawa::Common::Random::Reset(static_cast<unsigned long>(time(nullptr)));

		// Co������
		::CoInitialize(nullptr);

		// �Q�[���̍쐬
		::CatchAFairy::Game game;
		game.Initialize(hInstance, WindowProcedure);
		game.Activate(nCmdShow);
		endMsg = game.Run();
		game.Finalize();
	}
	catch(std::exception e){
		// Interruption end => ���f�I��
		::OutputDebugStringA("== Interruption end ==\n");
		// ���f���R
		::OutputDebugStringA(e.what());
		::OutputDebugStringA("\n");
		// ���b�Z�[�W�\��
		::MessageBoxW(nullptr, L"���f�I�����܂����B", L"�G���[����", MB_OK);
	}
	catch(...){
		// Interruption end => ���f�I��
		::OutputDebugStringA("== Interruption end ==\n");
		// Catched not inherited exception of std::exception. => std::exception�̌p������Ă��Ȃ���O���L���b�`���܂����B
		::OutputDebugStringA("Catched not inherited exception of std::exception.\n");
		// ���b�Z�[�W�\��
		::MessageBoxW(nullptr, L"���f�I�����܂����B", L"�G���[����", MB_OK);
	}

	// �V���O���g���C���X�^���X�̍폜
	::Ishikawa::Common::SingletonManager::GetInstance().Finalize();

	return endMsg;
}

/// <summary>
/// �E�B���h�E�v���V�[�W��
/// </summary>
/// <param name="hWnd">�E�B���h�E�n���h��</param>
/// <param name="msg">���b�Z�[�W</param>
/// <param name="wParam">���b�Z�[�W�̕t�����</param>
/// <param name="lParam">���b�Z�[�W�̕t�����</param>
/// <returns>�f�t�H���g�̃E�B���h�E�v���V�[�W������Ԃ����l</returns>
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	// �o�^����Ă���E�B���h�E�̃f�[�^������Ă����珈�����������ɓ�����
	::Ishikawa::Window::Manager& windowManager = Ishikawa::Common::SingletonManager::GetInstance<Ishikawa::Window::Manager>();
	::Ishikawa::Window::Interface* window = windowManager.GetHeadWindow();
	while(!(window->IsITWindowListEdge())){
		if(window->GetHandle() == hWnd){
			if(window->Procedure(msg, wParam, lParam)){
				return TRUE;
			}
			break;
		}
		window = window->GetNextITWindow();
	}

	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
