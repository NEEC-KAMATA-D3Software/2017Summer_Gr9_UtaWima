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
			WinFunc::OutLog("読み込み失敗 : " + filePath);
			assert(false && "ファイル読み込み失敗");
		}
		
	}

	TextLoader::~TextLoader()
	{
	}

	std::vector<std::vector<std::string>> TextLoader::split(const char delimiter)
	{
		//!縦
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
		//一行ずつ読み込む
		std::string str;
		while (std::getline(m_Ifs, str))
		{
			readFunc(str);
		}
	}

}