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
#include "../Shader/ShaderManager.h"
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
			Shader::VertexShader* m_vertexShader;
			// ピクセルシェーダー・インターフェイス
			ID3D11PixelShader* m_pixelShader;
			// コモンステート
			DirectX::CommonStates* m_commonStates;
			// 頂点バッファ
			ID3D11Buffer* m_vertexBuffer;
			// インデックスバッファ
			ID3D11Buffer* m_indexBuffer;

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
			// ビューポート
			D3D11_VIEWPORT m_viewport;
			D3D11_VIEWPORT m_defaultViewport;


			// <コンストラクタ>
		public:
			//--------------------------------------------------------------
			//! @parameter [void] なし
			//--------------------------------------------------------------
			Sprite();

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


			//--------------------------------------------------------------
			//! @summary   描画処理
			//!
			//! @parameter [lambda] レンダリング・パイプライン構築のラムダ式関数
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Draw(std::function<void()> lambda) const;


			// <セッター関数>
		public:
			//--------------------------------------------------------------
			//! @parameter [texture] テクスチャ
			//--------------------------------------------------------------
			inline void SetTexture(Utility::Texture* texture) { m_texture = texture; }


			//--------------------------------------------------------------
			//! @parameter [position] 座標
			//--------------------------------------------------------------
			inline void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; }


			//--------------------------------------------------------------
			//! @parameter [rect] 切り抜き座標
			//--------------------------------------------------------------
			inline void SetRect(const RECT& rect) { m_rect = rect; }


			//--------------------------------------------------------------
			//! @parameter [color] 色
			//--------------------------------------------------------------
			inline void SetColor(const DirectX::SimpleMath::Color& color) { m_color = color; }


			//--------------------------------------------------------------
			//! @parameter [rotation] 回転角
			//--------------------------------------------------------------
			inline void SetRotation(float rotation) { m_rotation = rotation; }


			//--------------------------------------------------------------
			//! @parameter [anchorPoint] アンカーポイント
			//--------------------------------------------------------------
			inline void SetAnchorPoint(const DirectX::SimpleMath::Vector2& anchorPoint) { m_anchorPoint = anchorPoint; }


			//--------------------------------------------------------------
			//! @parameter [scale] スケール
			//--------------------------------------------------------------
			inline void SetScale(const DirectX::SimpleMath::Vector2& scale) { m_scale = scale; }


			//--------------------------------------------------------------
			//! @parameter [viewport] ビューポートの設定
			//--------------------------------------------------------------
			inline void SetViewport(const D3D11_VIEWPORT& viewport) { m_viewport = viewport; }


			// <ゲッター関数>
		public:
			//--------------------------------------------------------------
			//! @return    テクスチャ
			//--------------------------------------------------------------
			inline Utility::Texture* GetTexture() const { return m_texture; }


			//--------------------------------------------------------------
			//! @return    座標
			//--------------------------------------------------------------
			inline const DirectX::SimpleMath::Vector2& GetPosition() const { return m_position; }


			//--------------------------------------------------------------
			//! @return    切り抜き座標
			//--------------------------------------------------------------
			inline const RECT& GetRect() const { return m_rect; }


			//--------------------------------------------------------------
			//! @return    色
			//--------------------------------------------------------------
			inline const DirectX::SimpleMath::Color& GetColor() const { return m_color; }


			//--------------------------------------------------------------
			//! @return    回転角
			//--------------------------------------------------------------
			inline float GetRotation() const { return m_rotation; }


			//--------------------------------------------------------------
			//! @return    アンカーポイント
			//--------------------------------------------------------------
			inline const DirectX::SimpleMath::Vector2& GetAnchorPoint() { return m_anchorPoint; }


			//--------------------------------------------------------------
			//! @return    スケール
			//--------------------------------------------------------------
			inline const DirectX::SimpleMath::Vector2& GetScale() const { return m_scale; }


			//--------------------------------------------------------------
			//! @return    ビューポートの設定
			//--------------------------------------------------------------
			inline const D3D11_VIEWPORT& GetViewport() const { return m_viewport; }
		};
	}
}