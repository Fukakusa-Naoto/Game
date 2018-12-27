//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		CubeShape.cpp
//!
//! @summary	立方体状衝突判定クラスのソースファイル
//!
//! @date		2018.11.26
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダーファイル>
#include "CubeShape.h"
#include "Collision.h"
#include "../Math/Transform.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX::SimpleMath;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [controller] 制御コンポーネント
//! @parameter [transform] 座標変換コンポーネント
//! @parameter [size] 立方体のサイズ
//--------------------------------------------------------------------
Collision::CubeShape::CubeShape(System::ControllerBase* controller, Math::Transform& transform, const Vector3& size) :
	CollisionShape(controller, transform),
	m_size(size)
{
	m_colliderType = ColliderType::TYPE_CUBE;
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Collision::CubeShape::~CubeShape()
{
	// 何もしない
}
