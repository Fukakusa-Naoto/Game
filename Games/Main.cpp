//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Main.h
//!
//! @summary	���C���֐��̃\�[�X�t�@�C��
//!
//! @date		2018.09.18
//!
//! @author		�[�����l
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� =================================================
// <�v���R���p�C���ς݃w�b�_�t�@�C��>
#include "pch.h"

// <����w�b�_�t�@�C��>
#include "Game/Game.h"


// using�f�B���N�e�B�u =====================================================
using namespace DirectX;



// Indicates to hybrid graphics systems to prefer the discrete part by default
// �|��F�n�C�u���b�h�O���t�B�b�N�X�V�X�e�����A�f�t�H���g�Ōʕ��i��D�悷�邱�Ƃ������܂�
extern "C"
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}



// �֐��̒�` ==============================================================
//--------------------------------------------------------------------
//! @summary   ���C���֐�
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
	// �������[���[�N���o
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
		// �쐬
		framework = new Game(hInstance, nCmdShow);
	}
	catch (...)
	{
		return 1;
	}

	// ����������
	framework->Initialize();

	// ���C�����b�Z�[�W���[�v
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
			// �X�V�E�`��
			framework->Tick();
		}
	}

	// �I������
	framework->Finalize();

	// �폜
	delete framework;
	framework = nullptr;

	CoUninitialize();

	return (int)msg.wParam;
}



//--------------------------------------------------------------------
//! @summary   ���C�����[�v�E�o�̏���
//!
//! @parameter [void] �Ȃ�
//!
//! @return    �Ȃ�
//--------------------------------------------------------------------
void ExitGame()
{
    PostQuitMessage(0);
}
