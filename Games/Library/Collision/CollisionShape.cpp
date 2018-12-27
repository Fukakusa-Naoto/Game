//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		CollisionShape.cpp
//!
//! @summary	衝突判定形状の基底クラスのソースファイル
//!
//! @date		2018.11.26
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダーファイル>
#include "CollisionShape.h"
#include "../Math/Transform.h"
#include "CollisionManager.h"
#include "Collision.h"
#include "../Physics/RigidBody.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [controller] 制御コンポーネント
//! @parameter [transform] 座標変換コンポーネント
//--------------------------------------------------------------------
Collision::CollisionShape::CollisionShape(System::ControllerBase* controller, Math::Transform& transform) :
	m_controller(controller),
	m_transform(transform),
	m_colliderType(ColliderType::TYPE_NONE),
	m_rigidBody(nullptr)
{
	m_collisionManager = Collision::CollisionManager::GetInstance();
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Collision::CollisionShape::~CollisionShape()
{
	m_collisionManager->Remove(this);
}



//--------------------------------------------------------------
//! @summary   衝突しているときに呼ばれる処理
//!
//! @parameter [collider] 衝突しているコライダー
//!
//! @return    なし
//--------------------------------------------------------------
void Collision::CollisionShape::Collide(CollisionShape* collider)
{
	m_controller->OnCollision(collider);
}



//--------------------------------------------------------------
//! @summary   衝突しているときに呼ばれる物理演算コンポーネントの処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------
void Collision::CollisionShape::CollideRigidBody()
{
	if(m_rigidBody) m_rigidBody->Collide();
}
