//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		PrimitiveManager3D.h
//!
//! @summary	3Dのプリミティブ描画のマネージャークラスのソースファイル
//!
//! @date		2018.10.09
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <標準ヘッダファイル>
#include <vector>

// <自作ヘッダファイル>
#include "PrimitiveManager3D.h"
#include "../Common/DeviceResources.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 定数の定義 ==============================================================
// 入力要素配列
const vector<D3D11_INPUT_ELEMENT_DESC> Graphic3D::PrimitiveManager3D::INPUT_LAYOUT =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Graphic3D::PrimitiveManager3D::PrimitiveManager3D() :
	m_device(nullptr),
	m_deviceContext(nullptr),
	m_commonStates(nullptr),
	m_vertexShader(nullptr),
	m_vertexShader2(nullptr),
	m_geometryShader(nullptr),
	m_pixelShader(nullptr),
	m_constBuffer(nullptr),
	m_vertexBuffer(nullptr),
	m_indexBuffer(nullptr),
	m_cubeVertexBuffer(nullptr),
	m_cubeIndexBuffer(nullptr),
	m_cubeWireframeIndexBuffer(nullptr),
	m_sphereVertexBuffer(nullptr),
	m_sphereIndexBuffer(nullptr)
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Graphic3D::PrimitiveManager3D::~PrimitiveManager3D()
{
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_commonStates = nullptr;
	m_vertexShader = nullptr;
	m_vertexShader2 = nullptr;
	m_geometryShader = nullptr;
	m_pixelShader = nullptr;

	m_constBuffer->Release();

	m_vertexBuffer->Release();
	m_indexBuffer->Release();
	m_cubeVertexBuffer->Release();
	m_cubeIndexBuffer->Release();
	m_cubeWireframeIndexBuffer->Release();
	m_sphereVertexBuffer->Release();
	m_sphereIndexBuffer->Release();
}



//--------------------------------------------------------------------
//! @summary   初期化処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Graphic3D::PrimitiveManager3D::Initialize()
{
	// デバイス・インターフェイスの取得
	m_device = Common::DeviceResources::GetInstance()->GetD3DDevice();
	// デバイス・コンテキスト・インターフェイスの取得
	m_deviceContext = Common::DeviceResources::GetInstance()->GetD3DDeviceContext();
	// コモンステートの取得
	m_commonStates = Common::DeviceResources::GetInstance()->GetCommonStates();

	// 頂点シェーダーのインスタンスの取得
	m_vertexShader = Shader::ShaderManager::GetInstance()->LoadVertexShader(L"Primitive3DVertexShader", INPUT_LAYOUT);
	m_vertexShader2 = Shader::ShaderManager::GetInstance()->LoadVertexShader(L"Primitive3DVertexShader2", INPUT_LAYOUT);
	// ジオメトリシェーダーのインスタンスの取得
	m_geometryShader = Shader::ShaderManager::GetInstance()->LoadGeometryShader(L"Primitive3DGeometryShader");
	// ピクセルシェーダーのインスタンスの取得
	m_pixelShader = Shader::ShaderManager::GetInstance()->LoadPixelShader(L"Primitive3DPixelShader");


	// 定数バッファの定義
	D3D11_BUFFER_DESC constBufferDesc;
	ZeroMemory(&constBufferDesc, sizeof(constBufferDesc));
	constBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufferDesc.ByteWidth = sizeof(ConstBuffer);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = 0;

	// 定数バッファの作成
	m_device->CreateBuffer(&constBufferDesc, nullptr, &m_constBuffer);

#pragma region 点・線・三角形・クワッド
	{
		// 頂点バッファの定義
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));						// 初期化
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;									// 動的使用法
		vertexBufferDesc.ByteWidth = sizeof(VertexBuffer) * 4;							// 使用頂点数
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// 頂点バッファとして使う
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;

		// 頂点バッファの作成
		m_device->CreateBuffer(&vertexBufferDesc, NULL, &m_vertexBuffer);


		// インデックスバッファの定義
		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));						// 初期化
		indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;									// 動的使用法
		indexBufferDesc.ByteWidth = sizeof(UINT) * 6;									// 使用頂点数
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;							// インデックスバッファとして使う
		indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		indexBufferDesc.MiscFlags = 0;

		// インデックスバッファの作成
		m_device->CreateBuffer(&indexBufferDesc, NULL, &m_indexBuffer);
	}
#pragma endregion

