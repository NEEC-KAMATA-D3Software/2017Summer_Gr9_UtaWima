#pragma once



namespace util {

	class Selector
	{
	public:
		/**
		* @brief			�R���X�g���N�^
		* @param selectNum  �Z���N�g��
		*/
		Selector(const int selectNum);
		~Selector();

		/**
		* @brief		������
		*/
		void init();
		
		/**
		* @brief		���ɐi�߂�
		*/
		void enter();

		/**
		* @brief		�O�ɖ߂�
		*/
		void back();

		/**
		* @brief		���݂̃Z���N�g���擾
		*/
		const int getSelectNum();

	private:
		//!�Z���N�g��
		const int m_SelectNum;
		//!�Z���N�g���o�b�N������Ƃ��Ɏg���I�t�Z�b�g
		const int m_DownOffst;
		//!���݂̃Z���N�g
		int m_CurrentSelect;
	};
}