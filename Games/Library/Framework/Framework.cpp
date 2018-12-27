//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Framework.cpp
//!
//! @summary	フレームワークの基底クラスのソースファイル
//!
//! @date		2018.09.18
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <標準ヘッダファイル>
#include <string>

// <自作ヘッダファイル>
#include "Framework.h"
#include "../Shader/ShaderManager.h"
#include "../Graphic2D/PrimitiveManager2D.h"
#include "../Input/KeyboardUtil.h"
#include "../Input/MouseUtil.h"
#include "../Scene/SceneManager.h"
#include "../Utility/TextureManager.h"
#include "../Graphic3D/PrimitiveManager3D.h"
#include "../Sound/SoundManager.h"
#include "../Graphic3D/Model/ModelManager.h"
#include "../Animation/MotionManager.h"
#include "../Collision/CollisionManager.h"
#include "../Physics/PhysicsManager.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace Library;


// 静的メンバ変数の定義 =====================================================
bool Framework::Framework::m_isExit = false;



// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [hInstance]
//! @parameter [nCmdShow]
//! @parameter [titleName] タイトル名
//--------------------------------------------------------------------
Framework::Framework::Framework(HINSTANCE hInstance, int nCmdShow, int screenWidth, int screenHeight, std::wstring titleName) :
	m_screenWidth(screenWidth),
	m_screenHeight(screenHeight)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, L"IDI_ICON");
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"GameWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, L"IDI_ICON");
	if (!RegisterClassEx(&wcex))
		throw 0;

	// ウィンド作成
	int w, h;
	GetDefaultSize(w, h);

	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = static_cast<LONG>(w);
	rc.bottom = static_cast<LONG>(h);

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	m_hwnd = CreateWindowEx(0, wcex.lpszClassName, L"Game", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);

	if (!m_hwnd)
		throw 0;

	ShowWindow(m_hwnd, nCmdShow);

	SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	GetClientRect(m_hwnd, &rc);

	SetWindowText(m_hwnd, titleName.c_str());

	m_deviceResources = Common::DeviceResources::GetInstance();
	m_deviceResources->RegisterDeviceNotify(this);

	m_deviceResources->SetWindow(m_hwnd, w, h);

	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	Input::MouseUtil::GetInstance()->SetWindow(m_hwnd);

#if defined(_DEBUG)
	// ウィンドウモードで実行
	m_deviceResources->GetSwapChain()->SetFullscreenState(false, NULL);
#else
	// フルスクリーンモードで実行
	m_deviceResources->GetSwapChain()->SetFullscreenState(true, NULL);
#endif
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Framework::Framework::~Framework()
{
}



#pragma region Frame Update
//--------------------------------------------------------------------
//! @summary   基礎ゲームループ処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Framework::Framework::Tick()
{
	m_timer.Tick([&]()
	{
		// キーボード入力マネージャーの更新処理
		Input::KeyboardUtil::GetInstance()->Update();

		// マウス入力マネージャーの更新処理
		Input::MouseUtil::GetInstance()->Update();

		// サウンドマネージャーの更新処理
		Sound::SoundManager::GetInstance()->Update();

		// 衝突判定マネージャーの更新処理
		Collision::CollisionManager::GetInstance()->Update();

		// 物理マネージャーの更新処理
		Physics::PhysicsManager::GetInstance()->Update(m_timer);

		Update(m_timer);
	});

	// <FPSの表示>
#if defined(_DEBUG)
	unsigned int fps = m_timer.GetFramesPerSecond();
	std::wstring str = L"FPS:" + std::to_wstring(fps);
	SetWindowText(m_hwnd, str.c_str());
#endif

	PreRender();
	Render();
	PostRender();

	if(m_isExit) PostQuitMessage(0);
}
#pragma endregion



#pragma region Frame Render
//--------------------------------------------------------------------
//! @summary   描画前の準備処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Framework::Framework::PreRender()
{
	// 最初の更新の前に何かをレンダリングしようとしないでください
	if (m_timer.GetFrameCount() == 0) return;

	Clear();

	m_deviceResources->PIXBeginEvent(L"Render");
}



