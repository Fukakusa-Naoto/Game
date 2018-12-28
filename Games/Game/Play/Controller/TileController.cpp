//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		TileController.cpp
//!
//! @summary	モトスのタイルコントローラークラスのソースファイル
//!
//! @date		2018.12.05
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../pch.h"

// <自作ヘッダーファイル>
#include "TileController.h"
#include "../../../Library/System/GameObject.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [gameObject] ゲームオブジェクト
//! @parameter [transform] 座標変換コンポーネント
//--------------------------------------------------------------------
Motos::Play::Controller::TileController::TileController(System::GameObject* gameObject, Math::Transform& transform) :
	ControllerBase(gameObject, transform)
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Controller::TileController::~TileController()
{
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Controller::TileController::Update(const Common::StepTimer& timer)
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   衝突判定処理
//!
//! @parameter [collider] 衝突しているコライダー
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Controller::TileController::OnCollision(Library::Collision::CollisionShape* collider)
{
}
