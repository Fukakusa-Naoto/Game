//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		RemainingLives.cpp
//!
//! @summary	モトスの残機描画クラスのソースファイル
//!
//! @date		2018.11.13
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../../pch.h"

// <自作ヘッダーファイル>
#include "RemainingLives.h"
#include "../../../../Library/Math/Math.h"
#include "UIFrame.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 定数の定義 ==============================================================
// 切り抜き座標の幅
const int Motos::Play::Object::UI::RemainingLives::RECT_WIDTH = 22;
// 切り抜き座標の高さ
const int Motos::Play::Object::UI::RemainingLives::RECT_HEIGHT = 27;
// 最大表示数
const int Motos::Play::Object::UI::RemainingLives::MAX_DIGIT_COUNT = 5;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::RemainingLives::RemainingLives() :
	m_position(Vector2::Zero)
{
	m_sprite = new Graphic2D::Sprite(L"Resources/Textures/UI/UIStates.png");

	// 切り抜き座標の計算
	RECT rect;
	rect.top = 45;
	rect.bottom = 45 + RECT_HEIGHT;
	rect.left = 0;
	rect.right = RECT_WIDTH;

	// 切り抜き座標の設定
	m_sprite->SetRect(rect);

	m_sprite->SetScale(UIFrame::PROPORTION);
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::RemainingLives::~RemainingLives()
{
	delete m_sprite;
	m_sprite = nullptr;
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [count] 残機数
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::RemainingLives::Draw(int count)
{
	Math::Clamp(count, 1, MAX_DIGIT_COUNT);

	// 9桁分表示する
	for (int i = 0; i < count; ++i)
	{
		// 座標の設定
		m_sprite->SetPosition(Vector2(static_cast<float>((RECT_WIDTH + 10) * i) + m_position.x, m_position.y));
		// ビューポートの設定
		m_sprite->SetViewport(m_viewport);
		// 描画
		m_sprite->Draw();
	}
}