#pragma region 立方体
	{
		// 立方体の頂点バッファとインデックスバッファの作成
		// 表示する立方体の頂点の作成
		m_cubeVertexData =
		{
			VertexBuffer(Vector3(-0.5f, 0.5f, -0.5f), Color(1.0f, 1.0f, 1.0f, 1.0f)),
			VertexBuffer(Vector3(0.5f, 0.5f, -0.5f), Color(1.0f, 1.0f, 1.0f, 1.0f)),
			VertexBuffer(Vector3(0.5f, -0.5f, -0.5f), Color(1.0f, 1.0f, 1.0f, 1.0f)),
			VertexBuffer(Vector3(-0.5f, -0.5f, -0.5f), Color(1.0f, 1.0f, 1.0f, 1.0f)),
			VertexBuffer(Vector3(-0.5f, 0.5f, 0.5f), Color(1.0f, 1.0f, 1.0f, 1.0f)),
			VertexBuffer(Vector3(0.5f, 0.5f, 0.5f), Color(1.0f, 1.0f, 1.0f, 1.0f)),
			VertexBuffer(Vector3(0.5f, -0.5f, 0.5f), Color(1.0f, 1.0f, 1.0f, 1.0f)),
			VertexBuffer(Vector3(-0.5f, -0.5f, 0.5f), Color(1.0f, 1.0f, 1.0f, 1.0f)),
		};

		// 頂点バッファの定義
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));						// 初期化
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;									// デフォルト使用法
		vertexBufferDesc.ByteWidth = sizeof(VertexBuffer) * m_cubeVertexData.size();	// 使用頂点数
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// 頂点バッファとして使う
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;


		// 頂点バッファのサブリソースの定義
		D3D11_SUBRESOURCE_DATA vertexSubresourceData;
		ZeroMemory(&vertexSubresourceData, sizeof(vertexSubresourceData));	// 初期化
		vertexSubresourceData.pSysMem = m_cubeVertexData.data();			// バッファ・データの初期化


		// 頂点バッファの作成
		m_device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &m_cubeVertexBuffer);


		// インデックスデータの作成
		vector<UINT> indexData;

		// ワイヤーフレームなし
		indexData =
		{
			0,1,3,
			1,2,3,
			1,5,2,
			5,6,2,
			5,4,6,
			4,7,6,
			4,5,0,
			5,1,0,
			4,0,7,
			0,3,7,
			3,2,7,
			2,6,7,
		};

		// インデックスバッファの定義
		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));			// 初期化
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;					// デフォルト使用法
		indexBufferDesc.ByteWidth = sizeof(UINT) * indexData.size();	// 使用インデックス数
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;			// インデックスバッファとして使う
		indexBufferDesc.CPUAccessFlags = 0;


		// インデックスバッファのサブリソースの定義
		D3D11_SUBRESOURCE_DATA indexSubresourceData;
		ZeroMemory(&indexSubresourceData, sizeof(indexSubresourceData));	// 初期化
		indexSubresourceData.pSysMem = indexData.data();					// バッファ・データの初期化


		// インデックスバッファの作成
		m_device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &m_cubeIndexBuffer);

		indexData.clear();

		// ワイヤーフレーム
		indexData =
		{
			0,1,1,2,2,3,3,0,	// 奥
			4,5,5,6,6,7,7,4,	// 手前
			0,4,3,7,			// 左上・左下
			5,1,6,2,			// 右上・右下
		};


		// インデックスバッファの定義
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));			// 初期化
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;					// デフォルト使用法
		indexBufferDesc.ByteWidth = sizeof(UINT) * indexData.size();	// 使用インデックス数
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;			// インデックスバッファとして使う
		indexBufferDesc.CPUAccessFlags = 0;


		// インデックスバッファのサブリソースの定義
		ZeroMemory(&indexSubresourceData, sizeof(indexSubresourceData));	// 初期化
		indexSubresourceData.pSysMem = indexData.data();					// バッファ・データの初期化


		// インデックスバッファの作成
		m_device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &m_cubeWireframeIndexBuffer);
	}
#pragma endregion

#pragma region 球
	{
	// 球の頂点バッファとインデックスバッファの作成
	// 表示する球の頂点の作成
	const int u_max = 30;
	const int v_max = 15;
	const int vertex_num = u_max * (v_max + 1);

	// サイズの確保
	m_sphereVertexData.resize(vertex_num);

	for (int v = 0; v <= v_max; v++)
	{
		for (int u = 0; u < u_max; u++)
		{
			float theta = XMConvertToRadians(180.0f * v / v_max);
			float phi = XMConvertToRadians(360.0f * u / u_max);
			float x = sinf(theta) * cosf(phi);
			float y = cosf(theta);
			float z = sinf(theta) * sinf(phi);
			m_sphereVertexData[u_max * v + u].position = XMVectorSet(x, y, z, 1.0f);
			m_sphereVertexData[u_max * v + u].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	// 頂点バッファの定義
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));						// 初期化
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;									// 動的使用法
	vertexBufferDesc.ByteWidth = sizeof(VertexBuffer) * vertex_num;					// 使用頂点数
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// 頂点バッファとして使う
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;


	// 頂点バッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA vertexSubresourceData;
	ZeroMemory(&vertexSubresourceData, sizeof(vertexSubresourceData));	// 初期化
	vertexSubresourceData.pSysMem = &m_sphereVertexData;				// バッファ・データの初期化


	// 頂点バッファの作成
	m_device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &m_sphereVertexBuffer);


	// インデックスデータの作成
	const int index_num = 2 * v_max * (u_max + 1);
	UINT indexData[index_num];
	int i = 0;
	for (int v = 0; v < v_max; v++)
	{
		for (int u = 0; u <= u_max; u++)
		{
			if (u == u_max) {
				indexData[i++] = v * u_max;
				indexData[i++] = (v + 1) * u_max;
			}
			else {
				indexData[i++] = (v * u_max) + u;
				indexData[i++] = indexData[i - 1] + u_max;
			}
		}
	}

	// インデックスバッファの定義
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));			// 初期化
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;					// デフォルト使用法
	indexBufferDesc.ByteWidth = sizeof(UINT) * index_num;			// 使用インデックス数
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;			// インデックスバッファとして使う
	indexBufferDesc.CPUAccessFlags = 0;


	// インデックスバッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA indexSubresourceData;
	ZeroMemory(&indexSubresourceData, sizeof(indexSubresourceData));	// 初期化
	indexSubresourceData.pSysMem = &indexData;					// バッファ・データの初期化


	// インデックスバッファの作成
	m_device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &m_sphereIndexBuffer);
	}
