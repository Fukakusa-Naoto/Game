//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		TileBackground.h
//!
//! @summary	モトスのタイル背景クラスのヘッダファイル
//!
//! @date		2018.11.01
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <自作ヘッダーファイル>
#include "../../../../Library/Graphic2D/UI/SpriteUI.h"
#include "../../../../Library/Graphic2D/Sprite.h"


// 名前空間の定義 ================================================================
namespace Motos
{
	namespace Title
	{
		namespace Object
		{
			namespace UI
			{
				// クラスの定義 ==================================================
				class TileBackground : public Library::Graphic2D::UI::SpriteUI
				{
					// <構造体>
					struct TimeBuffer
					{
						// 時間
						DirectX::SimpleMath::Vector4 time;
					};


					// <メンバ定数>
				public:
					// 入力要素配列
					static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;


					// <メンバ変数>
				private:
					Library::Graphic2D::Sprite* m_sprite;
					// デバイス・インターフェイス
					ID3D11Device* m_device;
					// デバイス・コンテキスト・インターフェイス
					ID3D11DeviceContext* m_deviceContext;
					// 頂点シェーダー・インターフェイス
					Library::Shader::VertexShader* m_vertexShader;
					// ピクセルシェーダー・インターフェイス
					ID3D11PixelShader* m_pixelShader;
					// コモンステート
					DirectX::CommonStates* m_commonStates;
					// テクスチャ変数
					Library::Utility::Texture* m_texture;
					Library::Utility::Texture* m_texture2;
					// 時間バッファ・オブジェクト
					ID3D11Buffer* m_timeBuffer;
					// 時間のカウント
					float m_timer;


					// <コンストラクタ>
				public:
					TileBackground();


					// <デストラクタ>
				public:
					~TileBackground();


					// <メンバ関数>
				public:
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
				};
			}
		}
	}
}
