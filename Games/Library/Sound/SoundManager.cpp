//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		SoundManager.cpp
//!
//! @summary	サウンドマネージャークラスのソースファイル
//!
//! @date		2018.10.12
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <標準ヘッダファイル>
#include <codecvt>

// <自作ヘッダファイル>
#include "SoundManager.h"

// usingディレクティブ =====================================================
using namespace std;
using namespace Library;


// メンバ定数の定義 ========================================================
// リソースディレクトリパス
const wstring Sound::SoundManager::RESOURCE_DIRECTORY = L"Resources/Sounds/";


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Sound::SoundManager::SoundManager()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Sound::SoundManager::~SoundManager()
{
	Finalize();
}



//--------------------------------------------------------------------
//! @summary   初期化処理
//!
//! @parameter [acf] 読み込むacfファイル
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundManager::Initialize(const wchar_t *acf)
{
	// エラーコールバック関数の登録
	criErr_SetCallback(user_error_callback_func);

	// メモリアロケータの登録
	criAtomEx_SetUserAllocator(user_alloc_func, user_free_func, NULL);

	// ライブラリ初期化
	CriAtomExConfig_WASAPI lib_config;
	CriFsConfig fs_config;
	criAtomEx_SetDefaultConfig_WASAPI(&lib_config);
	criFs_SetDefaultConfig(&fs_config);
	lib_config.atom_ex.max_virtual_voices = MAX_VIRTUAL_VOICE;
	lib_config.hca_mx.output_sampling_rate = SAMPLINGRATE_HCAMX;
	fs_config.num_loaders = MAX_CRIFS_LOADER;
	lib_config.atom_ex.fs_config = &fs_config;
	criAtomEx_Initialize_WASAPI(&lib_config, NULL, 0);

	// ストリーミング用バッファの作成
	m_dbasID = criAtomDbas_Create(NULL, NULL, 0);

	// フルパスに補完
	if (acf)
	{
		wstring fullpath_wstr = RESOURCE_DIRECTORY + acf;
		// stringに変換
		wstring_convert<codecvt_utf8<wchar_t>, wchar_t> cv;
		string fullpath_str = cv.to_bytes(fullpath_wstr);
		// ACFファイルの読み込みと登録
		criAtomEx_RegisterAcfFile(NULL, fullpath_str.c_str(), NULL, 0);
	}

	// DSPバス設定の登録
	criAtomEx_AttachDspBusSetting("DspBusSetting_0", NULL, 0);

	// ボイスプールの作成（最大ボイス数変更／最大ピッチ変更／ストリーム再生対応）
	CriAtomExStandardVoicePoolConfig standard_vpool_config;
	criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&standard_vpool_config);
	standard_vpool_config.num_voices = MAX_VOICE;
	standard_vpool_config.player_config.max_sampling_rate = MAX_SAMPLING_RATE;
	standard_vpool_config.player_config.streaming_flag = CRI_TRUE;
	m_standardVoicePool = criAtomExVoicePool_AllocateStandardVoicePool(&standard_vpool_config, NULL, 0);

	// HCA-MX再生用：ボイスプールの作成
	CriAtomExHcaMxVoicePoolConfig hcamx_vpool_config;
	criAtomExVoicePool_SetDefaultConfigForHcaMxVoicePool(&hcamx_vpool_config);
	hcamx_vpool_config.num_voices = MAX_VOICE;
	hcamx_vpool_config.player_config.max_sampling_rate = MAX_SAMPLING_RATE;
	hcamx_vpool_config.player_config.streaming_flag = CRI_TRUE;
	m_hcamxVoicePool = criAtomExVoicePool_AllocateHcaMxVoicePool(&hcamx_vpool_config, NULL, 0);

	// プレイヤーの作成
	m_player.Create();
}



//--------------------------------------------------------------------
//! @summary   終了処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundManager::Finalize()
{
	// プレイヤーの破棄
	m_player.Release();

	// 音楽情報記憶用変数のリストの破棄
	m_playbackList.clear();

	// DSPのデタッチ
	criAtomEx_DetachDspBusSetting();

	// ボイスプールの破棄
	criAtomExVoicePool_Free(m_hcamxVoicePool);
	criAtomExVoicePool_Free(m_standardVoicePool);

	// ACFの登録解除
	criAtomEx_UnregisterAcf();

	// D-BASの破棄
	criAtomDbas_Destroy(m_dbasID);

	// ライブラリの終了
	criAtomEx_Finalize_WASAPI();
}



