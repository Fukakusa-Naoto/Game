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
#include "../Utility/ShaderManager.h"
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
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
	m_pixelShader(nullptr)
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
	m_pixelShader = nullptr;

	m_constBuffer->Release();
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
	m_vertexShader = Utility::ShaderManager::GetInstance()->LoadVertexShader(L"Primitive3DVertexShader", INPUT_LAYOUT);
	// ピクセルシェーダーのインスタンスの取得
	m_pixelShader = Utility::ShaderManager::GetInstance()->LoadPixelShader(L"Primitive3DPixelShader");


	// 定数バッファの定義
	D3D11_BUFFER_DESC constBufferDesc;
	ZeroMemory(&constBufferDesc, sizeof(constBufferDesc));
	constBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufferDesc.ByteWidth = sizeof(ConstBuffer);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = 0;

	// 定数バッファの作成
	m_device->CreateBuffer(&constBufferDesc, nullptr, &m_constBuffer);

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
	// 表示する点の頂点の作成
	vector<VertexBuffer> vertices =
	{
		VertexBuffer(position, color)
	};


	// 頂点バッファの定義
	D3D11_BUFFER_DESC vertexBufferDesc;

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));						// 初期化
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;									// デフォルト使用法
	vertexBufferDesc.ByteWidth = sizeof(VertexBuffer) * vertices.size();			// 使用頂点数
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// 頂点バッファとして使う
	vertexBufferDesc.CPUAccessFlags = 0;


	// 頂点バッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA vertexSubresourceData;

	ZeroMemory(&vertexSubresourceData, sizeof(vertexSubresourceData));	// 初期化
	vertexSubresourceData.pSysMem = vertices.data();					// バッファ・データの初期化


	// 頂点バッファの作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	m_device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, vertexBuffer.GetAddressOf());


	// 頂点バッファをセット
	UINT stride = sizeof(VertexBuffer);
	UINT offset = 0;
	ID3D11Buffer* pVertexBuffer = vertexBuffer.Get();

	m_deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);


	// 定数バッファの更新
	ConstBuffer constBufferData;
	constBufferData.view = view.Transpose();
	constBufferData.projection = projection.Transpose();
	constBufferData.world = Matrix::CreateTranslation(position).Transpose();

	//定数バッファの内容更新
	m_deviceContext->UpdateSubresource(m_constBuffer, 0, NULL, &constBufferData, 0, 0);

	// 頂点シェーダーに値を渡す
	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);


	// 深度バッファに書き込み参照する
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);	// 深度テストをする・ステンシルテストをしない


	// ブレンドステートをセット
	ID3D11BlendState* blendstate = m_commonStates->AlphaBlend();			// 半透明合成（アルファ乗算済み）
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


	// プリミティブの種類を設定
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);	// 点リスト


	// シェーダーをセット
	// 頂点シェーダー
	m_deviceContext->VSSetShader(m_vertexShader->vertexShader, nullptr, 0);
	// ピクセルシェーダー
	m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


	// 入力レイアウトをセット
	m_deviceContext->IASetInputLayout(m_vertexShader->inputLayout);


	// 描画
	m_deviceContext->Draw(vertices.size(), 0);
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
	// 表示する点の頂点の作成
	vector<VertexBuffer> vertices =
	{
		VertexBuffer(startPosition, color),
		VertexBuffer(endPosition, color)
	};


	// 頂点バッファの定義
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));						// 初期化
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;									// デフォルト使用法
	vertexBufferDesc.ByteWidth = sizeof(VertexBuffer) * vertices.size();			// 使用頂点数
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// 頂点バッファとして使う
	vertexBufferDesc.CPUAccessFlags = 0;


	// 頂点バッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA vertexSubresourceData;
	ZeroMemory(&vertexSubresourceData, sizeof(vertexSubresourceData));	// 初期化
	vertexSubresourceData.pSysMem = vertices.data();					// バッファ・データの初期化


	// 頂点バッファの作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	m_device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, vertexBuffer.GetAddressOf());


	// 頂点バッファをセット
	UINT stride = sizeof(VertexBuffer);
	UINT offset = 0;
	ID3D11Buffer* pVertexBuffer = vertexBuffer.Get();
	m_deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);


	// 定数バッファの更新
	ConstBuffer constBufferData;
	constBufferData.view = view.Transpose();
	constBufferData.projection = projection.Transpose();
	constBufferData.world = Matrix::Identity.Transpose();

	//定数バッファの内容更新
	m_deviceContext->UpdateSubresource(m_constBuffer, 0, NULL, &constBufferData, 0, 0);

	// 頂点シェーダーに値を渡す
	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);


	// 深度バッファに書き込み参照する
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);	// 深度テストをする・ステンシルテストをしない


	// ブレンドステートをセット
	ID3D11BlendState* blendstate = m_commonStates->AlphaBlend();			// 半透明合成（アルファ乗算済み）
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


	// プリミティブの種類を設定
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);	// 線リスト


	// シェーダーをセット
	// 頂点シェーダー
	m_deviceContext->VSSetShader(m_vertexShader->vertexShader, nullptr, 0);
	// ピクセルシェーダー
	m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


	// 入力レイアウトをセット
	m_deviceContext->IASetInputLayout(m_vertexShader->inputLayout);


	// 描画
	m_deviceContext->Draw(vertices.size(), 0);
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
void Graphic3D::PrimitiveManager3D::DrawTriangle(const DirectX::SimpleMath::Vector3& vertex1, const DirectX::SimpleMath::Vector3& vertex2, const DirectX::SimpleMath::Vector3& vertex3, DirectX::XMFLOAT4 color, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection, bool wireframeFlag)
{
	// 表示する三角形の頂点の作成
	vector<VertexBuffer> vertices =
	{
		VertexBuffer(vertex1, color),
		VertexBuffer(vertex2, color),
		VertexBuffer(vertex3, color)
	};


	// インデックスデータの作成
	vector<UINT> indexData;
	if (!wireframeFlag) indexData = { 0,1,2 };
	else indexData = { 0,1,2,0 };


	// 定数バッファの作成
	ConstBuffer constBufferData;
	constBufferData.view = view.Transpose();
	constBufferData.projection = projection.Transpose();
	constBufferData.world = Matrix::Identity.Transpose();


	// 頂点バッファの定義
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));						// 初期化
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;									// デフォルト使用法
	vertexBufferDesc.ByteWidth = sizeof(VertexBuffer) * vertices.size();			// 使用頂点数
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// 頂点バッファとして使う
	vertexBufferDesc.CPUAccessFlags = 0;


	// 頂点バッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA vertexSubresourceData;
	ZeroMemory(&vertexSubresourceData, sizeof(vertexSubresourceData));	// 初期化
	vertexSubresourceData.pSysMem = vertices.data();					// バッファ・データの初期化


	// 頂点バッファの作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	m_device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, vertexBuffer.GetAddressOf());


	// 頂点バッファをセット
	UINT stride = sizeof(VertexBuffer);
	UINT offset = 0;
	ID3D11Buffer* pVertexBuffer = vertexBuffer.Get();
	m_deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);


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
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	m_device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, indexBuffer.GetAddressOf());


	// インデックスバッファをセット
	ID3D11Buffer* pIndexBuffer = indexBuffer.Get();
	m_deviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	//定数バッファの内容更新
	m_deviceContext->UpdateSubresource(m_constBuffer, 0, NULL, &constBufferData, 0, 0);


	// 頂点シェーダーに値を渡す
	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);


	// ラスタライザステートの設定
	m_deviceContext->RSSetState(m_commonStates->CullCounterClockwise());		// 反時計回りの面をカリングする


	// 深度バッファに書き込み参照する
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);	// 深度テストをする・ステンシルテストをしない


	// ブレンドステートをセット
	ID3D11BlendState* blendstate = m_commonStates->AlphaBlend();			// 半透明合成（アルファ乗算済み）
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


	// プリミティブの種類を設定
	if (!wireframeFlag) m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 三角形リスト
	else m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);	// 線ストリップ


	// シェーダーをセット
	// 頂点シェーダー
	m_deviceContext->VSSetShader(m_vertexShader->vertexShader, nullptr, 0);
	// ピクセルシェーダー
	m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);

	// 入力レイアウトをセット
	m_deviceContext->IASetInputLayout(m_vertexShader->inputLayout);


	// 描画
	m_deviceContext->DrawIndexed(indexData.size(), 0, 0);
}



