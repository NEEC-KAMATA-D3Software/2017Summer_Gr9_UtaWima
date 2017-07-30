#pragma once
#include<comutil.h>
#include<string>
#include<Source\Util\CSharp\SharpInstance.h>
#include<atlbase.h>

namespace util {

	class SharpClass
	{
	public:
		SharpClass(const std::string& className);
		~SharpClass();

		SharpInstance createInstance();

	private:
		CLSID m_ClassID;
	};


}