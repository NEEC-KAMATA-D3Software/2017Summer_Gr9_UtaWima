#include"SharpInstance.h"
#include<assert.h>

namespace util {

	SharpInstance::SharpInstance(CComPtr<IUnknown> instance)
		:m_Instance(instance)
	{
		IDispatch* pDisp = nullptr;
		HRESULT hResult = instance->QueryInterface(IID_IDispatch, (void**)&pDisp);
		assert(SUCCEEDED(hResult) && "インターフェース取得失敗しました");
		m_Interface.Attach(pDisp);
	}

	SharpInstance::~SharpInstance()
	{
	}

	void SharpInstance::addMethod(const std::string & methodName, int paramNum)
	{
		assert(m_Methods.find(methodName) == m_Methods.end() && "すでに登録されたいます");
		m_Methods[methodName] = std::make_shared<SharpMethod>(m_Interface.p, methodName, paramNum);
	}


}