#pragma endregion
}



//----------------------------------------------------------------------
//! @summary   点の描画
//!
//! @parameter [position] 座標
//! @parameter [color] 色
//! @parameter [view] ビュー行列
//! @parameter [projection] 射影行列
//!
//! @return    なし
//----------------------------------------------------------------------
void Graphic3D::PrimitiveManager3D::DrawPoint(const Vector3& position, const XMFLOAT4& color, const Matrix& view, const Matrix& projection)
{
	// 頂点データの作成
	vector<VertexBuffer> vertexData = { VertexBuffer(position, color) };

	// 定数データの作成
	ConstBuffer constBufferData;
	constBufferData.view = view.Transpose();
	constBufferData.projection = projection.Transpose();
	constBufferData.world = Matrix::CreateTranslation(position).Transpose();

	// 頂点バッファの更新
	D3D11_MAPPED_SUBRESOURCE msr;
	m_deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, vertexData.data(), sizeof(VertexBuffer)); // 1頂点分コピー
	m_deviceContext->Unmap(m_vertexBuffer, 0);

	//定数バッファの内容更新
	m_deviceContext->UpdateSubresource(m_constBuffer, 0, NULL, &constBufferData, 0, 0);


	// 頂点バッファをセット
	UINT stride = sizeof(VertexBuffer);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);



	// 頂点シェーダーに値を渡す
	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);

	// ラスタライザステートの設定
	m_deviceContext->RSSetState(m_commonStates->Wireframe());		// ワイヤーフレームで表示する


	// 深度・ステンシルステートの設定
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


	// ブレンドステートの設定
	ID3D11BlendState* blendstate = m_commonStates->NonPremultiplied();			// 半透明合成（アルファ未乗算）
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);



	// プリミティブの種類を設定
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);	// 点リスト


	// シェーダーをセット
	// 頂点シェーダー
	m_deviceContext->VSSetShader(m_vertexShader->vertexShader, nullptr, 0);
	// ジオメトリシェーダー
	m_deviceContext->GSSetShader(nullptr, nullptr, 0);
	// ピクセルシェーダー
	m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


	// 入力レイアウトをセット
	m_deviceContext->IASetInputLayout(m_vertexShader->inputLayout);


	// 描画
	m_deviceContext->Draw(1, 0);
}



//----------------------------------------------------------------------
//! @summary   線の描画
//!
//! @parameter [startPosition] 始点
//! @parameter [endPosition] 終点
//! @parameter [color] 色
//! @parameter [view] ビュー行列
//! @parameter [projection] 射影行列
//!
//! @return    なし
//----------------------------------------------------------------------
void Graphic3D::PrimitiveManager3D::DrawLine(const Vector3& startPosition, const Vector3& endPosition, const XMFLOAT4& color, const Matrix& view, const Matrix& projection)
{
	// 頂点データの作成
	vector<VertexBuffer> vertexData = { VertexBuffer(startPosition, color), VertexBuffer(endPosition, color) };

	// 定数データの作成
	ConstBuffer constBufferData;
	constBufferData.view = view.Transpose();
	constBufferData.projection = projection.Transpose();
	constBufferData.world = Matrix::Identity.Transpose();

	// 頂点バッファの更新
	D3D11_MAPPED_SUBRESOURCE msr;
	m_deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, vertexData.data(), sizeof(VertexBuffer) * 2); // 2頂点分コピー
	m_deviceContext->Unmap(m_vertexBuffer, 0);

	//定数バッファの内容更新
	m_deviceContext->UpdateSubresource(m_constBuffer, 0, NULL, &constBufferData, 0, 0);

	// 頂点バッファをセット
	UINT stride = sizeof(VertexBuffer);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);


	// 頂点シェーダーに値を渡す
	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);

	// ラスタライザステートの設定
	m_deviceContext->RSSetState(m_commonStates->Wireframe());		// ワイヤーフレームにする


	// 深度・ステンシルステートの設定
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


	// ブレンドステートの設定
	ID3D11BlendState* blendstate = m_commonStates->NonPremultiplied();			// 半透明合成（アルファ未乗算）
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


	// プリミティブの種類を設定
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);	// 線リスト


	// シェーダーをセット
	// 頂点シェーダー
	m_deviceContext->VSSetShader(m_vertexShader->vertexShader, nullptr, 0);
	// ジオメトリシェーダー
	m_deviceContext->GSSetShader(nullptr, nullptr, 0);
	// ピクセルシェーダー
	m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


	// 入力レイアウトをセット
	m_deviceContext->IASetInputLayout(m_vertexShader->inputLayout);


	// 描画
	m_deviceContext->Draw(2, 0);
}



