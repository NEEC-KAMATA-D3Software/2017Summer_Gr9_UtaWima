#include"WrapFunc.h"
#include<memory>

namespace util {






	std::wstring StringToWString(const std::string & src)
	{
		std::wstring dest;

		wchar_t* a;
		std::unique_ptr<wchar_t[]> pWcs = std::make_unique<wchar_t[]>(src.length() + 1);
		size_t ret;
		mbstowcs_s(&ret,pWcs.get(), src.length() + 1,src.c_str(), _TRUNCATE);
		dest = pWcs.get();
		return dest;
	};

}