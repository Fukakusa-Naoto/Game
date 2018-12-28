//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		ReadyAndGo.h
//!
//! @summary	モトスのReadyとGo!!のUIクラスのヘッダファイル
//!
//! @date		2018.12.14
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <自作ヘッダーファイル>
#include "../../../../Library/Graphic2D/UI/SpriteUI.h"
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
				class ReadyAndGo : public Library::Graphic2D::UI::SpriteUI
				{
					// <メンバ定数>
				public:
					// 切り抜き座標の配列
					static const RECT TEXTURE_RECT[2];


					// <メンバ変数>
				private:
					Library::Graphic2D::Sprite* m_sprite;
					float m_time;
					float m_animeTime;


					// <コンストラクタ>
				public:
					//------------------------------------------------------
					//! @parameter [viewport] ビューポート
					//------------------------------------------------------
					ReadyAndGo(const D3D11_VIEWPORT& viewport);


					// <デストラクタ>
				public:
					~ReadyAndGo();


					// <メンバ関数>
				public:
					//------------------------------------------------------
					//! @summary   更新処理
					//!
					//! @parameter [timer] 時間情報
					//!
					//! @return    なし
					//------------------------------------------------------
					void Update(const Library::Common::StepTimer & timer) override;



					//------------------------------------------------------
					//! @summary   描画処理
					//!
					//! @parameter [void] なし
					//!
					//! @return    なし
					//------------------------------------------------------
					void Draw() override;
				};
			}
		}
	}
}