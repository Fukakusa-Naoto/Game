//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Take8.cpp
//!
//! @summary	テイク8のアニメーションクラスのソースファイル
//!
//! @date		2018.11.12
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <自作ヘッダーファイル>
#include "Take8.h"
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
Motos::Play::Object::UI::Take8::Take8(Animator* animator, AnimeTakes* animeTakes, UIParts* uiParts) :
	AnimeCrip(animator, animeTakes, uiParts)
{
	m_endTime = 0.1f;
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::Take8::~Take8()
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
void Motos::Play::Object::UI::Take8::Start()
{
	m_startPositionPartsB = m_uiParts->partsB->GetPosition();
	m_startPositionNowScore = m_uiParts->nowScere->GetPosition();
	m_startPositionHighScore = m_uiParts->highScere->GetPosition();
	m_startPositionTopParts = m_uiParts->topParts->GetPosition();
	m_startPositionBottomParts = m_uiParts->bottomParts->GetPosition();
	m_startPositionItemBackground = m_uiParts->itemBackgroundParts->GetPosition();
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Take8::Update(const Common::StepTimer& timer)
{
	// 時間の更新
	m_time += static_cast<float>(timer.GetElapsedSeconds());

	if (m_time <= m_endTime)
	{
		// アニメーション
		m_uiParts->partsB->SetPosition(Lerp(m_startPositionPartsB, Vector2(0.0f, -92.0f) * UIFrame::PROPORTION, m_time / m_endTime));
		m_uiParts->nowScere->SetPosition(Lerp(m_startPositionNowScore, Vector2(0.0f, 78.0f) * UIFrame::PROPORTION, m_time / m_endTime));
		m_uiParts->highScere->SetPosition(Lerp(m_startPositionHighScore, Vector2(0.0f, 0.0f) * UIFrame::PROPORTION, m_time / m_endTime));
		m_uiParts->topParts->SetPosition(Lerp(m_startPositionTopParts, Vector2(0.0f, 128.0f) * UIFrame::PROPORTION, m_time / m_endTime));
		m_uiParts->itemBackgroundParts->SetPosition(Lerp(m_startPositionItemBackground, Vector2(0.0f, 150.0f) * UIFrame::PROPORTION, m_time / m_endTime));
		m_uiParts->bottomParts->SetPosition(Lerp(m_startPositionBottomParts, Vector2(0.0f, 600.0f) * UIFrame::PROPORTION, m_time / m_endTime));
	}
	else
	{
		// 微調整
		m_uiParts->partsB->SetPosition(Vector2(0.0f, -92.0f) * UIFrame::PROPORTION);
		m_uiParts->nowScere->SetPosition(Vector2(0.0f, 78.0f) * UIFrame::PROPORTION);
		m_uiParts->highScere->SetPosition(Vector2(0.0f, 0.0f) * UIFrame::PROPORTION);
		m_uiParts->topParts->SetPosition(Vector2(0.0f, 128.0f) * UIFrame::PROPORTION);
		m_uiParts->itemBackgroundParts->SetPosition(Vector2(0.0f, 150.0f) * UIFrame::PROPORTION);
		m_uiParts->bottomParts->SetPosition(Vector2(0.0f, 600.0f) * UIFrame::PROPORTION);
	}

	// 次のアニメーションに変更する
	if (m_time >= m_endTime + 0.1f)
	{
		m_animator->ChangeCrip(nullptr);
		m_animator->Stop();
	}
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Take8::Render()
{
	m_uiParts->backgroundParts->Draw();
	m_uiParts->largeScreenParts->Draw();
	m_uiParts->nowScere->Draw();
	m_uiParts->highScere->Draw();
	m_uiParts->partsB->Draw();
	m_uiParts->itemBackgroundParts->Draw();
	m_uiParts->topParts->Draw();
	m_uiParts->bottomParts->Draw();
	m_uiParts->smallScreenParts->Draw();
	m_uiParts->remainingLivesTop->Draw();
	m_uiParts->remainingLivesBottom->Draw();
}
