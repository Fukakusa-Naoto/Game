//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Enemy.cpp
//!
//! @summary	モトスの敵クラスのソースファイル
//!
//! @date		2018.12.05
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../../pch.h"

// <自作ヘッダーファイル>
#include "Enemy.h"
#include "../../../../Library/Collision/CollisionManager.h"
#include "../../../Utility/ObjactTag.h"
#include "../../../../Library/Collision/SphereShape.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::Enemy::Enemy::Enemy() :
	GameObject()
{
	m_controller = new Controller::EnemyController(this, m_transform);
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::Enemy::Enemy::~Enemy()
{
	delete m_sphereShape;
	m_sphereShape = nullptr;

	delete m_rigidBody;
	m_rigidBody = nullptr;
}



//--------------------------------------------------------------------
//! @summary   初期化処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::Enemy::Enemy::Start()
{
	m_primitiveRender = Graphic3D::PrimitiveManager3D::GetInstance();

	m_sphereShape = new Collision::SphereShape(m_controller, m_transform, 0.5f);

	m_rigidBody = new Physics::RigidBody(m_transform, m_sphereShape);

	// 登録
	Collision::CollisionManager::GetInstance()->Entry(m_sphereShape, ObjectTag::ENEMY);

	// タグの登録
	m_tag = ObjectTag::ENEMY;

	dynamic_cast<Controller::EnemyController*>(m_controller)->SetEnemyManager(m_enemyManager);
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::Enemy::Enemy::Update(const Common::StepTimer& timer)
{
	m_controller->Update(timer);
	m_transform.Update();
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::Enemy::Enemy::Draw()
{
	m_primitiveRender->DrawSphere(m_transform.GetPosition(), 0.5f, Color(1.0f, 0.0f, 0.0f, 1.0f), m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
}
