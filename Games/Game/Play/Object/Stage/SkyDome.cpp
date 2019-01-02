//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		SkyDome.cpp
//!
//! @summary	モトスのスカイドームクラスのソースファイル
//!
//! @date		2018.01.01
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../../pch.h"

// <自作ヘッダーファイル>
#include "SkyDome.h"
#include "../../../../Library/Common/DeviceResources.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 定数の定義 ==============================================================
// 入力要素配列
const vector<D3D11_INPUT_ELEMENT_DESC> Motos::Play::Object::Stage::SkyDome::INPUT_LAYOUT =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::Stage::SkyDome::SkyDome()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::Stage::SkyDome::~SkyDome()
{
	m_constBuffer->Release();
	m_vertexBuffer->Release();
}



//--------------------------------------------------------------------
//! @summary   初期化処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::Stage::SkyDome::Initialize()
{
	// インスタンスの取得
	m_device = Common::DeviceResources::GetInstance()->GetD3DDevice();
	m_deviceContext = Common::DeviceResources::GetInstance()->GetD3DDeviceContext();
	m_commonStates = Common::DeviceResources::GetInstance()->GetCommonStates();


	// シェーダーの読み込み
	// 頂点シェーダーのインスタンスの取得
	m_vertexShader = Shader::ShaderManager::GetInstance()->LoadVertexShader(L"SkyDomeVertexShader", INPUT_LAYOUT);
	// ジオメトリシェーダーのインスタンスの取得
	m_geometryShader = Shader::ShaderManager::GetInstance()->LoadGeometryShader(L"SkyDomeGeometryShader");
	// ピクセルシェーダーのインスタンスの取得
	m_pixelShader = Shader::ShaderManager::GetInstance()->LoadPixelShader(L"SkyDomePixelShader");

	// テクスチャの読み込み
	m_texture = Utility::TextureManager::GetInstance()->LoadTexture(L"Resources/Textures/Title_BaseBackground.jpg");

	// 定数バッファの定義
	D3D11_BUFFER_DESC constBufferDesc;
	ZeroMemory(&constBufferDesc, sizeof(constBufferDesc));
	constBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufferDesc.ByteWidth = sizeof(ConstBuffer);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = 0;

	// 定数バッファの作成
	m_device->CreateBuffer(&constBufferDesc, nullptr, &m_constBuffer);

	// 頂点バッファの定義
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));								// 初期化
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;											// デフォルト使用法
	vertexBufferDesc.ByteWidth = sizeof(VertexBuffer);										// 使用頂点数
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;									// 頂点バッファとして使う
	vertexBufferDesc.CPUAccessFlags = 0;

	vector<VertexBuffer> vertex = { VertexBuffer() };

	// 頂点バッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA vertexSubresourceData;
	ZeroMemory(&vertexSubresourceData, sizeof(vertexSubresourceData));		// 初期化
	vertexSubresourceData.pSysMem = vertex.data();					// バッファ・データの初期化


	// 頂点バッファの作成
	m_device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &m_vertexBuffer);
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::Stage::SkyDome::Update(const Common::StepTimer& timer)
{
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::Stage::SkyDome::Draw()
{
	Matrix trans = Matrix::CreateTranslation(Vector3(0, -10.0f, -10.0f));
	Matrix scale = Matrix::CreateScale(50.0f);
	Matrix rotation = Matrix::CreateRotationX(XMConvertToRadians(300.0f));
	Matrix world = scale * rotation * trans;

	// 定数バッファの作成
	ConstBuffer constBufferData;
	constBufferData.view = m_camera->GetViewMatrix().Transpose();
	constBufferData.projection = m_camera->GetProjectionMatrix().Transpose();
	constBufferData.world = world.Transpose();
	constBufferData.time = XMFLOAT4(0, 0, 0, 0);


	//定数バッファの内容更新
	m_deviceContext->UpdateSubresource(m_constBuffer, 0, NULL, &constBufferData, 0, 0);


	// 頂点バッファをセット
	UINT stride = sizeof(VertexBuffer);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// ジオメトリシェーダーに値を渡す
	m_deviceContext->GSSetConstantBuffers(0, 1, &m_constBuffer);


	// ラスタライザステートの設定
	m_deviceContext->RSSetState(m_commonStates->CullCounterClockwise());		// 反時計回りの面をカリングする

	// 深度・ステンシルステートの設定
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);	// 深度テストをするが、書き込みをしない・ステンシルテストをしない


	// ブレンドステートの設定
	ID3D11BlendState* blendstate = m_commonStates->NonPremultiplied();			// 半透明合成（アルファ未乗算）
	m_deviceContext->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);


	// シェーダーをセット
	m_deviceContext->VSSetShader(nullptr, nullptr, 0);
	m_deviceContext->GSSetShader(nullptr, nullptr, 0);
	m_deviceContext->PSSetShader(nullptr, nullptr, 0);

	// 頂点シェーダー
	m_deviceContext->VSSetShader(m_vertexShader->vertexShader, nullptr, 0);
	// ジオメトリシェーダー
	m_deviceContext->GSSetShader(m_geometryShader, nullptr, 0);
	// ピクセルシェーダー
	m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


	// 入力レイアウトをセット
	m_deviceContext->IASetInputLayout(m_vertexShader->inputLayout);

	// プリミティブの種類を設定
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);	// 三角形リスト

	// テクスチャのセット
	m_deviceContext->PSSetShaderResources(0, 1, &m_texture->texture);

	// サンプラーステートの設定
	ID3D11SamplerState* sampler = m_commonStates->LinearWrap();		// リニアフィルタリング／テクスチャラップ
	m_deviceContext->PSSetSamplers(0, 1, &sampler);


	// 描画
	m_deviceContext->Draw(1, 0);
}
