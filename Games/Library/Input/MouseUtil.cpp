#//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		MouseUtil.cpp
//!
//! @summary	マウス入力クラスのソースファイル
//!
//! @date		2018.09.20
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダファイル>
#include "MouseUtil.h"


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
Input::MouseUtil::MouseUtil() :
	m_mouse(nullptr),
	m_state(Mouse::State()),
	m_buttonStateTracker(Mouse::ButtonStateTracker())
{
	m_mouse = new Mouse();
	m_mouse->SetMode(Mouse::Mode::MODE_ABSOLUTE);

	// 押下状態の変数アドレスを記録
	m_buttonStates[Left] = &m_state.leftButton;
	m_buttonStates[Middle] = &m_state.middleButton;
	m_buttonStates[Right] = &m_state.rightButton;

	// トリガー状態の変数アドレスを記録
	m_buttonTrackerStates[Left] = &m_buttonStateTracker.leftButton;
	m_buttonTrackerStates[Middle] = &m_buttonStateTracker.middleButton;
	m_buttonTrackerStates[Right] = &m_buttonStateTracker.rightButton;
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Input::MouseUtil::~MouseUtil()
{
	delete m_mouse;
	m_mouse = nullptr;
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Input::MouseUtil::Update()
{
	// キーボードステートを取得
	m_state = m_mouse->GetState();
	// トリガー状態の更新
	m_buttonStateTracker.Update(m_state);
}



//--------------------------------------------------------------------
//! @summary   指定ボタンが押されているか判定を行う
//!
//! @parameter [button] キーの指定
//!
//! @return    true ：指定ボタンが押されている
//! @return    false：指定ボタンが押されていない
//--------------------------------------------------------------------
bool Input::MouseUtil::IsPressed(Button button)
{
	return *m_buttonStates[button];
}



//--------------------------------------------------------------------
//! @summary   指定ボタンが押された瞬間の判定を行う
//!
//! @parameter [button] ボタンの指定
//!
//! @return    true ：指定ボタンが押された
//! @return    false：指定ボタンが押されていない
//--------------------------------------------------------------------
bool Input::MouseUtil::IsTriggered(Button button)
{
	if (*m_buttonTrackerStates[button] == Mouse::ButtonStateTracker::ButtonState::PRESSED) return true;

	return false;
}



//--------------------------------------------------------------------
//! @summary   指定ボタンが離された瞬間の判定を行う
//!
//! @parameter [button] キーの指定
//!
//! @return    true ：指定ボタンが離された
//! @return    false：指定ボタンが離されていない
//--------------------------------------------------------------------
bool Input::MouseUtil::IsReleased(Button button)
{
	if (*m_buttonTrackerStates[button] == Mouse::ButtonStateTracker::ButtonState::RELEASED) return true;

	return false;
}



//--------------------------------------------------------------------
//! @summary   ホイール値のリセット
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Input::MouseUtil::ResetWheelValue()
{
	m_mouse->ResetScrollWheelValue();
}
