//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		ModelMesh.cpp
//!
//! @summary	モデルのメッシュクラスのソースファイル
//!
//! @date		2018.09.28
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../pch.h"

// <標準ヘッダファイル>
#include <string>

// <自作ヘッダファイル>
#include "ModelMesh.h"
#include "ModelManager.h"
#include "../../Common/DeviceResources.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX::SimpleMath;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Graphic3D::Model::ModelMesh::ModelMesh()
{
	// デバイス・インターフェイスの取得
	m_device = Common::DeviceResources::GetInstance()->GetD3DDevice();
	// デバイスコンテキスト・インターフェイスの取得
	m_deviceContext = Common::DeviceResources::GetInstance()->GetD3DDeviceContext();
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Graphic3D::Model::ModelMesh::~ModelMesh()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Graphic3D::Model::ModelMesh::Draw()
{
#pragma region 頂点バッファ
		// 頂点バッファの定義
		D3D11_BUFFER_DESC vertexBufferDesc;

		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));								// 初期化
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;											// デフォルト使用法
		vertexBufferDesc.ByteWidth = sizeof(SkinVertexBuffer) * m_skinVertices.size();			// 使用頂点数
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;									// 頂点バッファとして使う
		vertexBufferDesc.CPUAccessFlags = 0;


		// 頂点バッファのサブリソースの定義
		D3D11_SUBRESOURCE_DATA vertexSubresourceData;

		ZeroMemory(&vertexSubresourceData, sizeof(vertexSubresourceData));		// 初期化
		vertexSubresourceData.pSysMem = m_skinVertices.data();					// バッファ・データの初期化


		// 頂点バッファの作成
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

		m_device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, vertexBuffer.GetAddressOf());


		// 頂点バッファをセット
		UINT stride = sizeof(SkinVertexBuffer);
		UINT offset = 0;
		ID3D11Buffer* pVertexBuffer = vertexBuffer.Get();

		m_deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
#pragma endregion

#pragma region インデックスバッファ
	// インデックスバッファの定義
	D3D11_BUFFER_DESC indexBufferDesc;

	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));						// 初期化
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;								// デフォルト使用法
	indexBufferDesc.ByteWidth = sizeof(UINT) * m_indexes.size();				// 使用インデックス数
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;						// インデックスバッファとして使う
	indexBufferDesc.CPUAccessFlags = 0;


	// インデックスバッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA indexSubresourceData;

	ZeroMemory(&indexSubresourceData, sizeof(indexSubresourceData));	// 初期化
	indexSubresourceData.pSysMem = m_indexes.data();					// バッファ・データの初期化


	// インデックスバッファの作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	m_device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, indexBuffer.GetAddressOf());


	// インデックスバッファをセット
	ID3D11Buffer* pIndexBuffer = indexBuffer.Get();
	m_deviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
#pragma endregion

	// テクスチャのセット
	if(m_material.texture) m_deviceContext->PSSetShaderResources(0, 1, &m_material.texture->texture);

	// 描画
	m_deviceContext->DrawIndexed(m_indexes.size(), 0, 0);
}
