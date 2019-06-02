//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		UIFrame.h
//!
//! @summary	モトスのUIフレームクラスのヘッダファイル
//!
//! @date		2018.11.06
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <自作ヘッダーファイル>
#include "../../../../Library/Graphic2D/UI/SpriteUI.h"
#include "../../../../Library/Graphic2D/Sprite.h"
#include "TopParts.h"
#include "BottomParts.h"
#include "SmallScreenParts.h"
#include "LargeScreenParts.h"
#include "BackgroundParts.h"
#include "ItemBackgroundParts.h"

#include "Number.h"
#include "RemainingLives.h"
#include "Round.h"

#include "Take1.h"
#include "Take2.h"
#include "Take3.h"
#include "Take4.h"
#include "Take5.h"
#include "Take6.h"
#include "Take7.h"
#include "Take8.h"
#include "Animator.h"

#include "../../../Utility/ScoreManager.h"


// 名前空間の定義 ================================================================
namespace Motos
{
	namespace Play
	{
		namespace Object
		{
			namespace UI
			{
				// 構造体の定義 ==================================================
#pragma region UIParts
				struct UIParts
				{
					// <変数>
					Library::Graphic2D::Sprite* partsA;
					Library::Graphic2D::Sprite* partsB;
					Library::Graphic2D::Sprite* partsC;
					Library::Graphic2D::Sprite* highScere;
					Library::Graphic2D::Sprite* nowScere;
					Library::Graphic2D::Sprite* remainingLivesTop;
					Library::Graphic2D::Sprite* remainingLivesBottom;
					TopParts* topParts;
					BottomParts* bottomParts;
					SmallScreenParts* smallScreenParts;
					LargeScreenParts* largeScreenParts;
					BackgroundParts* backgroundParts;
					ItemBackgroundParts* itemBackgroundParts;


					// <コンストラクタ>
					//------------------------------------------------------
					//! @parameter [viewport] ビューポート
					//------------------------------------------------------
					UIParts(D3D11_VIEWPORT& viewport);


					// <デストラクタ>
					~UIParts();


					// <メンバ関数>
					inline void SetViewport(const D3D11_VIEWPORT& viewport)
					{
						partsA->SetViewport(viewport);
						partsB->SetViewport(viewport);
						partsC->SetViewport(viewport);
						highScere->SetViewport(viewport);
						nowScere->SetViewport(viewport);
						remainingLivesTop->SetViewport(viewport);
						remainingLivesBottom->SetViewport(viewport);

					}
				};
#pragma endregion

#pragma region UIStates
				struct UIStates
				{
					// <変数>
					Number* nowScore;
					Number* highScore;
					RemainingLives* remainingLives;
					Round* round;


					// <コンストラクタ>
					//------------------------------------------------------
					//! @parameter [void] なし
					//------------------------------------------------------
					UIStates();


					// <デストラクタ>
					~UIStates();


					// <メンバ関数>
					inline void SetViewport(const D3D11_VIEWPORT& viewport)
					{
						nowScore->SetViewport(viewport);
						highScore->SetViewport(viewport);
						remainingLives->SetViewport(viewport);
						round->SetViewport(viewport);
					}
				};
#pragma endregion

#pragma region AnimeTakes
				struct AnimeTakes
				{
					// <変数>
					Take1* take1;
					Take2* take2;
					Take3* take3;
					Take4* take4;
					Take5* take5;
					Take6* take6;
					Take7* take7;
					Take8* take8;


					// <コンストラクタ>
					//------------------------------------------------------
					//! @parameter [animator] アニメーター
					//! @parameter [uiParts] UIパーツ
					//------------------------------------------------------
					AnimeTakes(Animator* animator, UIParts* uiParts);


					// <デストラクタ>
					~AnimeTakes();
				};
#pragma endregion


				// クラスの定義 ==================================================
#pragma region UIFrame
				class UIFrame : public Library::Graphic2D::UI::SpriteUI
				{
					// <メンバ定数>
				public:
					// 画面サイズ変更時の比率調整
					static const DirectX::SimpleMath::Vector2 PROPORTION;


					// <メンバ変数>
				private:
					UIParts* m_uiParts;
					UIStates* m_uiStates;
					Animator* m_animator;
					AnimeTakes* m_animeTakes;
					Utility::ScoreManager* m_scoreManager;
					int m_playerLife;

					// <コンストラクタ>
				public:
					UIFrame();


					// <デストラクタ>
				public:
					~UIFrame();


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


				private:
					//------------------------------------------------------
					//! @summary   AからBの時間内かどうか調べる
					//!
					//! @parameter [time] 調べる時間
					//! @parameter [timeA] 時間A
					//! @parameter [timeB] 時間B
					//!
					//! @return    true :時間内
					//! @return    false:時間外
					//------------------------------------------------------
					bool IsTime(float time, float timeA, float timeB);


					// <ゲッター関数>
				public:
					//------------------------------------------------------
					//! @summary   アニメーションの状態を取得
					//------------------------------------------------------
					inline bool GetAnimeState() const { return m_animator->GetState(); }
				};
#pragma endregion
			}
		}
	}
}