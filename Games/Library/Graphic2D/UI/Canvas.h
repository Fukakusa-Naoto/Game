//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Canvas.h
//!
//! @summary	キャンバスクラスのヘッダファイル
//!
//! @date		2018.10.29
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../pch.h"

// <標準ヘッダファイル>
#include <list>

// <自作ヘッダファイル>
#include "SpriteUI.h"
#include "../../Common/DeviceResources.h"


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Graphic2D
	{
		namespace UI
		{
			// クラスの定義 ======================================================
			class Canvas
			{
				// <メンバ変数>
			private:
				// デバイス・コンテキストのインターフェイス
				ID3D11DeviceContext* m_deviceContext;
				// ビューポート
				D3D11_VIEWPORT m_viewport;
				// 登録しているスプライトの保管リスト
				std::list<SpriteUI*> m_spriteList;


				// <コンストラクタ>
			public:
				//----------------------------------------------------------
				//! @parameter [void] 何もしない
				//----------------------------------------------------------
				Canvas();


				// <デストラクタ>
			public:
				~Canvas();


				// <メンバ関数>
			public:
				//----------------------------------------------------------
				//! @summary   スプライトの登録処理
				//!
				//! @parameter [sprite] 登録するスプライト
				//!
				//! @return    なし
				//----------------------------------------------------------
				void Entry(SpriteUI* sprite);


				//----------------------------------------------------------
				//! @summary   更新処理
				//!
				//! @parameter [時間情報] timer
				//!
				//! @return    なし
				//----------------------------------------------------------
				void Update(const Common::StepTimer& timer);


				//----------------------------------------------------------
				//! @summary   描画処理
				//!
				//! @parameter [void] なし
				//!
				//! @return    なし
				//----------------------------------------------------------
				void Render();


				// <セッター関数>
			public:
				//----------------------------------------------------------
				//! @parameter [viewport] ビューポートの設定
				//----------------------------------------------------------
				inline void SetViewport(const D3D11_VIEWPORT& viewport) { m_viewport = viewport; }


				//----------------------------------------------------------
				//! @parameter [topLeftX] 左上のX座標
				//! @parameter [topLeftY] 左上のY座標
				//----------------------------------------------------------
				inline void SetPosition(float topLeftX, float topLeftY)
				{
					m_viewport.TopLeftX = topLeftX;
					m_viewport.TopLeftY = topLeftY;
				}


				//----------------------------------------------------------
				//! @parameter [width] 幅
				//! @parameter [height] 高さ
				//----------------------------------------------------------
				inline void SetSize(float width, float height)
				{
					m_viewport.Width = width;
					m_viewport.Height = height;
				}


				//----------------------------------------------------------
				//! @parameter [min] 最小深度
				//! @parameter [max] 最大深度
				//----------------------------------------------------------
				inline void SetDepth(float min, float max)
				{
					m_viewport.MinDepth = min;
					m_viewport.MaxDepth = max;
				}


				// <ゲッター関数>
				//----------------------------------------------------------
				//! @summary   ビューポートの取得
				//----------------------------------------------------------
				inline const D3D11_VIEWPORT& GetViewport() const { return m_viewport; }
			};
		}
	}
}
