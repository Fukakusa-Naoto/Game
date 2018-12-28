//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		RemainingLives.h
//!
//! @summary	モトスの残機表示クラスのヘッダファイル
//!
//! @date		2018.11.13
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
				class RemainingLives
				{
					// <メンバ定数>
					// 切り抜き座標の幅
					static const int RECT_WIDTH;
					// 切り抜き座標の高さ
					static const int RECT_HEIGHT;
					// 最大表示数
					static const int MAX_DIGIT_COUNT;


					// <メンバ変数>
				private:
					// スプライト
					Library::Graphic2D::Sprite* m_sprite;
					// 表示する座標
					DirectX::SimpleMath::Vector2 m_position;
					// ビューポート
					D3D11_VIEWPORT m_viewport;


					// <コンストラクタ>
				public:
					//------------------------------------------------------
					//! @summary   コンストラクタ
					//!
					//! @parameter [void] なし
					//------------------------------------------------------
					RemainingLives();


					// <デストラクタ>
				public:
					~RemainingLives();


					// <メンバ関数>
				public:
					//------------------------------------------------------
					//! @summary   描画処理
					//!
					//! @parameter [count] 残機数
					//!
					//! @return    なし
					//------------------------------------------------------
					void Draw(int count);


					// <セッター関数>
				public:
					//------------------------------------------------------
					//! @parameter [position] 座標
					//------------------------------------------------------
					inline void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; }


					//------------------------------------------------------
					//! @parameter [viewport] ビューポート
					//------------------------------------------------------
					inline void SetViewport(const D3D11_VIEWPORT& viewport) { m_viewport = viewport; }
				};
			}
		}
	}
}

