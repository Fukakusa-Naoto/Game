//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		SmallScreenParts.cpp
//!
//! @summary	モトスUIのスクリーン(小)パーツクラスのソースファイル
//!
//! @date		2018.11.09
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../../pch.h"

// <自作ヘッダーファイル>
#include "SmallScreenParts.h"
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
//! @parameter [veiwport] ビューポート
//--------------------------------------------------------------------
Motos::Play::Object::UI::SmallScreenParts::SmallScreenParts(D3D11_VIEWPORT& viewport) :
	m_viewport(viewport)
{
	m_sprite = new Graphic2D::Sprite(L"Resources/Textures/UI/UISpriteSheet.png");
	RECT rect;
	rect.top = 131;
	rect.bottom = 199;
	rect.left = 0;
	rect.right = 110;
	m_sprite->SetRect(rect);
	m_sprite->SetScale(UIFrame::PROPORTION);
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::SmallScreenParts::~SmallScreenParts()
{
	delete m_sprite;
	m_sprite = nullptr;
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::SmallScreenParts::Draw()
{
	m_sprite->SetViewport(m_viewport);
	m_sprite->Draw();
}
