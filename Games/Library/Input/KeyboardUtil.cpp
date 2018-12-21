//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		KeyboardUtil.cpp
//!
//! @summary	キーボード入力クラスのソースファイル
//!
//! @date		2018.09.20
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダファイル>
#include "KeyboardUtil.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Input::KeyboardUtil::KeyboardUtil()
{
	m_keyboard = new Keyboard();
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Input::KeyboardUtil::~KeyboardUtil()
{
	m_keyboard->Reset();
	delete m_keyboard;
	m_keyboard = nullptr;
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Input::KeyboardUtil::Update()
{
	m_state = m_keyboard->GetState();
	m_tracker.Update(m_state);
}



//--------------------------------------------------------------------
//! @summary   指定キーが押されているか判定を行う
//!
//! @parameter [key] キーの指定
//!
//! @return    true ：指定キーが押されている
//! @return    false：指定キーが押されていない
//--------------------------------------------------------------------
bool Input::KeyboardUtil::IsPressed(Keyboard::Keys key)
{
	return m_state.IsKeyDown(key);
}



//--------------------------------------------------------------------
//! @summary   指定キーが押された瞬間の判定を行う
//!
//! @parameter [key] キーの指定
//!
//! @return    true ：指定キーが押された
//! @return    false：指定キーが押されていない
//--------------------------------------------------------------------
bool Input::KeyboardUtil::IsTriggered(Keyboard::Keys key)
{
	return m_tracker.IsKeyPressed(key);
}



//--------------------------------------------------------------------
//! @summary   指定キーが離された瞬間の判定を行う
//!
//! @parameter [key] キーの指定
//!
//! @return    true ：指定キーが離された
//! @return    false：指定キーが離されていない
//--------------------------------------------------------------------
bool Input::KeyboardUtil::IsReleased(Keyboard::Keys key)
{
	return m_tracker.IsKeyReleased(key);
}