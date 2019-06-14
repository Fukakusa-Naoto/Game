//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		PrimitiveManager2D.h
//!
//! @summary	2Dのプリミティブ描画のマネージャークラスのソースファイル
//!
//! @date		2018.09.19
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <標準ヘッダファイル>
#include <vector>

// <自作ヘッダファイル>
#include "PrimitiveManager2D.h"
#include "../Common/DeviceResources.h"
#include "../Math/Math.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace Library;


// 定数の定義 ==============================================================
// 入力要素配列
const vector<D3D11_INPUT_ELEMENT_DESC> Graphic2D::PrimitiveManager2D::INPUT_LAYOUT =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Graphic2D::PrimitiveManager2D::PrimitiveManager2D() :
	m_device(nullptr),
	m_deviceContext(nullptr),
	m_commonStates(nullptr),
	m_vertexShader(nullptr),
	m_pixelShader(nullptr),
	m_defaultViewport(D3D11_VIEWPORT()),
	m_viewport(D3D11_VIEWPORT())
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Graphic2D::PrimitiveManager2D::~PrimitiveManager2D()
{
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_commonStates = nullptr;
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
}



//--------------------------------------------------------------------
//! @summary   初期化処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Graphic2D::PrimitiveManager2D::Initialize()
{
	// デバイス・インターフェイスの取得
	m_device = Common::DeviceResources::GetInstance()->GetD3DDevice();
	// デバイス・コンテキスト・インターフェイスの取得
	m_deviceContext = Common::DeviceResources::GetInstance()->GetD3DDeviceContext();
	// コモンステートの取得
	m_commonStates = Common::DeviceResources::GetInstance()->GetCommonStates();

	// 頂点シェーダーのインスタンスの取得
	m_vertexShader = Shader::ShaderManager::GetInstance()->LoadVertexShader(L"Primitive2DVertexShader", INPUT_LAYOUT);
	// ピクセルシェーダーのインスタンスの取得
	m_pixelShader = Shader::ShaderManager::GetInstance()->LoadPixelShader(L"Primitive2DPixelShader");

	// ビューポートの初期設定
	m_defaultViewport = m_viewport = Common::DeviceResources::GetInstance()->GetScreenViewport();
}



//----------------------------------------------------------------------
//! @summary   点の描画
//!
//! @parameter [pointX] X座標(ピクセル単位)
//! @parameter [pointY] Y座標(ピクセル単位)
//! @parameter [color] 色
//!
//! @return    なし
//----------------------------------------------------------------------
void Graphic2D::PrimitiveManager2D::DrawPoint(int pointX, int pointY, XMFLOAT4 color)
{
	// 表示する点の頂点の作成
	vector<VertexBuffer> vertexes =
	{
		VertexBuffer(Math::ConvertPixelToWorld(pointX, pointY, static_cast<int>(m_viewport.Width), static_cast<int>(m_viewport.Height)), color),
	};


	// 頂点バッファの定義
	D3D11_BUFFER_DESC vertexBufferDesc;

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));						// 初期化
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;									// デフォルト使用法
	vertexBufferDesc.ByteWidth = sizeof(VertexBuffer) * vertexes.size();			// 使用頂点数
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// 頂点バッファとして使う
	vertexBufferDesc.CPUAccessFlags = 0;


	// 頂点バッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA vertexSubresourceData;

	ZeroMemory(&vertexSubresourceData, sizeof(vertexSubresourceData));	// 初期化
	vertexSubresourceData.pSysMem = vertexes.data();					// バッファ・データの初期化


	// 頂点バッファの作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

	m_device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, vertexBuffer.GetAddressOf());


	// 頂点バッファをセット
	UINT stride = sizeof(VertexBuffer);
	UINT offset = 0;
	ID3D11Buffer* pVertexBuffer = vertexBuffer.Get();

	m_deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);


	// ビューポートの設定
	m_deviceContext->RSSetViewports(1, &m_viewport);

	// ブレンドステートをセット
	ID3D11BlendState* blendstate = m_commonStates->AlphaBlend();
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


	// 深度バッファに書き込み参照する
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthRead(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


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
	m_deviceContext->Draw(vertexes.size(), 0);

	// ビューポートを元に戻す
	m_deviceContext->RSSetViewports(1, &m_defaultViewport);
}



