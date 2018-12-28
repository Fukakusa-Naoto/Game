//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		PlayerController.h
//!
//! @summary	モトスのプレイヤーコントローラークラスのヘッダファイル
//!
//! @date		2018.12.05
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <自作ヘッダーファイル>
#include "../../../Library/System/ControllerBase.h"
#include "../../../Library/Physics/RigidBody.h"


// 名前空間の定義 ================================================================
namespace Motos
{
	namespace Play
	{
		namespace Controller
		{
			// クラスの定義 ======================================================
			class PlayerController final : public Library::System::ControllerBase
			{
				// <メンバ変数>
			private:
				// 速度
				DirectX::SimpleMath::Vector3 m_velocity;
				// 物理演算コンポーネント
				Library::Physics::RigidBody* m_rigidBody;
				// 生存フラグ
				bool m_isActive;


				// <コンストラクタ>
			public:
				//----------------------------------------------------------
				//! @summary   コンストラクタ
				//!
				//! @parameter [gameObject] ゲームオブジェクト
				//! @parameter [transform] 座標変換コンポーネント
				//----------------------------------------------------------
				PlayerController(Library::System::GameObject* gameObject, Library::Math::Transform& transform);


				// <デストラクタ>
			public:
				~PlayerController();


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


				// <ゲッター関数>
			public:
				//----------------------------------------------------------
				//! @summary   生存フラグの取得
				//----------------------------------------------------------
				inline bool GetActive() const { return m_isActive; }
			};
		}
	}
}
