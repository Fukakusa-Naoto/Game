//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		TileBackground.cpp
//!
//! @summary	モトスのタイル背景クラスのソースファイル
//!
//! @date		2018.11.01
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../../pch.h"

// <自作ヘッダーファイル>
#include "TileBackground.h"
#include "../../../Game.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 定数の定義 ==============================================================
// 入力要素配列
const vector<D3D11_INPUT_ELEMENT_DESC> Motos::Title::Object::UI::TileBackground::INPUT_LAYOUT =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(XMFLOAT3) + sizeof(XMFLOAT2), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Title::Object::UI::TileBackground::TileBackground() :
	m_timeBuffer(nullptr),
	m_timer(0.0f)
{
	// インスタンスの取得
	m_device = Common::DeviceResources::GetInstance()->GetD3DDevice();
	m_deviceContext = Common::DeviceResources::GetInstance()->GetD3DDeviceContext();
	m_commonStates = Common::DeviceResources::GetInstance()->GetCommonStates();

	// スプライトの作成
	m_sprite = new Graphic2D::Sprite();
	m_sprite->SetScale(Vector2(1.6f, 1.2f));


	// シェーダーの読み込み
	m_vertexShader = Shader::ShaderManager::GetInstance()->LoadVertexShader(L"TileBackgroundVertexShader", INPUT_LAYOUT);
	m_pixelShader = Shader::ShaderManager::GetInstance()->LoadPixelShader(L"TileBackgroundPixelShader");


	// 画像の読み込み
	m_texture = Utility::TextureManager::GetInstance()->LoadTexture(L"Resources/Textures/TileBackground.png");
	m_texture2 = Utility::TextureManager::GetInstance()->LoadTexture(L"Resources/Textures/RuleMap.png");

	m_drawingOrder = 1;

	// 切り抜き座標の初期化
	RECT rect;
	rect.top = rect.left = 0;
	rect.bottom = m_texture->desc.Height;
	rect.right = m_texture->desc.Width;
	m_sprite->SetRect(rect);

	m_sprite->SetTexture(m_texture);

	// 時間バッファ・オブジェクトの作成
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(TimeBuffer);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	m_device->CreateBuffer(&bufferDesc, nullptr, &m_timeBuffer);
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Title::Object::UI::TileBackground::~TileBackground()
{
	delete m_sprite;
	m_sprite = nullptr;

	m_timeBuffer->Release();
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Title::Object::UI::TileBackground::Update(const Common::StepTimer& timer)
{
	m_timer += static_cast<float>(timer.GetElapsedSeconds());
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Title::Object::UI::TileBackground::Draw()
{
	// パイプライン構築のラムダ式関数
	function<void()> lambda = [&]()
	{
		TimeBuffer timeBuffer;
		timeBuffer.time.x = m_timer;

		//定数バッファの内容更新
		m_deviceContext->UpdateSubresource(m_timeBuffer, 0, NULL, &timeBuffer, 0, 0);


		// ピクセルシェーダーにタイムバッファを渡す
		m_deviceContext->PSSetConstantBuffers(0, 1, &m_timeBuffer);

		// テクスチャをセット
		m_deviceContext->PSSetShaderResources(0, 1, &m_texture->texture);
		m_deviceContext->PSSetShaderResources(1, 1, &m_texture2->texture);


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
	};


	m_sprite->SetViewport(m_viewport);
	m_sprite->Draw(lambda);
}
