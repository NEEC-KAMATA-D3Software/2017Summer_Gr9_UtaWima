#pragma once
#include<Source\Util\IO\TextLoader.h>
/**
* @file	CSVLoader.h
* @brief	�J���}��؂�Ńt�@�C����ǂݍ���
* @authro	���{�D�P
* @date	2017/03/18
*/
namespace util {
	
	class CSVLoader
	{
	public:
		CSVLoader(const std::string& filePath);
		~CSVLoader();

		/**
		* @brief		���ׂẴt�@�C����ǂݍ���
		*/
		std::vector<std::vector<std::string>> load();

	private:
		TextLoader m_Loader;
	};

}
