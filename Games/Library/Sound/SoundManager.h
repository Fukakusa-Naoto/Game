//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		SoundManager.h
//!
//! @summary	�T�E���h�}�l�[�W���[�N���X�̃w�b�_�t�@�C��
//!
//! @date		2018.10.12
//!
//! @author		�[�����l
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ���d�C���N���[�h�̖h�~ =========================================================
#pragma once


// �w�b�_�t�@�C���̓ǂݍ��� =======================================================
// <�W���w�b�_�t�@�C��>
#include <string>
#include <map>

// <����w�b�_�t�@�C��>
#include "SoundPlayer.h"
#include "../System/SingletonBase.h"


// ���O��Ԃ̒�` ================================================================
namespace Library
{
	namespace Sound
	{
		// �N���X�̒�` ==========================================================
		class SoundManager : public System::SingletonBase<SoundManager>
		{
			// <�t�����h�o�^>
			friend class System::SingletonBase<SoundManager>;


			// <�����o�萔>
			// �ő�{�C�X���𑝂₷���߂̊֘A�p�����[�^
			static const int MAX_VOICE = 24;

			// �������葽��(�ʏ�{�C�X�{HCA-MX�{�C�X�{��)
			static const int MAX_VIRTUAL_VOICE = 64;

			// �������葽��(�ʏ�{�C�X�{HCA-MX�{�C�X�{��)
			static const int MAX_CRIFS_LOADER = 64;

			// �ő�T���v�����O���[�g�i�s�b�`�ύX�܂ށj
			static const int MAX_SAMPLING_RATE = (48000 * 2);

			// HCA-MX�R�[�f�b�N�̃T���v�����O���[�g
			static const int SAMPLINGRATE_HCAMX = 32000;

			// ���\�[�X�f�B���N�g���p�X
			static const std::wstring RESOURCE_DIRECTORY;


			// <�����o�ϐ�>
			// �{�C�X�v�[��(ADX/HCA�R�[�f�b�N�p)
			CriAtomExVoicePoolHn	m_standardVoicePool;

			// �{�C�X�v�[��(HCA-MX�p)
			CriAtomExVoicePoolHn	m_hcamxVoicePool;

			// D-BAS�n���h��
			CriAtomDbasId		m_dbasID;

			// �v���C���[
			SoundPlayer m_player;

			// ���y���L���p�ϐ��̃��X�g
			std::map<CriAtomExCueId, CriAtomExPlaybackId> m_playbackList;


			// <�R���X�g���N�^>
		private:
			//--------------------------------------------------------------
			//! @parameter [void] �Ȃ�
			//--------------------------------------------------------------
			SoundManager();


			// <�f�X�g���N�^>
		public:
			~SoundManager();


			// <�����o�֐�>
		public:
			//--------------------------------------------------------------
			//! @summary   ����������
			//!
			//! @parameter [acf] �ǂݍ���acf�t�@�C��
			//!
			//! @return    �Ȃ�
			//--------------------------------------------------------------
			void Initialize(const wchar_t *acf);


			//--------------------------------------------------------------
			//! @summary   �I������
			//!
			//! @parameter [void] �Ȃ�
			//!
			//! @return    �Ȃ�
			//--------------------------------------------------------------
			void Finalize();


			//--------------------------------------------------------------
			//! @summary   �X�V����
			//!
			//! @parameter [void] �Ȃ�
			//!
			//! @return    �Ȃ�
			//--------------------------------------------------------------
			void Update();


			//--------------------------------------------------------------
			//! @summary   Acb�t�@�C���̓ǂݍ���
			//!
			//! @parameter [acb] Acb�t�@�C����
			//! @parameter [awb] Awb�t�@�C����
			//!
			//! @return    �Ȃ�
			//--------------------------------------------------------------
			void LoadAcb(const wchar_t *acb, const wchar_t *awb = nullptr);


			//--------------------------------------------------------------
			//! @summary   �w��L���[�̍Đ�
			//!
			//! @parameter [cueID] �Đ����������̃L���[ID
			//! @parameter [volume] ����
			//!
			//! @return    �Ȃ�
			//--------------------------------------------------------------
			void Play(CriAtomExCueId cueID, float volume = 1.0f);


			//--------------------------------------------------------------
			//! @summary   ���ʂ̐ݒ�
			//!
			//! @parameter [volume] ����
			//!
			//! @return    �Ȃ�
			//--------------------------------------------------------------
			void SetVolume(float volume);


			//--------------------------------------------------------------
			//! @summary   �T�E���h�v���C���[�̃|�[�Y
			//!
			//! @parameter [void] �Ȃ�
			//!
			//! @return    �Ȃ�
			//--------------------------------------------------------------
			void Pause();


			//--------------------------------------------------------------
			//! @summary   �T�E���h�v���C���[�̃|�[�Y��Ԃ̎擾
			//!
			//! @parameter [void] �Ȃ�
			//!
			//! @return    true :�|�[�Y��
			//! @return    false:�|�[�Y�ȊO�̏��
			//--------------------------------------------------------------
			bool IsPause();


			//--------------------------------------------------------------
			//! @summary   �T�E���h�v���C���[�̒�~
			//!
			//! @parameter [void] �Ȃ�
			//!
			//! @return    �Ȃ�
			//--------------------------------------------------------------
			void Stop();


			//--------------------------------------------------------------
			//! @summary   �w�肵�����̒�~
			//!
			//! @parameter [cueID] ��~���������̃L���[ID
			//!
			//! @return    �Ȃ�
			//--------------------------------------------------------------
			void Stop(CriAtomExCueId cueID);


			//--------------------------------------------------------------
			//! @summary   �w�肵�����̍Đ���Ԃ̎擾
			//!
			//! @parameter [cueID] �擾���������̃L���[ID
			//!
			//! @return    true :�Đ���
			//! @return    false:�Đ����Ă��Ȃ�
			//--------------------------------------------------------------
			bool IsState(CriAtomExCueId cueID);


			// <�ÓI�ȃ����o�֐�>
		private:
			//--------------------------------------------------------------
			//! @summary   �G���[�R�[���o�b�N�֐��̃��[�U����
			//!
			//! @parameter [errid]
			//! @parameter [p1]
			//! @parameter [p2]
			//! @parameter [parray]
			//!
			//! @return    �Ȃ�
			//--------------------------------------------------------------
			static void user_error_callback_func(const CriChar8 *errid, CriUint32 p1, CriUint32 p2, CriUint32 *parray);


			//--------------------------------------------------------------
			//! @summary   �������m�ۊ֐��̃��[�U����
			//!
			//! @parameter [obj]
			//! @parameter [size] �m�ۂ��郁�����̃T�C�Y
			//!
			//! @return    �m�ۂ��ꂽ�������̃|�C���^
			//--------------------------------------------------------------
			static void *user_alloc_func(void *obj, CriUint32 size);


			//--------------------------------------------------------------
			//! @summary   ����������֐��̃��[�U����
			//!
			//! @parameter [obj]
			//! @parameter [ptr] ������郁����
			//!
			//! @return    �Ȃ�
			//--------------------------------------------------------------
			static void user_free_func(void *obj, void *ptr);


			// <�Q�b�^�[�֐�>
		public:
			//--------------------------------------------------------------
			//! @summary   �T�E���h�v���C���[�̎擾
			//--------------------------------------------------------------
			inline SoundPlayer* GetPlayer() { return &m_player; }

		};
	}
}