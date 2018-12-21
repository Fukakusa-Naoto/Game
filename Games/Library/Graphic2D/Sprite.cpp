//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Sprite.cpp
//!
//! @summary	スプライトクラスのソースファイル
//!
//! @date		2018.10.05
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダファイル>
#include "../Common/DeviceResources.h"
#include "Sprite.h"
#include "../Math/Math.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 定数の定義 ==============================================================
// 入力要素配列
const vector<D3D11_INPUT_ELEMENT_DESC> Graphic2D::Sprite::INPUT_LAYOUT =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(XMFLOAT3) + sizeof(XMFLOAT2), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [fileName] 読み込む画像ファイル名
//--------------------------------------------------------------------
Graphic2D::Sprite::Sprite(const wchar_t* fileName) :
	m_position(Vector2::Zero),
	m_color(Colors::White),
	m_rotation(0.0f),
	m_anchorPoint(Vector2::Zero),
	m_scale(Vector2::One)
{
	// インスタンスの取得
	m_device = Common::DeviceResources::GetInstance()->GetD3DDevice();
	m_deviceContext = Common::DeviceResources::GetInstance()->GetD3DDeviceContext();
	m_commonStates = Common::DeviceResources::GetInstance()->GetCommonStates();

	// シェーダーの読み込み
	m_vertexShader = Utility::ShaderManager::GetInstance()->LoadVertexShader(L"SpriteVertexShader", INPUT_LAYOUT);
	m_pixelShader = Utility::ShaderManager::GetInstance()->LoadPixelShader(L"SpritePixelShader");


	// 画像の読み込み
	m_texture = Utility::TextureManager::GetInstance()->LoadTexture(fileName);

	// 切り抜き座標の初期化
	m_rect.top = m_rect.left = 0;
	m_rect.bottom = m_texture->desc.Height;
	m_rect.right = m_texture->desc.Width;
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Graphic2D::Sprite::~Sprite()
{
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_commonStates = nullptr;
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Graphic2D::Sprite::Draw()
{
	// 画面幅の作成
	RECT screenRect = Common::DeviceResources::GetInstance()->GetOutputSize();
	int screenWidth = screenRect.right - screenRect.left;
	int screenHeight = screenRect.bottom - screenRect.top;

	// uv座標計算
	int width = m_rect.right - m_rect.left;
	int height = m_rect.bottom - m_rect.top;

	float top = static_cast<float>(m_rect.top) / static_cast<float>(m_texture->desc.Height);
	float bottom = static_cast<float>(m_rect.bottom) / static_cast<float>(m_texture->desc.Height);
	float left = static_cast<float>(m_rect.left) / static_cast<float>(m_texture->desc.Width);
	float right = static_cast<float>(m_rect.right) / static_cast<float>(m_texture->desc.Width);

	// アンカーポイント計算
	Vector2 originV2 = m_anchorPoint;
	originV2.x *= static_cast<float>(width);
	originV2.y *= static_cast<float>(height);


	// 表示する四角形の頂点の作成
	vector<VertexBuffer> vertexes =
	{
		// 左上
		VertexBuffer(Math::ConvertPixelToWorld(static_cast<int>(m_position.x - originV2.x), static_cast<int>(m_position.y - originV2.y), screenWidth, screenHeight), XMFLOAT2(left, top), m_color),
		// 右上
		VertexBuffer(Math::ConvertPixelToWorld(static_cast<int>(m_position.x + (width - originV2.x) * m_scale.x), static_cast<int>(m_position.y - originV2.y), screenWidth, screenHeight), XMFLOAT2(right, top), m_color),
		// 右下
		VertexBuffer(Math::ConvertPixelToWorld(static_cast<int>(m_position.x + (width - originV2.x) * m_scale.x), static_cast<int>(m_position.y + (height - originV2.y) * m_scale.y), screenWidth, screenHeight), XMFLOAT2(right, bottom), m_color),
		// 左下
		VertexBuffer(Math::ConvertPixelToWorld(static_cast<int>(m_position.x - originV2.x), static_cast<int>(m_position.y + (height - originV2.y) * m_scale.y), screenWidth, screenHeight), XMFLOAT2(left, bottom), m_color),
	};


	// 回転
	// 原点を作成
	XMFLOAT3 originF3 = Math::ConvertPixelToWorld(static_cast<int>(m_position.x), static_cast<int>(m_position.y), screenWidth, screenHeight);
	for (unsigned int i = 0; i < vertexes.size(); ++i)
	{
		// 原点に戻す
		vertexes[i].position.x -= originF3.x;
		vertexes[i].position.y -= originF3.y;

		// 座標の保存
		XMFLOAT3 tmp = vertexes[i].position;

		// 回転させる
		vertexes[i].position.x = tmp.x*cosf(m_rotation) - tmp.y*sinf(m_rotation);
		vertexes[i].position.y = tmp.x*sinf(m_rotation) + tmp.y*cosf(m_rotation);

		// 元に戻す
		vertexes[i].position.x += originF3.x;
		vertexes[i].position.y += originF3.y;
	}


	// インデックスバッファのサブリソースの初期値
	vector<UINT> indexData =
	{
		0,1,3,
		1,2,3,
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


	// インデックスバッファの定義
	D3D11_BUFFER_DESC indexBufferDesc;

	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));			// 初期化
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;					// デフォルト使用法
	indexBufferDesc.ByteWidth = sizeof(UINT) * indexData.size();		// 使用インデックス数
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;			// インデックスバッファとして使う
	indexBufferDesc.CPUAccessFlags = 0;


	// インデックスバッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA indexSubresourceData;

	ZeroMemory(&indexSubresourceData, sizeof(indexSubresourceData));		// 初期化
	indexSubresourceData.pSysMem = indexData.data();						// バッファ・データの初期化


	// インデックスバッファの作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	m_device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, indexBuffer.GetAddressOf());


	// インデックスバッファをセット
	ID3D11Buffer* pIndexBuffer = indexBuffer.Get();
	m_deviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	// テクスチャをセット
	m_deviceContext->PSSetShaderResources(0, 1, &m_texture->texture);


	// ラスタライザステートの設定
	m_deviceContext->RSSetState(m_commonStates->CullCounterClockwise());	// 反時計回り面をカリングする


	// サンプラーステートの設定
	ID3D11SamplerState* sampler = m_commonStates->LinearWrap();		// リニアフィルタリング／テクスチャラップ
	m_deviceContext->PSSetSamplers(0, 1, &sampler);


	// 深度・ステンシルステートの設定
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthRead(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


	// ブレンドステートの設定
	ID3D11BlendState* blendstate = m_commonStates->NonPremultiplied();			// 半透明合成（アルファ未乗算）
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


	// プリミティブの種類を設定
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 三角形リスト


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
