//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Canvas.cpp
//!
//! @summary	キャンバスクラスのソースファイル
//!
//! @date		2018.10.29
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダーファイル>
#include "Canvas.h"


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
Graphic2D::UI::Canvas::Canvas()
{
	// デバイス・コンテキストのインターフェイスの取得
	m_deviceContext = Common::DeviceResources::GetInstance()->GetD3DDeviceContext();


	// 画面サイズの取得
	RECT screenRect = Common::DeviceResources::GetInstance()->GetOutputSize();
	int width = screenRect.right - screenRect.left;
	int height = screenRect.bottom - screenRect.top;

	// ビューポートの初期設定
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_viewport.Width = static_cast<float>(width);
	m_viewport.Height = static_cast<float>(height);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Graphic2D::UI::Canvas::~Canvas()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   スプライトの登録処理
//!
//! @parameter [sprite] 登録するスプライト
//!
//! @return    なし
//--------------------------------------------------------------------
void Graphic2D::UI::Canvas::Entry(Graphic2D::UI::SpriteUI* sprite)
{
	m_spriteList.push_back(sprite);
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [時間情報] timer
//!
//! @return    なし
//--------------------------------------------------------------------
void Graphic2D::UI::Canvas::Update(const Common::StepTimer& timer)
{
	// 登録されているスプライトの更新
	for (list<SpriteUI*>::iterator itr = m_spriteList.begin(); itr != m_spriteList.end(); ++itr)
	{
		(*itr)->Update(timer);
	}
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Graphic2D::UI::Canvas::Render()
{
	// ビューポートの設定
	//m_deviceContext->RSSetViewports(1, &m_viewport);

	// 比較関数の定義
	function<bool(const Graphic2D::UI::SpriteUI*, const Graphic2D::UI::SpriteUI*)> comparison = [](const Graphic2D::UI::SpriteUI* spriteA, const Graphic2D::UI::SpriteUI* spriteB)
	{
		return spriteA->operator<(spriteB);
	};

	// 描画順にソート
	m_spriteList.sort(comparison);


	// 登録されているスプライトの描画
	for (list<SpriteUI*>::iterator itr = m_spriteList.begin(); itr != m_spriteList.end(); ++itr)
	{
		// ビューポートをセット
		(*itr)->SetViewport(m_viewport);
		(*itr)->Draw();
	}
}
