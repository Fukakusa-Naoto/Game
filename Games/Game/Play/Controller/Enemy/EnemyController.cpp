//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		EnemyController.cpp
//!
//! @summary	モトスの敵コントローラークラスのソースファイル
//!
//! @date		2018.12.05
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../../pch.h"

// <自作ヘッダーファイル>
#include "EnemyController.h"
#include "../../../../Library/System/GameObject.h"
#include "../../../../Library/Collision/CollisionShape.h"
#include "../../../Utility/ObjactTag.h"
#include "../../../Utility/EnemyManager.h"
#include "../../../Utility/ScoreManager.h"
#include "../../../../Library/Sound/SoundManager.h"

// <リソースファイル>
#include "../../../../Resources/Sounds/Play.h"



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
Motos::Play::Controller::EnemyController::EnemyController(System::GameObject* gameObject, Math::Transform& transform) :
	ControllerBase(gameObject, transform),
	m_velocity(Vector3::Zero),
	m_isActive(true),
	m_isFalling(false)
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Controller::EnemyController::~EnemyController()
{
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Controller::EnemyController::Update(const Common::StepTimer& timer)
{
	if (m_isActive)
	{
		if (m_velocity == Vector3::Zero)
		{
			// ターゲットの方向を求める
			Vector3 direction = m_target->GetTransform().GetPosition() - m_transform.GetPosition();

			// 下にめり込まないように制限
			direction.y = 0.0f;

			direction.Normalize();
			m_velocity = direction*0.05f;
		}

		m_transform.Move(m_velocity);

		// 落下時の処理
		Falling();
	}

	// 速度のリセット
	m_velocity = Vector3::Zero;
}



//--------------------------------------------------------------------
//! @summary   衝突判定処理
//!
//! @parameter [collider] 衝突しているコライダー
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Controller::EnemyController::OnCollision(Library::Collision::CollisionShape* collider)
{
	// 相手の方向を調べる
	Vector3 direction = collider->GetTransform().GetPosition() - m_transform.GetPosition();

	// めり込み防止
	direction.y = 0.0f;

	// 正規化
	direction.Normalize();

	// 方向を反転させる
	direction *= -1.0f;

	switch (collider->GetGemaObject()->GetTag())
	{
	case ObjectTag::PLAYER:		// プレイヤー
		Sound::SoundManager::GetInstance()->Play(CRI_PLAY_IMPACT);
		m_velocity = direction*2.5f;
		break;
	case ObjectTag::ENEMY:		// 敵
		Sound::SoundManager::GetInstance()->Play(CRI_PLAY_IMPACT);
		m_velocity = direction*1.0f;
		break;
	default:
		break;
	}
}



//--------------------------------------------------------------------
//! @summary   落下時の処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Controller::EnemyController::Falling()
{
	if ((!m_isFalling) && (m_transform.GetPosition().y <= -1.0))
	{
		m_isFalling = true;
		Sound::SoundManager::GetInstance()->Play(CRI_PLAY_FALLING);
	}

	if (m_transform.GetPosition().y <= -10.0)
	{
		m_isActive = false;

		// 敵のカウントを減らす
		m_enemyManager->SubCount();

		// 得点を増やす
		Utility::ScoreManager::GetInstance()->AddScore(100);
	}
}
