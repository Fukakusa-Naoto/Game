//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		PlayScene.cpp
//!
//! @summary	モトスのプレイシーンクラスのソースファイル
//!
//! @date		2018.10.29
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダーファイル>
#include "PlayScene.h"
#include "../../Library/Graphic3D/PrimitiveManager3D.h"
#include "../Game.h"
#include "../Utility/ScoreManager.h"
#include "../../Library/Debug/DebugCamera.h"
#include "../../Library/Input/KeyboardUtil.h"
#include "../../Library/Collision/CollisionManager.h"
#include "../Utility/ObjactTag.h"
#include "../../Library/Sound/SoundManager.h"

// <リソースファイル>
#include "../../Resources/Sounds/Play.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Scene::PlayScene::PlayScene() :
	m_canvas(nullptr)
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Scene::PlayScene::~PlayScene()
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
void Motos::Scene::PlayScene::Initialize()
{
#pragma region UIの初期化
	// UIキャンバスの作成
	m_canvas = new Graphic2D::UI::Canvas();
	// UI背景の作成
	m_uiFrame = new Play::Object::UI::UIFrame();

	// UIキャンバスの設定
	m_canvas->SetPosition(static_cast<float>(Game::SCREEN_WIDTH - 320), 0.0f);
	m_canvas->SetSize(320.0f, static_cast<float>(Game::SCREEN_HEIGHT));

	// UIキャンバスに登録
	m_canvas->Entry(m_uiFrame);
#pragma endregion

#pragma region ゲームの初期化
	// スコアの初期化
	Utility::ScoreManager::GetInstance()->Initialize();

	// カメラの作成
	m_camera = new Camera::Camera();
	m_camera->SetEyePosition(Vector3(0.0f, 14.0f, 9.0f));
	m_camera->SetEulerAngles(Vector3(-60.0f, 0.0f, 0.0f));
	m_camera->SetViewportSize(static_cast<float>(Game::SCREEN_WIDTH - 320), static_cast<float>(Game::SCREEN_HEIGHT));
	m_camera->SetAspect(static_cast<float>(Game::SCREEN_WIDTH - 320) / static_cast<float>(Game::SCREEN_HEIGHT));

	// タスクマネージャーの作成
	m_taskManager = new Utility::TaskManager();

	// ステージマネージャーの作成
	m_stageManager = new Utility::StageManager();

	// ステージの作成
	m_stageManager->Create(m_camera, m_taskManager);

	// プレイヤーの作成
	m_player = new Play::Object::Player();
	m_player->SetCamera(m_camera);

	// プレイヤーをタスクマネージャーに登録
	m_taskManager->Entry(m_player);


	// エネミーマネージャーの作成
	m_enemyManager = new Utility::EnemyManager();

	// 敵の生成
	m_enemyManager->Create(m_camera, m_taskManager, m_player);


	// 衝突判定のマスク設定
	Collision::CollisionManager* collisionManager = Collision::CollisionManager::GetInstance();
	collisionManager->SetCollisionGroup(ObjectTag::PLAYER | ObjectTag::PLAYER, false);
	collisionManager->SetCollisionGroup(ObjectTag::TILE | ObjectTag::TILE, false);
	collisionManager->SetCollisionGroup(ObjectTag::ENEMY | ObjectTag::ENEMY, true);
	collisionManager->SetCollisionGroup(ObjectTag::PLAYER | ObjectTag::ENEMY, true);
	collisionManager->SetCollisionGroup(ObjectTag::TILE | ObjectTag::PLAYER, true);
	collisionManager->SetCollisionGroup(ObjectTag::TILE | ObjectTag::ENEMY, true);

	// スカイドームの作成
	m_skyDome = new Play::Object::Stage::SkyDome();
	m_skyDome->Initialize();
	m_skyDome->SetCamere(m_camera);
#pragma endregion

#pragma region 演出の初期化
	// シーケンスの設定
	m_sequenceID = SequenceID::FADE_IN;

	// タイマーの初期化
	m_fadeTime = 1.0f;

	// 2Dのプリミティブ描画マネージャーのインスタンスの取得
	m_primitiveManager = Graphic2D::PrimitiveManager2D::GetInstance();

	m_screenViewport = Common::DeviceResources::GetInstance()->GetScreenViewport();

	m_readyAndGo = new Play::Object::UI::ReadyAndGo(m_camera->GetViewport());
#pragma endregion

	Sound::SoundManager::GetInstance()->LoadAcb(L"Play.acb", L"Play.awb");
}



