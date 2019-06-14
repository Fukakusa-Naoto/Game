//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		ModelData.h
//!
//! @summary	モデルデータ構造体のヘッダファイル
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

// <標準ヘッダファイル>
#include <vector>

// <標準ヘッダファイル>
#include "../../Utility/TextureManager.h"


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Graphic3D
	{
		namespace Model
		{
			// 定数の定義 ==========================================================
			// 入力要素配列
			static const std::vector<D3D11_INPUT_ELEMENT_DESC> SKINNING_INPUT_LAYOUT
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector2), D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "SKIN_INDEX",  0, DXGI_FORMAT_R16G16B16A16_UINT,  0, sizeof(DirectX::SimpleMath::Vector3) * 2 + sizeof(DirectX::SimpleMath::Vector2), D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "SKIN_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};



			// 構造体の定義 ==========================================================
			// 頂点バッファ
			struct VertexBuffer
			{
				// 座標
				DirectX::SimpleMath::Vector3 position;
				// uv座標
				DirectX::SimpleMath::Vector2 texcoord;
				// 法線
				DirectX::SimpleMath::Vector3 normal;


				// <コンストラクタ>
				VertexBuffer() :
					position(DirectX::SimpleMath::Vector3::Zero),
					texcoord(-DirectX::SimpleMath::Vector2::One),
					normal(DirectX::SimpleMath::Vector3::Zero)
				{
					// 何もしない
				}
			};
			struct SkinVertexBuffer
			{
				// 座標
				DirectX::SimpleMath::Vector3 position;
				// uv座標
				DirectX::SimpleMath::Vector2 texcoord;
				// 法線
				DirectX::SimpleMath::Vector3 normal;
				// ボーン番号
				unsigned short boneIndex[2];
				// ボーン1ウエイト
				unsigned char boneWeight;


				// <コンストラクタ>
				SkinVertexBuffer() :
					position(DirectX::SimpleMath::Vector3::Zero),
					texcoord(-DirectX::SimpleMath::Vector2::One),
					normal(DirectX::SimpleMath::Vector3::Zero),
					boneIndex(),
					boneWeight(0)
				{
					// 何もしない
				}
			};


			// 定数バッファ
			struct ConstBuffer
			{
				// ワールド行列
				DirectX::SimpleMath::Matrix world;
				// ビュー行列
				DirectX::SimpleMath::Matrix view;
				// 射影行列
				DirectX::SimpleMath::Matrix projection;


				// <コンストラクタ>
				ConstBuffer() {}
				ConstBuffer(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection, DirectX::XMVECTOR light) :
					world(world),
					view(view),
					projection(projection)
				{
					// 何もしない
				}
			};


			// マテリアル
			struct Material
			{
				std::wstring name;						// マテリアル名
				std::wstring textureName;				// テクスチャ名
				DirectX::SimpleMath::Vector4 diffuse;	// 減衰色
				Utility::Texture* texture;				// テクスチャ

				Material() :
					diffuse(DirectX::SimpleMath::Vector4::Zero),
					texture(nullptr)
				{
					// 何もしない
				}
			};


			// スキニングバッファ
			static const int NUM_BONES = 48;
			struct SkinningBuffer
			{
				DirectX::SimpleMath::Matrix skinTransform[NUM_BONES];
			};
		}
	}
}