//----------------------------------------------------------------------
//! @summary   線の描画
//!
//! @parameter [startX] 始点のX座標(ピクセル単位)
//! @parameter [startY] 始点のY座標(ピクセル単位)
//! @parameter [endX] 終点のX座標(ピクセル単位)
//! @parameter [endY] 終点のY座標(ピクセル単位)
//! @parameter [color] 色
//!
//! @return    なし
//----------------------------------------------------------------------
void Graphic2D::PrimitiveManager2D::DrawLine(int startX, int startY, int endX, int endY, XMFLOAT4 color)
{
	// 表示する線の頂点の作成
	vector<VertexBuffer> vertexes =
	{
		VertexBuffer(Math::ConvertPixelToWorld(startX, startY, static_cast<int>(m_viewport.Width), static_cast<int>(m_viewport.Height)), color),
		VertexBuffer(Math::ConvertPixelToWorld(endX, endY, static_cast<int>(m_viewport.Width), static_cast<int>(m_viewport.Height)), color),
	};


	// 頂点バッファの定義
	D3D11_BUFFER_DESC vertexBufferDesc;

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));						// 初期化
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;									// デフォルト使用法
	vertexBufferDesc.ByteWidth = sizeof(VertexBuffer) * vertexes.size();			// 使用頂点数
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// 頂点バッファとして使う
	vertexBufferDesc.CPUAccessFlags = 0;


	// 頂点バッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA vertexSubresourceData;

	ZeroMemory(&vertexSubresourceData, sizeof(vertexSubresourceData));	// 初期化
	vertexSubresourceData.pSysMem = vertexes.data();					// バッファ・データの初期化


	// 頂点バッファの作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

	m_device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, vertexBuffer.GetAddressOf());


	// 頂点バッファをセット
	UINT stride = sizeof(VertexBuffer);
	UINT offset = 0;
	ID3D11Buffer* pVertexBuffer = vertexBuffer.Get();

	m_deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// ビューポートの設定
	m_deviceContext->RSSetViewports(1, &m_viewport);

	// ブレンドステートをセット
	ID3D11BlendState* blendstate = m_commonStates->AlphaBlend();				// 半透明合成（アルファ乗算済み）
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


	// 深度バッファに書き込み参照する
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthRead(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


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
	m_deviceContext->Draw(vertexes.size(), 0);

	// ビューポートを元に戻す
	m_deviceContext->RSSetViewports(1, &m_defaultViewport);
}



//----------------------------------------------------------------------
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
//----------------------------------------------------------------------
void Graphic2D::PrimitiveManager2D::DrawTriangle(int vertexX1, int vertexY1, int vertexX2, int vertexY2, int vertexX3, int vertexY3, DirectX::XMFLOAT4 color, bool fillFlag)
{
	// 表示する三角形の頂点の作成
	vector<VertexBuffer> vertexes =
	{
		VertexBuffer(Math::ConvertPixelToWorld(vertexX1, vertexY1, static_cast<int>(m_viewport.Width), static_cast<int>(m_viewport.Height)), color),
		VertexBuffer(Math::ConvertPixelToWorld(vertexX3, vertexY3, static_cast<int>(m_viewport.Width), static_cast<int>(m_viewport.Height)), color),
		VertexBuffer(Math::ConvertPixelToWorld(vertexX2, vertexY2, static_cast<int>(m_viewport.Width), static_cast<int>(m_viewport.Height)), color),
	};


	// インデックスデータの作成
	vector<UINT> indexData;

	if (fillFlag) indexData = { 0, 1, 2 };
	else indexData = { 0, 1, 2, 0 };



	// 頂点バッファの定義
	D3D11_BUFFER_DESC vertexBufferDesc;

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));						// 初期化
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;									// デフォルト使用法
	vertexBufferDesc.ByteWidth = sizeof(VertexBuffer) * vertexes.size();			// 使用頂点数
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// 頂点バッファとして使う
	vertexBufferDesc.CPUAccessFlags = 0;


	// 頂点バッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA vertexSubresourceData;

	ZeroMemory(&vertexSubresourceData, sizeof(vertexSubresourceData));	// 初期化
	vertexSubresourceData.pSysMem = vertexes.data();					// バッファ・データの初期化


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


	// ビューポートの設定
	m_deviceContext->RSSetViewports(1, &m_viewport);

	// ラスタライザステートの設定
	m_deviceContext->RSSetState(m_commonStates->CullCounterClockwise());		// 反時計回りの面をカリングする


	// ブレンドステートをセット
	ID3D11BlendState* blendstate = m_commonStates->AlphaBlend();
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


	// 深度バッファに書き込み参照する
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthRead(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


	// プリミティブの種類を設定
	if (fillFlag) m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 三角形リスト
	else m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);				// 線ストリップ


	// シェーダーをセット
	// 頂点シェーダー
	m_deviceContext->VSSetShader(m_vertexShader->vertexShader, nullptr, 0);
	// ピクセルシェーダー
	m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


	// 入力レイアウトをセット
	m_deviceContext->IASetInputLayout(m_vertexShader->inputLayout);


	// 描画
	m_deviceContext->DrawIndexed(indexData.size(), 0, 0);

	// ビューポートを元に戻す
	m_deviceContext->RSSetViewports(1, &m_defaultViewport);
}



