//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		SpriteUI.h
//!
//! @summary	UI用2Dスプライトクラスのヘッダファイル
//!
//! @date		2018.10.30
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../pch.h"

// <自作ヘッダファイル>
#include "../../Common/StepTimer.h"


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Graphic2D
	{
		namespace UI
		{
			// クラスの定義 ======================================================
			class SpriteUI
			{
				// <メンバ変数>
			protected:
				D3D11_VIEWPORT m_viewport;
				// 描画順番
				int m_drawingOrder;


				// <コンストラクタ>
			public:
				SpriteUI() :
					m_viewport(D3D11_VIEWPORT()),
					m_drawingOrder(0)
				{
					// 何もしない
				}


				// <メンバ関数>
			public:
				virtual void Update(const Common::StepTimer& timer) = 0;
				virtual void Draw() = 0;


				// <セッター関数>
				//----------------------------------------------------------
				//! @parameter [viewport] ビューポートの設定
				//----------------------------------------------------------
				inline void SetViewport(const D3D11_VIEWPORT& viewport) { m_viewport = viewport; }


				// <演算子>
			public:
				inline bool operator<(const SpriteUI* sprite) const
				{
					return (m_drawingOrder < sprite->m_drawingOrder) ? true : false;
				}
			};
		}
	}
}