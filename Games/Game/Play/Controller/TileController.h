//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		TileController.h
//!
//! @summary	モトスのタイルコントローラークラスのヘッダファイル
//!
//! @date		2018.12.05
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <自作ヘッダーファイル>
#include "../../../Library/System/ControllerBase.h"


// 名前空間の定義 ================================================================
namespace Motos
{
	namespace Play
	{
		namespace Controller
		{
			// クラスの定義 ======================================================
			class TileController final : public Library::System::ControllerBase
			{
				// <メンバ変数>
			private:


				// <コンストラクタ>
			public:
				//----------------------------------------------------------
				//! @summary   コンストラクタ
				//!
				//! @parameter [gameObject] ゲームオブジェクト
				//! @parameter [transform] 座標変換コンポーネント
				//----------------------------------------------------------
				TileController(Library::System::GameObject* gameObject, Library::Math::Transform& transform);


				// <デストラクタ>
			public:
				~TileController();


				// <メンバ関数>
			public:
				//----------------------------------------------------------
				//! @summary   更新処理
				//!
				//! @parameter [timer] 時間情報
				//!
				//! @return    なし
				//----------------------------------------------------------
				void Update(const Library::Common::StepTimer & timer) override;


				//----------------------------------------------------------
				//! @summary   衝突判定処理
				//!
				//! @parameter [collider] 衝突しているオブジェクト
				//!
				//! @return    なし
				//----------------------------------------------------------
				void OnCollision(Library::Collision::CollisionShape* collider) override;
			};
		}
	}
}