//----------------------------------------------------------------------
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
//----------------------------------------------------------------------
void Graphic2D::PrimitiveManager2D::DrawBox(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY, XMFLOAT4 color, bool fillFlag)
{
	// 表示する四角形の頂点の作成
	vector<VertexBuffer> vertexes =
	{
		// 左上
		VertexBuffer(Math::ConvertPixelToWorld(topLeftX, topLeftY, static_cast<int>(m_viewport.Width), static_cast<int>(m_viewport.Height)), color),
		// 右上
		VertexBuffer(Math::ConvertPixelToWorld(bottomRightX, topLeftY, static_cast<int>(m_viewport.Width), static_cast<int>(m_viewport.Height)), color),
		// 右下
		VertexBuffer(Math::ConvertPixelToWorld(bottomRightX, bottomRightY, static_cast<int>(m_viewport.Width), static_cast<int>(m_viewport.Height)), color),
		// 左下
		VertexBuffer(Math::ConvertPixelToWorld(topLeftX, bottomRightY, static_cast<int>(m_viewport.Width), static_cast<int>(m_viewport.Height)), color),
	};


	// インデックスバッファのサブリソースの初期値
	vector<UINT> indexData;
	if (fillFlag)
	{
		// 塗りつぶしあり
		indexData =
		{
			0,1,3,
			1,2,3,
		};
	}
	else
	{
		// 塗りつぶしなし
		indexData =
		{
			0,1,2,3,0
		};
	}



	// 頂点バッファの定義
	D3D11_BUFFER_DESC vertexBufferDesc;

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));						// 初期化
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;									// デフォルト使用法
	vertexBufferDesc.ByteWidth = sizeof(VertexBuffer) * vertexes.size();			// 使用頂点数
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// 頂点バッファとして使う
	vertexBufferDesc.CPUAccessFlags = 0;


	// 頂点バッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA vertexSubresourceData;

	ZeroMemory(&vertexSubresourceData, sizeof(vertexSubresourceData));	// 初期化
	vertexSubresourceData.pSysMem = vertexes.data();					// バッファ・データの初期化


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

	// ビューポートの設定
	m_deviceContext->RSSetViewports(1, &m_viewport);


	// ラスタライザステートの設定
	m_deviceContext->RSSetState(m_commonStates->CullCounterClockwise());		// 反時計回りの面をカリングする


	// ブレンドステートをセット
	ID3D11BlendState* blendstate = m_commonStates->AlphaBlend();
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


	// 深度バッファに書き込み参照する
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthRead(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


	// プリミティブの種類を設定
	if (fillFlag) m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 三角形リスト
	else m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);				// 線ストリップ


	// シェーダーをセット
	// 頂点シェーダー
	m_deviceContext->VSSetShader(m_vertexShader->vertexShader, nullptr, 0);
	// ピクセルシェーダー
	m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


	// 入力レイアウトをセット
	m_deviceContext->IASetInputLayout(m_vertexShader->inputLayout);


	// 描画
	m_deviceContext->DrawIndexed(indexData.size(), 0, 0);

	// ビューポートを元に戻す
	m_deviceContext->RSSetViewports(1, &m_defaultViewport);
}



