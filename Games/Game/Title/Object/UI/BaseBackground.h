//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		BaseBackground.h
//!
//! @summary	モトスのタイトルのベース背景クラスのヘッダファイル
//!
//! @date		2018.11.01
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
	namespace Title
	{
		namespace Object
		{
			namespace UI
			{
				// クラスの定義 ==================================================
				class BaseBackground : public Library::Graphic2D::UI::SpriteUI
				{
					// <メンバ変数>
				private:
					Library::Graphic2D::Sprite* m_sprite;


					// <コンストラクタ>
				public:
					BaseBackground();


					// <デストラクタ>
				public:
					~BaseBackground();


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
