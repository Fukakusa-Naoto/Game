//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		CollisionManager.cpp
//!
//! @summary	当たり判定のマネージャークラスのソースファイル
//!
//! @date		2018.12.04
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <自作ヘッダファイル>
#include "CollisionManager.h"
#include "Collision.h"
#include "CollisionShape.h"
#include "SphereShape.h"
#include "CubeShape.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Collision::CollisionManager::CollisionManager()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Collision::CollisionManager::~CollisionManager()
{
	// 何もしない
}



//--------------------------------------------------------------
//! @summary   初期化処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------
void Collision::CollisionManager::Initialize()
{
	m_collisionSetting[0] = true;
}



//--------------------------------------------------------------------
//! @summary   コライダーの登録処理
//!
//! @parameter [collider] 登録するコライダー
//! @parameter [group] 所属するグループ
//!
//! @return    なし
//--------------------------------------------------------------------
void Collision::CollisionManager::Entry(CollisionShape* collider, unsigned int group)
{
	// グループが存在する
	if (m_colliderList.count(group) != 0)
	{
		// すでに登録済みでないか調べる
		for (list<CollisionShape*>::iterator itr = m_colliderList[group].begin(); itr != m_colliderList[group].end(); ++itr)
		{
			if (collider == (*itr)) return;
		}
	}

	// 登録する
	m_colliderList[group].push_back(collider);
}



//--------------------------------------------------------------------
//! @summary   コライダーを外す
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Collision::CollisionManager::Remove(CollisionShape* collider)
{
	// コライダーを探す
	for (map<unsigned int, list<CollisionShape*>>::iterator itr = m_colliderList.begin(); itr != m_colliderList.end();)
	{
		if (itr->second.size() == 0) return;

		for (list<CollisionShape*>::iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
		{
			if ((*itr2) == collider)
			{
				itr->second.erase(itr2);
				break;
			}
		}

		// 空になったリストを削除する
		if (itr->second.size() == 0) itr = m_colliderList.erase(itr);
		else ++itr;
	}
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Collision::CollisionManager::Update()
{
	// リストにコライダーが登録されている
	if(m_colliderList.size() == 0) return;

	// 当たり判定を行うグループを決める
	for (map<unsigned int, list<CollisionShape*>>::iterator itr = m_colliderList.begin(); itr != m_colliderList.end(); ++itr)
	{
		for (map<unsigned int, list<CollisionShape*>>::iterator itr2 = m_colliderList.begin(); itr2 != m_colliderList.end(); ++itr2)
		{
			pair<unsigned int, list<CollisionShape*>> p = *itr;
			pair<unsigned int, list<CollisionShape*>> p2 = *itr2;

			// グループの判定
			if (m_collisionSetting[p.first | p2.first])
			{
				// グループ内の要素同士で総当たり
				for (list<CollisionShape*>::iterator itr3 = p.second.begin(); itr3 != p.second.end(); ++itr3)
				{
					for (list<CollisionShape*>::iterator itr4 = p2.second.begin(); itr4 != p2.second.end(); ++itr4)
					{
						// 自分自身とは判定を行わない
						if ((*itr3) != (*itr4))
						{
							// 当たり判定を行う
							if (CollisionDetection(**itr3, **itr4))
							{
								(*itr3)->Collide(*itr4);
								(*itr3)->CollideRigidBody();
							}
						}
					}
				}
			}
		}
	}
}



//--------------------------------------------------------------------
//! @summary   当たり判定の処理
//!
//! @parameter [colliderA] コライダーA
//! @parameter [colliderB] コライダーB
//!
//! @return    true :当たっている
//! @return    false:当たっていない
//--------------------------------------------------------------------
bool Collision::CollisionManager::CollisionDetection(CollisionShape& colliderA, CollisionShape& colliderB)
{
	switch (colliderA.GetColliderType() | colliderB.GetColliderType())
	{
#pragma region 2D当たり判定
	//case TYPE_BOX | TYPE_BOX:
	//	return IsCollideAABB(dynamic_cast<BoxCollider&>(colliderA), dynamic_cast<BoxCollider&>(colliderB));
	//	break;
	//case TYPE_CIRCLE|TYPE_CIRCLE:
	//	return IsCollideXYR(dynamic_cast<CircleCollider&>(colliderA), dynamic_cast<CircleCollider&>(colliderB));
	//	break;
	//case TYPE_BOX|TYPE_CIRCLE:

	//	break;
#pragma endregion
#pragma region 3D当たり判定
	case TYPE_SPHERE | TYPE_SPHERE:		// 球と球
		return IsCollideSphereToSphere(dynamic_cast<SphereShape&>(colliderA), dynamic_cast<SphereShape&>(colliderB));
		break;
	case TYPE_CUBE | TYPE_CUBE:			// 立方体と立方体
		break;
	case TYPE_CUBE | TYPE_SPHERE:		// 立方体と球
		if (colliderA.GetColliderType() == TYPE_CUBE) return IsCollideCubeToSphere(dynamic_cast<CubeShape&>(colliderA), dynamic_cast<SphereShape&>(colliderB));
		else return IsCollideCubeToSphere(dynamic_cast<CubeShape&>(colliderB), dynamic_cast<SphereShape&>(colliderA));
		break;
#pragma endregion
	default:
		return false;
		break;
	}

	return false;
}
