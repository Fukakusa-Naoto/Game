//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Tile.h
//!
//! @summary	モトスのタイルクラスのヘッダファイル
//!
//! @date		2018.11.14
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <自作ヘッダーファイル>
#include "../../../../Library/System/GameObject.h"
#include "../../../../Library/Graphic3D/PrimitiveManager3D.h"
#include "../../../../Library/Collision/CubeShape.h"


// 名前空間の定義 ================================================================
namespace Motos
{
	namespace Play
	{
		namespace Object
		{
			namespace Stage
			{
				// クラスの定義 ======================================================
				class Tile : public Library::System::GameObject
				{
					// <メンバ定数>
				public:
					// サイズ
					static const DirectX::SimpleMath::Vector3 TILE_SIZE;


					// <メンバ変数>
				private:
					// 3Dプリミティブの描画マネージャー
					Library::Graphic3D::PrimitiveManager3D* m_primitiveRender;
					// デバイス・インターフェイス
					ID3D11Device* m_device;
					// デバイス・コンテキスト・インターフェイス
					ID3D11DeviceContext* m_deviceContext;
					// コモンステート
					DirectX::CommonStates* m_commonStates;
					// 頂点シェーダー・インターフェイス
					Library::Shader::VertexShader* m_vertexShader;
					// ジオメトリシェーダー
					ID3D11GeometryShader* m_geometryShader;
					// ピクセルシェーダー・インターフェイス
					ID3D11PixelShader* m_pixelShader;
					// 衝突判定
					Library::Collision::CubeShape* m_cubeShape;


					// <コンストラクタ>
				public:
					//------------------------------------------------------
					//! @summary   コンストラクタ
					//!
					//! @parameter [void] なし
					//------------------------------------------------------
					Tile();


					// <デストラクタ>
				public:
					~Tile();


					// <メンバ関数>
				public:
					//------------------------------------------------------
					//! @summary   初期化処理
					//!
					//! @parameter [void] なし
					//!
					//! @return    なし
					//------------------------------------------------------
					void Start() override;


					//------------------------------------------------------
					//! @summary   更新処理
					//!
					//! @parameter [timer] 時間情報
					//!
					//! @return    なし
					//------------------------------------------------------
					void Update(const Library::Common::StepTimer & timer) override;



					//------------------------------------------------------
					//! @summary   描画処理
					//!
					//! @parameter [void] なし
					//!
					//! @return    なし
					//------------------------------------------------------
					void Draw() override;


					// <セッター関数>
				public:
					//------------------------------------------------------
					//! @parameter [position] 座標
					//------------------------------------------------------
					inline void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_transform.SetPosition(position); }
				};
			}
		}
	}
}
