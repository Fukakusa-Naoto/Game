//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		AnimeCrip.h
//!
//! @summary	アニメーションクリップの基底クラスのヘッダファイル
//!
//! @date		2018.11.09
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../../pch.h"

// <自作ヘッダファイル>
#include "../../../../Library/Common/StepTimer.h"


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
				class AnimeCrip
				{
					// <メンバ変数>
				protected:
					float m_time;
					float m_endTime;
					Animator* m_animator;
					AnimeTakes* m_animeTakes;
					UIParts* m_uiParts;


					// <コンストラクタ>
				public:
					//------------------------------------------------------
					//! @parameter [animator] アニメーター
					//! @parameter [animeTakes] アニメーションテイク
					//! @parameter [uiParts] UIパーツ
					//------------------------------------------------------
					AnimeCrip(Animator* animator, AnimeTakes* animeTakes, UIParts* uiParts);


					// <デストラクタ>
				public:
					virtual ~AnimeCrip();


					// <メンバ関数>
				public:
					//------------------------------------------------------
					//! @summary   初期化処理
					//!
					//! @parameter [void] なし
					//!
					//! @return    なし
					//------------------------------------------------------
					virtual void Start() = 0;


					//------------------------------------------------------
					//! @summary   更新処理
					//!
					//! @parameter [timer] 時間情報
					//!
					//! @return    なし
					//------------------------------------------------------
					virtual void Update(const Library::Common::StepTimer & timer) = 0;



					//------------------------------------------------------
					//! @summary   描画処理
					//!
					//! @parameter [void] なし
					//!
					//! @return    なし
					//------------------------------------------------------
					virtual void Render() = 0;


					// <静的メンバ関数>
				public:
					//------------------------------------------------------
					//! @summary   補間
					//!
					//! @parameter [start] 開始位置
					//! @parameter [end] 終了位置
					//! @parameter [t] 両端の距離を1とした時の割合
					//!
					//! @return    なし
					//------------------------------------------------------
					static DirectX::SimpleMath::Vector2 Lerp(const DirectX::SimpleMath::Vector2& start, const DirectX::SimpleMath::Vector2& end, float t);
				};
			}
		}
	}
}