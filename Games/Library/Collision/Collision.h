//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Collision.h
//!
//! @summary	当たり判定関連のヘッダファイル
//!
//! @date		2018.07.18
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once


// ヘッダファイルの読み込み =======================================================
// <自作ヘッダファイル>
#include "../Utility/Flag.h"


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Collision
	{
		// 列挙型の定義 ==========================================================
		enum ColliderType
		{
			TYPE_NONE   = Utility::Flag::FLAG01,
			TYPE_CUBE   = Utility::Flag::FLAG02,
			TYPE_SPHERE = Utility::Flag::FLAG03,
		};


		// クラスの宣言 ==========================================================
		// 球
		class SphereShape;
		// 立方体
		class CubeShape;

		// 関数の宣言 ============================================================
#pragma region 2D当たり判定処理
		//------------------------------------------------------------------
		//! @summary   矩形同士の当たり判定
		//!
		//! @parameter [boxColliderA] 矩形コライダーA
		//! @parameter [boxColliderB] 矩形コライダーB
		//!
		//! @return    true :当たっている
		//! @return    false:当たっていない
		//------------------------------------------------------------------
		//bool IsCollideAABB(BoxCollider& boxColliderA, BoxCollider& boxColliderB);


		//------------------------------------------------------------------
		//! @summary   円同士の当たり判定
		//!
		//! @parameter [circleColliderA] 矩形コライダーA
		//! @parameter [circleColliderB] 矩形コライダーB
		//!
		//! @return    true :当たっている
		//! @return    false:当たっていない
		//------------------------------------------------------------------
		//bool IsCollideXYR(CircleCollider& circleColliderA, CircleCollider& circleColliderB);
#pragma endregion


#pragma region 3D当たり判定処理
		//------------------------------------------------------------------
		//! @summary   球同士の当たり判定
		//!
		//! @parameter [colliderA] コライダーA
		//! @parameter [colliderB] コライダーB
		//!
		//! @return    true :当たっている
		//! @return    false:当たっていない
		//------------------------------------------------------------------
		bool IsCollideSphereToSphere(SphereShape& colliderA, SphereShape& colliderB);


		//------------------------------------------------------------------
		//! @summary   立方体と球の当たり判定
		//!
		//! @parameter [cube] 立方体コライダー
		//! @parameter [sphere] 球コライダー
		//!
		//! @return    true :当たっている
		//! @return    false:当たっていない
		//------------------------------------------------------------------
		bool IsCollideCubeToSphere(CubeShape& cube, SphereShape& sphere);

#pragma endregion
	}
}