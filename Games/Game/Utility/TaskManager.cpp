//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		TaskManager.cpp
//!
//! @summary	モトスのタスク管理クラスのソースファイル
//!
//! @date		2018.11.14
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダーファイル>
#include "TaskManager.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Utility::TaskManager::TaskManager()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Motos::Utility::TaskManager::~TaskManager()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   タスクの登録処理
//!
//! @parameter [task] 登録するタスク
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Utility::TaskManager::Entry(Library::System::GameObject* task)
{
	task->Start();
	m_taskList.push_back(task);
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [timer] 時間情報
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Utility::TaskManager::Update(const Common::StepTimer& timer)
{
	if (m_taskList.size() == 0) return;
	for (list<Library::System::GameObject*>::iterator itr = m_taskList.begin(); itr != m_taskList.end(); ++itr)
	{
		(*itr)->Update(timer);
	}
}



//--------------------------------------------------------------------
//! @summary   描画処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Motos::Utility::TaskManager::Draw()
{
	if (m_taskList.size() == 0) return;
	for (list<Library::System::GameObject*>::iterator itr = m_taskList.begin(); itr != m_taskList.end(); ++itr)
	{
		(*itr)->Draw();
	}
}
