//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Transform.cpp
//!
//! @summary	オブジェクトの位置、回転、スケールを扱うクラスのソースファイル
//!
//! @date		2018.07.11
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダファイル>
#include "Transform.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 関数の定義 ==============================================================
//----------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//----------------------------------------------------------------------
Math::Transform::Transform() :
	m_world(Matrix::Identity),
	m_position(Vector3::Zero),
	m_eulerAngles(Vector3::Zero),
	m_rotation(Quaternion::Identity),
	m_scale(Vector3::One),
	m_parent(nullptr)
{
	// 何もしない
}



//----------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//----------------------------------------------------------------------
Math::Transform::~Transform()
{
	// 何もしない
}



//----------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//----------------------------------------------------------------------
void Math::Transform::Update()
{
	if (m_children.size() != 0)
	{
		for (list<Transform*>::iterator itr = m_children.begin(); itr != m_children.end(); ++itr) (*itr)->Update();
	}
}



//----------------------------------------------------------------------
//! @summary   移動処理
//!
//! @parameter [velocity] 速度
//!
//! @return    なし
//----------------------------------------------------------------------
void Math::Transform::Move(const Vector3& velocity)
{
	m_position += Vector3::Transform(velocity, m_rotation);
}



//----------------------------------------------------------------------
//! @summary   回転処理
//!
//! @parameter [rotation] 回転角
//!
//! @return    なし
//----------------------------------------------------------------------
void Math::Transform::Rotate(const Vector3& rotation)
{
	m_eulerAngles += rotation;
	m_rotation *= Quaternion::CreateFromYawPitchRoll(XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.z));
}

