//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		ModelMesh.h
//!
//! @summary	モデルのメッシュクラスのヘッダファイル
//!
//! @date		2018.09.28
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../pch.h"

// <自作ヘッダファイル>
#include "../../Utility/ShaderManager.h"
#include "ModelDeta.h"


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Graphic3D
	{
		namespace Model
		{
			// クラスの定義 ======================================================
			class ModelMesh
			{
				// <メンバ変数>
			private:
				// デバイス・インターフェイス
				ID3D11Device1* m_device;
				// デバイスコンテキスト・インターフェイス
				ID3D11DeviceContext1* m_deviceContext;

				// 頂点データ
				std::vector<SkinVertexBuffer> m_skinVertices;
				// インデックスデータ
				std::vector<UINT> m_indexes;
				// マテリアル
				Material m_material;


				// <コンストラクタ>
			public:
				//----------------------------------------------------------
				//! @parameter [void] なし
				//----------------------------------------------------------
				ModelMesh();


				// <デストラクタ>
			public:
				~ModelMesh();


				// <メンバ関数>
			public:
				//----------------------------------------------------------
				//! @summary   描画処理
				//!
				//! @parameter [void] なし
				//!
				//! @return    なし
				//----------------------------------------------------------
				void Draw();


				// <セッター関数>
			public:
				inline void SetSkinVertices(const std::vector<SkinVertexBuffer>& verteces) { m_skinVertices = verteces; }
				inline void SetIndexes(const std::vector<UINT>& indexes) { m_indexes = indexes; }
				inline void SetMaterial(const Material& material) { m_material = material; }
			};
		}
	}
}