//----------------------------------------------------------------------
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
//----------------------------------------------------------------------
void Graphic3D::PrimitiveManager3D::DrawTriangle(const Vector3& vertex1, const Vector3& vertex2, const Vector3& vertex3, XMFLOAT4 color, const Matrix& view, const Matrix& projection, bool wireframeFlag)
{
	// 頂点データの作成
	vector<VertexBuffer> vertexData =
	{
		VertexBuffer(vertex1, color),
		VertexBuffer(vertex2, color),
		VertexBuffer(vertex3, color),
	};

	// インデックスデータの作成
	vector<UINT> indexData = { 0,1,2 };


	// 定数バッファの更新
	ConstBuffer constBufferData;
	constBufferData.view = view.Transpose();
	constBufferData.projection = projection.Transpose();
	constBufferData.world = Matrix::Identity.Transpose();

	// 頂点バッファの更新
	D3D11_MAPPED_SUBRESOURCE msr;
	m_deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, vertexData.data(), sizeof(VertexBuffer) * vertexData.size()); // 頂点分コピー
	m_deviceContext->Unmap(m_vertexBuffer, 0);

	// インデックスバッファの更新
	D3D11_MAPPED_SUBRESOURCE indexMappedSubresource;
	m_deviceContext->Map(m_indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &indexMappedSubresource);
	memcpy(indexMappedSubresource.pData, indexData.data(), sizeof(UINT) * indexData.size()); // インデックス分コピー
	m_deviceContext->Unmap(m_indexBuffer, 0);


	//定数バッファの内容更新
	m_deviceContext->UpdateSubresource(m_constBuffer, 0, NULL, &constBufferData, 0, 0);


	// 頂点バッファをセット
	UINT stride = sizeof(VertexBuffer);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// インデックスバッファをセット
	m_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);


	// 頂点シェーダーに値を渡す
	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);

	// ジオメトリシェーダーに値を渡す
	m_deviceContext->GSSetConstantBuffers(0, 1, &m_constBuffer);


	// ラスタライザステートの設定
	if(!wireframeFlag) m_deviceContext->RSSetState(m_commonStates->CullCounterClockwise());		// 反時計回りの面をカリングする
	else m_deviceContext->RSSetState(m_commonStates->Wireframe());							// ワイヤーフレームにする

	// 深度・ステンシルステートの設定
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


	// ブレンドステートの設定
	ID3D11BlendState* blendstate = m_commonStates->NonPremultiplied();			// 半透明合成（アルファ未乗算）
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


	// プリミティブの種類を設定
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 三角形リスト


	// シェーダーをセット
	// 頂点シェーダー
	m_deviceContext->VSSetShader(m_vertexShader2->vertexShader, nullptr, 0);
	// ジオメトリシェーダー
	m_deviceContext->GSSetShader(m_geometryShader, nullptr, 0);
	// ピクセルシェーダー
	m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


	// 入力レイアウトをセット
	m_deviceContext->IASetInputLayout(m_vertexShader2->inputLayout);


	// 描画
	m_deviceContext->DrawIndexed(indexData.size(), 0, 0);
}



