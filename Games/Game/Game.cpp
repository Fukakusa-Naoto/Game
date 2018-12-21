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

#include "../Library/Graphic2D/PrimitiveManager2D.h"
#include "../Library/Graphic3D/PrimitiveManager3D.h"
#include "../Library/Debug/DebugCamera.h"
#include "../Library/Input/KeyboardUtil.h"
#include "../Library/Graphic3D/Model/ModelManager.h"

#include "../Resources/Sounds/Basic.h"


// 共有する変数・関数の宣言 =================================================
// ゲーム終了関数
extern void ExitGame();


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 定数の定義 ==============================================================
// ゲームタイトル
const wstring Game::GAME_TITLE = L"Game";


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [hInstance]
//! @parameter [nCmdShow]
//--------------------------------------------------------------------
Game::Game(HINSTANCE hInstance, int nCmdShow) :
	Framework(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE)
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
	// スプライトの作成
	//m_sprite = new Graphic2D::Sprite(L"Resources/Textures/TridentLogo.png");

	// カメラの作成
	//m_camera = new Camera::Camera();
	m_camera = new Debug::DebugCamera();

	// サウンドマネージャーのインスタンスの取得
	//m_soundManager = Sound::SoundManager::GetInstance();
	//m_soundManager->Initialize(L"ADX2_samples.acf");
	//m_soundManager->LoadAcb(L"Basic.acb", L"Basic.awb");

	// BGMの再生
	//m_soundManager->Play(CRI_BASIC_MUSIC1);

	// モデルの作成
	m_model = Graphic3D::Model::ModelManager::GetInstance()->LoadModelDataFromPMD(L"GUMI/GUMI");
	m_model2 = Graphic3D::Model::ModelManager::GetInstance()->LoadModelDataFromPMX(L"IA/IA");
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
	// スプライトの削除
	//delete m_sprite;

	// カメラの削除
	delete m_camera;
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
	// カメラの更新
	m_camera->Update();

	// スペースキーの入力
	//if (Input::KeyboardUtil::GetInstance()->IsTriggered(Keyboard::Keys::Space))
	//{
	//	// 鳴っていなかったら音を鳴らす
	//	if (!m_soundManager->IsState(CRI_BASIC_VOICE_RANDOM)) m_soundManager->Play(CRI_BASIC_VOICE_RANDOM);
	//}
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
#pragma region PrimitiveManager3D
	//// 点の描画
	//Graphic3D::PrimitiveManager3D::GetInstance()->DrawPoint(Vector3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
	//// 線の描画
	//Graphic3D::PrimitiveManager3D::GetInstance()->DrawLine(Vector3::Zero, Vector3::One, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
	//// 三角形の描画
	//Graphic3D::PrimitiveManager3D::GetInstance()->DrawTriangle(Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, -1.0f, 0.0f), Vector3(-1.0f, -1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix(), false);
	//// 立方体の描画
	//static float angle = 0;
	//Matrix world = Matrix::CreateRotationY(XMConvertToRadians(angle++));
	//Graphic3D::PrimitiveManager3D::GetInstance()->DrawCube(world, Vector3(0.5f, 0.5f, 0.5f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix(), false);
	//// 球体の描画
	//Graphic3D::PrimitiveManager3D::GetInstance()->DrawSphere(Vector3(1.0f, 0.0f, 0.0f), 1.0f, XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix(), true);
#pragma endregion

#pragma region Model
	// モデルの描画
	m_model->Draw(Matrix::Identity, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
	m_model2->Draw(Matrix::CreateTranslation(Vector3(15.0f, 0.0f, 0.0f)), m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
#pragma endregion

#pragma region PrimitiveManager2D
	//// 点の描画
	//Graphic2D::PrimitiveManager2D::GetInstance()->DrawPoint(10, 10, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

	//// 線の描画
	//Graphic2D::PrimitiveManager2D::GetInstance()->DrawLine(10, 20, 150, 20, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	//// 三角形の描画
	//Graphic2D::PrimitiveManager2D::GetInstance()->DrawTriangle(40, 360, 10, 450, 70, 450, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), true);
	//Graphic2D::PrimitiveManager2D::GetInstance()->DrawTriangle(115, 360, 80, 450, 150, 450, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), false);

	//// 四角形の描画
	//Graphic2D::PrimitiveManager2D::GetInstance()->DrawBox(10, 50, 70, 150, XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), true);
	//Graphic2D::PrimitiveManager2D::GetInstance()->DrawBox(80, 50, 150, 150, XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), false);

	//// 円の描画
	//Graphic2D::PrimitiveManager2D::GetInstance()->DrawCircle(40, 200, 30, XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), true);
	//Graphic2D::PrimitiveManager2D::GetInstance()->DrawCircle(110, 200, 30, XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), false);


	//Graphic2D::PrimitiveManager2D::GetInstance()->DrawCircle(450, 200, 50, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f), true);
	//Graphic2D::PrimitiveManager2D::GetInstance()->DrawCircle(410, 260, 50, XMFLOAT4(0.0f, 1.0f, 0.0f, 0.5f), true);
	//Graphic2D::PrimitiveManager2D::GetInstance()->DrawCircle(490, 260, 50, XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f), true);
#pragma endregion

#pragma region Sprite
	// スプライトの描画
	//m_sprite->Draw();
#pragma endregion


}
#pragma endregion
