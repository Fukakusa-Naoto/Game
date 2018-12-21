//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		SoundManager.cpp
//!
//! @summary	�T�E���h�}�l�[�W���[�N���X�̃\�[�X�t�@�C��
//!
//! @date		2018.10.12
//!
//! @author		�[�����l
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� =================================================
// <�W���w�b�_�t�@�C��>
#include <codecvt>

// <����w�b�_�t�@�C��>
#include "SoundManager.h"

// using�f�B���N�e�B�u =====================================================
using namespace std;
using namespace Library;


// �����o�萔�̒�` ========================================================
// ���\�[�X�f�B���N�g���p�X
const wstring Sound::SoundManager::RESOURCE_DIRECTORY = L"Resources/Sounds/";


// �֐��̒�` ==============================================================
//--------------------------------------------------------------------
//! @summary   �R���X�g���N�^
//!
//! @parameter [void] �Ȃ�
//--------------------------------------------------------------------
Sound::SoundManager::SoundManager()
{
	// �������Ȃ�
}



//--------------------------------------------------------------------
//! @summary   �f�X�g���N�^
//!
//! @parameter [void] �Ȃ�
//--------------------------------------------------------------------
Sound::SoundManager::~SoundManager()
{
	Finalize();
}



//--------------------------------------------------------------------
//! @summary   ����������
//!
//! @parameter [acf] �ǂݍ���acf�t�@�C��
//!
//! @return    �Ȃ�
//--------------------------------------------------------------------
void Sound::SoundManager::Initialize(const wchar_t *acf)
{
	// �G���[�R�[���o�b�N�֐��̓o�^
	criErr_SetCallback(user_error_callback_func);

	// �������A���P�[�^�̓o�^
	criAtomEx_SetUserAllocator(user_alloc_func, user_free_func, NULL);

	// ���C�u����������
	CriAtomExConfig_WASAPI lib_config;
	CriFsConfig fs_config;
	criAtomEx_SetDefaultConfig_WASAPI(&lib_config);
	criFs_SetDefaultConfig(&fs_config);
	lib_config.atom_ex.max_virtual_voices = MAX_VIRTUAL_VOICE;
	lib_config.hca_mx.output_sampling_rate = SAMPLINGRATE_HCAMX;
	fs_config.num_loaders = MAX_CRIFS_LOADER;
	lib_config.atom_ex.fs_config = &fs_config;
	criAtomEx_Initialize_WASAPI(&lib_config, NULL, 0);

	// �X�g���[�~���O�p�o�b�t�@�̍쐬
	m_dbasID = criAtomDbas_Create(NULL, NULL, 0);

	// �t���p�X�ɕ⊮
	if (acf)
	{
		wstring fullpath_wstr = RESOURCE_DIRECTORY + acf;
		// string�ɕϊ�
		wstring_convert<codecvt_utf8<wchar_t>, wchar_t> cv;
		string fullpath_str = cv.to_bytes(fullpath_wstr);
		// ACF�t�@�C���̓ǂݍ��݂Ɠo�^
		criAtomEx_RegisterAcfFile(NULL, fullpath_str.c_str(), NULL, 0);
	}

	// DSP�o�X�ݒ�̓o�^
	criAtomEx_AttachDspBusSetting("DspBusSetting_0", NULL, 0);

	// �{�C�X�v�[���̍쐬�i�ő�{�C�X���ύX�^�ő�s�b�`�ύX�^�X�g���[���Đ��Ή��j
	CriAtomExStandardVoicePoolConfig standard_vpool_config;
	criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&standard_vpool_config);
	standard_vpool_config.num_voices = MAX_VOICE;
	standard_vpool_config.player_config.max_sampling_rate = MAX_SAMPLING_RATE;
	standard_vpool_config.player_config.streaming_flag = CRI_TRUE;
	m_standardVoicePool = criAtomExVoicePool_AllocateStandardVoicePool(&standard_vpool_config, NULL, 0);

	// HCA-MX�Đ��p�F�{�C�X�v�[���̍쐬
	CriAtomExHcaMxVoicePoolConfig hcamx_vpool_config;
	criAtomExVoicePool_SetDefaultConfigForHcaMxVoicePool(&hcamx_vpool_config);
	hcamx_vpool_config.num_voices = MAX_VOICE;
	hcamx_vpool_config.player_config.max_sampling_rate = MAX_SAMPLING_RATE;
	hcamx_vpool_config.player_config.streaming_flag = CRI_TRUE;
	m_hcamxVoicePool = criAtomExVoicePool_AllocateHcaMxVoicePool(&hcamx_vpool_config, NULL, 0);

	// �v���C���[�̍쐬
	m_player.Create();
}



//--------------------------------------------------------------------
//! @summary   �I������
//!
//! @parameter [void] �Ȃ�
//!
//! @return    �Ȃ�
//--------------------------------------------------------------------
void Sound::SoundManager::Finalize()
{
	// �v���C���[�̔j��
	m_player.Release();

	// ���y���L���p�ϐ��̃��X�g�̔j��
	m_playbackList.clear();

	// DSP�̃f�^�b�`
	criAtomEx_DetachDspBusSetting();

	// �{�C�X�v�[���̔j��
	criAtomExVoicePool_Free(m_hcamxVoicePool);
	criAtomExVoicePool_Free(m_standardVoicePool);

	// ACF�̓o�^����
	criAtomEx_UnregisterAcf();

	// D-BAS�̔j��
	criAtomDbas_Destroy(m_dbasID);

	// ���C�u�����̏I��
	criAtomEx_Finalize_WASAPI();
}



