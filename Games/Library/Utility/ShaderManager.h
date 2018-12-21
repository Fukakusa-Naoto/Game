//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		ShaderManager.h
//!
//! @summary	シェーダー・オブジェクトの管理クラスのヘッダファイル
//!
//! @date		2018.09.18
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
#include <vector>


// <自作ヘッダファイル>
#include "../System/SingletonBase.h"


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Utility
	{
		// 構造体の定義 ==========================================================
		struct VertexShader
		{
			// <変数>
			// 頂点シェーダー・オブジェクト
			ID3D11VertexShader* vertexShader;
			// 入力レイアウト・オブジェクト
			ID3D11InputLayout* inputLayout;


			// <コンストラクタ>
			VertexShader(ID3D11VertexShader* vertexShader, ID3D11InputLayout* inputLayout) :
				vertexShader(vertexShader),
				inputLayout(inputLayout)
			{
				// 何もしない
			}


			// <デストラクタ>
			~VertexShader()
			{
				// 頂点シェーダー・オブジェクトの削除
				if(vertexShader) vertexShader->Release();
				// 入力レイアウト・オブジェクトの削除
				if(inputLayout) inputLayout->Release();
			}
		};


		// クラスの定義 ==========================================================
		class ShaderManager : public System::SingletonBase<ShaderManager>
		{
			// <フレンドの登録>
			friend class System::SingletonBase<ShaderManager>;


			// <メンバ変数>
			// デバイス・インターフェイス
			ID3D11Device1* m_device;
			// 頂点シェーダー管理のマップ変数
			std::map<const wchar_t*, VertexShader*> m_vertexShaderMap;
			// ピクセルシェーダー管理のマップ変数
			std::map<const wchar_t*, ID3D11PixelShader*> m_pixelShaderMap;


			// <コンストラクタ>
		private:
			//--------------------------------------------------------------
			//! @parameter [void] なし
			//--------------------------------------------------------------
			ShaderManager();


			// <デストラクタ>
		public:
			~ShaderManager();


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
			//! @summary   頂点シェーダーを読み込みシェーダー・オブジェクトと
			//!			   入力レイアウト・オブジェクトを作成する
			//!
			//! @parameter [fileName] 読み込むファイル
			//! @parameter [inputLayout] 入力要素配列
			//!
			//! @return    頂点シェーダーのインスタンス
			//--------------------------------------------------------------
			VertexShader* LoadVertexShader(const wchar_t* fileName, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayout);


			//--------------------------------------------------------------
			//! @summary   ピクセルシェーダーを読み込みシェーダー・オブジェクト
			//!			   を作成する
			//!
			//! @parameter [fileName] 読み込むファイル
			//!
			//! @return    ピクセルシェーダーのインスタンス
			//--------------------------------------------------------------
			ID3D11PixelShader* LoadPixelShader(const wchar_t* fileName);
		};
	}
}