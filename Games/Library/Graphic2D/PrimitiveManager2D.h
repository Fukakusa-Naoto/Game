//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		PrimitiveManager2D.h
//!
//! @summary	2Dのプリミティブ描画のマネージャークラスのヘッダファイル
//!
//! @date		2018.09.19
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once


// ヘッダファイルの読み込み =======================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダファイル>
#include "../System/SingletonBase.h"
#include "../Shader/ShaderManager.h"


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Graphic2D
	{
		// クラスの定義 ==========================================================
		class PrimitiveManager2D : public System::SingletonBase<PrimitiveManager2D>
		{
			// <フレンドの登録>
			friend class System::SingletonBase<PrimitiveManager2D>;


			// <構造体の定義>
			// 頂点バッファ
			struct VertexBuffer
			{
				// 座標
				DirectX::XMFLOAT3 position;
				// 色
				DirectX::XMFLOAT4 color;


				// <コンストラクタ>
				VertexBuffer() {}
				VertexBuffer(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 color) :
					position(position),
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
			// コモンステート
			DirectX::CommonStates* m_commonStates;
			// 頂点シェーダーのインスタンス
			Shader::VertexShader* m_vertexShader;
			// ピクセルシェーダーのインスタンス
			ID3D11PixelShader* m_pixelShader;
			// ビューポート
			D3D11_VIEWPORT m_viewport;
			D3D11_VIEWPORT m_defaultViewport;


			// <コンストラクタ>
		private:
			//--------------------------------------------------------------
			//! @parameter [void] なし
			//--------------------------------------------------------------
			PrimitiveManager2D();


			// <デストラクタ>
		public:
			~PrimitiveManager2D();


			// <メンバ関数>
		public:
			//--------------------------------------------------------------
			//! @summary   初期化処理
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Initialize();


			//--------------------------------------------------------------
			//! @summary   点の描画
			//!
			//! @parameter [pointX] X座標(ピクセル単位)
			//! @parameter [pointY] Y座標(ピクセル単位)
			//! @parameter [color] 色
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void DrawPoint(int pointX, int pointY, DirectX::XMFLOAT4 color);


			//--------------------------------------------------------------
			//! @summary   線の描画
			//!
			//! @parameter [startX] 始点のX座標(ピクセル単位)
			//! @parameter [startY] 始点のY座標(ピクセル単位)
			//! @parameter [endX] 終点のX座標(ピクセル単位)
			//! @parameter [endY] 終点のY座標(ピクセル単位)
			//! @parameter [color] 色
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void DrawLine(int startX, int startY, int endX, int endY, DirectX::XMFLOAT4 color);


			//--------------------------------------------------------------
			//! @summary   三角形の描画
			//!
			//! @parameter [vertexX1] 頂点1のX座標(ピクセル単位)
			//! @parameter [vertexY1] 頂点1のY座標(ピクセル単位)
			//! @parameter [vertexX2] 頂点2のX座標(ピクセル単位)
			//! @parameter [vertexY2] 頂点2のY座標(ピクセル単位)
			//! @parameter [vertexX3] 頂点3のX座標(ピクセル単位)
			//! @parameter [vertexY3] 頂点3のY座標(ピクセル単位)
			//! @parameter [color] 色
			//! @parameter [fillFlag] 塗りつぶしフラグ
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void DrawTriangle(int vertexX1, int vertexY1, int vertexX2, int vertexY2, int vertexX3, int vertexY3, DirectX::XMFLOAT4 color, bool fillFlag = true);


			//--------------------------------------------------------------
			//! @summary   四角形の描画
			//!
			//! @parameter [topLeftX] 左上のX座標(ピクセル単位)
			//! @parameter [topLeftY] 左上のY座標(ピクセル単位)
			//! @parameter [bottomRightX] 右下のX座標(ピクセル単位)
			//! @parameter [bottomRightY] 右下のY座標(ピクセル単位)
			//! @parameter [color] 色
			//! @parameter [fillFlag] 塗りつぶしフラグ
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void DrawBox(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY, DirectX::XMFLOAT4 color, bool fillFlag = true);


			//--------------------------------------------------------------
			//! @summary   円の描画
			//!
			//! @parameter [centerX] 中心のX座標(ピクセル単位)
			//! @parameter [centerY] 中心のY座標(ピクセル単位)
			//! @parameter [radius] 半径のX座標(ピクセル単位)
			//! @parameter [color] 色
			//! @parameter [fillFlag] 塗りつぶしフラグ
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void DrawCircle(int centerX, int centerY, int radius, DirectX::XMFLOAT4 color, bool fillFlag = true);


			// <セッター関数>
		public:
			//--------------------------------------------------------------
			//! @parameter [viewport] ビューポート
			//--------------------------------------------------------------
			inline void SetViewport(const D3D11_VIEWPORT& viewport) { m_viewport = viewport; }
		};
	}
}
