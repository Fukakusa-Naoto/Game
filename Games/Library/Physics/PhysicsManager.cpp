//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		PhysicsManager.cpp
//!
//! @summary	物理マネージャークラスのソースファイル
//!
//! @date		2018.11.24
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダーファイル>
#include "PhysicsManager.h"
#include "RigidBody.h"
#include "../Common/StepTimer.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Physics::PhysicsManager::PhysicsManager()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Physics::PhysicsManager::~PhysicsManager()
{
}



//--------------------------------------------------------------------
//! @summary   初期化処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Physics::PhysicsManager::Intialize()
{
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Physics::PhysicsManager::Update(const Common::StepTimer& timer)
{
	for (list<RigidBody*>::iterator itr = m_rigidBodyList.begin(); itr != m_rigidBodyList.end(); ++itr)
	{
		(*itr)->Update(timer);
	}
}



//--------------------------------------------------------------------
//! @summary   剛体の登録処理
//!
//! @parameter [rigidbody] 登録する剛体
//!
//! @return    なし
//--------------------------------------------------------------------
void Physics::PhysicsManager::Entry(RigidBody* rigidBody)
{
	// 同じものがないか調べる
	list<RigidBody*>::iterator itr = find(m_rigidBodyList.begin(), m_rigidBodyList.end(), rigidBody);
	unsigned int index = distance(m_rigidBodyList.begin(), itr);
	if (index == m_rigidBodyList.size()) m_rigidBodyList.push_back(rigidBody);
}



//--------------------------------------------------------------------
//! @summary   登録された剛体を処理から外す処理
//!
//! @parameter [rigidbody] はずす剛体
//!
//! @return    なし
//--------------------------------------------------------------------
void Physics::PhysicsManager::Remove(RigidBody* rigidBody)
{
	for (list<RigidBody*>::iterator itr = m_rigidBodyList.begin(); itr != m_rigidBodyList.end(); ++itr)
	{
		if ((*itr) == rigidBody)
		{
			m_rigidBodyList.erase(itr);
			break;
		}
	}
}
