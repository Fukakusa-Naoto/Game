//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Number.cpp
//!
//! @summary	モトスの数字描画クラスのソースファイル
//!
//! @date		2018.10.31
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../../pch.h"

// <自作ヘッダーファイル>
#include "Number.h"
#include "../../../../Library/Math/Math.h"
#include "UIFrame.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 定数の定義 ==============================================================
// 切り抜き座標の幅
const int Motos::Play::Object::UI::Number::RECT_WIDTH = 29;
// 切り抜き座標の高さ
const int Motos::Play::Object::UI::Number::RECT_HEIGHT = 29;
// 最大表示桁数
const int Motos::Play::Object::UI::Number::DIGIT_NUMBER = 9;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::Number::Number() :
	m_position(Vector2::Zero),
	m_color(Colors::White),
	m_viewport(D3D11_VIEWPORT())
{
	m_sprite = new Graphic2D::Sprite(L"Resources/Textures/UI/UIStates.png");
	m_sprite->SetScale(UIFrame::PROPORTION);
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::Number::~Number()
{
	delete m_sprite;
	m_sprite = nullptr;
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [number] 数字
//! @parameter [digitNumber] 表示最大桁数
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::Number::Draw(int number, int digitNumber)
{
	Math::Clamp(digitNumber, 1, DIGIT_NUMBER);

	// 9桁分表示する
	for (int i = 0; i < digitNumber; ++i)
	{
		// 切り抜き座標の計算
		RECT rect;
		rect.top = 0;
		rect.bottom = RECT_HEIGHT;
		rect.left = RECT_WIDTH * (number % 10);
		rect.right = RECT_WIDTH + RECT_WIDTH * (number % 10);

		// 切り抜き座標の設定
		m_sprite->SetRect(rect);
		// 座標の設定
		m_sprite->SetPosition(Vector2(static_cast<float>(-RECT_WIDTH * i) + m_position.x, m_position.y));
		// 色の設定
		m_sprite->SetColor(m_color);
		// ビューポートの設定
		m_sprite->SetViewport(m_viewport);
		// 描画
		m_sprite->Draw();

		number /= 10;
	}
}
