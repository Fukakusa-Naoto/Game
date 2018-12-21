//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		TextureManager.h
//!
//! @summary	テクスチャデータの管理クラスのヘッダファイル
//!
//! @date		2018.10.01
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダファイル>
#include "TextureManager.h"
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
Utility::TextureManager::TextureManager() :
	m_device(nullptr)
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Utility::TextureManager::~TextureManager()
{
	m_device = nullptr;

	// コンテナの削除
	for (map<wstring, Texture*>::iterator itr = m_textureList.begin(); itr != m_textureList.end(); ++itr)
	{
		pair<wstring, Texture*> p = *itr;
		delete p.second;
	}
	m_textureList.clear();
}



//--------------------------------------------------------------------
//! @summary   初期化処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Utility::TextureManager::Initialize()
{
	// デバイスの取得
	m_device = Common::DeviceResources::GetInstance()->GetD3DDevice();
}



//--------------------------------------------------------------------
//! @summary   テクスチャの読み込み
//!
//! @parameter [fileName] ファイル名
//!
//! @return    読み込み済みテクスチャ情報
//--------------------------------------------------------------------
Utility::Texture* Utility::TextureManager::LoadTexture(const wchar_t* fileName)
{
	// テクスチャが読み込み済みでないか調べる
	if (m_textureList.count(fileName) == 0)
	{
		Texture* texture = new Texture();

		// 拡張子を調べる
		wstring fullpath = wstring(fileName);
		int ext = fullpath.find_last_of(L".");
		std::wstring extname = fullpath.substr(ext, fullpath.size() - ext);

		if (extname == L".tga")
		{
			ScratchImage image;
			LoadFromTGAFile(fileName, nullptr, image);
			CreateShaderResourceView(m_device,
				image.GetImages(), image.GetImageCount(),
				image.GetMetadata(), &texture->texture);

			const Image* imageData = image.GetImages();

			texture->desc.Width = imageData->width;
			texture->desc.Height = imageData->height;
		}
		else
		{
			Microsoft::WRL::ComPtr<ID3D11Resource> resource;
			CreateWICTextureFromFile(m_device, fileName, resource.GetAddressOf(), &texture->texture);


			Microsoft::WRL::ComPtr<ID3D11Texture2D> resourceTexture;
			resource.As(&resourceTexture);

			// テクスチャ情報取得
			resourceTexture->GetDesc(&texture->desc);
		}



		// 読み込んだ情報を格納
		m_textureList[fileName] = texture;
	}

	return m_textureList[fileName];
}
