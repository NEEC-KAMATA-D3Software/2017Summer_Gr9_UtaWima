#pragma once
#include<vector>
#include<string>
#include<functional>
#include<fstream>

namespace util {

	class TextLoader
	{
	public:
		TextLoader(const std::string& filePath);
		~TextLoader();

		/**
		* @brief			区切り読み込み
		* @param delimiter	区切り文字
		* @return			読み込んだデータ	
		*/
		std::vector<std::vector<std::string>> split(const char delimiter);

	private:
		void each(std::function<void (std::string)> readFunc);

	private:
		std::ifstream m_Ifs;
	};

}