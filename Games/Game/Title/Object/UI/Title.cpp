//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Title.cpp
//!
//! @summary	モトスのタイトルクラスのソースファイル
//!
//! @date		2018.11.01
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../../pch.h"

// <自作ヘッダーファイル>
#include "Title.h"
#include "../../../Game.h"


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
Motos::Title::Object::UI::Title::Title()
{
	m_sprite = new Graphic2D::Sprite(L"Resources/Textures/Motos_Title.png");
	m_sprite->SetPosition(Vector2(static_cast<float>(Game::SCREEN_WIDTH / 2), 0.0f));
	m_sprite->SetAnchorPoint(Vector2(0.5f, 0.0f));
	m_sprite->SetScale(Vector2(1.6f, 1.2f));

	m_drawingOrder = 2;
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Title::Object::UI::Title::~Title()
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
void Motos::Title::Object::UI::Title::Update(const Common::StepTimer& timer)
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Title::Object::UI::Title::Draw()
{
	m_sprite->SetViewport(m_viewport);
	m_sprite->Draw();
}
