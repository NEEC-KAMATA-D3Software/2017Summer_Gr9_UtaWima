#include"CSVLoader.h"


namespace util {

	CSVLoader::CSVLoader(const std::string & filePath)
		:m_Loader(filePath)
	{
	}

	CSVLoader::~CSVLoader()
	{
	}

	std::vector<std::vector<std::string>> CSVLoader::load()
	{
		return m_Loader.split(',');
	}

}