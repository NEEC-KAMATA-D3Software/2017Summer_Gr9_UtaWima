#include"SharpClass.h"
#include<assert.h>

namespace util {

	SharpClass::SharpClass(const std::string& className)
	{
		//�����^�ϊ�
		BSTR bstrDLL = _com_util::ConvertStringToBSTR(className.c_str());

		//�N���XID�擾
		HRESULT hResult = ::CLSIDFromProgID(bstrDLL, &m_ClassID);
		assert(SUCCEEDED(hResult) && "�N���X�擾�Ɏ��s���܂���");
		::SysFreeString(bstrDLL);
	}

	SharpClass::~SharpClass()
	{
	}

	SharpInstance SharpClass::createInstance()
	{
		//�C���X�^���X�𐶐�
		IUnknown* pUnk = nullptr;
		HRESULT hResult = CoCreateInstance(m_ClassID, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pUnk);
		assert(SUCCEEDED(hResult) && "�C���X�^���X�쐬���s���܂���");
		CComPtr<IUnknown> pUnkSmart;
		pUnkSmart.Attach(pUnk);
		
		return SharpInstance(pUnkSmart);
	}


}