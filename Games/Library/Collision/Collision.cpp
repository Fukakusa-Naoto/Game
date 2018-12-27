//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Collision.cpp
//!
//! @summary	当当たり判定関連のソースファイル
//!
//! @date		2018.07.18
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダファイル>
#include "Collision.h"
#include "SphereShape.h"
#include "CubeShape.h"
#include "../Math/Transform.h"


// usingディレクティブ =====================================================
using namespace DirectX::SimpleMath;
using namespace Library;


// 関数の定義 ==============================================================
#if 0
#pragma region 2D当たり判定処理
//--------------------------------------------------------------------
//! @summary   矩形同士の当たり判定
//!
//! @parameter [boxColliderA] 矩形コライダーA
//! @parameter [boxColliderB] 矩形コライダーB
//!
//! @return    true :当たっている
//! @return    false:当たっていない
//--------------------------------------------------------------------
bool Collision::IsCollideAABB(BoxCollider& boxColliderA, BoxCollider& boxColliderB)
{
	float topA = boxColliderA.GetPosition().y - boxColliderA.GetHeight() / 2;
	float bottomA = boxColliderA.GetPosition().y + boxColliderA.GetHeight() / 2;
	float leftA = boxColliderA.GetPosition().x - boxColliderA.GetWidth() / 2;
	float rightA = boxColliderA.GetPosition().x + boxColliderA.GetWidth() / 2;

	float topB = boxColliderB.GetPosition().y - boxColliderB.GetHeight() / 2;
	float bottomB = boxColliderB.GetPosition().y + boxColliderB.GetHeight() / 2;
	float leftB = boxColliderB.GetPosition().x - boxColliderB.GetWidth() / 2;
	float rightB = boxColliderB.GetPosition().x + boxColliderB.GetWidth() / 2;

	if ((topA <= bottomB) && (bottomA >= topB) && (leftA <= rightB) && (rightA >= leftB)) return true;
	else return false;
}



//--------------------------------------------------------------------
//! @summary   円同士の当たり判定
//!
//! @parameter [circleColliderA] 矩形コライダーA
//! @parameter [circleColliderB] 矩形コライダーB
//!
//! @return    true :当たっている
//! @return    false:当たっていない
//--------------------------------------------------------------------
bool Collision::IsCollideXYR(CircleCollider& circleColliderA, CircleCollider& circleColliderB)
{
	Vector2 positionA = Vector2(circleColliderA.GetPosition().x, circleColliderA.GetPosition().y);
	Vector2 positionB = Vector2(circleColliderB.GetPosition().x, circleColliderB.GetPosition().y);
	float distance = Vector2::Distance(positionA, positionB);
	float r2 = (circleColliderA.SetRadius() + circleColliderB.SetRadius()) + (circleColliderA.SetRadius() + circleColliderB.SetRadius());

	return (distance >= r2) ? false : true;
}
#endif
#pragma endregion



#pragma region 3D当たり判定処理
//--------------------------------------------------------------------
//! @summary   球同士の当たり判定
//!
//! @parameter [sphereColliderA] 球型コライダーA
//! @parameter [sphereColliderB] 球型コライダーB
//!
//! @return    true :当たっている
//! @return    false:当たっていない
//--------------------------------------------------------------------
bool Collision::IsCollideSphereToSphere(Collision::SphereShape& sphereColliderA, Collision::SphereShape& sphereColliderB)
{
	// 中心間の距離の平方を計算
	Vector3 distance = sphereColliderA.GetTransform().GetPosition() - sphereColliderB.GetTransform().GetPosition();
	// 平方した距離が平方した半径の合計よりも小さい場合に球は交差している
	float range = sphereColliderA.GetRadius() + sphereColliderB.GetRadius();
	return (distance.Dot(distance) <= range*range) ? true : false;
}



//--------------------------------------------------------------------
//! @summary   立方体と球の当たり判定
//!
//! @parameter [cube] 立方体コライダー
//! @parameter [sphere] 球コライダー
//!
//! @return    true :当たっている
//! @return    false:当たっていない
//--------------------------------------------------------------------
bool Collision::IsCollideCubeToSphere(Collision::CubeShape& cube, Collision::SphereShape& sphere)
{
	// 最短距離を求める
	Math::Box3D box(cube.GetTransform().GetPosition(), cube.GetSize());
	float shortestRange = Math::CalculateShortestRangePointToBox(sphere.GetTransform().GetPosition(), box);
	return shortestRange <= sphere.GetRadius() * sphere.GetRadius();
}

#pragma endregion