//----------------------------------------------------------------------
//! @summary   クワッドの描画
//!
//! @parameter [world] ワールド行列
//! @parameter [color] 色
//! @parameter [view] ビュー行列
//! @parameter [projection] 射影行列
//! @parameter [wireframeFlag] ワイヤーフレーム表示フラグ
//!
//! @return    なし
//----------------------------------------------------------------------
void Graphic3D::PrimitiveManager3D::DrawQuad(const Matrix& world, XMFLOAT4 color, const Matrix& view, const Matrix& projection, bool wireframeFlag)
{
	// 表示する四角形の頂点の作成
	vector<VertexBuffer> vertexData =
	{
		// 左上
		VertexBuffer(Vector3(-0.5f, 0.5f, 0.0f), color),
		// 右上
		VertexBuffer(Vector3(0.5f, 0.5f, 0.0f), color),
		// 右下
		VertexBuffer(Vector3(0.5f, -0.5f, 0.0f), color),
		// 左下
		VertexBuffer(Vector3(-0.5f, -0.5f, 0.0f), color),
	};

	// 定数バッファの作成
	ConstBuffer constBufferData;
	constBufferData.view = view.Transpose();
	constBufferData.projection = projection.Transpose();
	constBufferData.world = world.Transpose();

	vector<UINT> indexData;

	// 塗りつぶしあり
	if (!wireframeFlag)
	{
		indexData =
		{
			0,1,3,
			1,2,3,
		};

		// プリミティブの種類を設定
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 三角形リスト
	}
	else    // 塗りつぶしなし
	{
		// インデックスバッファのサブリソースの初期値
		indexData =
		{
			0,1,2,3,0
		};


		// プリミティブの種類を設定
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);	// 線ストリップ
	}


	// 頂点バッファの更新
	D3D11_MAPPED_SUBRESOURCE msr;
	m_deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, vertexData.data(), sizeof(VertexBuffer) * vertexData.size()); // 頂点分コピー
	m_deviceContext->Unmap(m_vertexBuffer, 0);


	// インデックスバッファの更新
	D3D11_MAPPED_SUBRESOURCE indexMappedSubresource;
	m_deviceContext->Map(m_indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &indexMappedSubresource);
	memcpy(indexMappedSubresource.pData, indexData.data(), sizeof(UINT) * indexData.size()); // インデックス分コピー
	m_deviceContext->Unmap(m_indexBuffer, 0);


	//定数バッファの内容更新
	m_deviceContext->UpdateSubresource(m_constBuffer, 0, NULL, &constBufferData, 0, 0);


	// 頂点バッファをセット
	UINT stride = sizeof(VertexBuffer);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// インデックスバッファをセット
	m_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);


	// 頂点シェーダーに値を渡す
	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);

	// ジオメトリシェーダーに値を渡す
	m_deviceContext->GSSetConstantBuffers(0, 1, &m_constBuffer);


	// ラスタライザステートの設定
	if (!wireframeFlag) m_deviceContext->RSSetState(m_commonStates->CullCounterClockwise());		// 反時計回りの面をカリングする
	else m_deviceContext->RSSetState(m_commonStates->Wireframe());							// ワイヤーフレームにする

	// 深度・ステンシルステートの設定
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


	// ブレンドステートの設定
	ID3D11BlendState* blendstate = m_commonStates->NonPremultiplied();			// 半透明合成（アルファ未乗算）
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


	// シェーダーをセット
	// 頂点シェーダー
	m_deviceContext->VSSetShader(m_vertexShader2->vertexShader, nullptr, 0);
	// ジオメトリシェーダー
	m_deviceContext->GSSetShader(m_geometryShader, nullptr, 0);
	// ピクセルシェーダー
	m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


	// 入力レイアウトをセット
	m_deviceContext->IASetInputLayout(m_vertexShader2->inputLayout);


	// 描画
	m_deviceContext->DrawIndexed(indexData.size(), 0, 0);
}



//----------------------------------------------------------------------
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
//----------------------------------------------------------------------
void Graphic3D::PrimitiveManager3D::DrawCube(const Vector3& position, const Vector3& rotation, const Vector3& size, XMFLOAT4 color, const Matrix& view, const Matrix& projection, bool wireframeFlag)
{
	for (vector<VertexBuffer>::iterator itr = m_cubeVertexData.begin(); itr != m_cubeVertexData.end(); ++itr)
	{
		(*itr).color = color;
	}

	// 定数バッファの作成
	ConstBuffer constBufferData;
	constBufferData.view = view.Transpose();
	constBufferData.projection = projection.Transpose();

	Matrix trans = Matrix::CreateTranslation(position);
	Matrix rota = Matrix::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
	Matrix scale = Matrix::CreateScale(size);
	constBufferData.world = rota*scale*trans;


	// 頂点バッファの更新
	D3D11_MAPPED_SUBRESOURCE msr;
	m_deviceContext->Map(m_cubeVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, m_cubeVertexData.data(), sizeof(VertexBuffer) * m_cubeVertexData.size()); // 頂点分コピー
	m_deviceContext->Unmap(m_cubeVertexBuffer, 0);


	// 塗りつぶしなし
	if (!wireframeFlag)
	{
		// 頂点バッファをセット
		UINT stride = sizeof(VertexBuffer);
		UINT offset = 0;
		m_deviceContext->IASetVertexBuffers(0, 1, &m_cubeVertexBuffer, &stride, &offset);


		// インデックスバッファをセット
		m_deviceContext->IASetIndexBuffer(m_cubeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


		//定数バッファの内容更新
		m_deviceContext->UpdateSubresource(m_constBuffer, 0, NULL, &constBufferData, 0, 0);

		// 頂点シェーダーに値を渡す
		m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);

		// ジオメトリシェーダーに値を渡す
		m_deviceContext->GSSetConstantBuffers(0, 1, &m_constBuffer);


		// ラスタライザステートの設定
		m_deviceContext->RSSetState(m_commonStates->CullClockwise());		// 時計回りの面をカリングする


		// 深度・ステンシルステートの設定
		m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


		// ブレンドステートの設定
		ID3D11BlendState* blendstate = m_commonStates->NonPremultiplied();			// 半透明合成（アルファ未乗算）
		m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


		// プリミティブの種類を設定
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 三角形リスト


		// シェーダーをセット
		// 頂点シェーダー
		m_deviceContext->VSSetShader(m_vertexShader2->vertexShader, nullptr, 0);
		// ジオメトリシェーダー
		m_deviceContext->GSSetShader(m_geometryShader, nullptr, 0);
		// ピクセルシェーダー
		m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


		// 入力レイアウトをセット
		m_deviceContext->IASetInputLayout(m_vertexShader2->inputLayout);


		// 描画
		m_deviceContext->DrawIndexed(1, 0, 0);
	}
	else
	{
		// 頂点バッファをセット
		UINT stride = sizeof(VertexBuffer);
		UINT offset = 0;
		m_deviceContext->IASetVertexBuffers(0, 1, &m_cubeVertexBuffer, &stride, &offset);


		// インデックスバッファをセット
		m_deviceContext->IASetIndexBuffer(m_cubeWireframeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


		//定数バッファの内容更新
		m_deviceContext->UpdateSubresource(m_constBuffer, 0, NULL, &constBufferData, 0, 0);


		// 頂点シェーダーに値を渡す
		m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);


		// ラスタライザステートの設定
		m_deviceContext->RSSetState(m_commonStates->Wireframe());		// ワイヤーフレーム


		// 深度・ステンシルステートの設定
		m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


		// ブレンドステートの設定
		ID3D11BlendState* blendstate = m_commonStates->NonPremultiplied();			// 半透明合成（アルファ未乗算）
		m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


		// プリミティブの種類を設定
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);					// 線リスト


		// シェーダーをセット
		// 頂点シェーダー
		m_deviceContext->VSSetShader(m_vertexShader->vertexShader, nullptr, 0);
		// ジオメトリシェーダー
		m_deviceContext->GSSetShader(nullptr, nullptr, 0);
		// ピクセルシェーダー
		m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


		// 入力レイアウトをセット
		m_deviceContext->IASetInputLayout(m_vertexShader->inputLayout);


		// 描画
		m_deviceContext->DrawIndexed(24, 0, 0);
	}
}


