//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		RigidBody.h
//!
//! @summary	剛体クラスのヘッダファイル
//!
//! @date		2018.11.24
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================




// 名前空間の定義 ================================================================
namespace Library
{
	// クラスの定義 ==============================================================
	namespace Math { class Transform; }
	namespace Collision { class CollisionShape; }
	namespace Common { class StepTimer; }

	namespace Physics
	{
		// クラスの定義 ==========================================================
		// 物理エンジンマネージャー
		class PhysicsManager;


		// クラスの定義 ==========================================================
		class RigidBody
		{
			// <メンバ変数>
		private:
			// 物理マネージャーのインスタンス
			PhysicsManager* m_physicsManager;
			// 座標変換コンポーネント
			Math::Transform& m_transform;

			// 速度
			DirectX::SimpleMath::Vector3 m_velocity;
			// 衝突判定用フラグ
			bool m_isCollide;
			// 落下時間
			float m_fallingTime;


			// <コンストラクタ>
		public:
			//--------------------------------------------------------------
			//! @parameter [transform] 座標変換コンポーネント
			//! @parameter [collisionShape] 衝突判定用シェイプ
			//--------------------------------------------------------------
			RigidBody(Math::Transform& transform, Collision::CollisionShape* collisionShape);


			// <デストラクタ>
		public:
			~RigidBody();


			// <メンバ関数>
		public:
			//--------------------------------------------------------------
			//! @summary   更新処理
			//!
			//! @parameter [timer] 時間情報
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Update(const Common::StepTimer& timer);


			//--------------------------------------------------------------
			//! @summary   衝突時に行われる処理
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Collide();
		};
	}
}
