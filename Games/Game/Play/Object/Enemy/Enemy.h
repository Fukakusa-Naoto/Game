//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Enemy.h
//!
//! @summary	モトスの敵クラスのヘッダファイル
//!
//! @date		2018.12.05
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <自作ヘッダーファイル>
#include "../../../../Library/System/GameObject.h"
#include "../../../../Library/Graphic3D/PrimitiveManager3D.h"
#include "../../../../Library/Camera/Camera.h"
#include "../../../../Library/Collision/SphereShape.h"
#include "../../Controller/Enemy/EnemyController.h"
#include "../../../../Library/Physics/RigidBody.h"


// 名前空間の定義 ================================================================
namespace Motos
{
	// クラスの宣言 ==============================================================
	namespace Utility { class EnemyManager; }


	namespace Play
	{
		namespace Object
		{
			namespace Enemy
			{
				// クラスの定義 ==================================================
				class Enemy : public Library::System::GameObject
				{

					// <メンバ変数>
				private:
					// 3Dプリミティブの描画マネージャー
					Library::Graphic3D::PrimitiveManager3D* m_primitiveRender;
					// 衝突判定
					Library::Collision::SphereShape* m_sphereShape;
					// 物理演算コンポーネント
					Library::Physics::RigidBody* m_rigidBody;
					// 敵管理クラス
					Utility::EnemyManager* m_enemyManager;
					// デバイス・コンテキスト・インターフェイス
					ID3D11DeviceContext* m_deviceContext;
					// ピクセルシェーダー・インターフェイス
					ID3D11PixelShader* m_pixelShader;


					// <コンストラクタ>
				public:
					//------------------------------------------------------
					//! @summary   コンストラクタ
					//!
					//! @parameter [void] なし
					//------------------------------------------------------
					Enemy();


					// <デストラクタ>
				public:
					~Enemy();


					// <メンバ関数>
				public:
					//------------------------------------------------------
					//! @summary   初期化処理
					//!
					//! @parameter [void] なし
					//!
					//! @return    なし
					//------------------------------------------------------
					void Start() override;


					//------------------------------------------------------
					//! @summary   更新処理
					//!
					//! @parameter [timer] 時間情報
					//!
					//! @return    なし
					//------------------------------------------------------
					void Update(const Library::Common::StepTimer & timer) override;



					//------------------------------------------------------
					//! @summary   描画処理
					//!
					//! @parameter [void] なし
					//!
					//! @return    なし
					//------------------------------------------------------
					void Draw() override;


					// <セッター関数>
				public:
					//------------------------------------------------------
					//! @parameter [target] ターゲットの情報
					//------------------------------------------------------
					inline void SetTartet(Library::System::GameObject* target) { dynamic_cast<Controller::EnemyController*>(m_controller)->SetTarget(target); }


					//------------------------------------------------------
					//! @parameter [position] 座標
					//------------------------------------------------------
					inline void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_transform.SetPosition(position); }


					//------------------------------------------------------
					//! @parameter [enemyManager] 敵管理クラス
					//------------------------------------------------------
					inline void SetEnemyManager(Utility::EnemyManager* enemyManager) { m_enemyManager = enemyManager; }
				};
			}
		}
	}
}
