//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		RigidBody.cpp
//!
//! @summary	剛体クラスのソースファイル
//!
//! @date		2018.11.24
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダーファイル>
#include "RigidBody.h"
#include "PhysicsManager.h"
#include "../Math/Transform.h"
#include "../Collision/CollisionShape.h"
#include "../Common/StepTimer.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX::SimpleMath;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [transform] 座標変換コンポーネント
//! @parameter [collisionShape] 衝突判定用シェイプ
//--------------------------------------------------------------------
Physics::RigidBody::RigidBody(Math::Transform& transform, Collision::CollisionShape* collisionShape) :
	m_transform(transform),
	m_velocity(Vector3::Zero),
	m_isCollide(true),
	m_fallingTime(0.0f)
{
	// 物理マネージャーのインスタンスの取得
	m_physicsManager = PhysicsManager::GetInstance();

	// 物理マネージャーへの登録処理
	m_physicsManager->Entry(this);

	// コライダーに登録
	collisionShape->SetRigidBody(this);
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Physics::RigidBody::~RigidBody()
{
	// 物理エンジンマネージャーから外す
	m_physicsManager->Remove(this);
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Physics::RigidBody::Update(const Common::StepTimer& timer)
{
	if (!m_isCollide)
	{
		// 落下時間をカウント
		m_fallingTime += static_cast<float>(timer.GetElapsedSeconds());

		// 重力落下
		m_transform.Move(Vector3(0.0f, -1.225f*m_fallingTime, 0.0f));
	}

	// フラグを戻す
	m_isCollide = false;

}



//--------------------------------------------------------------------
//! @summary   衝突時に行われる処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Physics::RigidBody::Collide()
{
	// 落下時間のリセット
	m_fallingTime = 0.0f;

	m_isCollide = true;
}
