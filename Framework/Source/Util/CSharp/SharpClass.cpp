#include"SharpClass.h"
#include<assert.h>

namespace util {

	SharpClass::SharpClass(const std::string& className)
	{
		//文字型変換
		BSTR bstrDLL = _com_util::ConvertStringToBSTR(className.c_str());

		//クラスID取得
		HRESULT hResult = ::CLSIDFromProgID(bstrDLL, &m_ClassID);
		assert(SUCCEEDED(hResult) && "クラス取得に失敗しました");
		::SysFreeString(bstrDLL);
	}

	SharpClass::~SharpClass()
	{
	}

	SharpInstance SharpClass::createInstance()
	{
		//インスタンスを生成
		IUnknown* pUnk = nullptr;
		HRESULT hResult = CoCreateInstance(m_ClassID, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pUnk);
		assert(SUCCEEDED(hResult) && "インスタンス作成失敗しました");
		CComPtr<IUnknown> pUnkSmart;
		pUnkSmart.Attach(pUnk);
		
		return SharpInstance(pUnkSmart);
	}


}