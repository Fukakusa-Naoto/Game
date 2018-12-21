//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		SoundManager.h
//!
//! @summary	サウンドマネージャークラスのヘッダファイル
//!
//! @date		2018.10.12
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once


// ヘッダファイルの読み込み =======================================================
// <標準ヘッダファイル>
#include <string>
#include <map>

// <自作ヘッダファイル>
#include "SoundPlayer.h"
#include "../System/SingletonBase.h"


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Sound
	{
		// クラスの定義 ==========================================================
		class SoundManager : public System::SingletonBase<SoundManager>
		{
			// <フレンド登録>
			friend class System::SingletonBase<SoundManager>;


			// <メンバ定数>
			// 最大ボイス数を増やすための関連パラメータ
			static const int MAX_VOICE = 24;

			// ざっくり多め(通常ボイス＋HCA-MXボイス＋α)
			static const int MAX_VIRTUAL_VOICE = 64;

			// ざっくり多め(通常ボイス＋HCA-MXボイス＋α)
			static const int MAX_CRIFS_LOADER = 64;

			// 最大サンプリングレート（ピッチ変更含む）
			static const int MAX_SAMPLING_RATE = (48000 * 2);

			// HCA-MXコーデックのサンプリングレート
			static const int SAMPLINGRATE_HCAMX = 32000;

			// リソースディレクトリパス
			static const std::wstring RESOURCE_DIRECTORY;


			// <メンバ変数>
			// ボイスプール(ADX/HCAコーデック用)
			CriAtomExVoicePoolHn	m_standardVoicePool;

			// ボイスプール(HCA-MX用)
			CriAtomExVoicePoolHn	m_hcamxVoicePool;

			// D-BASハンドル
			CriAtomDbasId		m_dbasID;

			// プレイヤー
			SoundPlayer m_player;

			// 音楽情報記憶用変数のリスト
			std::map<CriAtomExCueId, CriAtomExPlaybackId> m_playbackList;


			// <コンストラクタ>
		private:
			//--------------------------------------------------------------
			//! @parameter [void] なし
			//--------------------------------------------------------------
			SoundManager();


			// <デストラクタ>
		public:
			~SoundManager();


			// <メンバ関数>
		public:
			//--------------------------------------------------------------
			//! @summary   初期化処理
			//!
			//! @parameter [acf] 読み込むacfファイル
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Initialize(const wchar_t *acf);


			//--------------------------------------------------------------
			//! @summary   終了処理
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Finalize();


			//--------------------------------------------------------------
			//! @summary   更新処理
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Update();


			//--------------------------------------------------------------
			//! @summary   Acbファイルの読み込み
			//!
			//! @parameter [acb] Acbファイル名
			//! @parameter [awb] Awbファイル名
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void LoadAcb(const wchar_t *acb, const wchar_t *awb = nullptr);


			//--------------------------------------------------------------
			//! @summary   指定キューの再生
			//!
			//! @parameter [cueID] 再生したい音のキューID
			//! @parameter [volume] 音量
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Play(CriAtomExCueId cueID, float volume = 1.0f);


			//--------------------------------------------------------------
			//! @summary   音量の設定
			//!
			//! @parameter [volume] 音量
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void SetVolume(float volume);


			//--------------------------------------------------------------
			//! @summary   サウンドプレイヤーのポーズ
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Pause();


			//--------------------------------------------------------------
			//! @summary   サウンドプレイヤーのポーズ状態の取得
			//!
			//! @parameter [void] なし
			//!
			//! @return    true :ポーズ中
			//! @return    false:ポーズ以外の状態
			//--------------------------------------------------------------
			bool IsPause();


			//--------------------------------------------------------------
			//! @summary   サウンドプレイヤーの停止
			//!
			//! @parameter [void] なし
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Stop();


			//--------------------------------------------------------------
			//! @summary   指定した音の停止
			//!
			//! @parameter [cueID] 停止したい音のキューID
			//!
			//! @return    なし
			//--------------------------------------------------------------
			void Stop(CriAtomExCueId cueID);


			//--------------------------------------------------------------
			//! @summary   指定した音の再生状態の取得
			//!
			//! @parameter [cueID] 取得したい音のキューID
			//!
			//! @return    true :再生中
			//! @return    false:再生していない
			//--------------------------------------------------------------
			bool IsState(CriAtomExCueId cueID);


			// <静的なメンバ関数>
		private:
			//--------------------------------------------------------------
			//! @summary   エラーコールバック関数のユーザ実装
			//!
			//! @parameter [errid]
			//! @parameter [p1]
			//! @parameter [p2]
			//! @parameter [parray]
			//!
			//! @return    なし
			//--------------------------------------------------------------
			static void user_error_callback_func(const CriChar8 *errid, CriUint32 p1, CriUint32 p2, CriUint32 *parray);


			//--------------------------------------------------------------
			//! @summary   メモリ確保関数のユーザ実装
			//!
			//! @parameter [obj]
			//! @parameter [size] 確保するメモリのサイズ
			//!
			//! @return    確保されたメモリのポインタ
			//--------------------------------------------------------------
			static void *user_alloc_func(void *obj, CriUint32 size);


			//--------------------------------------------------------------
			//! @summary   メモリ解放関数のユーザ実装
			//!
			//! @parameter [obj]
			//! @parameter [ptr] 解放するメモリ
			//!
			//! @return    なし
			//--------------------------------------------------------------
			static void user_free_func(void *obj, void *ptr);


			// <ゲッター関数>
		public:
			//--------------------------------------------------------------
			//! @summary   サウンドプレイヤーの取得
			//--------------------------------------------------------------
			inline SoundPlayer* GetPlayer() { return &m_player; }

		};
	}
}