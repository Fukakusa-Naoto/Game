//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		TextureManager.h
//!
//! @summary	テクスチャデータの管理クラスのヘッダファイル
//!
//! @date		2018.10.01
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <標準ヘッダファイル>
#include <map>

// <自作ヘッダファイル>
#include "../System/SingletonBase.h"
#include "../Common/DeviceResources.h"


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Utility
	{
		// 構造体の定義 ==========================================================
		struct Texture
		{
			ID3D11ShaderResourceView* texture;
			CD3D11_TEXTURE2D_DESC desc;


			// <コンストラクタ>
			Texture() :
				texture(nullptr)
			{
				// 何もしない
			}


			// <デストラクタ>
			~Texture()
			{
				texture->Release();
			}
		};



		// クラスの定義 ==========================================================
		class TextureManager : public Library::System::SingletonBase<TextureManager>
		{
			// <フレンド登録>
			friend class Library::System::SingletonBase<TextureManager>;


			// <メンバ変数>
		private:
			// デバイス・インターフェイス
			ID3D11Device1* m_device;

			// 読み込んだテクスチャの保存コンテナ
			std::map<std::wstring, Texture*> m_textureList;


			// <コンストラクタ>
		private:
			//--------------------------------------------------------------
			//! @parameter [void] なし
			//--------------------------------------------------------------
			TextureManager();


			// <デストラクタ>
		public:
			~TextureManager();


			// <メンバ関数>
		public:
			//--------------------------------------------------------------
			//! @summary   初期化処理
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Initialize();


			//--------------------------------------------------------------
			//! @summary   テクスチャの読み込み
			//!
			//! @parameter [fileName] ファイル名
			//!
			//! @return    読み込み済みテクスチャ情報
			//--------------------------------------------------------------
			Texture* LoadTexture(const wchar_t* fileName);
		};
	}
}
