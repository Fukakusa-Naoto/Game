//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		SphereShape.cpp
//!
//! @summary	球状衝突判定クラスのソースファイル
//!
//! @date		2018.11.26
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダーファイル>
#include "SphereShape.h"
#include "../Math/Transform.h"
#include "Collision.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [controller] 制御コンポーネント
//! @parameter [transform] 座標変換コンポーネント
//! @parameter [radius] 半径
//--------------------------------------------------------------------
Collision::SphereShape::SphereShape(System::ControllerBase* controller, Math::Transform& transform, float radius) :
	CollisionShape(controller, transform),
	m_radius(radius)
{
	m_colliderType = ColliderType::TYPE_SPHERE;
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Collision::SphereShape::~SphereShape()
{
	// 何もしない
}
