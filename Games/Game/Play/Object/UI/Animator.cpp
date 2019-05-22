//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Animator.cpp
//!
//! @summary	アニメーションクラスのソースファイル
//!
//! @date		2018.11.09
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../../pch.h"

// <自作ヘッダーファイル>
#include "Animator.h"
#include "AnimeCrip.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::Animator::Animator() :
	m_animeCrip(nullptr),
	m_animationState(false)
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::Animator::~Animator()
{
	m_animeCrip = nullptr;
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Animator::Update(const Common::StepTimer& timer)
{
	if((m_animeCrip) && (m_animationState)) m_animeCrip->Update(timer);
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Animator::Render()
{
	if (m_animeCrip) m_animeCrip->Render();
}



//--------------------------------------------------------------------
//! @summary   アニメーションの開始処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Animator::Play()
{
	m_animationState = true;
}



//--------------------------------------------------------------------
//! @summary   アニメーションの停止処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Animator::Stop()
{
	m_animationState = false;
}



//--------------------------------------------------------------------
//! @summary   アニメーションクリップの変更処理
//!
//! @parameter [animeCrip] 変更するアニメーションクリップ
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Animator::ChangeCrip(AnimeCrip* animeCrip)
{
	m_animeCrip = animeCrip;
	if(m_animeCrip) m_animeCrip->Start();
}
