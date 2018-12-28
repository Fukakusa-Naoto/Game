//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Take3.cpp
//!
//! @summary	テイク3のアニメーションクラスのソースファイル
//!
//! @date		2018.11.12
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <自作ヘッダーファイル>
#include "Take3.h"
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
Motos::Play::Object::UI::Take3::Take3(Animator* animator, AnimeTakes* animeTakes, UIParts* uiParts) :
	AnimeCrip(animator, animeTakes, uiParts)
{
	m_endTime = 0.16f;
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::Take3::~Take3()
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
void Motos::Play::Object::UI::Take3::Start()
{
	m_startPositionPartsB = m_uiParts->partsB->GetPosition();
	m_startPositionPartsC = m_uiParts->partsC->GetPosition();
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Take3::Update(const Common::StepTimer& timer)
{
	// 時間の更新
	m_time += static_cast<float>(timer.GetElapsedSeconds());

	if (m_time <= m_endTime)
	{
		// アニメーション
		m_uiParts->partsB->SetPosition(Lerp(m_startPositionPartsB, Vector2(0.0f, 365.0f) * UIFrame::PROPORTION, m_time / m_endTime));
		m_uiParts->partsC->SetPosition(Lerp(m_startPositionPartsC, Vector2(0.0f, 455.0f) * UIFrame::PROPORTION, m_time / m_endTime));
	}
	else
	{
		// 微調整
		m_uiParts->partsB->SetPosition(Vector2(0.0f, 365.0f) * UIFrame::PROPORTION);
		m_uiParts->partsC->SetPosition(Vector2(0.0f, 455.0f) * UIFrame::PROPORTION);
	}

	// 次のアニメーションに変更する
	if (m_time >= m_endTime + 0.2f) m_animator->ChangeCrip(m_animeTakes->take4);
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Take3::Render()
{
	m_uiParts->backgroundParts->Draw();
	m_uiParts->partsB->Draw();
	m_uiParts->partsC->Draw();
	m_uiParts->largeScreenParts->Draw();
	m_uiParts->topParts->Draw();
	m_uiParts->bottomParts->Draw();
	m_uiParts->smallScreenParts->Draw();
}
