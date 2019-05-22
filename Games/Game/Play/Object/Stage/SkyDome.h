//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		SkyDome.h
//!
//! @summary	モトスのスカイドームクラスのヘッダファイル
//!
//! @date		2019.01.01
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../Game.h"

// <自作ヘッダーファイル>
#include "../../../../Library/Shader/ShaderManager.h"
#include "../../../../Library/Utility/TextureManager.h"
#include "../../../../Library/Camera/Camera.h"


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
				class SkyDome
				{
					// <構造体の定義>
					// 頂点バッファ
					struct VertexBuffer
					{
						// 座標
						DirectX::SimpleMath::Vector3 position;


						// <コンストラクタ>
						VertexBuffer() :
							position(DirectX::SimpleMath::Vector3::Zero)
						{
							// 何もしない
						}
					};

					// 定数バッファ
					struct ConstBuffer
					{
						DirectX::SimpleMath::Matrix	world;
						DirectX::SimpleMath::Matrix	view;
						DirectX::SimpleMath::Matrix	projection;
						DirectX::XMFLOAT4 time;
					};


					// <メンバ定数>
					static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;


					// <メンバ変数>
				private:
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
					// テクスチャ
					Library::Utility::Texture* m_texture;
					// 定数バッファ
					ID3D11Buffer* m_constBuffer;
					// 頂点バッファ・オブジェクト
					ID3D11Buffer* m_vertexBuffer;
					// カメラ
					Library::Camera::Camera* m_camera;


					// <コンストラクタ>
				public:
					//------------------------------------------------------
					//! @parameter [void] なし
					//------------------------------------------------------
					SkyDome();


					// <デストラクタ>
				public:
					~SkyDome();


					// <メンバ関数>
				public:
					//------------------------------------------------------
					//! @summary   初期化処理
					//!
					//! @parameter [void] なし
					//!
					//! @return    なし
					//------------------------------------------------------
					void Initialize();


					//------------------------------------------------------
					//! @summary   更新処理
					//!
					//! @parameter [timer] 時間情報
					//!
					//! @return    なし
					//------------------------------------------------------
					void Update(const Library::Common::StepTimer & timer);



					//------------------------------------------------------
					//! @summary   描画処理
					//!
					//! @parameter [void] なし
					//!
					//! @return    なし
					//------------------------------------------------------
					void Draw();


					// <セッター関数>
				public:
					//------------------------------------------------------
					//! @parameter [camera] カメラ
					//------------------------------------------------------
					inline void SetCamere(Library::Camera::Camera* camera) { m_camera = camera; }
				};
			}
		}
	}
}
