//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		GameManager.h
//!
//! @summary	�Q�[���Ǘ��N���X�̃w�b�_�t�@�C��
//!
//! @date		2019.06.02
//!
//! @author		�[�����l
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ���d�C���N���[�h�̖h�~ =========================================================
#pragma once



// �w�b�_�t�@�C���̓ǂݍ��� =======================================================
// <����w�b�_�t�@�C��>
#include "../../Library/System/SingletonBase.h"


// ���O��Ԃ̒�` ================================================================
namespace Motos
{
	namespace Utility
	{
		// �N���X�̒�` ==========================================================
		class GameManager : public Library::System::SingletonBase<GameManager>
		{
			// <�t�����h�̓o�^>
			friend Library::System::SingletonBase<GameManager>;


			// <�����o�ϐ�>
		private:
			// �v���C���[�̎c�@
			int m_playerLife;
			// ���݂̃X�e�[�W
			int m_stageNumber;


			// <�R���X�g���N�^>
		private:
			GameManager();


			// <�f�X�g���N�^>
		public:
			~GameManager();


			// <�����o�[�֐�>
		public:
			//----------------------------------------------------------
			//! @summary   �v���C���[�̎c�@������₷
			//!
			//! @parameter [void] �Ȃ�
			//!
			//! @return    �Ȃ�
			//----------------------------------------------------------
			void AddPlayerLife();


			//----------------------------------------------------------
			//! @summary   �v���C���[�̎c�@������炷
			//!
			//! @parameter [void] �Ȃ�
			//!
			//! @return    �Ȃ�
			//----------------------------------------------------------
			void SubPlayerLife();


			//----------------------------------------------------------
			//! @summary   �X�e�[�W��i�߂�
			//!
			//! @parameter [void] �Ȃ�
			//!
			//! @return    �Ȃ�
			//----------------------------------------------------------
			void UpStageNumber();


			// <�Z�b�^�[�֐�>
		public:
			inline void SetPlayerLife(int playerLife) { m_playerLife = playerLife; }


			// <�Q�b�^�[�֐�>
		public:
			inline int GetPlayerLife() const { return m_playerLife; }
			inline int GetStageNumber() const { return m_stageNumber; }
		};
	}
}