void Graphic3D::PrimitiveManager3D::DrawCube(const Vector3& position, const Quaternion& rotation, const Vector3& size, XMFLOAT4 color, const Matrix& view, const Matrix& projection, bool wireframeFlag)
{
	// 頂点データの色情報の更新
	for (vector<VertexBuffer>::iterator itr = m_cubeVertexData.begin(); itr != m_cubeVertexData.end(); ++itr)
	{
		(*itr).color = color;
	}

	// 定数バッファの作成
	ConstBuffer constBufferData;
	constBufferData.view = view.Transpose();
	constBufferData.projection = projection.Transpose();

	Matrix trans = Matrix::CreateTranslation(position);
	Matrix rota = Matrix::CreateFromQuaternion(rotation);
	Matrix scale = Matrix::CreateScale(size);
	Matrix world = scale*rota*trans;
	constBufferData.world = world.Transpose();

	// 頂点バッファの更新
	D3D11_MAPPED_SUBRESOURCE msr;
	m_deviceContext->Map(m_cubeVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, m_cubeVertexData.data(), sizeof(VertexBuffer) * m_cubeVertexData.size()); // 頂点分コピー
	m_deviceContext->Unmap(m_cubeVertexBuffer, 0);


	// 塗りつぶしなし
	if (!wireframeFlag)
	{
		// 頂点バッファをセット
		UINT stride = sizeof(VertexBuffer);
		UINT offset = 0;
		m_deviceContext->IASetVertexBuffers(0, 1, &m_cubeVertexBuffer, &stride, &offset);


		// インデックスバッファをセット
		m_deviceContext->IASetIndexBuffer(m_cubeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


		//定数バッファの内容更新
		m_deviceContext->UpdateSubresource(m_constBuffer, 0, NULL, &constBufferData, 0, 0);

		// 頂点シェーダーに値を渡す
		m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);

		// ジオメトリシェーダーに値を渡す
		m_deviceContext->GSSetConstantBuffers(0, 1, &m_constBuffer);

		// ラスタライザステートの設定
		m_deviceContext->RSSetState(m_commonStates->CullClockwise());		// 時計回りの面をカリングする


		// 深度・ステンシルステートの設定
		m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


		// ブレンドステートの設定
		ID3D11BlendState* blendstate = m_commonStates->NonPremultiplied();			// 半透明合成（アルファ未乗算）
		m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


		// プリミティブの種類を設定
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 三角形リスト


		// シェーダーをセット
		// 頂点シェーダー
		m_deviceContext->VSSetShader(m_vertexShader2->vertexShader, nullptr, 0);
		// ジオメトリシェーダー
		m_deviceContext->GSSetShader(m_geometryShader, nullptr, 0);
		// ピクセルシェーダー
		m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


		// 入力レイアウトをセット
		m_deviceContext->IASetInputLayout(m_vertexShader2->inputLayout);


		// 描画
		m_deviceContext->DrawIndexed(36, 0, 0);
	}
	else
	{
		// 頂点バッファをセット
		UINT stride = sizeof(VertexBuffer);
		UINT offset = 0;
		m_deviceContext->IASetVertexBuffers(0, 1, &m_cubeVertexBuffer, &stride, &offset);


		// インデックスバッファをセット
		m_deviceContext->IASetIndexBuffer(m_cubeWireframeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


		//定数バッファの内容更新
		m_deviceContext->UpdateSubresource(m_constBuffer, 0, NULL, &constBufferData, 0, 0);


		// 頂点シェーダーに値を渡す
		m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);


		// ラスタライザステートの設定
		m_deviceContext->RSSetState(m_commonStates->Wireframe());		// ワイヤーフレーム


		// 深度・ステンシルステートの設定
		m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


		// ブレンドステートの設定
		ID3D11BlendState* blendstate = m_commonStates->NonPremultiplied();			// 半透明合成（アルファ未乗算）
		m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


		// プリミティブの種類を設定
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);					// 線リスト


		// シェーダーをセット
		// 頂点シェーダー
		m_deviceContext->VSSetShader(m_vertexShader->vertexShader, nullptr, 0);
		// ジオメトリシェーダー
		m_deviceContext->GSSetShader(nullptr, nullptr, 0);
		// ピクセルシェーダー
		m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


		// 入力レイアウトをセット
		m_deviceContext->IASetInputLayout(m_vertexShader->inputLayout);


		// 描画
		m_deviceContext->DrawIndexed(24, 0, 0);
	}
}



