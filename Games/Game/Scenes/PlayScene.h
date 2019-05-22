//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		PlayScene.h
//!
//! @summary	モトスのプレイシーンクラスのヘッダファイル
//!
//! @date		2018.10.29
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <自作ヘッダーファイル>
#include "../../Library/Scene/SceneBase.h"
#include "../../Library/Graphic2D/UI/Canvas.h"
#include "../../Library/Camera/Camera.h"
#include "../Play/Object/UI/UIFrame.h"
#include "../Utility/TaskManager.h"
#include "../Utility/StageManager.h"
#include "../Play/Object/Player.h"
#include "../Utility/EnemyManager.h"
#include "../../Library/Graphic2D/PrimitiveManager2D.h"
#include "../Play/Object/UI/ReadyAndGo.h"
#include "../Play/Object/Stage/SkyDome.h"


// 名前空間の定義 ================================================================
namespace Motos
{
	namespace Scene
	{
		// クラスの定義 ==========================================================
		class PlayScene : public Library::Scene::SceneBase
		{
			// <列挙型>
			enum SequenceID
			{
				FADE_IN,
				READY,
				NORMAL,
				FADE_OUT
			};


			// <メンバ変数>
		private:
			// カメラ
			Library::Camera::Camera* m_camera;
			// タスクマネージャー
			Utility::TaskManager* m_taskManager;
			// ステージマネージャー
			Utility::StageManager* m_stageManager;
			// エネミーマネージャー
			Utility::EnemyManager* m_enemyManager;
			// プレイヤー
			Play::Object::Player* m_player;
			// スカイドーム
			Play::Object::Stage::SkyDome* m_skyDome;


			// --- UI ---
			// UIキャンバス
			Library::Graphic2D::UI::Canvas* m_canvas;
			// UI背景
			Play::Object::UI::UIFrame* m_uiFrame;
			// ReadyとGo!!
			Play::Object::UI::ReadyAndGo* m_readyAndGo;


			// ----- 演出 -----
			// シーケンス状態
			SequenceID m_sequenceID;
			// フェードイン・フェードアウトの時間
			float m_fadeTime;
			// 2Dのプリミティブ描画マネージャー
			Library::Graphic2D::PrimitiveManager2D* m_primitiveManager;
			// スクリーンビューポート
			D3D11_VIEWPORT m_screenViewport;


			// <コンストラクタ>
		public:
			//--------------------------------------------------------------
			//! @parameter [void] なし
			//--------------------------------------------------------------
			PlayScene();


			// <デストラクタ>
			~PlayScene();


			// <メンバ関数>
		public:
			//--------------------------------------------------------------
			//! @summary   初期化処理
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Initialize() override;



			//--------------------------------------------------------------
			//! @summary   終了処理
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Finalize() override;


		private:
			//--------------------------------------------------------------
			//! @summary   更新処理
			//!
			//! @parameter [timer] 時間情報
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Update(const Library::Common::StepTimer & timer) override;



			//--------------------------------------------------------------
			//! @summary   描画処理
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Render() override;
		};
	}
}
