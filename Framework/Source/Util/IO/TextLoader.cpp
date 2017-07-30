#include"TextLoader.h"
#include<assert.h>
#include<sstream>
#include<iostream>
#include<Source\Util\Win\WinFunc.h>

namespace util {

	TextLoader::TextLoader(const std::string& filePath)
		:m_Ifs(filePath)
	{
		if (!m_Ifs) {
			WinFunc::OutLog("“Ç‚İ‚İ¸”s : " + filePath);
			assert(false && "ƒtƒ@ƒCƒ‹“Ç‚İ‚İ¸”s");
		}
		
	}

	TextLoader::~TextLoader()
	{
	}

	std::vector<std::vector<std::string>> TextLoader::split(const char delimiter)
	{
		//!c
		std::vector<std::vector<std::string>> longitudinal;
		each([&](std::string readTokn) {
		
			std::string token;
			std::istringstream stream(readTokn);

			std::vector<std::string> side;
			while (std::getline(stream, token,delimiter)) {
				side.emplace_back(token);
			}
			longitudinal.emplace_back(side);
		});

		return longitudinal;
	}

	void TextLoader::each(std::function<void(std::string)> readFunc)
	{
		//ˆês‚¸‚Â“Ç‚İ‚Ş
		std::string str;
		while (std::getline(m_Ifs, str))
		{
			readFunc(str);
		}
	}

}