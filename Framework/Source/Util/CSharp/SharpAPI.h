#pragma once


namespace util {

	class SharpAPI
	{
	public:
		/**
		* @brief		API������
		*/
		static void initAPI();

		/**
		* @brief		API�I������
		*/
		static void endAPI();

	private:
		//�C���X�^���X�����֎~
		SharpAPI();
	};
}