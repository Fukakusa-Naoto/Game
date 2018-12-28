//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		SelectUI.cpp
//!
//! @summary	モトスのセレクトUIクラスのソースファイル
//!
//! @date		2018.12.13
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../../pch.h"

// <自作ヘッダーファイル>
#include "SelectUI.h"
#include "../../../Game.h"
#include "../../../../Library/Input/KeyboardUtil.h"
#include "../../../../Library/Math/Math.h"


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
Motos::Title::Object::UI::SelectUI::SelectUI() :
	m_nowSelect(0)
{
	m_sprite = new Graphic2D::Sprite(L"Resources/Textures/TitleUI.png");
	m_sprite->SetAnchorPoint(Vector2(0.5f, 0.5f));

	m_drawingOrder = 2;
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Title::Object::UI::SelectUI::~SelectUI()
{
	delete m_sprite;
	m_sprite = nullptr;
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Title::Object::UI::SelectUI::Update(const Common::StepTimer& timer)
{
	// キーボード入力マネージャーのインスタンスの取得
	Input::KeyboardUtil* keyboard = Input::KeyboardUtil::GetInstance();

	// カーソルの移動
	if (keyboard->IsTriggered(Keyboard::Keys::Up)) --m_nowSelect;
	else if (keyboard->IsTriggered(Keyboard::Keys::Down)) ++m_nowSelect;

	// カーソル移動の制限
	Math::Clamp(m_nowSelect, 0, static_cast<int>(SelectID::SELECT_NUM) - 1);
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Title::Object::UI::SelectUI::Draw()
{
	m_sprite->SetViewport(m_viewport);

	Vector2 position(m_viewport.Width*0.5f, m_viewport.Height*0.5f);
	for (int i = 0; i < SelectID::SELECT_NUM; ++i)
	{
		// 背景の表示
		RECT rect;
		rect.top = rect.left = 0;
		rect.bottom = 48;
		rect.right = 372;
		m_sprite->SetRect(rect);
		m_sprite->SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
		m_sprite->SetPosition(Vector2(0.0f, static_cast<float>(i)*70.0f) + position);
		m_sprite->Draw();

		// テキストを表示
		rect.top = 48 * (i + 1);
		rect.bottom = rect.top + 48;
		rect.left = 0;
		rect.right = 372;
		m_sprite->SetRect(rect);
		if (m_nowSelect == i) m_sprite->SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
		else m_sprite->SetColor(Color(0.5f, 0.5f, 0.5f, 1.0f));
		m_sprite->Draw();
	}
}

