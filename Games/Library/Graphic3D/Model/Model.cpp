//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Model.cpp
//!
//! @summary	モデルクラスのソースファイル
//!
//! @date		2018.09.27
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../pch.h"

// <自作ヘッダファイル>
#include "Model.h"
#include "../../Common/DeviceResources.h"
#include "ModelManager.h"
#include "../../Utility/String.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX::SimpleMath;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [pmdModel] モデルデータ
//! @parameter [fileName] ファイル名
//--------------------------------------------------------------------
Graphic3D::Model::Model::Model(const Graphic3D::Model::PMDModel* pmdModel, const wchar_t* fileName)
{
	// モデルの作成
	CreateModel(pmdModel, fileName);

	// デバイス・インターフェイスの取得
	m_device = Common::DeviceResources::GetInstance()->GetD3DDevice();
	// デバイスコンテキスト・インターフェイスの取得
	m_deviceContext = Common::DeviceResources::GetInstance()->GetD3DDeviceContext();
	// コモンステートの取得
	m_commonStates = Common::DeviceResources::GetInstance()->GetCommonStates();

	// 頂点シェーダーのインスタンスの取得
	m_vertexShader = Utility::ShaderManager::GetInstance()->LoadVertexShader(L"SkinningModelVertexShader", SKINNING_INPUT_LAYOUT);
	// ピクセルシェーダーのインスタンスの取得
	m_pixelShader = Utility::ShaderManager::GetInstance()->LoadPixelShader(L"SkinningModelPixelShader");
}



