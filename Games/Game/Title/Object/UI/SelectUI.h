//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		SelectUI.h
//!
//! @summary	モトスのセレクトUIクラスのヘッダファイル
//!
//! @date		2018.12.13
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
				class SelectUI : public Library::Graphic2D::UI::SpriteUI
				{
					// <列挙型>
				public:
					enum SelectID
					{
						START_GAME,
						EXIT,

						SELECT_NUM		// 項目の総数
					};


					// <メンバ変数>
				private:
					Library::Graphic2D::Sprite* m_sprite;
					int m_nowSelect;


					// <コンストラクタ>
				public:
					SelectUI();


					// <デストラクタ>
				public:
					~SelectUI();


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


					// <ゲッター関数>
				public:
					//------------------------------------------------------
					//! @summary   選択状態の取得
					//------------------------------------------------------
					inline int GetSelectState() const { return m_nowSelect; }
				};
			}
		}
	}
}
