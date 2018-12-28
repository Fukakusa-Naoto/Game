//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		AnimeCrip.cpp
//!
//! @summary	アニメーションクリップの基底クラスのソースファイル
//!
//! @date		2018.11.09
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../../pch.h"

// <自作ヘッダーファイル>
#include "AnimeCrip.h"


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
Motos::Play::Object::UI::AnimeCrip::AnimeCrip(Animator* animator, AnimeTakes* animeTakes, UIParts* uiParts) :
	m_time(0.0f),
	m_endTime(0.0f),
	m_animator(animator),
	m_animeTakes(animeTakes),
	m_uiParts(uiParts)
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::AnimeCrip::~AnimeCrip()
{
}



//------------------------------------------------------
//! @summary   補間
//!
//! @parameter [start] 開始位置
//! @parameter [end] 終了位置
//! @parameter [t] 両端の距離を1とした時の割合
//!
//! @return    なし
//------------------------------------------------------
Vector2 Motos::Play::Object::UI::AnimeCrip::Lerp(const Vector2& start, const Vector2& end, float t)
{
	return DirectX::SimpleMath::Vector2::Lerp(start, end, t);
}
