#include"SharpMethod.h"
#include<assert.h>

namespace util {

	SharpMethod::SharpMethod(IDispatch* pDisp,const std::string& methodName,int methodNum)
		:m_Signature(methodNum)
	{
		//ユニコードに変換文字
		USES_CONVERSION;
		LPOLESTR funcName = A2OLE(methodName.c_str());

		//メソッドID取得
		HRESULT hr = pDisp->GetIDsOfNames(IID_NULL, &funcName, 1, LOCALE_SYSTEM_DEFAULT, &m_MethodID);
		assert(SUCCEEDED(hr) && "メソッドの取得に失敗しました");
	}

	SharpMethod::~SharpMethod()
	{
	}

	//void SharpMethod::callMethod(IDispatch* pDisp)
	//{
	//	VARIANT ret;
	//	VariantInit(&ret);
	//	pDisp->Invoke(m_MethodID, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, m_Signature.getParam(), &ret, NULL, NULL);
	//	int a = ret.lVal;
	//}

}