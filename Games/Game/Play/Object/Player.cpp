//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Player.cpp
//!
//! @summary	モトスのプレイヤークラスのソースファイル
//!
//! @date		2018.11.16
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../pch.h"

// <自作ヘッダーファイル>
#include "Player.h"
#include "../../../Library/Collision/CollisionManager.h"
#include "../../Utility/ObjactTag.h"
#include "../../../Library/Camera/Camera.h"
#include "../../../Library/Common/DeviceResources.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::Player::Player() :
	GameObject(),
	m_primitiveRender(nullptr),
	m_sphereShape(nullptr),
	m_rigidBody(nullptr),
	m_deviceContext(nullptr),
	m_pixelShader(nullptr)
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::Player::~Player()
{
	delete m_sphereShape;
	m_sphereShape = nullptr;

	delete m_rigidBody;
	m_rigidBody = nullptr;
}



//--------------------------------------------------------------------
//! @summary   初期化処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::Player::Start()
{
	m_deviceContext = Common::DeviceResources::GetInstance()->GetD3DDeviceContext();
	m_primitiveRender = Graphic3D::PrimitiveManager3D::GetInstance();

	m_controller = new Controller::PlayerController(this, m_transform);

	m_sphereShape = new Collision::SphereShape(m_controller, m_transform, 0.5f);

	m_rigidBody = new Physics::RigidBody(m_transform, m_sphereShape);

	// 登録
	Collision::CollisionManager::GetInstance()->Entry(m_sphereShape, ObjectTag::PLAYER);

	// タグの登録
	m_tag = ObjectTag::PLAYER;

	// ピクセルシェーダーのインスタンスの取得
	m_pixelShader = Shader::ShaderManager::GetInstance()->LoadPixelShader(L"LightingPixelShader");
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::Player::Update(const Common::StepTimer& timer)
{
	m_controller->Update(timer);
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::Player::Draw()
{
	// パイプライン構築のラムダ式関数
	function<void()> lambda = [&]()
	{
		// ピクセルシェーダー
		m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);
	};

	m_primitiveRender->DrawSphere(m_transform.GetPosition(), 0.5f, Color(0.0f, 0.0f, 1.0f, 1.0f), m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix(), lambda);
}