//----------------------------------------------------------------------
//! @summary   立方体の描画
//!
//! @parameter [world] ワールド行列
//! @parameter [size] サイズ(x:幅, y:高さ, z:奥行)
//! @parameter [color] 色
//! @parameter [view] ビュー行列
//! @parameter [projection] 射影行列
//! @parameter [wireframeFlag] ワイヤーフレーム表示フラグ
//!
//! @return    なし
//----------------------------------------------------------------------
void Graphic3D::PrimitiveManager3D::DrawCube(const Matrix& world, const Vector3& size, XMFLOAT4 color, const Matrix& view, const Matrix& projection, bool wireframeFlag)
{
	// 表示する立方体の頂点の作成
	vector<VertexBuffer> vertices =
	{
		VertexBuffer(Vector3(-size.x, size.y, -size.z), color),
		VertexBuffer(Vector3(size.x, size.y, -size.z), color),
		VertexBuffer(Vector3(size.x, -size.y, -size.z), color),
		VertexBuffer(Vector3(-size.x, -size.y, -size.z), color),
		VertexBuffer(Vector3(-size.x, size.y, size.z), color),
		VertexBuffer(Vector3(size.x, size.y, size.z), color),
		VertexBuffer(Vector3(size.x, -size.y, size.z), color),
		VertexBuffer(Vector3(-size.x, -size.y, size.z), color),
	};


	// インデックスデータの作成
	vector<UINT> indexData;
	if (!wireframeFlag)
	{
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
	}
	else
	{
		indexData =
		{
			0,1,1,2,2,3,3,0,	// 奥
			4,5,5,6,6,7,7,4,	// 手前
			0,4,3,7,			// 左上・左下
			5,1,6,2,			// 右上・右下
		};
	}


	// 定数バッファの作成
	ConstBuffer constBufferData;
	constBufferData.view = view.Transpose();
	constBufferData.projection = projection.Transpose();
	constBufferData.world = world.Transpose();


	// 頂点バッファの定義
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));						// 初期化
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;									// デフォルト使用法
	vertexBufferDesc.ByteWidth = sizeof(VertexBuffer) * vertices.size();			// 使用頂点数
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// 頂点バッファとして使う
	vertexBufferDesc.CPUAccessFlags = 0;


	// 頂点バッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA vertexSubresourceData;
	ZeroMemory(&vertexSubresourceData, sizeof(vertexSubresourceData));	// 初期化
	vertexSubresourceData.pSysMem = vertices.data();					// バッファ・データの初期化


	// 頂点バッファの作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	m_device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, vertexBuffer.GetAddressOf());


	// 頂点バッファをセット
	UINT stride = sizeof(VertexBuffer);
	UINT offset = 0;
	ID3D11Buffer* pVertexBuffer = vertexBuffer.Get();
	m_deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);


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
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	m_device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, indexBuffer.GetAddressOf());


	// インデックスバッファをセット
	ID3D11Buffer* pIndexBuffer = indexBuffer.Get();
	m_deviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	//定数バッファの内容更新
	m_deviceContext->UpdateSubresource(m_constBuffer, 0, NULL, &constBufferData, 0, 0);


	// 頂点シェーダーに値を渡す
	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);


	// ラスタライザステートの設定
	m_deviceContext->RSSetState(m_commonStates->CullClockwise());		// 時計回りの面をカリングする


	// 深度・ステンシルステートの設定
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


	// ブレンドステートの設定
	ID3D11BlendState* blendstate = m_commonStates->NonPremultiplied();			// 半透明合成（アルファ未乗算）
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


	// プリミティブの種類を設定
	if (!wireframeFlag) m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 三角形リスト
	else m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);					// 線リスト


	// シェーダーをセット
	// 頂点シェーダー
	m_deviceContext->VSSetShader(m_vertexShader->vertexShader, nullptr, 0);
	// ピクセルシェーダー
	m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


	// 入力レイアウトをセット
	m_deviceContext->IASetInputLayout(m_vertexShader->inputLayout);


	// 描画
	m_deviceContext->DrawIndexed(indexData.size(), 0, 0);
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
	const int u_max = 30;
	const int v_max = 15;
	const int vertex_num = u_max * (v_max + 1);

	VertexBuffer vertices[vertex_num];
	for (int v = 0; v <= v_max; v++)
	{
		for (int u = 0; u < u_max; u++)
		{
			float theta = XMConvertToRadians(180.0f * v / v_max);
			float phi = XMConvertToRadians(360.0f * u / u_max);
			float x = sinf(theta) * cosf(phi);
			float y = cosf(theta);
			float z = sinf(theta) * sinf(phi);
			vertices[u_max * v + u].position = XMVectorSet(x, y, z, 1.0f);
		}
	}


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


	// 定数バッファの作成
	ConstBuffer constBufferData;
	constBufferData.view = view.Transpose();
	constBufferData.projection = projection.Transpose();
	Matrix world = Matrix::CreateScale(radius) * Matrix::CreateTranslation(position);
	constBufferData.world = world.Transpose();


	// 頂点バッファの定義
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));						// 初期化
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;									// デフォルト使用法
	vertexBufferDesc.ByteWidth = sizeof(VertexBuffer) * vertex_num;					// 使用頂点数
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// 頂点バッファとして使う
	vertexBufferDesc.CPUAccessFlags = 0;


	// 頂点バッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA vertexSubresourceData;
	ZeroMemory(&vertexSubresourceData, sizeof(vertexSubresourceData));	// 初期化
	vertexSubresourceData.pSysMem = &vertices;							// バッファ・データの初期化


	// 頂点バッファの作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	m_device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, vertexBuffer.GetAddressOf());


	// 頂点バッファをセット
	UINT stride = sizeof(VertexBuffer);
	UINT offset = 0;
	ID3D11Buffer* pVertexBuffer = vertexBuffer.Get();
	m_deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);


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
	indexSubresourceData.pSysMem = &indexData;							// バッファ・データの初期化


	// インデックスバッファの作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	m_device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, indexBuffer.GetAddressOf());


	// インデックスバッファをセット
	ID3D11Buffer* pIndexBuffer = indexBuffer.Get();
	m_deviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	//定数バッファの内容更新
	m_deviceContext->UpdateSubresource(m_constBuffer, 0, NULL, &constBufferData, 0, 0);


	// 頂点シェーダーに値を渡す
	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);


	// ラスタライザステートの設定
	if (!wireframeFlag) m_deviceContext->RSSetState(m_commonStates->CullCounterClockwise());		// 反時計回りの面をカリングする
	else m_deviceContext->RSSetState(m_commonStates->Wireframe());									// ワイヤーフレーム表示


	// 深度・ステンシルステートの設定
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


	// ブレンドステートの設定
	ID3D11BlendState* blendstate = m_commonStates->NonPremultiplied();			// 半透明合成（アルファ未乗算）
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


	// プリミティブの種類を設定
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	// 三角形ストリップ


	// シェーダーをセット
	// 頂点シェーダー
	m_deviceContext->VSSetShader(m_vertexShader->vertexShader, nullptr, 0);
	// ピクセルシェーダー
	m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


	// 入力レイアウトをセット
	m_deviceContext->IASetInputLayout(m_vertexShader->inputLayout);


	// 描画
	m_deviceContext->DrawIndexed(index_num, 0, 0);
}
