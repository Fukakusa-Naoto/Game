//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Take1.cpp
//!
//! @summary	テイク1のアニメーションクラスのソースファイル
//!
//! @date		2018.11.09
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <自作ヘッダーファイル>
#include "Take1.h"
#include "UIFrame.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [animator] アニメーター
//! @parameter [animeTakes] アニメーションテイク
//! @parameter [uiParts] UIパーツ
//--------------------------------------------------------------------
Motos::Play::Object::UI::Take1::Take1(Animator* animator, AnimeTakes* animeTakes, UIParts* uiParts) :
	AnimeCrip(animator, animeTakes, uiParts)
{
	m_endTime = 1.7f;
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::Take1::~Take1()
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
void Motos::Play::Object::UI::Take1::Start()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Take1::Update(const Common::StepTimer& timer)
{
	// 時間の更新
	m_time += static_cast<float>(timer.GetElapsedSeconds());

	// 次のアニメーションに変更する
	if (m_time >= m_endTime)
	{
		m_animator->ChangeCrip(m_animeTakes->take2);
	}
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Take1::Render()
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
