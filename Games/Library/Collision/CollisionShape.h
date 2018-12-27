//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		CollisionShape.h
//!
//! @summary	衝突判定形状の基底クラスのヘッダファイル
//!
//! @date		2018.11.26
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once


// ヘッダファイルの読み込み =======================================================
#include "../System/ControllerBase.h"


// 名前空間の定義 ================================================================
namespace Library
{
	// クラスの定義 ==============================================================
	// 座標変換クラス
	namespace Math { class Transform; }
	// 物理演算
	namespace Physics { class RigidBody; }


	namespace Collision
	{
		// 列挙型の宣言 ==========================================================
		enum ColliderType;


		// クラスの宣言 ==========================================================
		// 衝突判定マネージャー
		class CollisionManager;


		// クラスの定義 ==========================================================
		class CollisionShape
		{
			// <メンバ変数>
		protected:
			// 衝突判定マネージャー
			CollisionManager* m_collisionManager;
			// 制御コンポーネント
			System::ControllerBase* m_controller;
			// 座標変換
			Math::Transform& m_transform;
			// コライダータイプ
			ColliderType m_colliderType;
			// 物理演算
			Physics::RigidBody* m_rigidBody;


			// <コンストラクタ>
		public:
			//--------------------------------------------------------------
			//! @parameter [controller] 制御コンポーネント
			//! @parameter [transform] 座標変換コンポーネント
			//--------------------------------------------------------------
			CollisionShape(System::ControllerBase* controller, Math::Transform& transform);


			// <デストラクタ>
		public:
			virtual ~CollisionShape();


			// <メンバ関数>
		public:
			//--------------------------------------------------------------
			//! @summary   衝突しているときに呼ばれる処理
			//!
			//! @parameter [collider] 衝突しているコライダー
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Collide(CollisionShape* collider);


			//--------------------------------------------------------------
			//! @summary   衝突しているときに呼ばれる物理演算コンポーネントの処理
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void CollideRigidBody();


			// <セッター関数>
			//--------------------------------------------------------------
			//! @parameter [rigidBody] 物理演算コンポーネント
			//--------------------------------------------------------------
			inline void SetRigidBody(Physics::RigidBody* rigidBody) { m_rigidBody = rigidBody; }


			// <ゲッター関数>
		public:
			//--------------------------------------------------------------
			//! @summary   コライダータイプの取得
			//--------------------------------------------------------------
			inline const ColliderType& GetColliderType() const { return m_colliderType; }


			//--------------------------------------------------------------
			//! @summary   Transformの取得
			//--------------------------------------------------------------
			inline const Math::Transform& GetTransform() const { return m_transform; }


			//--------------------------------------------------------------
			//! @summary   ゲームオブジェクトの取得
			//--------------------------------------------------------------
			inline System::GameObject* GetGemaObject() const { return m_controller->GetGemaObject(); }
		};
	}
}
