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
		* @brief			‹æØ‚è“Ç‚İ‚İ
		* @param delimiter	‹æØ‚è•¶š
		* @return			“Ç‚İ‚ñ‚¾ƒf[ƒ^	
		*/
		std::vector<std::vector<std::string>> split(const char delimiter);

	private:
		void each(std::function<void (std::string)> readFunc);

	private:
		std::ifstream m_Ifs;
	};

}