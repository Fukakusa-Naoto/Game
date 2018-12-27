//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		CollisionManager.h
//!
//! @summary	当たり判定のマネージャークラスのヘッダファイル
//!
//! @date		2018.12.04
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once


// ヘッダファイルの読み込み =======================================================
// <標準ヘッダファイル>
#include <list>
#include <map>

// <自作ヘッダファイル>
#include "../System/SingletonBase.h"


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Collision
	{
		// クラスの宣言 ==========================================================
		class CollisionShape;


		// クラスの定義 ==========================================================
		class CollisionManager : public System::SingletonBase<CollisionManager>
		{
			// <フレンド登録>
			friend class System::SingletonBase<CollisionManager>;


			// <メンバ変数>
			std::map<unsigned int, std::list<CollisionShape*>> m_colliderList;
			std::map<unsigned int, bool> m_collisionSetting;


			// <コンストラクタ>
		private:
			//--------------------------------------------------------------
			//! @parameter [void] なし
			//--------------------------------------------------------------
			CollisionManager();


			// <デストラクタ>
		public:
			~CollisionManager();


			// <メンバ関数>
		public:
			//--------------------------------------------------------------
			//! @summary   初期化処理
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Initialize();


			//--------------------------------------------------------------
			//! @summary   更新処理
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Update();


			//--------------------------------------------------------------
			//! @summary   コライダーの登録処理
			//!
			//! @parameter [collider] 登録するコライダー
			//! @parameter [group] 所属するグループ
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Entry(CollisionShape* collider, unsigned int group = 0);


			//--------------------------------------------------------------
			//! @summary   コライダーを外す
			//!
			//! @parameter [collider] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Remove(CollisionShape* collider);


		private:
			//--------------------------------------------------------------
			//! @summary   当たり判定の処理
			//!
			//! @parameter [colliderA] コライダーA
			//! @parameter [colliderB] コライダーB
			//!
			//! @return    true :当たっている
			//! @return    false:当たっていない
			//--------------------------------------------------------------
			bool CollisionDetection(CollisionShape& colliderA, CollisionShape& colliderB);


			// <セッター関数>
		public:
			//--------------------------------------------------------------
			//! @parameter [group] グループ
			//! @parameter [flag] 当たり判定を行うか否か
			//--------------------------------------------------------------
			inline void SetCollisionGroup(unsigned int group, bool flag) { m_collisionSetting[group] = flag; }

		};
	}
}