//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		SceneManager.cpp
//!
//! @summary	シーンマネージャークラスのソースファイル
//!
//! @date		2018.09.20
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <自作ヘッダーファイル>
#include "SceneManager.h"
#include "SceneBase.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Scene::SceneManager::SceneManager() :
	m_nextScene(nullptr)
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Scene::SceneManager::~SceneManager()
{
	for (list<SceneBase*>::iterator itr = m_activeScene.begin(); itr != m_activeScene.end(); ++itr) (*itr)->Finalize();

	m_activeScene.clear();

	for (map<string, SceneBase*>::iterator itr = m_sceneList.begin(); itr != m_sceneList.end(); ++itr)
	{
		std::pair<string, SceneBase*> scene = *itr;
		delete scene.second;
	}
	m_sceneList.clear();
}



//--------------------------------------------------------------------
//! @summary   シーンの登録処理
//!
//! @parameter [entryScene] 登録するシーン
//!
//! @return    なし
//--------------------------------------------------------------------
void Scene::SceneManager::Entry(SceneBase* entryScene)
{
	std::string sceneName(typeid(*entryScene).name());
	if (m_sceneList.count(sceneName.substr(6, sceneName.size() - 6)) == 0)
	{
		m_sceneList[std::string(sceneName.substr(6, sceneName.size() - 6))] = entryScene;
	}
}



//--------------------------------------------------------------------
//! @summary   最初のシーンを設定する
//!
//! @parameter [startScene] 最初のシーン
//!
//! @return    なし
//--------------------------------------------------------------------
void Scene::SceneManager::Start(string startScene)
{
	if (m_sceneList.count(startScene) != 0)
	{
		m_activeScene.push_back(m_sceneList[startScene]);
		m_activeScene.back()->Initialize();
	}
}



//--------------------------------------------------------------------
//! @summary   活動中のシーンの更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Scene::SceneManager::UpdateActiveScene(const Common::StepTimer& timer)
{
	if (m_nextScene) ChangeScene();

	if (!m_activeScene.empty()) m_activeScene.back()->Update(timer);
}



//--------------------------------------------------------------------
//! @summary   活動中のシーンの描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Scene::SceneManager::RenderActiveScene()
{
	if (!m_activeScene.empty())
	{
		for (list<SceneBase*>::iterator itr = m_activeScene.begin(); itr != m_activeScene.end(); ++itr) (*itr)->Render();
	}
}



//--------------------------------------------------------------------
//! @summary   活動中の全てのシーンを破棄して新しくシーンを開始する
//!
//! @parameter [nextScene] 読み込む次のシーン
//!
//! @return    true :シーン変更ができた
//! @return    false:シーン変更ができなかった
//--------------------------------------------------------------------
bool Scene::SceneManager::LoadScene(string nextScene)
{
	if (m_sceneList.count(nextScene) != 0)
	{
		m_nextScene = m_sceneList[nextScene];
		return true;
	}
	else return false;

}



//--------------------------------------------------------------------
//! @summary   シーンを上乗せする
//!
//! @parameter [stackScene] 上乗せするシーン
//!
//! @return    true :シーンの上乗せができた
//! @return    false:シーンの上乗せができなかった
//--------------------------------------------------------------------
bool Scene::SceneManager::StackScene(string stackScene)
{
	if (m_sceneList.count(stackScene) == 0)
	{
		m_activeScene.push_back(m_sceneList[stackScene]);
		m_activeScene.back()->Initialize();

		return true;
	}
	else return false;
}



//--------------------------------------------------------------------
//! @summary   シーンを戻る
//!
//! @parameter [backScene] 目的のシーン
//!
//! @return    true :戻ることができた
//! @return    false:戻ることができなかった
//--------------------------------------------------------------------
bool Scene::SceneManager::BackScene(string backScene)
{
	SceneBase* tmp = CheckScene(m_activeScene, *m_sceneList[backScene]);

	if (tmp != nullptr)
	{
		for (list<SceneBase*>::iterator itr = std::next(m_activeScene.end(), -1); itr != std::find(m_activeScene.begin(), m_activeScene.end(), tmp); itr--)
		{
			(*itr)->Finalize();
			itr = m_activeScene.erase(itr);
		}

		return true;
	}
	else return false;
}



//--------------------------------------------------------------------
//! @summary   リスト内に欲しいシーンがあるか調べる
//!
//! @parameter [sceneList] 検索をかけるリスト
//! @parameter [scene] 調べるシーン
//!
//! @return    アドレス :リスト内にシーンがあった
//! @return    nullptr :リスト内にシーンがなかった
//--------------------------------------------------------------------
Scene::SceneBase* Scene::SceneManager::CheckScene(const list<SceneBase*>& sceneList, const SceneBase& scene)
{
	for (_List_const_iterator<_List_val<_List_simple_types<SceneBase*>>> itr = sceneList.begin(); itr != sceneList.end(); ++itr)
	{
		if (typeid(**itr) == typeid(scene)) return (*itr);
	}

	return nullptr;
}



//--------------------------------------------------------------------
//! @summary   シーンの変更
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Scene::SceneManager::ChangeScene()
{
	for (list<SceneBase*>::iterator itr = m_activeScene.begin(); itr != m_activeScene.end(); ++itr) (*itr)->Finalize();
	m_activeScene.clear();

	m_activeScene.push_back(m_nextScene);
	m_activeScene.back()->Initialize();

	m_nextScene = nullptr;
}
