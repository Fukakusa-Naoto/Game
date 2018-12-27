//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Model.h
//!
//! @summary	モデルクラスのヘッダファイル
//!
//! @date		2018.09.27
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../pch.h"

// <自作ヘッダファイル>
#include "../../Shader/ShaderManager.h"
#include "ModelDeta.h"
#include "PMDData.h"
#include "ModelMesh.h"
#include "ModelBone.h"
#include "PMXData.h"


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Graphic3D
	{
		namespace Model
		{
			// クラスの定義 ======================================================
			class Model
			{
				// <メンバ変数>
			private:
				// デバイス・インターフェイス
				ID3D11Device1* m_device;
				// デバイスコンテキスト・インターフェイス
				ID3D11DeviceContext1* m_deviceContext;
				// コモンステート
				DirectX::CommonStates* m_commonStates;
				// 頂点シェーダーのインスタンス
				Shader::VertexShader* m_vertexShader;
				// ピクセルシェーダーのインスタンス
				ID3D11PixelShader* m_pixelShader;

				// メッシュ配列
				std::vector<ModelMesh> m_meshes;
				// ボーンの配列
				std::vector<ModelBone> m_bones;


				// <コンストラクタ>
			public:
				//----------------------------------------------------------
				//! @parameter [pmdModel] モデルデータ
				//! @parameter [fileName] ファイル名
				//----------------------------------------------------------
				Model(const PMDModel* pmdModel, const wchar_t* fileName);


				//----------------------------------------------------------
				//! @parameter [pmxModel] モデルデータ
				//! @parameter [fileName] ファイル名
				//----------------------------------------------------------
				Model(const PMXModel* pmxModel, const wchar_t* fileName);


				// <デストラクタ>
			public:
				~Model();


				// <メンバ関数>
			public:
				//----------------------------------------------------------
				//! @summary   描画処理
				//!
				//! @parameter [world] ワールド行列
				//! @parameter [view] ビュー行列
				//! @parameter [projection] 射影行列
				//!
				//! @return    なし
				//----------------------------------------------------------
				void Draw(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);


			private:
				//----------------------------------------------------------
				//! @summary   モデルの作成
				//!
				//! @parameter [pmdModel] PMDモデル
				//! @parameter [fileName] ファイル名
				//!
				//! @return    なし
				//----------------------------------------------------------
				void CreateModel(const PMDModel* pmdModel, const wchar_t* fileName);


				//----------------------------------------------------------
				//! @summary   モデルの作成
				//!
				//! @parameter [pmxModel] PMXモデル
				//! @parameter [fileName] ファイル名
				//!
				//! @return    なし
				//----------------------------------------------------------
				void CreateModel(const PMXModel* pmdModel, const wchar_t* fileName);
			};
		}
	}
}
