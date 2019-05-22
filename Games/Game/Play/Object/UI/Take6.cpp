//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Take6.cpp
//!
//! @summary	テイク6のアニメーションクラスのソースファイル
//!
//! @date		2018.11.12
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <自作ヘッダーファイル>
#include "Take6.h"
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
Motos::Play::Object::UI::Take6::Take6(Animator* animator, AnimeTakes* animeTakes, UIParts* uiParts) :
	AnimeCrip(animator, animeTakes, uiParts)
{
	m_endTime = 0.1f;
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::Take6::~Take6()
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
void Motos::Play::Object::UI::Take6::Start()
{
	m_startPositionPartsB = m_uiParts->partsB->GetPosition();
	m_startPositionRemainingLivesTop = m_uiParts->remainingLivesTop->GetPosition();
	m_startPositionRemainingLivesBottom = m_uiParts->remainingLivesBottom->GetPosition();
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Take6::Update(const Common::StepTimer& timer)
{
	// 時間の更新
	m_time += static_cast<float>(timer.GetElapsedSeconds());

	if (m_time <= m_endTime)
	{
		// アニメーション
		m_uiParts->partsB->SetPosition(Lerp(m_startPositionPartsB, Vector2(0.0f, 98.0f) * UIFrame::PROPORTION, m_time / m_endTime));
		m_uiParts->remainingLivesTop->SetPosition(Lerp(m_startPositionRemainingLivesTop, Vector2(0.0f, 580.0f) * UIFrame::PROPORTION, m_time / m_endTime));
		m_uiParts->remainingLivesBottom->SetPosition(Lerp(m_startPositionRemainingLivesBottom, Vector2(0.0f, 590.0f) * UIFrame::PROPORTION, m_time / m_endTime));
	}
	else
	{
		// 微調整
		m_uiParts->partsB->SetPosition(Vector2(0.0f, 98.0f) * UIFrame::PROPORTION);
		m_uiParts->remainingLivesTop->SetPosition(Vector2(0.0f, 580.0f) * UIFrame::PROPORTION);
		m_uiParts->remainingLivesBottom->SetPosition(Vector2(0.0f, 590.0f) * UIFrame::PROPORTION);
	}

	// 次のアニメーションに変更する
	if (m_time >= m_endTime + 0.1f) m_animator->ChangeCrip(m_animeTakes->take7);
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Take6::Render()
{
	m_uiParts->backgroundParts->Draw();
	m_uiParts->largeScreenParts->Draw();
	m_uiParts->nowScere->Draw();
	m_uiParts->partsB->Draw();
	m_uiParts->itemBackgroundParts->Draw();
	m_uiParts->topParts->Draw();
	m_uiParts->bottomParts->Draw();
	m_uiParts->smallScreenParts->Draw();
	m_uiParts->remainingLivesTop->Draw();
	m_uiParts->remainingLivesBottom->Draw();
}
