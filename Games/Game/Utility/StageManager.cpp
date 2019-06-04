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

// <標準ヘッダファイル>
#include <fstream>
#include <iostream>
#include <string>

// <自作ヘッダーファイル>
#include "StageManager.h"
#include "TaskManager.h"
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
//! @parameter [stageData] ステージデータ
//! @parameter [camera] カメラ
//! @parameter [taskManager] タスクマネージャー
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Utility::StageManager::Create(vector<int>& stageData, Library::Camera::Camera* camera, TaskManager* taskManager)
{
	for (unsigned int i = 0; i < stageData.size(); ++i)
	{
		switch (stageData[i])
		{
		case 1:
		{
			Play::Object::Stage::Tile* tile = new Play::Object::Stage::Tile();
			tile->SetCamera(camera);

			float x = static_cast<float>(i % 12) - 6.0f;
			float z = static_cast<float>(i / 12) - 6.0f;
			tile->SetPosition(Vector3(x, 0.0f, z));

			taskManager->Entry(tile);
			m_tileList.push_back(tile);
		}
			break;
		default:
			break;
		}
	}
}



//--------------------------------------------------------------
//! @summary   ステージデータの読み込み
//!
//! @parameter [stageNumber] 読み込むステージデータ
//! @parameter [stageData] ステージデータ
//! @parameter [objectData] オブジェクトデータ
//!
//! @return    なし
//--------------------------------------------------------------
void Motos::Utility::StageManager::LoadStageData(int stageNumber, std::vector<int>& stageData, std::vector<int>& objectData)
{
	// 整数を文字列に変換する
	string str;
	str = to_string(stageNumber);
	str.insert(1, "_");

	// 読み込むファイルの文字列を作成する
	string filePass("Resources/Data/");
	filePass += str;
	filePass += ".md";
	ifstream ifs(filePass);

	if (ifs.fail())
	{
		wstring message(L"ステージファイルが開けませんでした");
		MessageBox(NULL, message.c_str(), TEXT("エラー"), MB_OK);
		PostQuitMessage(0);
	}

	string lineString;

	enum ID
	{
		NONE,
		STAGE,
		OBJECT,
	};

	ID channel = NONE;
	while (getline(ifs, lineString))
	{
		switch (channel)
		{
		case NONE:
			break;
		case STAGE:
			for (unsigned int i = 0; i < lineString.size(); ++i)
			{
				switch (lineString[i])
				{
				case '0':	// なし
					stageData.push_back(0);
					break;
				case '1':	// タイル
					stageData.push_back(1);
					break;
				default:
					break;
				}
			}
			break;
		case OBJECT:
			for (unsigned int i = 0; i < lineString.size(); ++i)
			{
				switch (lineString[i])
				{
				case '0':	// なし
					objectData.push_back(0);
					break;
				case '1':	// プレイヤー
					objectData.push_back(1);
					break;
				case '4':	// 敵
					objectData.push_back(2);
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}

		// 扱うデータを切り替える
		if (lineString == "STAGE")	channel = STAGE;
		else if (lineString == "OBJECT")	channel = OBJECT;

	}
}
