//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		Game.h
//!
//! @summary	ゲームクラスのヘッダファイル
//!
//! @date		2018.09.18
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 ===================================================
#pragma once



// ヘッダファイルの読み込み =================================================
// <自作ヘッダーファイル>
#include "../Library/Framework/Framework.h"
#include "../../Library/Graphic2D/Sprite.h"
#include "../../Library/Camera/Camera.h"
#include "../Library/Sound/SoundManager.h"
#include "../Library/Graphic3D/Model/Model.h"


// クラスの定義 ============================================================
class Game : public Library::Framework::Framework
{
	// <メンバ定数>
public:
	// 画面の横幅
	static const int SCREEN_WIDTH = 800;

	// 画面の高さ
	static const int SCREEN_HEIGHT = 600;

	// ゲームタイトル
	static const std::wstring GAME_TITLE;


	// <メンバ変数>
private:
	// スプライト
	Library::Graphic2D::Sprite* m_sprite;
	// カメラ
	Library::Camera::Camera* m_camera;
	// サウンドマネージャ－
	Library::Sound::SoundManager* m_soundManager;
	// モデル
	Library::Graphic3D::Model::Model* m_model;
	Library::Graphic3D::Model::Model* m_model2;


	// <コンストラクタ>
public:
	//----------------------------------------------------------------
	//! @parameter [hInstance]
	//! @parameter [nCmdShow]
	//----------------------------------------------------------------
	Game(HINSTANCE hInstance, int nCmdShow);


	// <メンバ関数>
public:
	//----------------------------------------------------------------
	//! @summary   初期化処理
	//!
	//! @parameter [void] なし
	//!
	//! @return    なし
	//----------------------------------------------------------------
    void Initialize() override;



	//----------------------------------------------------------------
	//! @summary   終了処理
	//!
	//! @parameter [void] なし
	//!
	//! @return    なし
	//----------------------------------------------------------------
	void Finalize() override;


private:
	//----------------------------------------------------------------
	//! @summary   更新処理
	//!
	//! @parameter [timer] 時間情報
	//!
	//! @return    なし
	//----------------------------------------------------------------
    void Update(const Library::Common::StepTimer & timer) override;



	//----------------------------------------------------------------
	//! @summary   描画処理
	//!
	//! @parameter [void] なし
	//!
	//! @return    なし
	//----------------------------------------------------------------
    void Render() override;
};