//----------------------------------------------------------------------
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
//----------------------------------------------------------------------
void Graphic3D::PrimitiveManager3D::DrawCube(const Vector3& position, const Quaternion& rotation, const Vector3& size, XMFLOAT4 color, const Matrix& view, const Matrix& projection, function<void()> lambda)
{
	// 頂点データの色情報の更新
	for (vector<VertexBuffer>::iterator itr = m_cubeVertexData.begin(); itr != m_cubeVertexData.end(); ++itr)
	{
		(*itr).color = color;
	}

	// 定数バッファの作成
	ConstBuffer constBufferData;
	constBufferData.view = view.Transpose();
	constBufferData.projection = projection.Transpose();

	Matrix trans = Matrix::CreateTranslation(position);
	Matrix rota = Matrix::CreateFromQuaternion(rotation);
	Matrix scale = Matrix::CreateScale(size);
	Matrix world = scale*rota*trans;
	constBufferData.world = world.Transpose();

	// 頂点バッファの更新
	D3D11_MAPPED_SUBRESOURCE msr;
	m_deviceContext->Map(m_cubeVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, m_cubeVertexData.data(), sizeof(VertexBuffer) * m_cubeVertexData.size()); // 頂点分コピー
	m_deviceContext->Unmap(m_cubeVertexBuffer, 0);


	// 頂点バッファをセット
	UINT stride = sizeof(VertexBuffer);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_cubeVertexBuffer, &stride, &offset);


	// インデックスバッファをセット
	m_deviceContext->IASetIndexBuffer(m_cubeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	//定数バッファの内容更新
	m_deviceContext->UpdateSubresource(m_constBuffer, 0, NULL, &constBufferData, 0, 0);

	// 頂点シェーダーに値を渡す
	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);

	// ジオメトリシェーダーに値を渡す
	m_deviceContext->GSSetConstantBuffers(0, 1, &m_constBuffer);


	lambda();

	// 描画
	m_deviceContext->DrawIndexed(36, 0, 0);

	m_deviceContext->GSSetShader(nullptr, nullptr, 0);
}



