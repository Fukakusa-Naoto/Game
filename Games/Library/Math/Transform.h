//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Transform.h
//!
//! @summary	オブジェクトの位置、回転、スケールを扱うクラスのヘッダファイル
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

// <標準ヘッダファイル>
#include <list>

// <自作ヘッダファイル>
#include "Math.h"


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Math
	{
		// クラスの定義 ==========================================================
		class Transform
		{
			// <メンバ変数>
		private:
			DirectX::SimpleMath::Matrix m_world;
			DirectX::SimpleMath::Vector3 m_position;
			DirectX::SimpleMath::Vector3 m_eulerAngles;
			DirectX::SimpleMath::Quaternion m_rotation;
			DirectX::SimpleMath::Vector3 m_scale;
			Transform* m_parent;
			std::list<Transform*> m_children;


			// <コンストラクタ>
		public:
			//--------------------------------------------------------------
			//! @parameter [void] なし
			//--------------------------------------------------------------
			Transform();


			// <デストラクタ>
		public:
			~Transform();


			// <メンバ変数>
		public:
			//--------------------------------------------------------------
			//! @summary   更新処理
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Update();


			//--------------------------------------------------------------
			//! @summary   移動処理
			//!
			//! @parameter [velocity] 速度
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Move(const DirectX::SimpleMath::Vector3& velocity);


			//--------------------------------------------------------------
			//! @summary   回転処理
			//!
			//! @parameter [rotation] 回転角
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Rotate(const DirectX::SimpleMath::Vector3& rotation);


			// <セッター関数>
		public:
			//--------------------------------------------------------------
			//! @parameter [position] 座標
			//--------------------------------------------------------------
			inline void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }


			//--------------------------------------------------------------
			//! @parameter [eulerAngles] オイラー角
			//--------------------------------------------------------------
			inline void SetEulerAngles(const DirectX::SimpleMath::Vector3& eulerAngles)
			{
				m_eulerAngles = eulerAngles;
				m_rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(m_eulerAngles.y), DirectX::XMConvertToRadians(m_eulerAngles.x), DirectX::XMConvertToRadians(m_eulerAngles.z));
			}


			//--------------------------------------------------------------
			//! @parameter [rotation] 回転角
			//--------------------------------------------------------------
			inline void SetRotation(const DirectX::SimpleMath::Quaternion& rotation)
			{
				m_rotation = rotation;
				m_eulerAngles = Math::ConvertQuaternionToEulerAngles(m_rotation);
			}


			//--------------------------------------------------------------
			//! @parameter [scale] スケール
			//--------------------------------------------------------------
			inline void SetScele(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }


			//--------------------------------------------------------------
			//! @parameter [parent] 親のTransform
			//--------------------------------------------------------------
			inline void SetParent(Transform* parent) { m_parent = parent; }


			//--------------------------------------------------------------
			//! @parameter [child] 子のTransform
			//--------------------------------------------------------------
			inline void SetChild(Transform* child)
			{
				m_children.push_back(child);
				child->SetParent(this);
			}



			// <ゲッター関数>
		public:
			//--------------------------------------------------------------
			//! @summary   ワールド行列の取得
			//--------------------------------------------------------------
			inline const DirectX::SimpleMath::Matrix& GetWorld()
			{
				m_world = DirectX::SimpleMath::Matrix::CreateScale(m_scale) * DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_rotation) * DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

				if (m_parent != nullptr) m_world *= m_parent->GetWorld();

				return m_world;
			}


			//--------------------------------------------------------------
			//! @summary   座標の取得
			//--------------------------------------------------------------
			inline const DirectX::SimpleMath::Vector3& GetPosition() const { return m_position; }


			//--------------------------------------------------------------
			//! @summary   オイラー角の取得
			//--------------------------------------------------------------
			inline const DirectX::SimpleMath::Vector3& GetEulerAngles() const { return m_eulerAngles; }


			//--------------------------------------------------------------
			//! @summary   回転角の取得
			//--------------------------------------------------------------
			inline const DirectX::SimpleMath::Quaternion& GetRotation() const { return m_rotation; }


			//--------------------------------------------------------------
			//! @summary   スケールの取得
			//--------------------------------------------------------------
			inline const DirectX::SimpleMath::Vector3& GetScale() const { return m_scale; }


			//--------------------------------------------------------------
			//! @summary   親のTransformの取得
			//--------------------------------------------------------------
			inline const Transform* GetParent()const { return m_parent; }

		};
	}
}