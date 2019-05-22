﻿//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Take1.h
//!
//! @summary	テイク1のアニメーションクラスのヘッダファイル
//!
//! @date		2018.11.09
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
				class Take1 : public AnimeCrip
				{
					// <コンストラクタ>
				public:
					//------------------------------------------------------
					//! @parameter [animator] アニメーター
					//! @parameter [animeTakes] アニメーションテイク
					//! @parameter [UIParts] UIパーツ
					//------------------------------------------------------
					Take1(Animator* animator, AnimeTakes* animeTakes, UIParts* uiParts);


					// <デストラクタ>
				public:
					~Take1();


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