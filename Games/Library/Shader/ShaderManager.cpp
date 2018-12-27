//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		ShaderManager.cpp
//!
//! @summary	シェーダー・オブジェクトの管理クラスのソースファイル
//!
//! @date		2018.09.18
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダファイル>
#include "ShaderManager.h"
#include "../Utility/BinaryFileLoader.h"
#include "../Common/DeviceResources.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Shader::ShaderManager::ShaderManager() :
	m_device(nullptr)
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Shader::ShaderManager::~ShaderManager()
{
	m_device = nullptr;

	// 頂点シェーダー管理のマップ変数の削除
	for (map<const wchar_t*, VertexShader*>::iterator itr = m_vertexShaderMap.begin(); itr != m_vertexShaderMap.end(); ++itr)
	{
		pair<const wchar_t*, VertexShader*> p = *itr;
		delete p.second;
	}
	m_vertexShaderMap.clear();

	// ジオメトリシェーダー管理のマップ変数の削除
	for (map<const wchar_t*, ID3D11GeometryShader*>::iterator itr = m_geometryShaderMap.begin(); itr != m_geometryShaderMap.end(); ++itr)
	{
		pair<const wchar_t*, ID3D11GeometryShader*> p = *itr;
		if(p.second) p.second->Release();
	}
	m_geometryShaderMap.clear();

	// ピクセルシェーダー管理のマップ変数の削除
	for (map<const wchar_t*, ID3D11PixelShader*>::iterator itr = m_pixelShaderMap.begin(); itr != m_pixelShaderMap.end(); ++itr)
	{
		pair<const wchar_t*, ID3D11PixelShader*> p = *itr;
		if(p.second) p.second->Release();
	}
	m_pixelShaderMap.clear();
}



//--------------------------------------------------------------------
//! @summary  初期化処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Shader::ShaderManager::Initialize()
{
	// デバイス・インターフェイスの取得
	m_device = Common::DeviceResources::GetInstance()->GetD3DDevice();
}



//--------------------------------------------------------------------
//! @summary   頂点シェーダーを読み込みシェーダー・オブジェクトと
//!			   入力レイアウト・オブジェクトを作成する
//!
//! @parameter [fileName] 読み込むファイル
//! @parameter [inputLayout] 入力要素配列
//!
//! @return    頂点シェーダーのインスタンス
//--------------------------------------------------------------------
Shader::VertexShader* Shader::ShaderManager::LoadVertexShader(const wchar_t* fileName, const vector<D3D11_INPUT_ELEMENT_DESC>& inputLayout)
{
	// 頂点シェーダー・オブジェクトがない
	if (m_vertexShaderMap.count(fileName) == 0)
	{
		// シェーダーの読み込み
		// ファイルまでのパスを作成
		wstring fullPass = wstring(L"Resources/Shaders/VertexShader/") + fileName + wstring(L".cso");
		Utility::BinaryData vertexShader = Utility::LoadBinaryFile(fullPass.c_str());

		// 入力レイアウト・オブジェクトの作成
		ID3D11InputLayout* inputLayoutObject;
		if (FAILED(m_device->CreateInputLayout(inputLayout.data(),
			inputLayout.size(),
			vertexShader.GetData(),
			vertexShader.size,
			&inputLayoutObject)))
		{
			wstring message = fileName + wstring(L"の入力レイアウト・オブジェクトの作成に失敗しました");
			MessageBox(NULL, message.c_str(), TEXT("エラー"), MB_OK);
		}

		// 頂点シェーダー・オブジェクトの作成
		ID3D11VertexShader* vertexShaderObject;
		if (FAILED(m_device->CreateVertexShader(vertexShader.GetData(), vertexShader.size, NULL, &vertexShaderObject)))
		{
			wstring message = fileName + wstring(L"・オブジェクトの作成に失敗しました");
			MessageBox(NULL, message.c_str(), TEXT("エラー"), MB_OK);
		}

		// マップに保存
		m_vertexShaderMap[fileName] = new VertexShader(vertexShaderObject, inputLayoutObject);
	}

	// 頂点シェーダーのインスタンスを渡す
	return m_vertexShaderMap[fileName];
}



//--------------------------------------------------------------------
//! @summary   ピクセルシェーダーを読み込みシェーダー・オブジェクト
//!			   を作成する
//!
//! @parameter [fileName] 読み込むファイル
//!
//! @return    ピクセルシェーダーのインスタンス
//--------------------------------------------------------------------
ID3D11PixelShader* Shader::ShaderManager::LoadPixelShader(const wchar_t* fileName)
{
	// ピクセルシェーダー・オブジェクトがない
	if (m_pixelShaderMap.count(fileName) == 0)
	{
		// シェーダーの読み込み
		// ファイルまでのパスを作成
		wstring fullPass = wstring(L"Resources/Shaders/PixelShader/") + fileName + wstring(L".cso");
		Utility::BinaryData pixelShader = Utility::LoadBinaryFile(fullPass.c_str());

		// ピクセルシェーダー・オブジェクトの作成
		ID3D11PixelShader* pixelShaderObject;
		if (FAILED(m_device->CreatePixelShader(pixelShader.GetData(), pixelShader.size, NULL, &pixelShaderObject)))
		{
			wstring message = fileName + wstring(L"・オブジェクトの作成に失敗しました");
			MessageBox(NULL, message.c_str(), TEXT("エラー"), MB_OK);
		}

		// マップに保存
		m_pixelShaderMap[fileName] = pixelShaderObject;
	}

	// ピクセルシェーダーのインスタンスを渡す
	return m_pixelShaderMap[fileName];
}



//--------------------------------------------------------------------
//! @summary   ジオメトリシェーダーを読み込みシェーダー・オブジェクト
//!			   を作成する
//!
//! @parameter [fileName] 読み込むファイル
//!
//! @return    ジオメトリシェーダーのインスタンス
//--------------------------------------------------------------------
ID3D11GeometryShader* Shader::ShaderManager::LoadGeometryShader(const wchar_t* fileName)
{
	// ピクセルシェーダー・オブジェクトがない
	if (m_geometryShaderMap.count(fileName) == 0)
	{
		// シェーダーの読み込み
		// ファイルまでのパスを作成
		wstring fullPass = wstring(L"Resources/Shaders/GeometryShader/") + fileName + wstring(L".cso");
		Utility::BinaryData geometryShader = Utility::LoadBinaryFile(fullPass.c_str());

		// ピクセルシェーダー・オブジェクトの作成
		ID3D11GeometryShader* geometryShaderObject;
		if (FAILED(m_device->CreateGeometryShader(geometryShader.GetData(), geometryShader.size, NULL, &geometryShaderObject)))
		{
			wstring message = fileName + wstring(L"・オブジェクトの作成に失敗しました");
			MessageBox(NULL, message.c_str(), TEXT("エラー"), MB_OK);
		}

		// マップに保存
		m_geometryShaderMap[fileName] = geometryShaderObject;
	}

	// ピクセルシェーダーのインスタンスを渡す
	return m_geometryShaderMap[fileName];
}
