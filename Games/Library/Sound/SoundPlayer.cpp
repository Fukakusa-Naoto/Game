//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		SoundPlyer.cpp
//!
//! @summary	サウンドプレイヤークラスのソースファイル
//!
//! @date		2018.10.12
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <自作ヘッダファイル>
#include "SoundPlayer.h"


// usingディレクティブ =====================================================
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Sound::SoundPlayer::SoundPlayer()
{
	m_acbHandle = nullptr;
	m_player = nullptr;
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Sound::SoundPlayer::~SoundPlayer()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   サウンドプレイヤーの作成
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundPlayer::Create()
{
	// プレイヤーの作成
	if (m_player == nullptr) m_player = criAtomExPlayer_Create(nullptr, nullptr, 0);
}



//--------------------------------------------------------------------
//! @summary   Acbファイルの読み込み
//!
//! @parameter [acb] Acbファイル名
//! @parameter [awb] Awbファイル名
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundPlayer::LoadAcb(const char *acb, const char *awb)
{
	// ACBファイル読み込み
	if (m_acbHandle != nullptr)
	{
		// ACBハンドルの破棄
		criAtomExAcb_Release(m_acbHandle);
	}
	m_acbHandle = criAtomExAcb_LoadAcbFile(nullptr, acb, nullptr, awb, nullptr, 0);
}



//--------------------------------------------------------------------
//! @summary   サウンドプレイヤーの開放処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundPlayer::Release()
{
	// プレーヤハンドルの破棄
	if (m_player != nullptr) criAtomExPlayer_Destroy(m_player);
	// ACBハンドルの破棄
	if (m_acbHandle != nullptr) criAtomExAcb_Release(m_acbHandle);
}



//--------------------------------------------------------------------
//! @summary   指定キューの再生
//!
//! @parameter [cueID] 再生したいキューID
//! @parameter [volume] 音量
//!
//! @return    プレイバックID
//--------------------------------------------------------------------
CriAtomExPlaybackId Sound::SoundPlayer::Play(CriAtomExCueId cueID, float volume)
{
	// 音量の設定
	criAtomExPlayer_SetVolume(m_player, volume);
	// キューIDの指定
	criAtomExPlayer_SetCueId(m_player, m_acbHandle, cueID);

	// 再生の開始
	return criAtomExPlayer_Start(m_player);
}



//--------------------------------------------------------------------
//! @summary   音量の設定
//!
//! @parameter [volume] 音量
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundPlayer::SetVolume(float volume)
{
	criAtomExPlayer_SetVolume(m_player, volume);
	criAtomExPlayer_UpdateAll(m_player);
}


//--------------------------------------------------------------------
//! @summary   指定した音の音量の設定
//!
//! @parameter [playbackID] プレイバックID
//! @parameter [volume] 音量
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundPlayer::SetVolume(CriAtomExPlaybackId playbackID, float volume)
{
	criAtomExPlayer_SetVolume(m_player, volume);
	criAtomExPlayer_Update(m_player, playbackID);
}



//--------------------------------------------------------------------
//! @summary   サウンドプレイヤーのポーズ
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundPlayer::Pause()
{
	// ポーズ中？
	if (criAtomExPlayer_IsPaused(m_player) == CRI_TRUE)
	{
		// ポーズ解除
		criAtomExPlayer_Pause(m_player, CRI_FALSE);
	}
	else
	{
		// ポーズ
		criAtomExPlayer_Pause(m_player, CRI_TRUE);
	}
}



//--------------------------------------------------------------------
//! @summary   指定した音をポーズする
//!
//! @parameter [playbackID] プレイバックID
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundPlayer::Pause(CriAtomExPlaybackId playbackID)
{
	// ポーズ中？
	if (criAtomExPlayback_IsPaused(playbackID) == CRI_TRUE)
	{
		// ポーズ解除
		criAtomExPlayback_Pause(playbackID, CRI_FALSE);
	}
	else
	{
		// ポーズ
		criAtomExPlayback_Pause(playbackID, CRI_TRUE);
	}
}



//--------------------------------------------------------------------
//! @summary   サウンドプレイヤーのポーズ状態の取得
//!
//! @parameter [void] なし
//!
//! @return    true :ポーズ中
//! @return    false:ポーズ以外の状態
//--------------------------------------------------------------------
bool Sound::SoundPlayer::IsPause()
{
	return (criAtomExPlayer_IsPaused(m_player) == CRI_TRUE) ? true : false;
}



//--------------------------------------------------------------------
//! @summary   指定した音のポーズ状態の取得
//!
//! @parameter [playbackID] プレイバックID
//!
//! @return    true :ポーズ中
//! @return    false:ポーズ以外の状態
//--------------------------------------------------------------------
bool Sound::SoundPlayer::IsPause(CriAtomExPlaybackId playbackID)
{
	return (criAtomExPlayback_IsPaused(playbackID) == CRI_TRUE) ? true : false;
}



//--------------------------------------------------------------------
//! @summary   サウンドプレイヤーの停止
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundPlayer::Stop()
{
	criAtomExPlayer_Stop(m_player);
}



//--------------------------------------------------------------------
//! @summary   指定した音の停止
//!
//! @parameter [playbackID] プレイバックID
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundPlayer::Stop(CriAtomExPlaybackId playbackID)
{
	criAtomExPlayback_Stop(playbackID);
}



//--------------------------------------------------------------------
//! @summary   指定した音の再生状態の取得
//!
//! @parameter [playbackID] プレイバックID
//!
//! @return    true :再生中
//! @return    false:再生していない
//--------------------------------------------------------------------
bool Sound::SoundPlayer::IsPlayStateByID(CriAtomExPlaybackId playbackID)
{
	return (criAtomExPlayback_GetStatus(playbackID) == CRIATOMEXPLAYBACK_STATUS_PLAYING) ? true : false;
}
