#include"Framework.h"
#include"SharpAPI.h"
#include<comutil.h>

namespace util {

	void SharpAPI::initAPI()
	{
		::CoInitialize(0);
	}

	void SharpAPI::endAPI()
	{
		::CoUninitialize();
	}
}
