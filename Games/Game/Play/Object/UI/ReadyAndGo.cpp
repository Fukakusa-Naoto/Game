//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		ReadyAndGo.cpp
//!
//! @summary	モトスのReadyとGo!!のUIクラスのソースファイル
//!
//! @date		2018.12.14
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../../pch.h"

// <自作ヘッダーファイル>
#include "ReadyAndGo.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 定数の定義 ==============================================================
// 切り抜き座標の配列
const RECT Motos::Play::Object::UI::ReadyAndGo::TEXTURE_RECT[2] =
{
	{0,   0, 377, 107},
	{0, 107, 377, 214}
};


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [viewport] ビューポート
//--------------------------------------------------------------------
Motos::Play::Object::UI::ReadyAndGo::ReadyAndGo(const D3D11_VIEWPORT& viewport) :
	m_time(0.0f),
	m_animeTime(0.0f)
{
	m_sprite = new Graphic2D::Sprite(L"Resources/Textures/READY_GO!!.png");
	m_sprite->SetViewport(viewport);
	m_sprite->SetAnchorPoint(Vector2(0.5f, 0.5f));
	m_sprite->SetRect(TEXTURE_RECT[0]);
	m_sprite->SetPosition(Vector2(static_cast<float>(viewport.Width) * 0.5f, static_cast<float>(viewport.Height) * 0.5f));
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::UI::ReadyAndGo::~ReadyAndGo()
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
void Motos::Play::Object::UI::ReadyAndGo::Update(const Common::StepTimer& timer)
{
	m_time += static_cast<float>(timer.GetElapsedSeconds());

	if (m_time >= 1.7f)
	{
		m_sprite->SetRect(TEXTURE_RECT[1]);
		m_animeTime += static_cast<float>(timer.GetElapsedSeconds());
	}
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::UI::ReadyAndGo::Draw()
{
	if (m_time <= 2.7f)
	{
		m_sprite->SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f - m_animeTime));
		m_sprite->SetScale(Vector2::One + Vector2::One*m_animeTime);
		m_sprite->Draw();
	}
}
