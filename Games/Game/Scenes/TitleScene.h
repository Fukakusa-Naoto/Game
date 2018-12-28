//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		TitleScene.h
//!
//! @summary	モトスのタイトルシーンクラスのヘッダファイル
//!
//! @date		2018.11.01
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <自作ヘッダーファイル>
#include "../../Library/Scene/SceneBase.h"
#include "../../Library/Graphic2D/UI/Canvas.h"
#include "../Title/Object/UI/Title.h"
#include "../Title/Object/UI/BaseBackground.h"
#include "../Title/Object/UI/TileBackground.h"
#include "../Title/Object/UI/SelectUI.h"
#include "../../Library/Graphic2D/PrimitiveManager2D.h"


// 名前空間の定義 ================================================================
namespace Motos
{
	namespace Scene
	{
		// クラスの定義 ==========================================================
		class TitleScene : public Library::Scene::SceneBase
		{
			// <列挙型>
			enum SequenceID
			{
				FADE_IN,
				NORMAL,
				FADE_OUT
			};

			// <メンバ変数>
		private:
			// UIキャンバス
			Library::Graphic2D::UI::Canvas* m_canvas;
			// タイトル
			Title::Object::UI::Title* m_title;
			// ベース背景
			Title::Object::UI::BaseBackground* m_baseBackground;
			// タイル背景
			Title::Object::UI::TileBackground* m_tileBackground;
			// セレクトUI
			Title::Object::UI::SelectUI* m_selectUI;
			// シーケンス状態
			SequenceID m_sequenceID;
			// フェードイン・フェードアウトの時間
			float m_fadeTime;
			// 2Dのプリミティブ描画マネージャー
			Library::Graphic2D::PrimitiveManager2D* m_primitiveManager;


			// <コンストラクタ>
		public:
			//--------------------------------------------------------------
			//! @parameter [void] なし
			//--------------------------------------------------------------
			TitleScene();


			// <デストラクタ>
			~TitleScene();


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
