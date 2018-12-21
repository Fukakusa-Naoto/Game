//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		DebugCamera.cpp
//!
//! @summary	デバッグ用カメラクラスのソースファイル
//!
//! @date		2018.10.11
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <標準ヘッダファイル>
#include <algorithm>

// <自作ヘッダファイル>
#include "DebugCamera.h"
#include "../Input/MouseUtil.h"


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
Debug::DebugCamera::DebugCamera() :
	Camera::Camera()
{
	m_startMousePosition = m_nowMousePosition = Vector2::Zero;
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Debug::DebugCamera::Update()
{
	Input::MouseUtil* mouse = Input::MouseUtil::GetInstance();

	// 相対モードなら何もしない
	if (mouse->GetMode() == Mouse::MODE_RELATIVE) return;


	// マウスが押された瞬間
	if ((mouse->IsTriggered(Input::MouseUtil::Button::Right)) ||
		(mouse->IsTriggered(Input::MouseUtil::Button::Middle)) ||
		(mouse->IsTriggered(Input::MouseUtil::Button::Left)))
	{
		// マウス座標の保存
		m_startMousePosition = Vector2(static_cast<float>(mouse->GetX()), static_cast<float>(mouse->GetY()));
	}


	// <前進・後退>
	if (mouse->GetWheelValue() > 0)
	{
		m_eyeTransform.Move(Vector3(0.0f, 0.0f, -1.0f));

		mouse->ResetWheelValue();
	}
	else if (mouse->GetWheelValue() < 0)
	{
		m_eyeTransform.Move(Vector3(0.0f, 0.0f, 1.0f));

		mouse->ResetWheelValue();
	}


	// <平行移動>
	// マウスが押されている時、マウス座標を更新する
	if (mouse->IsPressed(Input::MouseUtil::Button::Middle))
	{
		m_nowMousePosition = Vector2(static_cast<float>(mouse->GetX()), static_cast<float>(mouse->GetY()));

		// マウスの移動量の計算
		Vector3 shift = Vector3(m_nowMousePosition.x - m_startMousePosition.x, m_nowMousePosition.y - m_startMousePosition.y, 0.0f);
		shift.x *= -1.0f;
		// 移動
		m_eyeTransform.Move(shift * 0.01f);
	}



	// <カメラの回転>
	if (mouse->IsPressed(Input::MouseUtil::Button::Right))
	{
		m_nowMousePosition = Vector2(static_cast<float>(mouse->GetX()), static_cast<float>(mouse->GetY()));

		// カメラを回転させる
		Vector3 difference = Vector3(m_nowMousePosition.x - m_startMousePosition.x, m_nowMousePosition.y - m_startMousePosition.y, 0.0f);

		m_eyeTransform.SetEulerAngles(m_eyeTransform.GetEulerAngles() + Vector3(-difference.y * 0.1f, -difference.x * 0.1f, 0.0f));
	}


	// マウスが離された時
	if ((mouse->IsReleased(Input::MouseUtil::Button::Right)) ||
		(mouse->IsReleased(Input::MouseUtil::Button::Middle)) ||
		(mouse->IsReleased(Input::MouseUtil::Button::Left)))
	{
		// 値をリセットする
		m_startMousePosition = m_nowMousePosition = Vector2::Zero;
	}


	// マウス座標の更新
	m_startMousePosition = m_nowMousePosition;

	// 座標の更新
	m_eyeTransform.Update();

	// 視点と注視点が同じときメッセージボックスで知らせる
	if (m_eyeTransform.GetPosition() == m_targetTransform.GetWorld().Translation())
		MessageBox(NULL, TEXT("視点と注視点が同じです"), TEXT("エラー"), MB_OK);

	// 射影行列の作成
	m_projection = Matrix::CreatePerspectiveFieldOfView(m_viewingAngle, m_aspect, m_nearClip, m_farClip);

	// ビュー行列の作成
	m_view = Matrix::CreateLookAt(m_eyeTransform.GetWorld().Translation(), m_targetTransform.GetWorld().Translation(), m_upVector);
}
