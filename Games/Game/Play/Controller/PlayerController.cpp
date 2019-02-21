//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		PlayerController.cpp
//!
//! @summary	モトスのプレイヤーコントローラークラスのソースファイル
//!
//! @date		2018.12.05
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../pch.h"

// <自作ヘッダーファイル>
#include "PlayerController.h"
#include "../../../Library/Input/KeyboardUtil.h"
#include "../../../Library/Collision/CollisionShape.h"
#include "../../../Library/System/GameObject.h"
#include "../../Utility/ObjactTag.h"
#include "../Object/Player.h"
#include "../../../Library/Sound/SoundManager.h"

// <リソースファイル>
#include "../../../Resources/Sounds/Play.h"


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
Motos::Play::Controller::PlayerController::PlayerController(System::GameObject* gameObject, Math::Transform& transform) :
	ControllerBase(gameObject, transform),
	m_velocity(Vector3::Zero),
	m_isActive(true)
{
	m_transform.SetPosition(Vector3(0.0f, 0.5f, 5.0f));
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Controller::PlayerController::~PlayerController()
{
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Controller::PlayerController::Update(const Common::StepTimer& timer)
{
	// 入力マネージャーのインスタンスの取得
	Input::KeyboardUtil* keyboard = Input::KeyboardUtil::GetInstance();

	if (m_isActive)
	{
		if (m_velocity == Vector3::Zero)
		{
			// 移動
			if (keyboard->IsPressed(Keyboard::Keys::Up))		// 上キー
			{
				m_velocity += Vector3(0.0f, 0.0f, -1.0f);
			}
			else if (keyboard->IsPressed(Keyboard::Keys::Down))	// 下キー
			{
				m_velocity += Vector3(0.0f, 0.0f, 1.0f);
			}
			if (keyboard->IsPressed(Keyboard::Keys::Right))		// 右キー
			{
				m_velocity += Vector3(1.0f, 0.0f, 0.0f);
			}
			else if (keyboard->IsPressed(Keyboard::Keys::Left))	// 左キー
			{
				m_velocity += Vector3(-1.0f, 0.0f, 0.0f);
			}

			m_velocity.Normalize();
			m_velocity *= 0.1f;
		}

		m_transform.Move(m_velocity);

		// 落下時の処理
		Falling();
	}


	//　速度のリセット
	m_velocity = Vector3::Zero;

	m_transform.Update();
}



//--------------------------------------------------------------------
//! @summary   衝突判定処理
//!
//! @parameter [collider] 衝突しているコライダー
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Controller::PlayerController::OnCollision(Library::Collision::CollisionShape* collider)
{
	// 敵との衝突判定
	if (collider->GetGemaObject()->GetTag() == ObjectTag::ENEMY)
	{
		Sound::SoundManager::GetInstance()->Play(CRI_PLAY_IMPACT);

		// <弾く>
		// 相手の方向を調べる
		Vector3 direction = collider->GetTransform().GetPosition() - m_transform.GetPosition();

		// めり込み防止
		direction.y = 0.0f;

		// 正規化
		direction.Normalize();

		// 方向を反転させる
		direction *= -1.0f;

		// 移動させる
		m_velocity = direction*0.5f;
	}
}



//--------------------------------------------------------------------
//! @summary   落下時の処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Controller::PlayerController::Falling()
{
	static bool isFalling = false;
	if ((!isFalling) && (m_transform.GetPosition().y <= -1.0))
	{
		isFalling = true;
		Sound::SoundManager::GetInstance()->Play(CRI_PLAY_FALLING);
	}
	if (m_transform.GetPosition().y <= -10.0) m_isActive = false;
}
