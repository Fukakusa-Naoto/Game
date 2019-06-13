//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Player.h
//!
//! @summary	モトスのプレイヤークラスのヘッダファイル
//!
//! @date		2018.11.16
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <自作ヘッダーファイル>
#include "../../../Library/System/GameObject.h"
#include "../../../Library/Graphic3D/PrimitiveManager3D.h"
#include "../../../Library/Collision/SphereShape.h"
#include "../../../Library//Physics/RigidBody.h"
#include "../Controller/PlayerController.h"


// 名前空間の定義 ================================================================
namespace Motos
{
	namespace Play
	{
		namespace Object
		{
			// クラスの定義 ======================================================
			class Player : public Library::System::GameObject
			{
				// <メンバ変数>
			private:
				// 3Dプリミティブの描画マネージャー
				Library::Graphic3D::PrimitiveManager3D* m_primitiveRender;
				// 衝突判定
				Library::Collision::SphereShape* m_sphereShape;
				// 物理演算コンポーネント
				Library::Physics::RigidBody* m_rigidBody;
				// デバイス・コンテキスト・インターフェイス
				ID3D11DeviceContext* m_deviceContext;
				// ピクセルシェーダー・インターフェイス
				ID3D11PixelShader* m_pixelShader;


				// <コンストラクタ>
			public:
				//----------------------------------------------------------
				//! @summary   コンストラクタ
				//!
				//! @parameter [void] なし
				//----------------------------------------------------------
				Player();


				// <デストラクタ>
			public:
				~Player();


				// <メンバ関数>
			public:
				//----------------------------------------------------------
				//! @summary   初期化処理
				//!
				//! @parameter [void] なし
				//!
				//! @return    なし
				//----------------------------------------------------------
				void Start() override;


				//----------------------------------------------------------
				//! @summary   更新処理
				//!
				//! @parameter [timer] 時間情報
				//!
				//! @return    なし
				//----------------------------------------------------------
				void Update(const Library::Common::StepTimer & timer) override;



				//----------------------------------------------------------
				//! @summary   描画処理
				//!
				//! @parameter [void] なし
				//!
				//! @return    なし
				//----------------------------------------------------------
				void Draw() override;


				// <ゲッター関数>
			public:
				//----------------------------------------------------------
				//! @summary   物理演算コンポーネントの取得
				//----------------------------------------------------------
				inline Library::Physics::RigidBody* GetRigidBody() const { return m_rigidBody; }


				//----------------------------------------------------------
				//! @summary   生存フラグの取得
				//----------------------------------------------------------
				inline bool GetActive() const { return dynamic_cast<Controller::PlayerController*>(m_controller)->GetActive(); }
			};
		}
	}
}
