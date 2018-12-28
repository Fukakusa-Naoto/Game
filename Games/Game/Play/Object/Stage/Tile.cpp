//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Tile.cpp
//!
//! @summary	モトスのタイルクラスのソースファイル
//!
//! @date		2018.11.14
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../../../pch.h"

// <自作ヘッダーファイル>
#include "Tile.h"
#include "../../../../Library/Common/DeviceResources.h"
#include "../../../../Library/Collision/CollisionManager.h"
#include "../../../Utility/ObjactTag.h"
#include "../../Controller/TileController.h"
#include "../../../../Library/Camera/Camera.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 定数の定義 ==============================================================
// サイズ
const Vector3 Motos::Play::Object::Stage::Tile::TILE_SIZE = Vector3(1.0f, 0.1f, 1.0f);


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::Stage::Tile::Tile() :
	GameObject()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Play::Object::Stage::Tile::~Tile()
{
	delete m_cubeShape;
	m_cubeShape = nullptr;
}



//--------------------------------------------------------------------
//! @summary   初期化処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::Stage::Tile::Start()
{
	m_primitiveRender = Graphic3D::PrimitiveManager3D::GetInstance();
	// インスタンスの取得
	m_device = Common::DeviceResources::GetInstance()->GetD3DDevice();
	m_deviceContext = Common::DeviceResources::GetInstance()->GetD3DDeviceContext();
	m_commonStates = Common::DeviceResources::GetInstance()->GetCommonStates();


	// シェーダーの読み込み
	// 頂点シェーダーのインスタンスの取得
	m_vertexShader = Shader::ShaderManager::GetInstance()->LoadVertexShader(L"Primitive3DVertexShader2", Graphic3D::PrimitiveManager3D::INPUT_LAYOUT);
	// ジオメトリシェーダーのインスタンスの取得
	m_geometryShader = Shader::ShaderManager::GetInstance()->LoadGeometryShader(L"Primitive3DGeometryShader");
	// ピクセルシェーダーのインスタンスの取得
	m_pixelShader = Shader::ShaderManager::GetInstance()->LoadPixelShader(L"TilePixelShader");

	m_controller = new Controller::TileController(this, m_transform);

	m_cubeShape = new Collision::CubeShape(m_controller, m_transform, TILE_SIZE);

	// 登録
	Collision::CollisionManager::GetInstance()->Entry(m_cubeShape, ObjectTag::TILE);

	// タグの登録
	m_tag = ObjectTag::TILE;
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::Stage::Tile::Update(const Common::StepTimer& timer)
{
	m_controller->Update(timer);
	m_transform.Update();
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Play::Object::Stage::Tile::Draw()
{
	// パイプライン構築のラムダ式関数
	function<void()> lambda = [&]()
	{
		// ラスタライザステートの設定
		m_deviceContext->RSSetState(m_commonStates->CullClockwise());	// 反時計回り面をカリングする

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
		// ジオメトリシェーダー
		m_deviceContext->GSSetShader(m_geometryShader, nullptr, 0);
		// ピクセルシェーダー
		m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);


		// 入力レイアウトをセット
		m_deviceContext->IASetInputLayout(m_vertexShader->inputLayout);
	};

	m_primitiveRender->DrawCube(m_transform.GetPosition(), m_transform.GetRotation(), TILE_SIZE, Color(1.0f, 1.0f, 1.0f, 1.0f), m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix(), lambda);
}
