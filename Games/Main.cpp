//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Main.h
//!
//! @summary	メイン関数のソースファイル
//!
//! @date		2018.09.18
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "pch.h"

// <自作ヘッダファイル>
#include "Game/Game.h"


// usingディレクティブ =====================================================
using namespace DirectX;



// Indicates to hybrid graphics systems to prefer the discrete part by default
// 翻訳：ハイブリッドグラフィックスシステムが、デフォルトで個別部品を優先することを示します
extern "C"
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}



// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   メイン関数
//!
//! @parameter [hInstance]
//! @parameter [hPrevInstance]
//! @parameter [lpCmdLine]
//! @parameter [nCmdShow]
//!
//! @return
//--------------------------------------------------------------------
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	// メモリーリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	Library::Framework::Framework* framework;

    HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
    if (FAILED(hr)) return 1;

	try
	{
#if DEBUG
		FNLibrary::Debug::ConsoleWindow::Create();
#endif
		// 作成
		framework = new Game(hInstance, nCmdShow);
	}
	catch (...)
	{
		return 1;
	}

	// 初期化処理
	framework->Initialize();

	// メインメッセージループ
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 更新・描画
			framework->Tick();
		}
	}

	// 終了処理
	framework->Finalize();

	// 削除
	delete framework;
	framework = nullptr;

	CoUninitialize();

	return (int)msg.wParam;
}



//--------------------------------------------------------------------
//! @summary   メインループ脱出の処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void ExitGame()
{
    PostQuitMessage(0);
}
