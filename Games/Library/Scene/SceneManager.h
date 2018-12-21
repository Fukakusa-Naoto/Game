//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		SceneManager.h
//!
//! @summary	シーンマネージャークラスのヘッダファイル
//!
//! @date		2018.09.20
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <標準ヘッダファイル>
#include <list>
#include <map>
#include <string>

// <自作ヘッダファイル>
#include "../Common/StepTimer.h"
#include "../System/SingletonBase.h"


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Scene
	{
		// クラスの宣言 ==========================================================
		class SceneBase;



		// クラスの定義 ==========================================================
		class SceneManager : public System::SingletonBase<SceneManager>
		{
			// <フレンド登録>
			friend class System::SingletonBase<SceneManager>;


			// <メンバ変数>
		private:
			std::list<SceneBase*> m_activeScene;
			std::map<std::string, SceneBase*> m_sceneList;


			// <コンストラクタ>
		private:
			//--------------------------------------------------------------
			//! @parameter [void] なし
			//--------------------------------------------------------------
			SceneManager();


			// <デストラクタ>
		public:
			~SceneManager();


			// <メンバ関数>
		public:
			//--------------------------------------------------------------
			//! @summary   シーンの登録処理
			//!
			//! @parameter [entryScene] 登録するシーン
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Entry(SceneBase* entryScene);


			//--------------------------------------------------------------
			//! @summary   最初のシーンを設定する
			//!
			//! @parameter [startScene] 最初のシーン
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Start(std::string startScene);


			//--------------------------------------------------------------
			//! @summary   活動中のシーンの更新処理
			//!
			//! @parameter [timer] 時間情報
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void UpdateActiveScene(const Common::StepTimer& timer);



			//--------------------------------------------------------------
			//! @summary   活動中のシーンの描画処理
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void RenderActiveScene();


			//--------------------------------------------------------------
			//! @summary   活動中の全てのシーンを破棄して新しくシーンを開始する
			//!
			//! @parameter [nextScene] 読み込む次のシーン
			//!
			//! @return    true :シーン変更ができた
			//! @return    false:シーン変更ができなかった
			//--------------------------------------------------------------
			bool LoadScene(std::string nextScene);


			//--------------------------------------------------------------
			//! @summary   シーンを上乗せする
			//!
			//! @parameter [stackScene] 上乗せするシーン
			//!
			//! @return    true :シーンの上乗せができた
			//! @return    false:シーンの上乗せができなかった
			//--------------------------------------------------------------
			bool StackScene(std::string stackScene);


			//--------------------------------------------------------------
			//! @summary   シーンを戻る
			//!
			//! @parameter [backScene] 目的のシーン
			//!
			//! @return    true :戻ることができた
			//! @return    false:戻ることができなかった
			//--------------------------------------------------------------
			bool BackScene(std::string backScene);


		private:
			//--------------------------------------------------------------
			//! @summary   リスト内に欲しいシーンがあるか調べる
			//!
			//! @parameter [sceneList] 検索をかけるリスト
			//! @parameter [scene] 調べるシーン
			//!
			//! @return    アドレス:リスト内にシーンがあった
			//! @return    nullptr :リスト内にシーンがなかった
			//--------------------------------------------------------------
			SceneBase* CheckScene(const std::list<SceneBase*>& sceneList, const SceneBase& scene);


			// <ゲッター関数>
		public:
			//--------------------------------------------------------------
			//! @summary   現在活動中のシーンの取得
			//--------------------------------------------------------------
			inline SceneBase* GetActiveScene()
			{
				if (m_activeScene.size() != 0) return m_activeScene.back();
				else return nullptr;
			}
		};
	}
}