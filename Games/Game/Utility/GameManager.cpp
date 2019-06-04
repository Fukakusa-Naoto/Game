//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		GameManager.cpp
//!
//! @summary	ゲーム管理クラスのソースファイル
//!
//! @date		2019.06.02
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <自作ヘッダファイル>
#include "GameManager.h"



// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Utility::GameManager::GameManager() :
	m_playerLife(0),
	m_stageNumber(11)
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Utility::GameManager::~GameManager()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   プレイヤーの残機を一つ増やす
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Utility::GameManager::AddPlayerLife()
{
	++m_playerLife;
}



//--------------------------------------------------------------------
//! @summary   プレイヤーの残機を一つ減らす
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Utility::GameManager::SubPlayerLife()
{
	--m_playerLife;
}



//--------------------------------------------------------------------
//! @summary   ステージを進める
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Utility::GameManager::UpStageNumber()
{
	m_stageNumber += (m_stageNumber % 10 == 5) ? 6 : 1;
}



//--------------------------------------------------------------------
//! @summary   ステージをリセットする
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Utility::GameManager::ResetStageNumber()
{
	m_stageNumber = 11;
}
