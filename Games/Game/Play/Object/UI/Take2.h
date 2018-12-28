//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Take2.h
//!
//! @summary	テイク2のアニメーションクラスのヘッダファイル
//!
//! @date		2018.11.12
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <自作ヘッダファイル>
#include "AnimeCrip.h"


// 名前空間の定義 ================================================================
namespace Motos
{
	namespace Play
	{
		namespace Object
		{
			namespace UI
			{
				// 構造体の宣言 ==================================================
				struct AnimeTakes;
				struct UIParts;


				// クラスの宣言 ==================================================
				class Animator;


				// クラスの定義 ==================================================
				class Take2 : public AnimeCrip
				{
					// <メンバ変数>
				private:
					DirectX::SimpleMath::Vector2 m_startPositionPartsA;
					DirectX::SimpleMath::Vector2 m_startPositionPartsB;
					DirectX::SimpleMath::Vector2 m_startPositionRemainingLivesTop;
					DirectX::SimpleMath::Vector2 m_startPositionRemainingLivesBottom;
					DirectX::SimpleMath::Vector2 m_startPositionTopParts;
					DirectX::SimpleMath::Vector2 m_startPositionBottomParts;
					DirectX::SimpleMath::Vector2 m_startPositionSmallScreenParts;


					// <コンストラクタ>
				public:
					//------------------------------------------------------
					//! @parameter [animator] アニメーター
					//! @parameter [animeTakes] アニメーションテイク
					//! @parameter [UIParts] UIパーツ
					//------------------------------------------------------
					Take2(Animator* animator, AnimeTakes* animeTakes, UIParts* uiParts);


					// <デストラクタ>
				public:
					~Take2();


					// <メンバ関数>
				public:
					//------------------------------------------------------
					//! @summary   初期化処理
					//!
					//! @parameter [void] なし
					//!
					//! @return    なし
					//------------------------------------------------------
					void Start() override;


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
					void Render() override;
				};
			}
		}
	}
}