//----------------------------------------------------------------------
//! @summary   円の描画
//!
//! @parameter [centerX] 中心のX座標(ピクセル単位)
//! @parameter [centerY] 中心のY座標(ピクセル単位)
//! @parameter [radius] 半径のX座標(ピクセル単位)
//! @parameter [color] 色
//! @parameter [fillFlag] 塗りつぶしフラグ
//!
//! @return    なし
//----------------------------------------------------------------------
void Graphic2D::PrimitiveManager2D::DrawCircle(int centerX, int centerY, int radius, XMFLOAT4 color, bool fillFlag)
{
	static const int DIVIDE = 30;

	// 表示する頂点の作成
	vector<VertexBuffer> vertexes;

	// 周囲の頂点 + 中心の頂点
	vertexes.resize(DIVIDE + 1);

	// 中心座標
	vertexes[0].position = Math::ConvertPixelToWorld(centerX, centerY, static_cast<int>(m_viewport.Width), static_cast<int>(m_viewport.Height));
	vertexes[0].color = color;

	// 周囲の座標
	for (int i = 1; i < DIVIDE + 1; ++i)
	{
		// 座標の計算
		float x = radius * cosf(XM_2PI / DIVIDE * i);
		float y = radius * sinf(XM_2PI / DIVIDE * i);

		// ピクセル座標をワールド座標へ変換
		vertexes[i].position = Math::ConvertPixelToWorld(static_cast<int>(x) + centerX, static_cast<int>(y) + centerY, static_cast<int>(m_viewport.Width), static_cast<int>(m_viewport.Height));

		// 色の設定
		vertexes[i].color = color;
	}


	// インデックスデータの作成
	vector<UINT> indexData;
	if (fillFlag)
	{
		indexData.resize(DIVIDE * 3);

		// インデックステーブル
		for (int i = 0; i < DIVIDE; ++i)
		{
			indexData[i * 3 + 0] = 0; // 中心の座標
			indexData[i * 3 + 1] = i + 1;
			indexData[i * 3 + 2] = i + 2;
		}

		// 最後の頂点だけ修正
		indexData[(DIVIDE * 3) - 1] = 1;
	}
	else
	{
		indexData.resize(vertexes.size());
		for (unsigned int i = 0; i < vertexes.size() - 1; ++i) indexData[i] = i + 1;

		// 最後の頂点だけ修正
		indexData[vertexes.size() - 1] = 1;
	}


	// 頂点バッファの定義
	D3D11_BUFFER_DESC vertexBufferDesc;

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));						// 初期化
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;									// デフォルト使用法
	vertexBufferDesc.ByteWidth = sizeof(VertexBuffer) * vertexes.size();			// 使用頂点数
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// 頂点バッファとして使う
	vertexBufferDesc.CPUAccessFlags = 0;


	// 頂点バッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA vertexSubresourceData;

	ZeroMemory(&vertexSubresourceData, sizeof(vertexSubresourceData));	// 初期化
	vertexSubresourceData.pSysMem = vertexes.data();					// バッファ・データの初期化


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

	// ビューポートの設定
	m_deviceContext->RSSetViewports(1, &m_viewport);


	// ラスタライザステートの設定
	m_deviceContext->RSSetState(m_commonStates->CullCounterClockwise());		// 反時計回りの面をカリングする


	// ブレンドステートをセット
	ID3D11BlendState* blendstate = m_commonStates->AlphaBlend();
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


	// 深度バッファに書き込み参照する
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthRead(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


	// プリミティブの種類を設定
	if (fillFlag) m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 三角形リスト
	else m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);				// 線ストリップ


	// シェーダーをセット
	// 頂点シェーダー
	m_deviceContext->VSSetShader(m_vertexShader->vertexShader, nullptr, 0);
	// ピクセルシェーダー
	m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


	// 入力レイアウトをセット
	m_deviceContext->IASetInputLayout(m_vertexShader->inputLayout);


	// 描画
	m_deviceContext->DrawIndexed(indexData.size(), 0, 0);

	// ビューポートを元に戻す
	m_deviceContext->RSSetViewports(1, &m_defaultViewport);
}