//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [pmxModel] モデルデータ
//! @parameter [fileName] ファイル名
//--------------------------------------------------------------------
Graphic3D::Model::Model::Model(const Graphic3D::Model::PMXModel* pmxModel, const wchar_t* fileName)
{
	// モデルの作成
	CreateModel(pmxModel, fileName);

	// デバイス・インターフェイスの取得
	m_device = Common::DeviceResources::GetInstance()->GetD3DDevice();
	// デバイスコンテキスト・インターフェイスの取得
	m_deviceContext = Common::DeviceResources::GetInstance()->GetD3DDeviceContext();
	// コモンステートの取得
	m_commonStates = Common::DeviceResources::GetInstance()->GetCommonStates();

	// 頂点シェーダーのインスタンスの取得
	m_vertexShader = Utility::ShaderManager::GetInstance()->LoadVertexShader(L"SkinningModelVertexShader", SKINNING_INPUT_LAYOUT);
	// ピクセルシェーダーのインスタンスの取得
	m_pixelShader = Utility::ShaderManager::GetInstance()->LoadPixelShader(L"SkinningModelPixelShader");
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Graphic3D::Model::Model::~Model()
{
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_commonStates = nullptr;
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [world] ワールド行列
//! @parameter [view] ビュー行列
//! @parameter [projection] 射影行列
//!
//! @return    なし
//--------------------------------------------------------------------
void Graphic3D::Model::Model::Draw(const Matrix& world, const Matrix& view, const Matrix& projection)
{
#pragma region 定数バッファ
	// 定数バッファのサブリソースの初期作成
	ConstBuffer constBufferData;
	constBufferData.view = view.Transpose();
	constBufferData.projection = projection.Transpose();
	constBufferData.world = world.Transpose();

	// 定数バッファの定義
	D3D11_BUFFER_DESC constBufferDesc;
	ZeroMemory(&constBufferDesc, sizeof(constBufferDesc));
	constBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufferDesc.ByteWidth = sizeof(ConstBuffer);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = 0;


	// 定数バッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA constSubresourceData;

	ZeroMemory(&constSubresourceData, sizeof(constSubresourceData));	// 初期化
	constSubresourceData.pSysMem = &constBufferData;					// バッファ・データの初期化


	// 定数バッファの作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> constBuffer;
	m_device->CreateBuffer(&constBufferDesc, &constSubresourceData, constBuffer.GetAddressOf());


	// 頂点シェーダーにセット
	ID3D11Buffer* cb = constBuffer.Get();
	m_deviceContext->VSSetConstantBuffers(0, 1, &cb);
#pragma endregion

#pragma region スキニングバッファ
	// スキニングバッファのサブリソースの初期作成
	SkinningBuffer skinningBuffer;
	memset(&skinningBuffer, 0, sizeof(skinningBuffer));

	// 行列パレットを取得
	std::vector<Matrix> skinTransform;
	for (unsigned i = 0; (i < NUM_BONES) || (i < skinTransform.size()); ++i)
	{
		//skinningBuffer.skinTransform[i] = skinTransform[i];
	}


	// スキニングバッファの定義
	D3D11_BUFFER_DESC skinningBufferDesc;
	ZeroMemory(&skinningBufferDesc, sizeof(skinningBufferDesc));
	skinningBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	skinningBufferDesc.ByteWidth = sizeof(SkinningBuffer);
	skinningBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	skinningBufferDesc.CPUAccessFlags = 0;


	// スキニングバッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA skinSubresourceData;

	ZeroMemory(&skinSubresourceData, sizeof(skinSubresourceData));	// 初期化
	skinSubresourceData.pSysMem = &skinningBuffer;					// バッファ・データの初期化


	// スキニングバッファの作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> skinBuffer;
	m_device->CreateBuffer(&skinningBufferDesc, &skinSubresourceData, skinBuffer.GetAddressOf());


	// 頂点シェーダーにセット
	ID3D11Buffer* sb = skinBuffer.Get();
	m_deviceContext->VSSetConstantBuffers(1, 1, &sb);
#pragma endregion

#pragma region レンダリング・パイプラインの構築
	// ラスタライザステートの設定
	m_deviceContext->RSSetState(m_commonStates->CullClockwise());	// 時計回り面をカリングする


	// サンプラーステートの設定
	ID3D11SamplerState* sampler = m_commonStates->LinearWrap();
	m_deviceContext->PSSetSamplers(0, 1, &sampler);


	// 深度・ステンシルステートの設定
	m_deviceContext->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);	// 深度テストをする・ステンシルテストをしない


	// ブレンドステートの設定
	ID3D11BlendState* blendstate = m_commonStates->AlphaBlend();			// 半透明合成（アルファ乗算済み）
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
#pragma endregion


	// 描画
	for (unsigned i = 0; i < m_meshes.size(); ++i) m_meshes[i].Draw();
}



//--------------------------------------------------------------------
//! @summary   モデルの作成
//!
//! @parameter [PMDModel] PMDモデル
//! @parameter [fileName] ファイル名
//!
//! @return    なし
//--------------------------------------------------------------------
void Graphic3D::Model::Model::CreateModel(const PMDModel* pmdModel, const wchar_t* fileName)
{
	// <メッシュ>
	// サイズの確保
	m_meshes.resize(pmdModel->materials.size());

	unsigned totalCount = 0;
	for (unsigned i = 0; i < m_meshes.size(); ++i)
	{
		// マテリアル
		{
			Material material;

			//テクスチャファイル
			// stringをwstringに変換
			material.textureName = Utility::String::ConvertToWstring(pmdModel->materials[i].textureFilename);
			material.diffuse = Vector4(pmdModel->materials[i].diffuse);

			// テクスチャがある
			if (material.textureName.size() != 0)
			{
				// ファイルまでのディレクトリパスを作成
				// 文字列を分ける
				vector<wstring> tmp;
				tmp = Utility::String::SliceString(fileName, L'/');

				wstring fullPass = wstring(L"Resources/Models/PMD/") + tmp[0] + L"/" + material.textureName.c_str();
				material.texture = Utility::TextureManager::GetInstance()->LoadTexture(fullPass.c_str());
			}
			else material.texture = nullptr;

			m_meshes[i].SetMaterial(material);
		}


		// ボーンの取得
		{
			std::vector<ModelBone> bones(pmdModel->bones.size());
			for (unsigned i = 0; i < pmdModel->bones.size(); ++i)
			{

			}
		}


		// 頂点データ
		{
			vector<SkinVertexBuffer> vertices(pmdModel->vertexes.size());
			for (unsigned i = 0; i < pmdModel->vertexes.size(); ++i)
			{
				vertices[i].position = Vector3(pmdModel->vertexes[i].position);
				vertices[i].normal = Vector3(pmdModel->vertexes[i].normal);
				vertices[i].texcoord = Vector2(pmdModel->vertexes[i].uv);
				vertices[i].boneIndex[0] = pmdModel->vertexes[i].boneIndex[0];
				vertices[i].boneIndex[1] = pmdModel->vertexes[i].boneIndex[1];
				vertices[i].boneWeight = pmdModel->vertexes[i].boneWeight;
			}

			m_meshes[i].SetSkinVertices(vertices);
		}


		// インデックスデータ
		vector<UINT> indexes(pmdModel->materials[i].indexCount);

		int maxLoop = totalCount + pmdModel->materials[i].indexCount;
		for (int i = totalCount, j = 0; i < maxLoop; ++i, ++j)
		{
			indexes[j] = pmdModel->indices[i];

			++totalCount;
		}

		m_meshes[i].SetIndexes(indexes);
	}
}



//--------------------------------------------------------------------
//! @summary   モデルの作成
//!
//! @parameter [PMXModel] PMXモデル
//! @parameter [fileName] ファイル名
//!
//! @return    なし
//--------------------------------------------------------------------
void Graphic3D::Model::Model::CreateModel(const PMXModel* pmxModel, const wchar_t* fileName)
{
	// <メッシュ>
	// サイズの確保
	m_meshes.resize(pmxModel->materials.size());

	unsigned totalCount = 0;
	for (unsigned i = 0; i < m_meshes.size(); ++i)
	{
		// マテリアル
		{
			Material material;

			//テクスチャファイル
			if (pmxModel->materials[i].diffuseTextureIndex != -1)
			{
				material.textureName = pmxModel->textures[pmxModel->materials[i].diffuseTextureIndex];
			}
			material.diffuse = Vector4(pmxModel->materials[i].diffuse);

			// テクスチャがある
			if (material.textureName.size() != 0)
			{
				// ファイルまでのディレクトリパスを作成
				// 文字列を分ける
				vector<wstring> tmp;
				tmp = Utility::String::SliceString(fileName, L'/');

				wstring fullPass = wstring(L"Resources/Models/PMX/") + tmp[0] + L"/" + material.textureName.c_str();

				// ファイルがあるか調べる
				std::ifstream ifs(fullPass);
				if(ifs.is_open()) material.texture = Utility::TextureManager::GetInstance()->LoadTexture(fullPass.c_str());
				else material.texture = nullptr;
			}
			else material.texture = nullptr;

			m_meshes[i].SetMaterial(material);
		}


		// ボーンの取得
		{
			std::vector<ModelBone> bones(pmxModel->bones.size());
			for (unsigned i = 0; i < pmxModel->bones.size(); ++i)
			{

			}
		}


		// 頂点データ
		{
			vector<SkinVertexBuffer> vertices(pmxModel->vertexes.size());
			for (unsigned i = 0; i < pmxModel->vertexes.size(); ++i)
			{
				vertices[i].position = Vector3(pmxModel->vertexes[i].position);
				vertices[i].normal = Vector3(pmxModel->vertexes[i].normal);
				vertices[i].texcoord = Vector2(pmxModel->vertexes[i].uv);
			}

			m_meshes[i].SetSkinVertices(vertices);
		}


		// インデックスデータ
		vector<UINT> indexes(pmxModel->materials[i].indexCount);

		int maxLoop = totalCount + pmxModel->materials[i].indexCount;
		for (int i = totalCount, j = 0; i < maxLoop; ++i, ++j)
		{
			indexes[j] = pmxModel->indices[i];

			++totalCount;
		}

		m_meshes[i].SetIndexes(indexes);
	}
}
