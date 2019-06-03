//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		StageManager.h
//!
//! @summary	モトスのステージ管理クラスのヘッダファイル
//!
//! @date		2018.11.16
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <標準ヘッダファイル>
#include <list>
#include <vector>

// <自作ヘッダファイル>
#include "../../Library/Camera/Camera.h"


// 名前空間の定義 ================================================================
namespace Motos
{
	// クラスの宣言 ==============================================================
	namespace Play
	{
		namespace Object
		{
			namespace Stage
			{
				class Tile;
			}
		}
	}


	namespace Utility
	{
		// クラスの宣言 ==========================================================
		// タスクマネージャー
		class TaskManager;


		// クラスの定義 ==========================================================
		class StageManager
		{
			// <メンバ変数>
		private:
			// 生成したタイルの保存用コンテナ
			std::list<Play::Object::Stage::Tile*> m_tileList;


			// <コンストラクタ>
		public:
			//--------------------------------------------------------------
			//! @parameter [void] なし
			//--------------------------------------------------------------
			StageManager();


			// <デストラクタ>
		public:
			~StageManager();


			// <メンバ関数>
		public:
			//--------------------------------------------------------------
			//! @summary   ステージの作成処理
			//!
			//! @parameter [stageData] ステージデータ
			//! @parameter [camera] カメラ
			//! @parameter [taskManager] タスクマネージャー
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Create(std::vector<int>& stageData, Library::Camera::Camera* camera, TaskManager* taskManager);


			//--------------------------------------------------------------
			//! @summary   ステージデータの読み込み
			//!
			//! @parameter [stageNumber] 読み込むステージデータ
			//! @parameter [stageData] ステージデータ
			//! @parameter [objectData] オブジェクトデータ
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void LoadStageData(int stageNumber, std::vector<int>& stageData, std::vector<int>& objectData);
		};
	}
}
