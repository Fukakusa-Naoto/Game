//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		EnemyController.h
//!
//! @summary	モトスの敵コントローラークラスのヘッダファイル
//!
//! @date		2018.12.05
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../../pch.h"

// <自作ヘッダーファイル>
#include "../../../../Library/System/ControllerBase.h"


// 名前空間の定義 ================================================================
namespace Motos
{
	// クラスの宣言 ==============================================================
	namespace Utility { class EnemyManager; }


	namespace Play
	{
		namespace Controller
		{
			// クラスの定義 ======================================================
			class EnemyController final : public Library::System::ControllerBase
			{
				// <メンバ変数>
			private:
				// 速度
				DirectX::SimpleMath::Vector3 m_velocity;
				// ターゲットの情報
				Library::System::GameObject* m_target;
				// 敵管理クラス
				Utility::EnemyManager* m_enemyManager;
				// 生存フラグ
				bool m_isActive;
				bool m_isFalling;


				// <コンストラクタ>
			public:
				//----------------------------------------------------------
				//! @summary   コンストラクタ
				//!
				//! @parameter [gameObject] ゲームオブジェクト
				//! @parameter [transform] 座標変換コンポーネント
				//----------------------------------------------------------
				EnemyController(Library::System::GameObject* gameObject, Library::Math::Transform& transform);


				// <デストラクタ>
			public:
				~EnemyController();


				// <メンバ関数>
			public:
				//----------------------------------------------------------
				//! @summary   更新処理
				//!
				//! @parameter [timer] 時間情報
				//!
				//! @return    なし
				//----------------------------------------------------------
				void Update(const Library::Common::StepTimer & timer) override;


				//----------------------------------------------------------
				//! @summary   衝突判定処理
				//!
				//! @parameter [collider] 衝突しているオブジェクト
				//!
				//! @return    なし
				//----------------------------------------------------------
				void OnCollision(Library::Collision::CollisionShape* collider) override;


			private:
				//----------------------------------------------------------
				//! @summary   落下時の処理
				//!
				//! @parameter [void] なし
				//!
				//! @return    なし
				//----------------------------------------------------------
				void Falling();


				// <セッター関数>
			public:
				//----------------------------------------------------------
				//! @parameter [target] 追いかけるターゲットの情報
				//----------------------------------------------------------
				inline void SetTarget(Library::System::GameObject* target) { m_target = target; }


				//----------------------------------------------------------
				//! @parameter [enemyManager] 敵管理クラス
				//----------------------------------------------------------
				inline void SetEnemyManager(Utility::EnemyManager* enemyManager) { m_enemyManager = enemyManager; }
			};
		}
	}
}
