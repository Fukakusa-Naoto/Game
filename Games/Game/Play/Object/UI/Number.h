//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Number.h
//!
//! @summary	モトスの数字表示クラスのヘッダファイル
//!
//! @date		2018.10.31
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <自作ヘッダーファイル>
#include "../../../../Library/Graphic2D/Sprite.h"


// 名前空間の定義 ================================================================
namespace Motos
{
	namespace Play
	{
		namespace Object
		{
			namespace UI
			{
				// クラスの定義 ==================================================
				class Number
				{
					// <メンバ定数>
					// 切り抜き座標の幅
					static const int RECT_WIDTH;
					// 切り抜き座標の高さ
					static const int RECT_HEIGHT;
					// 最大表示桁数
					static const int DIGIT_NUMBER;


					// <メンバ変数>
				private:
					// スプライト
					Library::Graphic2D::Sprite* m_sprite;
					// 表示する座標
					DirectX::SimpleMath::Vector2 m_position;
					// 表示する色
					DirectX::SimpleMath::Color m_color;
					// ビューポート
					D3D11_VIEWPORT m_viewport;


					// <コンストラクタ>
				public:
					//------------------------------------------------------
					//! @summary   コンストラクタ
					//!
					//! @parameter [void] なし
					//------------------------------------------------------
					Number();


					// <デストラクタ>
				public:
					~Number();


					// <メンバ関数>
				public:
					//------------------------------------------------------
					//! @summary   描画処理
					//!
					//! @parameter [number] 数字
					//! @parameter [digitNumber] 表示最大桁数
					//!
					//! @return    なし
					//------------------------------------------------------
					void Draw(int number, int digitNumber = DIGIT_NUMBER);


					// <セッター関数>
				public:
					//------------------------------------------------------
					//! @parameter [position] 座標
					//------------------------------------------------------
					inline void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; }


					//------------------------------------------------------
					//! @parameter [color] 色
					//------------------------------------------------------
					inline void SetColor(const DirectX::SimpleMath::Color& color) { m_color = color; }


					//------------------------------------------------------
					//! @parameter [viewport] ビューポート
					//------------------------------------------------------
					inline void SetViewport(const D3D11_VIEWPORT& viewport) { m_viewport = viewport; }
				};
			}
		}
	}
}
