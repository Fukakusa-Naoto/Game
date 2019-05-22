//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Take2.cpp
//!
//! @summary	テイク2のアニメーションクラスのソースファイル
//!
//! @date		2018.11.12
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <自作ヘッダーファイル>
#include "Take2.h"
#include "UIFrame.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX::SimpleMath;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [animator] アニメーター
//! @parameter [animeTakes] アニメーションテイク
//! @parameter [uiParts] UIパーツ
//--------------------------------------------------------------------
Motos::Play::Object::UI::Take2::Take2(Animator* animator, AnimeTakes* animeTakes, UIParts* uiParts) :
	AnimeCrip(animator, animeTakes, uiParts)
{
	m_endTime = 0.26f;
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::Take2::~Take2()
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
void Motos::Play::Object::UI::Take2::Start()
{
	m_startPositionPartsA = m_uiParts->partsA->GetPosition();
	m_startPositionPartsB = m_uiParts->partsB->GetPosition();
	m_startPositionRemainingLivesTop = m_uiParts->remainingLivesTop->GetPosition();
	m_startPositionRemainingLivesBottom = m_uiParts->remainingLivesBottom->GetPosition();
	m_startPositionTopParts = m_uiParts->topParts->GetPosition();
	m_startPositionBottomParts = m_uiParts->bottomParts->GetPosition();
	m_startPositionSmallScreenParts = m_uiParts->smallScreenParts->GetPosition();
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Take2::Update(const Common::StepTimer& timer)
{
	// 時間の更新
	m_time += static_cast<float>(timer.GetElapsedSeconds());

	if (m_time <= m_endTime)
	{
		// アニメーション
		m_uiParts->partsA->SetPosition(Lerp(m_startPositionPartsA, Vector2(213.0f, 600.0f) * UIFrame::PROPORTION, m_time / m_endTime));
		m_uiParts->partsB->SetPosition(Lerp(m_startPositionPartsB, Vector2(0.0f, 420.0f) * UIFrame::PROPORTION, m_time / m_endTime));
		m_uiParts->remainingLivesTop->SetPosition(Lerp(m_startPositionRemainingLivesTop, Vector2(0.0f, 600.0f) * UIFrame::PROPORTION, m_time / m_endTime));
		m_uiParts->remainingLivesBottom->SetPosition(Lerp(m_startPositionRemainingLivesBottom, Vector2(0.0f, 651.0f) * UIFrame::PROPORTION, m_time / m_endTime));
		m_uiParts->topParts->SetPosition(Lerp(m_startPositionTopParts, Vector2(0.0f, 512.0f) * UIFrame::PROPORTION, m_time / m_endTime));
		m_uiParts->bottomParts->SetPosition(Lerp(m_startPositionBottomParts, Vector2(0.0f, 532.0f) * UIFrame::PROPORTION, m_time / m_endTime));
		m_uiParts->smallScreenParts->SetPosition(Lerp(m_startPositionSmallScreenParts, Vector2(210.0f, 532.0f) * UIFrame::PROPORTION, m_time / m_endTime));
	}
	else
	{
		// 微調整
		m_uiParts->partsA->SetPosition(Vector2(213.0f, 600.0f) * UIFrame::PROPORTION);
		m_uiParts->partsB->SetPosition(Vector2(0.0f, 420.0f) * UIFrame::PROPORTION);
		m_uiParts->remainingLivesTop->SetPosition(Vector2(0.0f, 600.0f) * UIFrame::PROPORTION);
		m_uiParts->remainingLivesBottom->SetPosition(Vector2(0.0f, 610.0f) * UIFrame::PROPORTION);
		m_uiParts->topParts->SetPosition(Vector2(0.0f, 512.0f) * UIFrame::PROPORTION);
		m_uiParts->bottomParts->SetPosition(Vector2(0.0f, 532.0f) * UIFrame::PROPORTION);
		m_uiParts->smallScreenParts->SetPosition(Vector2(210.0f, 532.0f) * UIFrame::PROPORTION);
	}


	// 次のアニメーションに変更する
	if (m_time >= m_endTime + 0.54) m_animator->ChangeCrip(m_animeTakes->take3);
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Take2::Render()
{
	m_uiParts->backgroundParts->Draw();
	m_uiParts->partsA->Draw();
	m_uiParts->partsB->Draw();
	m_uiParts->largeScreenParts->Draw();
	m_uiParts->topParts->Draw();
	m_uiParts->bottomParts->Draw();
	m_uiParts->smallScreenParts->Draw();
	m_uiParts->remainingLivesTop->Draw();
	m_uiParts->remainingLivesBottom->Draw();
}
