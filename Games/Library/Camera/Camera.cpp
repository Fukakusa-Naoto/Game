//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Camera.cpp
//!
//! @summary	カメラクラスのソースファイル
//!
//! @date		2018.06.04
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <標準ヘッダファイル>
#include <algorithm>

// <自作ヘッダファイル>
#include "Camera.h"
#include "../Math/Math.h"
#include "../Common/DeviceResources.h"


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
Camera::Camera::Camera() :
	m_view(Matrix::Identity),
	m_upVector(Vector3::UnitY),
	m_projection(Matrix::Identity),
	m_viewingAngle(XM_PI / 4.0f),
	m_nearClip(0.01f),
	m_farClip(100.0f)
{
	// 画面サイズの作成
	RECT screenRect = Common::DeviceResources::GetInstance()->GetOutputSize();
	int screenWidth = screenRect.right - screenRect.left;
	int screenHeight = screenRect.bottom - screenRect.top;

	// アスペクト比の算出
	m_aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

	m_targetTransform.SetPosition(Vector3(0.0f, 0.0f, -10.0f));
	m_eyeTransform.SetChild(&m_targetTransform);
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Camera::Camera::Update()
{
	m_eyeTransform.Update();

	// 視点と注視点が同じときメッセージボックスで知らせる
	if (m_eyeTransform.GetPosition() == m_targetTransform.GetWorld().Translation())
		MessageBox(NULL, TEXT("視点と注視点が同じです"), TEXT("エラー"), MB_OK);

	// 射影行列の作成
	m_projection = Matrix::CreatePerspectiveFieldOfView(m_viewingAngle, m_aspect, m_nearClip, m_farClip);

	// ビュー行列の作成
	m_view = Matrix::CreateLookAt(m_eyeTransform.GetWorld().Translation(), m_targetTransform.GetWorld().Translation(), m_upVector);
}



//--------------------------------------------------------------------
//! @summary   指定した座標を見る
//!
//! @parameter [target] ターゲットの座標
//!
//! @return    なし
//--------------------------------------------------------------------
void Camera::Camera::LookAt(const Vector3& target)
{
	// ターゲットまでの距離を作る
	Vector3 direction = target - m_eyeTransform.GetWorld().Translation();

	// ターゲットのTransformのローカル座標を設定する
	m_targetTransform.SetPosition(Vector3(0.0f, 0.0f, direction.Length()));


	// 3軸の各要素を元に行列を作る
	direction.Normalize();
	Vector3 x = Math::Cross(Vector3::Up, direction);
	x.Normalize();
	Vector3 y = Math::Cross(direction, x);
	y.Normalize();

	// 回転行列の作成
	Matrix m = Matrix::Identity;
	m._11 = x.x; m._12 = y.x; m._13 = direction.x;
	m._21 = x.y; m._22 = y.y; m._23 = direction.y;
	m._31 = x.z; m._32 = y.z; m._33 = direction.z;

	// 回転行列からクォータニオンの作成
	m_eyeTransform.SetRotation(Math::CreateQuaternion(m));
}
