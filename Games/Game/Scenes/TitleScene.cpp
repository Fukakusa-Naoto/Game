//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		TitleScene.cpp
//!
//! @summary	モトスのタイトルシーンクラスのソースファイル
//!
//! @date		2018.11.01
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダーファイル>
#include "TitleScene.h"
#include "../Game.h"
#include "../../Library/Scene/SceneManager.h"
#include "../../Library/Input/KeyboardUtil.h"
#include "../../Library/Sound/SoundManager.h"
#include "..//Utility/GameManager.h"

// <リソースファイル>
#include "../../Resources/Sounds/Title.h"


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
Motos::Scene::TitleScene::TitleScene() :
	m_canvas(nullptr),
	m_title(nullptr),
	m_baseBackground(nullptr),
	m_tileBackground(nullptr),
	m_selectUI(nullptr)
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Scene::TitleScene::~TitleScene()
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
void Motos::Scene::TitleScene::Initialize()
{
	// ----- <オブジェクトの作成> -----
	// UIキャンバス
	m_canvas = new Graphic2D::UI::Canvas();
	// タイトル
	m_title = new Title::Object::UI::Title();
	// ベース背景
	m_baseBackground = new Title::Object::UI::BaseBackground();
	// タイル背景
	m_tileBackground = new Title::Object::UI::TileBackground();
	// セレクトUI
	m_selectUI = new Title::Object::UI::SelectUI();


	// ----- <UIキャンバスに登録> -----
	m_canvas->Entry(m_title);
	m_canvas->Entry(m_baseBackground);
	m_canvas->Entry(m_tileBackground);
	m_canvas->Entry(m_selectUI);

	// シーケンスの設定
	m_sequenceID = SequenceID::FADE_IN;

	// タイマーの初期化
	m_fadeTime = 1.0f;

	// 2Dのプリミティブ描画マネージャーのインスタンスの取得
	m_primitiveManager = Graphic2D::PrimitiveManager2D::GetInstance();

	Sound::SoundManager::GetInstance()->LoadAcb(L"Title.acb", L"Title.awb");
}



//--------------------------------------------------------------------
//! @summary   終了処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Scene::TitleScene::Finalize()
{
	// ----- <オブジェクトの削除> -----
	// キャンバス
	delete m_canvas;
	m_canvas = nullptr;
	// タイトル
	delete m_title;
	m_title = nullptr;
	// ベース背景
	delete m_baseBackground;
	m_baseBackground = nullptr;
	// タイル背景
	delete m_tileBackground;
	m_tileBackground = nullptr;
	// セレクトUI
	delete m_selectUI;
	m_selectUI = nullptr;
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Scene::TitleScene::Update(const Common::StepTimer & timer)
{
	if(!Sound::SoundManager::GetInstance()->IsState(CRI_TITLE_TITLEBGM)) Sound::SoundManager::GetInstance()->Play(CRI_TITLE_TITLEBGM);

	switch (m_sequenceID)
	{
	case SequenceID::FADE_IN:
		m_fadeTime -= static_cast<float>(timer.GetElapsedSeconds());
		if (m_fadeTime <= 0.0f)
		{
			m_fadeTime = 0.0f;
			m_sequenceID = SequenceID::NORMAL;
		}
		break;
	case SequenceID::NORMAL:
		// UIの更新
		m_canvas->Update(timer);
		// シーンの移動
		if (Input::KeyboardUtil::GetInstance()->IsTriggered(Keyboard::Keys::Space))
		{
			Sound::SoundManager::GetInstance()->Play(CRI_TITLE_TITLESELECT);
			m_sequenceID = SequenceID::FADE_OUT;
		}
		break;
	case SequenceID::FADE_OUT:
		m_fadeTime += static_cast<float>(timer.GetElapsedSeconds());
		if (m_fadeTime >= 1.0f)
		{
			switch (m_selectUI->GetSelectState())
			{
			case  Title::Object::UI::SelectUI::SelectID::START_GAME:
				// プレイヤーの残機を初期化
				Utility::GameManager::GetInstance()->SetPlayerLife(3);
				Library::Scene::SceneManager::GetInstance()->LoadScene("Motos::Scene::PlayScene");
				break;
			case Title::Object::UI::SelectUI::SelectID::EXIT:
				Game::ExitGame();
				break;
			default:
				break;
			}
		}

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
void Motos::Scene::TitleScene::Render()
{
	// UIの描画
	m_canvas->Render();

	m_primitiveManager->SetViewport(m_canvas->GetViewport());
	switch (m_sequenceID)
	{
	case Motos::Scene::TitleScene::FADE_IN:
		m_primitiveManager->DrawBox(0, 0, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, Color(0.0f, 0.0f, 0.0f, m_fadeTime));
		break;
	case Motos::Scene::TitleScene::NORMAL:
		// 何もしない
		break;
	case Motos::Scene::TitleScene::FADE_OUT:
		m_primitiveManager->DrawBox(0, 0, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, Color(0.0f, 0.0f, 0.0f, m_fadeTime));
		break;
	default:
		break;
	}
}