//--------------------------------------------------------------------
//! @summary   �X�V����
//!
//! @parameter [void] �Ȃ�
//!
//! @return    �Ȃ�
//--------------------------------------------------------------------
void Sound::SoundManager::Update()
{
	criAtomEx_ExecuteMain();
}




//--------------------------------------------------------------------
//! @summary   Acb�t�@�C���̓ǂݍ���
//!
//! @parameter [acb] Acb�t�@�C����
//! @parameter [awb] Awb�t�@�C����
//!
//! @return    �Ȃ�
//--------------------------------------------------------------------
void Sound::SoundManager::LoadAcb(const wchar_t *acb, const wchar_t *awb)
{
	// �t���p�X�ɕ⊮
	wstring fullpath_acb_wstr = RESOURCE_DIRECTORY + acb;
	wstring fullpath_awb_wstr = RESOURCE_DIRECTORY + awb;
	// string�ɕϊ�
	wstring_convert<codecvt_utf8<wchar_t>, wchar_t> cv;
	string fullpath_acb_str = cv.to_bytes(fullpath_acb_wstr);
	string fullpath_awb_str = cv.to_bytes(fullpath_awb_wstr);

	m_player.LoadAcb(fullpath_acb_str.c_str(), fullpath_awb_str.c_str());
}



//--------------------------------------------------------------------
//! @summary   �w��L���[�̍Đ�
//!
//! @parameter [cueID] �Đ����������̃L���[ID
//! @parameter [volume] ����
//!
//! @return    �Ȃ�
//--------------------------------------------------------------------
void Sound::SoundManager::Play(CriAtomExCueId cueID, float volume)
{
	if (!IsState(cueID)) m_playbackList[cueID] = m_player.Play(cueID, volume);
	else m_player.Play(cueID, volume);
}



//--------------------------------------------------------------------
//! @summary   ���ʂ̐ݒ�
//!
//! @parameter [volume] ����
//!
//! @return    �Ȃ�
//--------------------------------------------------------------------
void Sound::SoundManager::SetVolume(float volume)
{
	m_player.SetVolume(volume);
}



//--------------------------------------------------------------------
//! @summary   �T�E���h�v���C���[�̃|�[�Y
//!
//! @parameter [void] �Ȃ�
//!
//! @return    �Ȃ�
//--------------------------------------------------------------------
void Sound::SoundManager::Pause()
{
	m_player.Pause();
}



//--------------------------------------------------------------------
//! @summary   �T�E���h�v���C���[�̃|�[�Y��Ԃ̎擾
//!
//! @parameter [void] �Ȃ�
//!
//! @return    true :�|�[�Y��
//! @return    false:�|�[�Y�ȊO�̏��
//--------------------------------------------------------------------
bool Sound::SoundManager::IsPause()
{
	return m_player.IsPause();
}



//--------------------------------------------------------------------
//! @summary   �T�E���h�v���C���[�̒�~
//!
//! @parameter [void] �Ȃ�
//!
//! @return    �Ȃ�
//--------------------------------------------------------------------
void Sound::SoundManager::Stop()
{
	m_player.Stop();
}



//--------------------------------------------------------------------
//! @summary   �w�肵�����̒�~
//!
//! @parameter [cueID] ��~���������̃L���[ID
//!
//! @return    �Ȃ�
//--------------------------------------------------------------------
void Sound::SoundManager::Stop(CriAtomExCueId cueID)
{
	m_player.Stop(m_playbackList[cueID]);
}



//--------------------------------------------------------------------
//! @summary   �w�肵�����̍Đ���Ԃ̎擾
//!
//! @parameter [cueID] �擾���������̃L���[ID
//!
//! @return    true :�Đ���
//! @return    false:�Đ����Ă��Ȃ�
//--------------------------------------------------------------------
bool Sound::SoundManager::IsState(CriAtomExCueId cueID)
{
	return m_player.IsPlayStateByID(m_playbackList[cueID]);
}



//--------------------------------------------------------------------
//! @summary   �G���[�R�[���o�b�N�֐��̃��[�U����
//!
//! @parameter [errid]
//! @parameter [p1]
//! @parameter [p2]
//! @parameter [parray]
//!
//! @return    �Ȃ�
//--------------------------------------------------------------------
void Sound::SoundManager::user_error_callback_func(const CriChar8 *errid, CriUint32 p1, CriUint32 p2, CriUint32 *parray)
{
	const CriChar8 *errmsg;

	// �G���[�R�[�h����G���[������ɕϊ����ăR���\�[���o�͂���
	errmsg = criErr_ConvertIdToMessage(errid, p1, p2);

	const unsigned int dataSize = 256;
	wchar_t wlocal[dataSize + 1] = { 0x00 };

	// �}���`�o�C�g������unicode�֕ϊ�����
	MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED,
		errmsg,
		dataSize,
		wlocal,
		dataSize + 1);

	// �R���\�[���o�͂���
	OutputDebugString(wlocal);

	return;
}



//--------------------------------------------------------------------
//! @summary   �������m�ۊ֐��̃��[�U����
//!
//! @parameter [obj]
//! @parameter [size] �m�ۂ��郁�����̃T�C�Y
//!
//! @return    �m�ۂ��ꂽ�������̃|�C���^
//--------------------------------------------------------------------
void* Sound::SoundManager::user_alloc_func(void* obj, CriUint32 size)
{
	void *ptr;
	ptr = malloc(size);
	return ptr;
}



//--------------------------------------------------------------------
//! @summary   ����������֐��̃��[�U����
//!
//! @parameter [obj]
//! @parameter [ptr] ������郁����
//!
//! @return    �Ȃ�
//--------------------------------------------------------------------
void Sound::SoundManager::user_free_func(void* obj, void* ptr)
{
	free(ptr);
}
