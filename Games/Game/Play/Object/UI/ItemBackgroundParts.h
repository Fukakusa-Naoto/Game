//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		ItemBackgroundParts.h
//!
//! @summary	モトスUIのアイテム背景パーツクラスのヘッダファイル
//!
//! @date		2018.11.09
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <自作ヘッダーファイル>
#include "../../../../Library/Graphic2D/Sprite.h"


// 名前空間の定義 ================================================================
namespace Motos
{
	namespace Play
	{
		namespace Object
		{
			namespace UI
			{
				// クラスの定義 ==================================================
				class ItemBackgroundParts
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
					// ビューポート
					D3D11_VIEWPORT& m_viewport;
					//// デバイス・インターフェイス
					//ID3D11Device* m_device;
					//// デバイス・コンテキスト・インターフェイス
					//ID3D11DeviceContext* m_deviceContext;
					//// 頂点シェーダー・インターフェイス
					//Library::Utility::VertexShader* m_vertexShader;
					//// ピクセルシェーダー・インターフェイス
					//ID3D11PixelShader* m_pixelShader;
					//// コモンステート
					//DirectX::CommonStates* m_commonStates;
					//// テクスチャ変数
					//Library::Utility::Texture* m_texture;
					//// 時間バッファ・オブジェクト
					//ID3D11Buffer* m_timeBuffer;


					// <コンストラクタ>
				public:
					//------------------------------------------------------
					//! @parameter [veiwport] ビューポート
					//------------------------------------------------------
					ItemBackgroundParts(D3D11_VIEWPORT& viewport);


					// <デストラクタ>
				public:
					~ItemBackgroundParts();


					// <メンバ関数>
				public:
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
					//! @parameter [position] 座標
					//------------------------------------------------------
					inline void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_sprite->SetPosition(position); }


					// <ゲッター関数>
				public:
					//------------------------------------------------------
					//! @return    座標
					//------------------------------------------------------
					inline const DirectX::SimpleMath::Vector2& GetPosition() const { return m_sprite->GetPosition(); }
				};
			}
		}
	}
}
