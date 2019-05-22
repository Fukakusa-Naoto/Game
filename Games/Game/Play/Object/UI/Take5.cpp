//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Take5.cpp
//!
//! @summary	テイク5のアニメーションクラスのソースファイル
//!
//! @date		2018.11.12
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <自作ヘッダーファイル>
#include "Take5.h"
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
Motos::Play::Object::UI::Take5::Take5(Animator* animator, AnimeTakes* animeTakes, UIParts* uiParts) :
	AnimeCrip(animator, animeTakes, uiParts)
{
	m_endTime = 0.1f;
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::Take5::~Take5()
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
void Motos::Play::Object::UI::Take5::Start()
{
	m_startPositionTopParts = m_uiParts->topParts->GetPosition();
	m_startPositionItemBackgroundParts = m_uiParts->itemBackgroundParts->GetPosition();
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Take5::Update(const Common::StepTimer& timer)
{
	// 時間の更新
	m_time += static_cast<float>(timer.GetElapsedSeconds());

	if (m_time <= m_endTime)
	{
		// アニメーション
		m_uiParts->topParts->SetPosition(Lerp(m_startPositionTopParts, Vector2(0.0f, 240.0f) * UIFrame::PROPORTION, m_time / m_endTime));
		m_uiParts->itemBackgroundParts->SetPosition(Lerp(m_startPositionItemBackgroundParts, Vector2(0.0f, 262.0f) * UIFrame::PROPORTION, m_time / m_endTime));
	}
	else
	{
		// 微調整
		m_uiParts->topParts->SetPosition(Vector2(0.0f, 240.0f) * UIFrame::PROPORTION);
		m_uiParts->itemBackgroundParts->SetPosition(Vector2(0.0f, 262.0f) * UIFrame::PROPORTION);
	}

	// 次のアニメーションに変更する
	if (m_time >= m_endTime + 0.84f) m_animator->ChangeCrip(m_animeTakes->take6);
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Take5::Render()
{
	m_uiParts->backgroundParts->Draw();
	m_uiParts->partsB->Draw();
	m_uiParts->partsC->Draw();
	m_uiParts->largeScreenParts->Draw();
	m_uiParts->itemBackgroundParts->Draw();
	m_uiParts->topParts->Draw();
	m_uiParts->bottomParts->Draw();
	m_uiParts->smallScreenParts->Draw();
}
