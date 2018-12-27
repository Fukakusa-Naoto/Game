//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		ControllerBase.h
//!
//! @summary	コントローラーの基底クラスのヘッダファイル
//!
//! @date		2018.12.06
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <自作ヘッダーファイル>
#include "../Collision/CollisionInterface.h"
#include "../Math/Transform.h"


// 名前空間の定義 ================================================================
namespace Library
{
	// クラスの宣言 ==============================================================
	namespace Common { class StepTimer; }


	namespace System
	{
		// クラスの宣言 ==========================================================
		// ゲームオブジェクトの基底クラス
		class GameObject;


		// クラスの定義 ==========================================================
		class ControllerBase : public Collision::CollisionInterface
		{
			// <メンバ変数>
		protected:
			Math::Transform& m_transform;
			GameObject* m_gameObject;


			// <コンストラクタ>
		public:
			//--------------------------------------------------------------
			//! @summary   コンストラクタ
			//!
			//! @parameter [gameObject] ゲームオブジェクト
			//! @parameter [transform] 座標変換コンポーネント
			//--------------------------------------------------------------
			ControllerBase(GameObject* gameObject, Library::Math::Transform& transform) :
				m_gameObject(gameObject),
				m_transform(transform)
			{
				// 何もしない
			};


			// <デストラクタ>
		public:
			virtual ~ControllerBase() {};


			// <メンバ関数>
		public:
			//-------------------------------------------------------------------
			//! @summary   更新処理
			//!
			//! @parameter [timer] 時間情報
			//!
			//! @return    なし
			//--------------------------------------------------------------
			virtual void Update(const Library::Common::StepTimer & timer) = 0;


			// <ゲッター関数>
		public:
			//--------------------------------------------------------------
			//! @summary   ゲームオブジェクトの取得
			//--------------------------------------------------------------
			inline GameObject* GetGemaObject() const { return m_gameObject; }
		};
	}
}

