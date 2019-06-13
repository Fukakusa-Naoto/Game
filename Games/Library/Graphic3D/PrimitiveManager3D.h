//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		PrimitiveManager3D.h
//!
//! @summary	3Dのプリミティブ描画のマネージャークラスのヘッダファイル
//!
//! @date		2018.10.09
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
	namespace Graphic3D
	{
		// クラスの定義 ==========================================================
		class PrimitiveManager3D : public System::SingletonBase<PrimitiveManager3D>
		{
			// <フレンドの登録>
			friend class System::SingletonBase<PrimitiveManager3D>;


			// <構造体の定義>
			// 頂点バッファ
			struct VertexBuffer
			{
				// 座標
				DirectX::SimpleMath::Vector3 position;
				// 色
				DirectX::XMFLOAT4 color;


				// <コンストラクタ>
				VertexBuffer() :
					position(DirectX::SimpleMath::Vector3::Zero),
					color(DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f))
				{
					// 何もしない
				}

				VertexBuffer(DirectX::SimpleMath::Vector3 position, const DirectX::XMFLOAT4& color) :
					position(position),
					color(color)
				{
					// 何もしない
				}
			};


			// 定数バッファ
			struct ConstBuffer
			{
				// ワールド行列
				DirectX::SimpleMath::Matrix world;
				// ビュー行列
				DirectX::SimpleMath::Matrix view;
				// 射影行列
				DirectX::SimpleMath::Matrix projection;

				// <コンストラクタ>
				ConstBuffer() {}
				ConstBuffer(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection) :
					world(world),
					view(view),
					projection(projection)
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
			Shader::VertexShader* m_vertexShader2;
			// ジオメトリシェーダーのインスタンス
			ID3D11GeometryShader* m_geometryShader;
			// ピクセルシェーダーのインスタンス
			ID3D11PixelShader* m_pixelShader;
			// 定数バッファ・オブジェクト
			ID3D11Buffer* m_constBuffer;


			// 点・線・三角形・クワッドの頂点バッファ・オブジェクト
			ID3D11Buffer* m_vertexBuffer;
			ID3D11Buffer* m_indexBuffer;

			// 立方体のバッファ・オブジェクト
			std::vector<VertexBuffer> m_cubeVertexData;
			ID3D11Buffer* m_cubeVertexBuffer;
			ID3D11Buffer* m_cubeIndexBuffer;
			ID3D11Buffer* m_cubeWireframeIndexBuffer;

			// 球のバッファ・オブジェクト
			std::vector<VertexBuffer> m_sphereVertexData;
			ID3D11Buffer* m_sphereVertexBuffer;
			ID3D11Buffer* m_sphereIndexBuffer;



			// <コンストラクタ>
		private:
			//--------------------------------------------------------------
			//! @parameter [void] なし
			//--------------------------------------------------------------
			PrimitiveManager3D();


			// <デストラクタ>
		public:
			~PrimitiveManager3D();


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
			//! @parameter [position] 座標
			//! @parameter [color] 色
			//! @parameter [view] ビュー行列
			//! @parameter [projection] 射影行列
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void DrawPoint(const DirectX::SimpleMath::Vector3& position, const DirectX::XMFLOAT4& color, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);


			//--------------------------------------------------------------
			//! @summary   線の描画
			//!
			//! @parameter [startPosition] 始点
			//! @parameter [endPosition] 終点
			//! @parameter [color] 色
			//! @parameter [view] ビュー行列
			//! @parameter [projection] 射影行列
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void DrawLine(const DirectX::SimpleMath::Vector3& startPosition, const DirectX::SimpleMath::Vector3& endPosition, const DirectX::XMFLOAT4& color, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);


			//--------------------------------------------------------------
			//! @summary   三角形の描画
			//!
			//! @parameter [vertex1] 頂点1
			//! @parameter [vertex2] 頂点2
			//! @parameter [vertex3] 頂点3
			//! @parameter [color] 色
			//! @parameter [view] ビュー行列
			//! @parameter [projection] 射影行列
			//! @parameter [wireframeFlag] ワイヤーフレーム表示フラグ
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void DrawTriangle(const DirectX::SimpleMath::Vector3& vertex1, const DirectX::SimpleMath::Vector3& vertex2, const DirectX::SimpleMath::Vector3& vertex3, DirectX::XMFLOAT4 color, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection, bool wireframeFlag = false);


			//--------------------------------------------------------------
			//! @summary   クワッドの描画
			//!
			//! @parameter [world] ワールド行列
			//! @parameter [color] 色
			//! @parameter [view] ビュー行列
			//! @parameter [projection] 射影行列
			//! @parameter [wireframeFlag] ワイヤーフレーム表示フラグ
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void DrawQuad(const DirectX::SimpleMath::Matrix& world, DirectX::XMFLOAT4 color, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection, bool wireframeFlag = false);


			//--------------------------------------------------------------
			//! @summary   立方体の描画
			//!
			//! @parameter [position] 座標
			//! @parameter [rotation] 回転角
			//! @parameter [size] サイズ(x:幅, y:高さ, z:奥行)
			//! @parameter [color] 色
			//! @parameter [view] ビュー行列
			//! @parameter [projection] 射影行列
			//! @parameter [wireframeFlag] ワイヤーフレーム表示フラグ
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void DrawCube(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation, const DirectX::SimpleMath::Vector3& size, DirectX::XMFLOAT4 color, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection, bool wireframeFlag = false);
			void DrawCube(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Quaternion& rotation, const DirectX::SimpleMath::Vector3& size, DirectX::XMFLOAT4 color, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection, bool wireframeFlag = false);


			//--------------------------------------------------------------
			//! @summary   立方体の描画
			//!
			//! @parameter [position] 座標
			//! @parameter [rotation] 回転角
			//! @parameter [size] サイズ(x:幅, y:高さ, z:奥行)
			//! @parameter [color] 色
			//! @parameter [view] ビュー行列
			//! @parameter [projection] 射影行列
			//! @parameter [lambda] レンダリング・パイプライン構築のラムダ式関数
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void DrawCube(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Quaternion& rotation, const DirectX::SimpleMath::Vector3& size, DirectX::XMFLOAT4 color, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection, std::function<void()> lambda);


			//--------------------------------------------------------------
			//! @summary   球の描画
			//!
			//! @parameter [position] 中心座標
			//! @parameter [radius] 半径
			//! @parameter [color] 色
			//! @parameter [view] ビュー行列
			//! @parameter [projection] 射影行列
			//! @parameter [wireframeFlag] ワイヤーフレーム表示フラグ
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void DrawSphere(const DirectX::SimpleMath::Vector3& position, float radius, const DirectX::XMFLOAT4& color, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection, bool wireframeFlag = false);


			//--------------------------------------------------------------
			//! @summary   球の描画
			//!
			//! @parameter [position] 中心座標
			//! @parameter [radius] 半径
			//! @parameter [color] 色
			//! @parameter [view] ビュー行列
			//! @parameter [projection] 射影行列
			//! @parameter [lambda] レンダリング・パイプライン構築のラムダ式関数
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void DrawSphere(const DirectX::SimpleMath::Vector3& position, float radius, const DirectX::XMFLOAT4& color, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection, std::function<void()> lambda);
		};
	}
}