//----------------------------------------------------------------------
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
//----------------------------------------------------------------------
void Graphic3D::PrimitiveManager3D::DrawSphere(const Vector3& position, float radius, const XMFLOAT4& color, const Matrix& view, const Matrix& projection, bool wireframeFlag)
{
	// 頂点データの色情報の更新
	for (vector<VertexBuffer>::iterator itr = m_sphereVertexData.begin(); itr != m_sphereVertexData.end(); ++itr)
	{
		(*itr).color = color;
	}

	// 定数バッファの作成
	ConstBuffer constBufferData;
	constBufferData.view = view.Transpose();
	constBufferData.projection = projection.Transpose();
	Matrix world = Matrix::CreateScale(radius) * Matrix::CreateTranslation(position);
	constBufferData.world = world.Transpose();

	// 頂点バッファの更新
	D3D11_MAPPED_SUBRESOURCE msr;
	m_deviceContext->Map(m_sphereVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, m_sphereVertexData.data(), sizeof(VertexBuffer) * m_sphereVertexData.size()); // 頂点分コピー
	m_deviceContext->Unmap(m_sphereVertexBuffer, 0);


	// 頂点バッファをセット
	UINT stride = sizeof(VertexBuffer);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_sphereVertexBuffer, &stride, &offset);


	// インデックスバッファをセット
	m_deviceContext->IASetIndexBuffer(m_sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	//定数バッファの内容更新
	m_deviceContext->UpdateSubresource(m_constBuffer, 0, NULL, &constBufferData, 0, 0);


	// ラスタライザステートの設定
	if (!wireframeFlag)
	{
		// 頂点シェーダーに値を渡す
		m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);

		// ジオメトリシェーダーに値を渡す
		m_deviceContext->GSSetConstantBuffers(0, 1, &m_constBuffer);


		// ラスタライザステートの設定
		m_deviceContext->RSSetState(m_commonStates->CullCounterClockwise());		// 反時計回りの面をカリングする

		// シェーダーをセット
		// 頂点シェーダー
		m_deviceContext->VSSetShader(m_vertexShader2->vertexShader, nullptr, 0);
		// ジオメトリシェーダー
		m_deviceContext->GSSetShader(m_geometryShader, nullptr, 0);

		// 入力レイアウトをセット
		m_deviceContext->IASetInputLayout(m_vertexShader2->inputLayout);
	}
	else
	{
		// 頂点シェーダーに値を渡す
		m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);

		// ラスタライザステートの設定
		m_deviceContext->RSSetState(m_commonStates->Wireframe());			// ワイヤーフレーム表示

		// シェーダーをセット
		// 頂点シェーダー
		m_deviceContext->VSSetShader(m_vertexShader->vertexShader, nullptr, 0);
		// ジオメトリシェーダー
		m_deviceContext->GSSetShader(nullptr, nullptr, 0);

		// 入力レイアウトをセット
		m_deviceContext->IASetInputLayout(m_vertexShader->inputLayout);
	}

	// 深度・ステンシルステートの設定
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


	// ブレンドステートの設定
	ID3D11BlendState* blendstate = m_commonStates->NonPremultiplied();			// 半透明合成（アルファ未乗算）
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


	// プリミティブの種類を設定
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	// 三角形ストリップ


	// シェーダーをセット
	// ピクセルシェーダー
	m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


	// 描画
	m_deviceContext->DrawIndexed(930, 0, 0);

	m_deviceContext->GSSetShader(nullptr, nullptr, 0);
}



//----------------------------------------------------------------------
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
//----------------------------------------------------------------------
void Graphic3D::PrimitiveManager3D::DrawSphere(const Vector3& position, float radius, const XMFLOAT4& color, const Matrix& view, const Matrix& projection, function<void()> lambda)
{
	// 頂点データの色情報の更新
	for (vector<VertexBuffer>::iterator itr = m_sphereVertexData.begin(); itr != m_sphereVertexData.end(); ++itr)
	{
		(*itr).color = color;
	}

	// 定数バッファの作成
	ConstBuffer constBufferData;
	constBufferData.view = view.Transpose();
	constBufferData.projection = projection.Transpose();
	Matrix world = Matrix::CreateScale(radius) * Matrix::CreateTranslation(position);
	constBufferData.world = world.Transpose();

	// 頂点バッファの更新
	D3D11_MAPPED_SUBRESOURCE msr;
	m_deviceContext->Map(m_sphereVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, m_sphereVertexData.data(), sizeof(VertexBuffer) * m_sphereVertexData.size()); // 頂点分コピー
	m_deviceContext->Unmap(m_sphereVertexBuffer, 0);


	// 頂点バッファをセット
	UINT stride = sizeof(VertexBuffer);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_sphereVertexBuffer, &stride, &offset);


	// インデックスバッファをセット
	m_deviceContext->IASetIndexBuffer(m_sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	//定数バッファの内容更新
	m_deviceContext->UpdateSubresource(m_constBuffer, 0, NULL, &constBufferData, 0, 0);


	// 頂点シェーダーに値を渡す
	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);

	// ジオメトリシェーダーに値を渡す
	m_deviceContext->GSSetConstantBuffers(0, 1, &m_constBuffer);


	// ラスタライザステートの設定
	m_deviceContext->RSSetState(m_commonStates->CullCounterClockwise());		// 反時計回りの面をカリングする

	// シェーダーをセット
	// 頂点シェーダー
	m_deviceContext->VSSetShader(m_vertexShader2->vertexShader, nullptr, 0);
	// ジオメトリシェーダー
	m_deviceContext->GSSetShader(m_geometryShader, nullptr, 0);

	// 入力レイアウトをセット
	m_deviceContext->IASetInputLayout(m_vertexShader2->inputLayout);

	// 深度・ステンシルステートの設定
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


	// ブレンドステートの設定
	ID3D11BlendState* blendstate = m_commonStates->NonPremultiplied();			// 半透明合成（アルファ未乗算）
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


	// プリミティブの種類を設定
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	// 三角形ストリップ


	// シェーダーをセット
	// ピクセルシェーダー
	m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


	lambda();


	// 描画
	m_deviceContext->DrawIndexed(930, 0, 0);

	m_deviceContext->GSSetShader(nullptr, nullptr, 0);
}