//--------------------------------------------------------------------
//! @summary   終了処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Scene::PlayScene::Finalize()
{
	// キャンバスの削除
	delete m_canvas;
	m_canvas = nullptr;

	// UIの削除
	delete m_uiFrame;
	m_uiFrame = nullptr;

	// カメラの削除
	delete m_camera;
	m_camera = nullptr;

	// タスクマネージャーの削除
	delete m_taskManager;
	m_taskManager = nullptr;

	// ステージマネージャーの削除
	delete m_stageManager;
	m_stageManager = nullptr;

	// プレイヤーの削除
	delete m_player;
	m_player = nullptr;

	// 敵マネージャーの削除
	delete m_enemyManager;
	m_enemyManager = nullptr;

	// スタートUIの削除
	delete m_readyAndGo;
	m_readyAndGo = nullptr;

	// スカイドームの削除
	delete m_skyDome;
	m_skyDome = nullptr;

	// ハイスコアの更新
	Utility::ScoreManager::GetInstance()->UpdateHighScore();

	Sound::SoundManager::GetInstance()->Stop();
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Scene::PlayScene::Update(const Common::StepTimer & timer)
{
	static float time = 0.0f;

	m_skyDome->Update(timer);

	// カメラの更新
	m_camera->Update();

	// UIの更新
	m_canvas->Update(timer);


	switch (m_sequenceID)
	{
	case SequenceID::FADE_IN:
		if (!Sound::SoundManager::GetInstance()->IsState(CRI_PLAY_STARTSE)) Sound::SoundManager::GetInstance()->Play(CRI_PLAY_STARTSE);

		m_fadeTime -= static_cast<float>(timer.GetElapsedSeconds());
		if (m_fadeTime <= 0.0f)
		{
			m_fadeTime = 0.0f;
			m_sequenceID = SequenceID::READY;
		}
		break;
	case SequenceID::READY:
		// UIの更新
		m_canvas->Update(timer);
		m_readyAndGo->Update(timer);
		// アニメーションの終了
		if (!m_uiFrame->GetAnimeState()) time += static_cast<float>(timer.GetElapsedSeconds());

		if (time >= 1.7f)
		{
			time = 0.0f;
			m_sequenceID = SequenceID::NORMAL;
		}
		break;
	case SequenceID::NORMAL:
		// BGMの再生
		if (!Sound::SoundManager::GetInstance()->IsState(CRI_PLAY_PLAYBGM)) Sound::SoundManager::GetInstance()->Play(CRI_PLAY_PLAYBGM);

		// タスクの更新
		m_taskManager->Update(timer);
		m_readyAndGo->Update(timer);
		// シーンの移動
		if ((!m_player->GetActive()) || (m_enemyManager->GetEnemyCount() == 0)) m_sequenceID = SequenceID::FADE_OUT;
		break;
	case SequenceID::FADE_OUT:
		m_fadeTime += static_cast<float>(timer.GetElapsedSeconds());
		// シーン遷移
		if (m_fadeTime >= 1.0f) Library::Scene::SceneManager::GetInstance()->LoadScene("Motos::Scene::TitleScene");
		break;
	default:
		break;
	}
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Scene::PlayScene::Render()
{
	// UIの描画
	m_canvas->Render();

	// 3Dオブジェクトの描画
	m_camera->Begin();
	m_skyDome->Draw();

	// タスクの描画
	m_taskManager->Draw();
	m_camera->End();

	m_readyAndGo->Draw();

	m_primitiveManager->SetViewport(m_screenViewport);
	switch (m_sequenceID)
	{
	case SequenceID::FADE_IN:
		m_primitiveManager->DrawBox(0, 0, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, Color(0.0f, 0.0f, 0.0f, m_fadeTime));
		break;
	case SequenceID::READY:
	case SequenceID::NORMAL:
		// 何もしない
		break;
	case SequenceID::FADE_OUT:
		m_primitiveManager->DrawBox(0, 0, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, Color(0.0f, 0.0f, 0.0f, m_fadeTime));
		break;
	default:
		break;
	}
}
