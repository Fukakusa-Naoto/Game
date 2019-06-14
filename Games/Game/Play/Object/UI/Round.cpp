//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Round.cpp
//!
//! @summary	モトスのラウンド描画クラスのソースファイル
//!
//! @date		2018.11.13
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../../pch.h"

// <自作ヘッダーファイル>
#include "Round.h"
#include "UIFrame.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::Round::Round() :
	m_position(Vector2::Zero),
	m_viewport(D3D11_VIEWPORT())
{
	m_hyphen = new Graphic2D::Sprite(L"Resources/Textures/UI/UIStates.png");

	// 切り抜き座標の計算
	RECT rect;
	rect.top = 32;
	rect.bottom = 40;
	rect.left = 0;
	rect.right = 18;

	// 切り抜き座標の設定
	m_hyphen->SetRect(rect);

	m_number = new Number();

	// 色の設定
	m_number->SetColor(Color(0.1765, 0.9020, 0.000));

	m_hyphen->SetScale(UIFrame::PROPORTION);
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::Round::~Round()
{
	delete m_hyphen;
	m_hyphen = nullptr;

	delete m_number;
	m_number = nullptr;
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [count] 残機数
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Round::Draw(int count)
{
	m_hyphen->SetViewport(m_viewport);
	m_number->SetViewport(m_viewport);

	m_hyphen->SetPosition(m_position);
	m_hyphen->Draw();

	m_number->SetPosition(Vector2(-25.0f, -10.0f) + m_position);
	m_number->Draw((count / 10) % 10, 1);

	m_number->SetPosition(Vector2(25.0f, -10.0f) + m_position);
	m_number->Draw(count % 10, 1);
}
