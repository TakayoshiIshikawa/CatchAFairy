//=============================================================================
//	WinMain.cpp
//
//	エントリポイントのソースファイル
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


// ウィンドウプロシージャ
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// デフォルトでディスクリート部品を優先するようにハイブリッドグラフィックシステムに指示する
extern "C"{
	__declspec(dllexport) unsigned long int NvOptimusEnablement = 0x00000001UL;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

/// <summary>
/// エントリポイント
/// </summary>
/// <param name="hInstance">インスタンスハンドル</param>
/// <param name="hPrevInstance">Win16時代に使っていたもの[今は常にNULL]</param>
/// <param name="lpCmdLine">コマンドライン引数</param>
/// <param name="nCmdShow">アプリケーションの初期表示方法</param>
/// <returns>最後に取得したメッセージ</returns>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
#if __IS_DEBUG
	::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	return ::Main::Execute(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

/// <summary>
/// 実際の実行部分
/// [他クラスからのfriendが通らなかったので仕方なく別にした]
/// </summary>
/// <param name="hInstance">インスタンスハンドル</param>
/// <param name="hPrevInstance">Win16時代に使っていたもの[今は常にNULL]</param>
/// <param name="lpCmdLine">コマンドライン引数</param>
/// <param name="nCmdShow">アプリケーションの初期表示方法</param>
/// <returns>最後に取得したメッセージ</returns>
int Main::Execute(HINSTANCE__* hInstance, HINSTANCE__* hPrevInstance, char* lpCmdLine, int nCmdShow){
	NOT_USING(hPrevInstance);
	NOT_USING(lpCmdLine);

	int endMsg = 0;
	try{
		// 乱数初期化
		::Ishikawa::Common::Random::Reset(static_cast<unsigned long>(time(nullptr)));

		// Co初期化
		::CoInitialize(nullptr);

		// ゲームの作成
		::CatchAFairy::Game game;
		game.Initialize(hInstance, WindowProcedure);
		game.Activate(nCmdShow);
		endMsg = game.Run();
		game.Finalize();
	}
	catch(std::exception e){
		// Interruption end => 中断終了
		::OutputDebugStringA("== Interruption end ==\n");
		// 中断理由
		::OutputDebugStringA(e.what());
		::OutputDebugStringA("\n");
		// メッセージ表示
		::MessageBoxW(nullptr, L"中断終了しました。", L"エラー発生", MB_OK);
	}
	catch(...){
		// Interruption end => 中断終了
		::OutputDebugStringA("== Interruption end ==\n");
		// Catched not inherited exception of std::exception. => std::exceptionの継承されていない例外をキャッチしました。
		::OutputDebugStringA("Catched not inherited exception of std::exception.\n");
		// メッセージ表示
		::MessageBoxW(nullptr, L"中断終了しました。", L"エラー発生", MB_OK);
	}

	// シングルトンインスタンスの削除
	::Ishikawa::Common::SingletonManager::GetInstance().Finalize();

	return endMsg;
}

/// <summary>
/// ウィンドウプロシージャ
/// </summary>
/// <param name="hWnd">ウィンドウハンドル</param>
/// <param name="msg">メッセージ</param>
/// <param name="wParam">メッセージの付加情報</param>
/// <param name="lParam">メッセージの付加情報</param>
/// <returns>デフォルトのウィンドウプロシージャから返される値</returns>
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	// 登録されているウィンドウのデータが回ってきたら処理をそっちに投げる
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
