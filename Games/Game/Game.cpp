//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Game.cpp
//!
//! @summary	ゲームクラスのソースファイル
//!
//! @date		2018.09.18
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../pch.h"

// <自作ヘッダーファイル>
#include "Game.h"
#include "Scenes/PlayScene.h"
#include "Scenes/TitleScene.h"
#include "Utility/ScoreManager.h"
#include "../Library/Sound/SoundManager.h"
#include "Utility/GameManager.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 定数の定義 ==============================================================
// 画面の幅
const int Game::SCREEN_WIDTH = 1280;
// 画面の高さ
const int Game::SCREEN_HEIGHT = 720;
// ゲームタイトル
const wstring Game::GAME_TITLE = L"Motos";


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [hInstance]
//! @parameter [nCmdShow]
//--------------------------------------------------------------------
Game::Game(HINSTANCE hInstance, int nCmdShow) :
	Framework(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE),
	m_sceneManager(nullptr)
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   初期化処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Game::Initialize()
{
	// サウンドファイルの読み込み
	Sound::SoundManager::GetInstance()->Initialize(L"Motos.acf");

	// シーンマネージャーのインスタンスの取得
	m_sceneManager = Scene::SceneManager::GetInstance();

	// シーンの登録
	m_sceneManager->Entry(new Motos::Scene::PlayScene());
	m_sceneManager->Entry(new Motos::Scene::TitleScene());

	// 最初のシーンの設定
	m_sceneManager->Start("Motos::Scene::TitleScene");
}



//--------------------------------------------------------------------
//! @summary   終了処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Game::Finalize()
{
	// スコアマネージャーの削除
	Motos::Utility::ScoreManager::Reset();
	// ゲームマネージャーの削除
	Motos::Utility::GameManager::Reset();
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
#pragma region Frame Update
void Game::Update(const Common::StepTimer & timer)
{
	// 活動中のシーンを更新する
	m_sceneManager->UpdateActiveScene(timer);
}
#pragma endregion



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
#pragma region Frame Render
void Game::Render()
{
	// 活動中のシーンを描画する
	m_sceneManager->RenderActiveScene();
}
#pragma endregion
