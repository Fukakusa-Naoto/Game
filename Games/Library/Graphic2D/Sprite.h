//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Sprite.h
//!
//! @summary	スプライトクラスのヘッダファイル
//!
//! @date		2018.10.05
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダファイル>
#include "../Utility/ShaderManager.h"
#include "../Utility/TextureManager.h"


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Graphic2D
	{
		// クラスの定義 ==========================================================
		class Sprite
		{
			// <構造体>
			// 頂点バッファ
			struct VertexBuffer
			{
				// 座標
				DirectX::XMFLOAT3 position;
				// uv座標
				DirectX::XMFLOAT2 texcoord;
				// 色
				DirectX::XMFLOAT4 color;


				// <コンストラクタ>
				VertexBuffer(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 texcoord, DirectX::XMFLOAT4 color) :
					position(position),
					texcoord(texcoord),
					color(color)
				{
					// 何もしない
				}
			};


			// <メンバ定数>
		public:
			// 入力要素配列
			static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;


			// <メンバ変数>
		private:
			// デバイス・インターフェイス
			ID3D11Device1* m_device;
			// デバイス・コンテキスト・インターフェイス
			ID3D11DeviceContext1* m_deviceContext;
			// 頂点シェーダー・インターフェイス
			Utility::VertexShader* m_vertexShader;
			// ピクセルシェーダー・インターフェイス
			ID3D11PixelShader* m_pixelShader;
			// コモンステート
			DirectX::CommonStates* m_commonStates;

			// テクスチャ
			Utility::Texture* m_texture;
			// 座標
			DirectX::SimpleMath::Vector2 m_position;
			// 切り抜き座標
			RECT m_rect;
			// 色
			DirectX::SimpleMath::Color m_color;
			// 回転(Z軸)
			float m_rotation;
			// アンカーポイント
			DirectX::SimpleMath::Vector2 m_anchorPoint;
			// スケール
			DirectX::SimpleMath::Vector2 m_scale;


			// <コンストラクタ>
		private:
			//--------------------------------------------------------------
			//! @parameter [void] 読なし
			//--------------------------------------------------------------
			Sprite() {};

		public:
			//--------------------------------------------------------------
			//! @parameter [fileName] 読み込む画像ファイル名
			//--------------------------------------------------------------
			Sprite(const wchar_t* fileName);


			// <デストラクタ>
		public:
			~Sprite();


			// <メンバ関数>
		public:
			//--------------------------------------------------------------
			//! @summary   描画処理
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Draw();
		};
	}
}