//--------------------------------------------------------------------
//! @summary   スクリーンへの出力処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Framework::Framework::PostRender()
{
	m_deviceResources->PIXEndEvent();

	// 新しいフレームを表示します
	m_deviceResources->Present();
}



//--------------------------------------------------------------------
//! @summary   バックバッファをクリアする処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Framework::Framework::Clear()
{
	m_deviceResources->PIXBeginEvent(L"Clear");

	// ビューをクリアします
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();

	// ウィンドウの背景はここで変更できる ------------------↓
	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	// ビューポートを設定します。
	D3D11_VIEWPORT viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	m_deviceResources->PIXEndEvent();
}
#pragma endregion



#pragma region Message Handlers
//--------------------------------------------------------------------
//! @summary   フレームワークがアクティブウィンドウになっている時の処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Framework::Framework::OnActivated()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   フレームワークが背景のウィンドウになっている時の処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Framework::Framework::OnDeactivated()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   フレームワークが停電中(最小化中)の時の処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Framework::Framework::OnSuspending()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   フレームワークが電源再開(最小化から復帰)された時の処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Framework::Framework::OnResuming()
{
	m_timer.ResetElapsedTime();
}



//--------------------------------------------------------------------
//! @summary   フレームワークウィンドウのサイズが変更された時の処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Framework::Framework::OnWindowSizeChanged(int width, int height)
{
	if (!m_deviceResources->WindowSizeChanged(width, height))
		return;

	CreateWindowSizeDependentResources();
}



//--------------------------------------------------------------------
//! @summary   希望のデフォルトウィンドウサイズに変更する
//!			   (最小サイズは320x200)
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Framework::Framework::GetDefaultSize(int& width, int& height) const
{
	width = m_screenWidth;
	height = m_screenHeight;
}
#pragma endregion



#pragma region Direct3D Resources
// デバイスに依存するリソース
//--------------------------------------------------------------------
//! @summary   デバイス依存オブジェクトの初期化処理
//!			   (ウィンドウサイズに関係なく)
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Framework::Framework::CreateDeviceDependentResources()
{
	ID3D11Device1* device = m_deviceResources->GetD3DDevice();
	ID3D11DeviceContext1* context = m_deviceResources->GetD3DDeviceContext();

	// シェーダーマネージャーの初期化処理
	Shader::ShaderManager::GetInstance()->Initialize();

	// 2Dのプリミティブ描画のマネージャークラスの初期化処理
	Graphic2D::PrimitiveManager2D::GetInstance()->Initialize();

	// テクスチャデータの管理クラスの初期化
	Utility::TextureManager::GetInstance()->Initialize();

	// 3Dのプリミティブ描画のマネージャークラスの初期化処理
	Graphic3D::PrimitiveManager3D::GetInstance()->Initialize();

	// サウンドマネージャーの初期化処理
	Sound::SoundManager::GetInstance()->Initialize(nullptr);

	// モデルマネージャーの初期化処理
	Graphic3D::Model::ModelManager::GetInstance();

	// 衝突判定マネージャーの初期化処理
	Collision::CollisionManager::GetInstance()->Initialize();

	// 物理マネージャーの初期化処理
	Physics::PhysicsManager::GetInstance()->Intialize();
}



