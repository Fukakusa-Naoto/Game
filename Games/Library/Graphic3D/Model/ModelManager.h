//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		ModelManager.h
//!
//! @summary	モデルデータ管理クラスのヘッダファイル
//!
//! @date		2018.09.27
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <標準ヘッダファイル>
#include <fbxsdk.h>
#include <map>

// <自作ヘッダファイル>
#include "../../System/SingletonBase.h"
#include "ModelDeta.h"
#include "Model.h"
#include "../../Utility/String.h"


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Graphic3D
	{
		namespace Model
		{
			// クラスの定義 ======================================================
			class ModelManager : public System::SingletonBase<ModelManager>
			{
				// <フレンドの登録>
				friend class System::SingletonBase<ModelManager>;


				// <メンバ変数>
			private:
				// モデルデータの保管用コンテナ
				std::map<const wchar_t*, Model*> m_modelMap;


				// <コンストラクタ>
			private:
				//----------------------------------------------------------
				//! @parameter [void] なし
				//----------------------------------------------------------
				ModelManager();


			public:
				~ModelManager();


				// <メンバ関数>
			public:
				//----------------------------------------------------------
				//! @summary   PMDのモデルデータの読み込み処理
				//!
				//! @parameter [fileName] 読み込むモデルデータのファイル名
				//!
				//! @return    モデルデータのインスタンス
				//----------------------------------------------------------
				Model* LoadModelDataFromPMD(const wchar_t* fileName);


				//----------------------------------------------------------
				//! @summary   PMXのモデルデータの読み込み処理
				//!
				//! @parameter [fileName] 読み込むモデルデータのファイル名
				//!
				//! @return    モデルデータのインスタンス
				//----------------------------------------------------------
				Model* LoadModelDataFromPMX(const wchar_t* fileName);


			private:
				//----------------------------------------------------------
				//! @summary   ファイルからPMDModelを生成する
				//!
				//! @parameter [stream] 読み込まれたファイル
				//!
				//! @return    生成されたPMDModel
				//----------------------------------------------------------
				PMDModel* CreatePMDModel(std::ifstream* stream);


				//----------------------------------------------------------
				//! @summary   ファイルからPMXModelを生成する
				//!
				//! @parameter [stream] 読み込まれたファイル
				//!
				//! @return    生成されたPMXModel
				//----------------------------------------------------------
				PMXModel* CreatePMXModel(std::ifstream* stream);


				//----------------------------------------------------------
				//! @summary   文字列を読み込む
				//!
				//! @parameter [stream] 読み込まれたファイル
				//! @parameter [encoding] エンコード
				//!
				//! @return    読み込んだ文字列
				//----------------------------------------------------------
				std::wstring ReadString(std::istream *stream, uint8_t encoding);
			};
		}
	}
}
