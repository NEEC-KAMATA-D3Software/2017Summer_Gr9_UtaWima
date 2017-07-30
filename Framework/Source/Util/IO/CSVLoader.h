#pragma once
#include<Source\Util\IO\TextLoader.h>
/**
* @file	CSVLoader.h
* @brief	カンマ区切りでファイルを読み込む
* @authro	高須優輝
* @date	2017/03/18
*/
namespace util {
	
	class CSVLoader
	{
	public:
		CSVLoader(const std::string& filePath);
		~CSVLoader();

		/**
		* @brief		すべてのファイルを読み込む
		*/
		std::vector<std::vector<std::string>> load();

	private:
		TextLoader m_Loader;
	};

}