// ウィンドウのSizeChangedイベントで変更されるすべてのメモリリソースを割り当てます。
//--------------------------------------------------------------------
//! @summary   ウィンドウサイズに依存するオブジェクトの初期化処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Framework::Framework::CreateWindowSizeDependentResources()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   Direct3Dリソースクリーンアップの追加処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Framework::Framework::OnDeviceLost()
{
	// シェーダーマネージャーの終了処理
	Shader::ShaderManager::Reset();

	// 2Dのプリミティブ描画のマネージャークラスの終了処理
	Graphic2D::PrimitiveManager2D::Reset();

	// キーボード入力クラスのインスタンスの破棄
	Input::KeyboardUtil::Reset();

	// マウス入力クラスのインスタンスの破棄
	Input::MouseUtil::Reset();

	// シーンマネージャーの終了処理
	Scene::SceneManager::Reset();

	// テクスチャデータの管理クラスの終了処理
	Utility::TextureManager::Reset();

	// 3Dのプリミティブ描画のマネージャークラスの終了処理
	Graphic3D::PrimitiveManager3D::Reset();

	// サウンドマネージャーの終了処理
	Sound::SoundManager::Reset();

	// モデルマネージャーの終了処理
	Graphic3D::Model::ModelManager::Reset();

	// モーションマネージャーの終了処理
	Animation::MotionManager::Reset();

	// 衝突判定マネージャーの終了処理
	Collision::CollisionManager::Reset();

	// 物理マネージャーの終了処理
	Physics::PhysicsManager::Reset();
}



//--------------------------------------------------------------------
//! @summary   デバイスの復帰処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Framework::Framework::OnDeviceRestored()
{
	CreateDeviceDependentResources();

	CreateWindowSizeDependentResources();
}
#pragma endregion



//--------------------------------------------------------------------
//! @summary   Windowsへの手続き
//!
//! @parameter [hWnd]
//! @parameter [message]
//! @parameter [wParam]
//! @parameter [lParam]
//!
//! @return
//--------------------------------------------------------------------
LRESULT CALLBACK Framework::Framework::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	static bool s_in_sizemove = false;
	static bool s_in_suspend = false;
	static bool s_minimized = false;

	auto framework = reinterpret_cast<Framework*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			if (!s_minimized)
			{
				s_minimized = true;
				if ((!s_in_suspend) && (framework)) framework->OnSuspending();
				s_in_suspend = true;
			}
		}
		else if (s_minimized)
		{
			s_minimized = false;
			if ((s_in_suspend) && (framework)) framework->OnResuming();
			s_in_suspend = false;
		}
		break;

	case WM_ENTERSIZEMOVE:
		s_in_sizemove = true;
		break;

	case WM_EXITSIZEMOVE:
		s_in_sizemove = false;
		if (framework)
		{
			RECT rc;
			GetClientRect(hWnd, &rc);

			framework->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
		}
		break;

	case WM_GETMINMAXINFO:
	{
		auto info = reinterpret_cast<MINMAXINFO*>(lParam);
		info->ptMinTrackSize.x = 320;
		info->ptMinTrackSize.y = 200;
	}
	break;

	case WM_ACTIVATEAPP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		Mouse::ProcessMessage(message, wParam, lParam);
		if (framework)
		{
			if (wParam) framework->OnActivated();
			else framework->OnDeactivated();
		}
		break;

	case WM_POWERBROADCAST:
		switch (wParam)
		{
		case PBT_APMQUERYSUSPEND:
			if ((!s_in_suspend) && (framework)) framework->OnSuspending();
			s_in_suspend = true;
			return TRUE;

		case PBT_APMRESUMESUSPEND:
			if (!s_minimized)
			{
				if ((s_in_suspend) && (framework)) framework->OnResuming();
				s_in_suspend = false;
			}
			return TRUE;
		}
		break;

	case WM_DESTROY:
		framework->OnDeviceLost();
		PostQuitMessage(0);
		break;
	case WM_MENUCHAR:
		// A menu is active and the user presses a key that does not correspond
		// to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
		// 翻訳：メニューがアクティブで、ユーザーはニーモニックまたはアクセラレータキーに対応しないキーを押します。
		//		 無視してエラービープ音を鳴らしません。

		return MAKELRESULT(0, MNC_CLOSE);
	case WM_SYSKEYDOWN:
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}



//--------------------------------------------------------------------
//! @summary   ゲームの終了処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Framework::Framework::ExitGame()
{
	m_isExit = true;
}
