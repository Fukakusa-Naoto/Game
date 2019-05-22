//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		StageManager.cpp
//!
//! @summary	モトスのステージ管理クラスのソースファイル
//!
//! @date		2018.11.16
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダーファイル>
#include "StageManager.h"
#include "TaskManager.h"
// <自作ヘッダーファイル>
#include "../Play/Object/Stage/Tile.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Utility::StageManager::StageManager()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Utility::StageManager::~StageManager()
{
	if (m_tileList.size() != 0)
	{
		for (list<Play::Object::Stage::Tile*>::iterator itr = m_tileList.begin(); itr != m_tileList.end(); ++itr)
		{
			delete (*itr);
		}
	}

	m_tileList.clear();
}



//--------------------------------------------------------------------
//! @summary   ステージの作成処理
//!
//! @parameter [camera] カメラ
//! @parameter [taskManager] タスクマネージャー
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Utility::StageManager::Create(Library::Camera::Camera* camera, TaskManager* taskManager)
{
	for (int i = -6; i < 6; ++i)
	{
		for (int j = -6; j < 6; ++j)
		{
			Play::Object::Stage::Tile* tile = new Play::Object::Stage::Tile();
			tile->SetCamera(camera);

			tile->SetPosition(Vector3(static_cast<float>(i * Play::Object::Stage::Tile::TILE_SIZE.x) + Play::Object::Stage::Tile::TILE_SIZE.x * 0.5f, 0.0f, static_cast<float>(j * Play::Object::Stage::Tile::TILE_SIZE.z) + Play::Object::Stage::Tile::TILE_SIZE.z * 0.5f));

			taskManager->Entry(tile);
			m_tileList.push_back(tile);
		}
	}
}