//--------------------------------------------------------------------
//! @summary   更新処理
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundManager::Update()
{
	criAtomEx_ExecuteMain();
}




//--------------------------------------------------------------------
//! @summary   Acbファイルの読み込み
//!
//! @parameter [acb] Acbファイル名
//! @parameter [awb] Awbファイル名
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundManager::LoadAcb(const wchar_t *acb, const wchar_t *awb)
{
	// フルパスに補完
	wstring fullpath_acb_wstr = RESOURCE_DIRECTORY + acb;
	wstring fullpath_awb_wstr = RESOURCE_DIRECTORY + awb;
	// stringに変換
	wstring_convert<codecvt_utf8<wchar_t>, wchar_t> cv;
	string fullpath_acb_str = cv.to_bytes(fullpath_acb_wstr);
	string fullpath_awb_str = cv.to_bytes(fullpath_awb_wstr);

	m_player.LoadAcb(fullpath_acb_str.c_str(), fullpath_awb_str.c_str());
}



//--------------------------------------------------------------------
//! @summary   指定キューの再生
//!
//! @parameter [cueID] 再生したい音のキューID
//! @parameter [volume] 音量
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundManager::Play(CriAtomExCueId cueID, float volume)
{
	if (!IsState(cueID)) m_playbackList[cueID] = m_player.Play(cueID, volume);
	else m_player.Play(cueID, volume);
}



//--------------------------------------------------------------------
//! @summary   音量の設定
//!
//! @parameter [volume] 音量
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundManager::SetVolume(float volume)
{
	m_player.SetVolume(volume);
}



//--------------------------------------------------------------------
//! @summary   サウンドプレイヤーのポーズ
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundManager::Pause()
{
	m_player.Pause();
}



//--------------------------------------------------------------------
//! @summary   サウンドプレイヤーのポーズ状態の取得
//!
//! @parameter [void] なし
//!
//! @return    true :ポーズ中
//! @return    false:ポーズ以外の状態
//--------------------------------------------------------------------
bool Sound::SoundManager::IsPause()
{
	return m_player.IsPause();
}



//--------------------------------------------------------------------
//! @summary   サウンドプレイヤーの停止
//!
//! @parameter [void] なし
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundManager::Stop()
{
	m_player.Stop();
}



//--------------------------------------------------------------------
//! @summary   指定した音の停止
//!
//! @parameter [cueID] 停止したい音のキューID
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundManager::Stop(CriAtomExCueId cueID)
{
	m_player.Stop(m_playbackList[cueID]);
}



//--------------------------------------------------------------------
//! @summary   指定した音の再生状態の取得
//!
//! @parameter [cueID] 取得したい音のキューID
//!
//! @return    true :再生中
//! @return    false:再生していない
//--------------------------------------------------------------------
bool Sound::SoundManager::IsState(CriAtomExCueId cueID)
{
	return m_player.IsPlayStateByID(m_playbackList[cueID]);
}



//--------------------------------------------------------------------
//! @summary   エラーコールバック関数のユーザ実装
//!
//! @parameter [errid]
//! @parameter [p1]
//! @parameter [p2]
//! @parameter [parray]
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundManager::user_error_callback_func(const CriChar8 *errid, CriUint32 p1, CriUint32 p2, CriUint32 *parray)
{
	const CriChar8 *errmsg;

	// エラーコードからエラー文字列に変換してコンソール出力する
	errmsg = criErr_ConvertIdToMessage(errid, p1, p2);

	const unsigned int dataSize = 256;
	wchar_t wlocal[dataSize + 1] = { 0x00 };

	// マルチバイト文字をunicodeへ変換する
	MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED,
		errmsg,
		dataSize,
		wlocal,
		dataSize + 1);

	// コンソール出力する
	OutputDebugString(wlocal);

	return;
}



//--------------------------------------------------------------------
//! @summary   メモリ確保関数のユーザ実装
//!
//! @parameter [obj]
//! @parameter [size] 確保するメモリのサイズ
//!
//! @return    確保されたメモリのポインタ
//--------------------------------------------------------------------
void* Sound::SoundManager::user_alloc_func(void* obj, CriUint32 size)
{
	void *ptr;
	ptr = malloc(size);
	return ptr;
}



//--------------------------------------------------------------------
//! @summary   メモリ解放関数のユーザ実装
//!
//! @parameter [obj]
//! @parameter [ptr] 解放するメモリ
//!
//! @return    なし
//--------------------------------------------------------------------
void Sound::SoundManager::user_free_func(void* obj, void* ptr)
{
	free(ptr);
}
