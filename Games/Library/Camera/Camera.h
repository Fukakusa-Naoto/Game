//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Camera.h
//!
//! @summary	カメラクラスのヘッダファイル
//!
//! @date		2018.10.11
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once


// ヘッダファイルの読み込み =======================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダファイル>
#include "../Math/Transform.h"


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Camera
	{
		// クラスの定義 ==========================================================
		class Camera
		{
			// <メンバ関数>
		protected:
			// 視点
			Math::Transform m_eyeTransform;
			// 注視点
			Math::Transform m_targetTransform;
			// ビュー行列
			DirectX::SimpleMath::Matrix m_view;
			// 上方向ベクトル
			DirectX::SimpleMath::Vector3 m_upVector;
			// 射影行列
			DirectX::SimpleMath::Matrix m_projection;
			// 視野角
			float m_viewingAngle;
			// アスペクト比
			float m_aspect;
			// ニアクリップ
			float m_nearClip;
			// ファークリップ
			float m_farClip;
			// デバイス・コンテキスト・インターフェイス
			ID3D11DeviceContext1* m_deviceContext;
			// ビューポート
			D3D11_VIEWPORT m_viewport;


			// <コンストラクタ>
		public:
			//--------------------------------------------------------------
			//! @parameter [void] なし
			//--------------------------------------------------------------
			Camera();


			// <メンバ関数>
		public:
			//--------------------------------------------------------------
			//! @summary   更新処理
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			virtual void Update();


			//--------------------------------------------------------------
			//! @summary   指定した座標を見る
			//!
			//! @parameter [target] ターゲットの座標
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void LookAt(const DirectX::SimpleMath::Vector3& target);


			//--------------------------------------------------------------
			//! @summary   カメラに設定されたビューポートでの描画を開始(End関数が呼び出されるまで)
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Begin();


			//--------------------------------------------------------------
			//! @summary   カメラに設定されたビューポートでの描画を終了
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void End();


			// <セッター関数>
		public:
			//--------------------------------------------------------------
			//! @parameter [parent] 親のTransform
			//--------------------------------------------------------------
			virtual inline void SetParent(Math::Transform* parent) { m_eyeTransform.SetParent(parent); }


			//--------------------------------------------------------------
			//! @parameter [position] 視点の座標
			//--------------------------------------------------------------
			virtual inline void SetEyePosition(const DirectX::SimpleMath::Vector3& position) { m_eyeTransform.SetPosition(position); }


			//--------------------------------------------------------------
			//! @parameter [eulerAngles] オイラー角
			//--------------------------------------------------------------
			virtual inline void SetEulerAngles(const DirectX::SimpleMath::Vector3& eulerAngles)
			{
				m_eyeTransform.SetEulerAngles(eulerAngles);
				m_eyeTransform.Update();
			}


			//--------------------------------------------------------------
			//! @parameter [rotation] 回転角
			//--------------------------------------------------------------
			virtual inline void SetRotation(const DirectX::SimpleMath::Quaternion& rotation) { m_eyeTransform.SetRotation(rotation); }


			//--------------------------------------------------------------
			//! @parameter [aspect] アスペクト比
			//--------------------------------------------------------------
			inline void SetAspect(float aspect) { m_aspect = aspect; }


			//----------------------------------------------------------
			//! @parameter [viewport] ビューポートの設定
			//----------------------------------------------------------
			inline void SetViewport(const D3D11_VIEWPORT& viewport) { m_viewport = viewport; }


			//----------------------------------------------------------
			//! @parameter [topLeftX] 左上のX座標
			//! @parameter [topLeftY] 左上のY座標
			//----------------------------------------------------------
			inline void SetViewportPosition(float topLeftX, float topLeftY)
			{
				m_viewport.TopLeftX = topLeftX;
				m_viewport.TopLeftY = topLeftY;
			}


			//----------------------------------------------------------
			//! @parameter [width] 幅
			//! @parameter [height] 高さ
			//----------------------------------------------------------
			inline void SetViewportSize(float width, float height)
			{
				m_viewport.Width = width;
				m_viewport.Height = height;
			}


			//----------------------------------------------------------
			//! @parameter [min] 最小深度
			//! @parameter [max] 最大深度
			//----------------------------------------------------------
			inline void SetViewportDepth(float min, float max)
			{
				m_viewport.MinDepth = min;
				m_viewport.MaxDepth = max;
			}


			// <ゲッター関数>
		public:
			//--------------------------------------------------------------
			//! @summary   ビュー行列の取得
			//--------------------------------------------------------------
			inline const DirectX::SimpleMath::Matrix& GetViewMatrix() const { return m_view; }


			//--------------------------------------------------------------
			//! @summary   視点の取得
			//--------------------------------------------------------------
			inline DirectX::SimpleMath::Vector3 GetEyePosition() { return m_eyeTransform.GetWorld().Translation(); }


			//--------------------------------------------------------------
			//! @summary   ターゲット座標の取得
			//--------------------------------------------------------------
			inline const DirectX::SimpleMath::Vector3& GetTargetPosition() { return m_targetTransform.GetWorld().Translation(); }


			//--------------------------------------------------------------
			//! @summary   オイラー角の取得
			//--------------------------------------------------------------
			inline const DirectX::SimpleMath::Vector3& GetEulerAngles() const { return m_eyeTransform.GetEulerAngles(); }


			//--------------------------------------------------------------
			//! @summary   回転角の取得
			//--------------------------------------------------------------
			inline const DirectX::SimpleMath::Quaternion& GetRotation() const { return m_eyeTransform.GetRotation(); }


			//--------------------------------------------------------------
			//! @summary   射影行列の取得
			//--------------------------------------------------------------
			inline const DirectX::SimpleMath::Matrix& GetProjectionMatrix()const { return m_projection; }


			//--------------------------------------------------------------
			//! @summary   ビルボード行列の取得
			//--------------------------------------------------------------
			inline DirectX::SimpleMath::Matrix GetBillboard(const DirectX::SimpleMath::Vector3& position) const { return DirectX::SimpleMath::Matrix::CreateBillboard(position, m_eyeTransform.GetPosition(), DirectX::SimpleMath::Vector3::Up); }


			//--------------------------------------------------------------
			//! @summary   X軸ビルボード行列の取得
			//--------------------------------------------------------------
			inline DirectX::SimpleMath::Matrix GetBillboardConstrainX(const DirectX::SimpleMath::Vector3& position) const { return DirectX::SimpleMath::Matrix::CreateConstrainedBillboard(position, m_eyeTransform.GetPosition(), DirectX::SimpleMath::Vector3::UnitX); }


			//--------------------------------------------------------------
			//! @summary   Y軸ビルボード行列の取得
			//--------------------------------------------------------------
			inline DirectX::SimpleMath::Matrix GetBillboardConstrainY(const DirectX::SimpleMath::Vector3& position) const { return DirectX::SimpleMath::Matrix::CreateConstrainedBillboard(position, m_eyeTransform.GetPosition(), DirectX::SimpleMath::Vector3::UnitY); }


			//--------------------------------------------------------------
			//! @summary   Z軸ビルボード行列の取得
			//--------------------------------------------------------------
			inline DirectX::SimpleMath::Matrix GetBillboardConstrainZ(const DirectX::SimpleMath::Vector3& position) const { return DirectX::SimpleMath::Matrix::CreateConstrainedBillboard(position, m_eyeTransform.GetPosition(), DirectX::SimpleMath::Vector3::UnitZ); }


			//--------------------------------------------------------------
			//! @summary   ビューポートの取得
			//--------------------------------------------------------------
			inline const D3D11_VIEWPORT& GetViewport() const { return m_viewport; }
		};
	}
}
