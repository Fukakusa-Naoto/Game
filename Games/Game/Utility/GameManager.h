//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		GameManager.h
//!
//! @summary	ゲーム管理クラスのヘッダファイル
//!
//! @date		2019.06.02
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <自作ヘッダファイル>
#include "../../Library/System/SingletonBase.h"


// 名前空間の定義 ================================================================
namespace Motos
{
	namespace Utility
	{
		// クラスの定義 ==========================================================
		class GameManager : public Library::System::SingletonBase<GameManager>
		{
			// <フレンドの登録>
			friend Library::System::SingletonBase<GameManager>;


			// <メンバ変数>
		private:
			// プレイヤーの残機
			int m_playerLife;
			// 現在のステージ
			int m_stageNumber;


			// <コンストラクタ>
		private:
			GameManager();


			// <デストラクタ>
		public:
			~GameManager();


			// <メンバー関数>
		public:
			//----------------------------------------------------------
			//! @summary   プレイヤーの残機を一つ増やす
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//----------------------------------------------------------
			void AddPlayerLife();


			//----------------------------------------------------------
			//! @summary   プレイヤーの残機を一つ減らす
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//----------------------------------------------------------
			void SubPlayerLife();


			//----------------------------------------------------------
			//! @summary   ステージを進める
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//----------------------------------------------------------
			void UpStageNumber();


			// <セッター関数>
		public:
			inline void SetPlayerLife(int playerLife) { m_playerLife = playerLife; }


			// <ゲッター関数>
		public:
			inline int GetPlayerLife() const { return m_playerLife; }
			inline int GetStageNumber() const { return m_stageNumber; }
		};
	}
}