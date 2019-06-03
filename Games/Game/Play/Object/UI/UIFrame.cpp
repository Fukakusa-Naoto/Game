//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		UIFrame.cpp
//!
//! @summary	モトスのUIフレームクラスのソースファイル
//!
//! @date		2018.11.06
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../../pch.h"

// <自作ヘッダーファイル>
#include "UIFrame.h"
#include "../../../../Library/Graphic2D/PrimitiveManager2D.h"
#include "../../../Utility/GameManager.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 定数の定義 ==============================================================
// 画面サイズ変更時の比率調整
const Vector2 Motos::Play::Object::UI::UIFrame::PROPORTION = Vector2(1.0f, 1.2f);


// 関数の定義 ==============================================================
#pragma region UIParts
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [viewport] ビューポート
//--------------------------------------------------------------------
Motos::Play::Object::UI::UIParts::UIParts(D3D11_VIEWPORT& viewport)
{
	RECT rect;

	rect.top = 0;
	rect.bottom = 61;
	rect.left = 0;
	rect.right = 107;
	partsA = new Graphic2D::Sprite(L"Resources/Textures/UI/UISpriteSheet.png");
	partsA->SetRect(rect);
	partsA->SetScale(UIFrame::PROPORTION);

	rect.top = 257;
	rect.bottom = 349;
	rect.left = 0;
	rect.right = 320;
	partsB = new Graphic2D::Sprite(L"Resources/Textures/UI/UISpriteSheet.png");
	partsB->SetRect(rect);
	partsB->SetScale(UIFrame::PROPORTION);

	rect.top = 776;
	rect.bottom = 834;
	rect.left = 0;
	rect.right = 320;
	partsC = new Graphic2D::Sprite(L"Resources/Textures/UI/UISpriteSheet.png");
	partsC->SetRect(rect);
	partsC->SetScale(UIFrame::PROPORTION);

	rect.top = 1229;
	rect.bottom = 1270;
	rect.left = 0;
	rect.right = 320;
	highScere = new Graphic2D::Sprite(L"Resources/Textures/UI/UISpriteSheet.png");
	highScere->SetRect(rect);
	highScere->SetScale(UIFrame::PROPORTION);

	rect.top = 1271;
	rect.bottom = 1316;
	rect.left = 0;
	rect.right = 320;
	nowScere = new Graphic2D::Sprite(L"Resources/Textures/UI/UISpriteSheet.png");
	nowScere->SetRect(rect);
	nowScere->SetScale(UIFrame::PROPORTION);

	rect.top = 1425;
	rect.bottom = 1476;
	rect.left = 0;
	rect.right = 213;
	remainingLivesTop = new Graphic2D::Sprite(L"Resources/Textures/UI/UISpriteSheet.png");
	remainingLivesTop->SetRect(rect);
	remainingLivesTop->SetScale(UIFrame::PROPORTION);

	rect.top = 1477;
	rect.bottom = 1487;
	rect.left = 0;
	rect.right = 213;
	remainingLivesBottom = new Graphic2D::Sprite(L"Resources/Textures/UI/UISpriteSheet.png");
	remainingLivesBottom->SetRect(rect);
	remainingLivesBottom->SetScale(UIFrame::PROPORTION);

	topParts = new TopParts(viewport);
	bottomParts = new BottomParts(viewport);
	smallScreenParts = new SmallScreenParts(viewport);
	largeScreenParts = new LargeScreenParts(viewport);
	backgroundParts = new BackgroundParts(viewport);
	itemBackgroundParts = new ItemBackgroundParts(viewport);

	// 座標の初期化
	partsA->SetPosition(Vector2(213.0f, 539.0f) * UIFrame::PROPORTION);
	partsB->SetPosition(Vector2(0.0f, 359.0f) * UIFrame::PROPORTION);
	partsC->SetPosition(Vector2(0.0f, 513.0f) * UIFrame::PROPORTION);
	bottomParts->SetPosition(Vector2(0.0f, 471.0f) * UIFrame::PROPORTION);
	topParts->SetPosition(Vector2(0.0f, 451.0f) * UIFrame::PROPORTION);
	smallScreenParts->SetPosition(Vector2(210.0f, 471.0f) * UIFrame::PROPORTION);
	largeScreenParts->SetPosition(Vector2(0.0f, 0.0f) * UIFrame::PROPORTION);
	backgroundParts->SetPosition(Vector2(0.0f, 148.0f) * UIFrame::PROPORTION);
	remainingLivesTop->SetPosition(Vector2(0.0f, 539.0f) * UIFrame::PROPORTION);
	remainingLivesBottom->SetPosition(Vector2(0.0f, 590.0f) * UIFrame::PROPORTION);
	itemBackgroundParts->SetPosition(Vector2(0.0f, 524.0f) * UIFrame::PROPORTION);
	nowScere->SetPosition(Vector2(0.0f, 190.0f) * UIFrame::PROPORTION);
	highScere->SetPosition(Vector2(0.0f, 112.0f) * UIFrame::PROPORTION);
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::UIParts::~UIParts()
{
	delete partsA;
	partsA = nullptr;

	delete partsB;
	partsB = nullptr;

	delete partsC;
	partsC = nullptr;

	delete highScere;
	highScere = nullptr;

	delete nowScere;
	nowScere = nullptr;

	delete remainingLivesTop;
	remainingLivesTop = nullptr;

	delete remainingLivesBottom;
	remainingLivesBottom = nullptr;

	delete topParts;
	topParts = nullptr;

	delete bottomParts;
	bottomParts = nullptr;

	delete smallScreenParts;
	smallScreenParts = nullptr;

	delete largeScreenParts;
	largeScreenParts = nullptr;

	delete backgroundParts;
	backgroundParts = nullptr;

	delete itemBackgroundParts;
	itemBackgroundParts = nullptr;
}
#pragma endregion



#pragma region UIStates
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::UIStates::UIStates()
{
	nowScore = new Number();
	highScore = new Number();
	remainingLives = new RemainingLives();
	round = new Round();

	// 座標の設定
	nowScore->SetPosition(Vector2(260.0f, 92.0f) * UIFrame::PROPORTION);
	highScore->SetPosition(Vector2(260.0f, 45.0f) * UIFrame::PROPORTION);
	remainingLives->SetPosition(Vector2(30.0f, 558.0f) * UIFrame::PROPORTION);
	round->SetPosition(Vector2(255.0f, 560.0f) * UIFrame::PROPORTION);

	// 色の設定
	nowScore->SetColor(Color(0.1765, 0.9020, 0.000));
	highScore->SetColor(Color(0.1765, 0.9020, 0.000));
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::UIStates::~UIStates()
{
	delete nowScore;
	nowScore = nullptr;

	delete highScore;
	highScore = nullptr;

	delete remainingLives;
	remainingLives = nullptr;

	delete round;
	round = nullptr;
}
#pragma endregion



#pragma region AnimeTakes
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [animator] アニメーター
//! @parameter [uiParts] UIパーツ
//--------------------------------------------------------------------
Motos::Play::Object::UI::AnimeTakes::AnimeTakes(Animator* animator, UIParts* uiParts)
{
	take1 = new Take1(animator, this, uiParts);
	take2 = new Take2(animator, this, uiParts);
	take3 = new Take3(animator, this, uiParts);
	take4 = new Take4(animator, this, uiParts);
	take5 = new Take5(animator, this, uiParts);
	take6 = new Take6(animator, this, uiParts);
	take7 = new Take7(animator, this, uiParts);
	take8 = new Take8(animator, this, uiParts);
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::AnimeTakes::~AnimeTakes()
{
	delete take1;
	take1 = nullptr;

	delete take2;
	take2 = nullptr;

	delete take3;
	take3 = nullptr;

	delete take4;
	take4 = nullptr;

	delete take5;
	take5 = nullptr;

	delete take6;
	take6 = nullptr;

	delete take7;
	take7 = nullptr;

	delete take7;
	take7 = nullptr;

	delete take8;
	take8 = nullptr;
}
#pragma endregion



#pragma region UIFrame
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::UIFrame::UIFrame()
{
	m_uiParts = new UIParts(m_viewport);
	m_uiStates = new UIStates();
	m_animator = new Animator();
	m_animeTakes = new AnimeTakes(m_animator, m_uiParts);
	m_scoreManager = Utility::ScoreManager::GetInstance();
	m_playerLife = Utility::GameManager::GetInstance()->GetPlayerLife();
	m_stageNumber = Utility::GameManager::GetInstance()->GetStageNumber();

	m_animator->ChangeCrip(m_animeTakes->take1);
	m_animator->Play();
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::UIFrame::~UIFrame()
{
	delete m_uiParts;
	m_uiParts = nullptr;

	delete m_uiStates;
	m_uiStates = nullptr;

	delete m_animator;
	m_animator = nullptr;

	delete m_animeTakes;
	m_animeTakes = nullptr;
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::UIFrame::Update(const Common::StepTimer& timer)
{
	m_animator->Update(timer);
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::UIFrame::Draw()
{
	// 背景を黒くする
	Graphic2D::PrimitiveManager2D::GetInstance()->SetViewport(m_viewport);
	Graphic2D::PrimitiveManager2D::GetInstance()->DrawBox(0, 0, 320, 600, Color(0, 0, 0, 1));

	// ビューポートの設定
	m_uiParts->SetViewport(m_viewport);
	m_uiStates->SetViewport(m_viewport);

	if(m_animator->GetState()) m_animator->Render();
	else
	{
		m_uiParts->largeScreenParts->Draw();
		m_uiParts->nowScere->Draw();
		m_uiStates->nowScore->Draw(m_scoreManager->GetNowScore(), 6);
		m_uiParts->highScere->Draw();
		m_uiStates->highScore->Draw(m_scoreManager->GetHighScore());
		m_uiParts->itemBackgroundParts->Draw();
		m_uiParts->topParts->Draw();
		m_uiParts->smallScreenParts->Draw();
		m_uiStates->round->Draw(m_stageNumber);
		m_uiParts->remainingLivesTop->Draw();
		m_uiParts->remainingLivesBottom->Draw();
		m_uiStates->remainingLives->Draw(m_playerLife);
	}
}



//--------------------------------------------------------------------
//! @summary   AからBの時間内かどうか調べる
//!
//! @parameter [time] 調べる時間
//! @parameter [timeA] 時間A
//! @parameter [timeB] 時間B
//!
//! @return    true :時間内
//! @return    false:時間外
//--------------------------------------------------------------------
bool Motos::Play::Object::UI::UIFrame::IsTime(float time, float timeA, float timeB)
{
	if ((time >= timeA) && (time <= timeB)) return true;
	else return false;
}
#pragma endregion
