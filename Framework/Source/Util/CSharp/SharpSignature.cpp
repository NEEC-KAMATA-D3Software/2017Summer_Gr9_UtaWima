#include"SharpSignature.h"
#include<Source\Util\WrapFunc.h>

namespace util {

	SharpSignature::SharpSignature(int paramNum)
	{
		::ZeroMemory(&m_Param,sizeof(m_Param));
		m_Param.cArgs = paramNum;
		m_Param.cNamedArgs = 0;
		m_Param.rgdispidNamedArgs = NULL;
		if (paramNum == 0)return;
		m_ParamData = std::shared_ptr<VARIANTARG>(new VARIANTARG[paramNum],std::default_delete<VARIANTARG>());
		::ZeroMemory(m_ParamData.get(), sizeof(VARIANTARG) * paramNum);
		m_Param.rgvarg = m_ParamData.get();
	}

	SharpSignature::~SharpSignature()
	{
	}

	SharpSignature::SharpSignature(SharpSignature & otehr)
	{
		this->m_Param = otehr.m_Param;
		this->m_ParamData = otehr.m_ParamData;
	}

	void SharpSignature::setParam(bool param, int location)
	{
		paramCheck(location);
		m_ParamData.get()[location].vt = VT_BOOL;
		m_ParamData.get()[location].boolVal = param;
	}

	void SharpSignature::setParam(int param, int location)
	{
		paramCheck(location);
		m_ParamData.get()[location].vt = VT_I4;
		m_ParamData.get()[location].lVal = param;
	}

	void SharpSignature::setParam(short param, int location)
	{
		paramCheck(location);
		m_ParamData.get()[location].vt = VT_I2;
		m_ParamData.get()[location].iVal = param;
	}

	void SharpSignature::setParam(float param, int location)
	{
		paramCheck(location);
		m_ParamData.get()[location].vt = VT_R4;
		m_ParamData.get()[location].fltVal = param;
	}

	void SharpSignature::setParam(const double & param, int location)
	{
		paramCheck(location);
		m_ParamData.get()[location].vt = VT_R8;
		m_ParamData.get()[location].dblVal = param;
	}

	void SharpSignature::setParam(const std::string & param, int location)
	{
		paramCheck(location);
		m_ParamData.get()[location].vt = VT_BSTR;
		m_ParamData.get()[location].bstrVal = _com_util::ConvertStringToBSTR(param.c_str());
	}

	DISPPARAMS* SharpSignature::getParam()
	{
		return &m_Param;
	}
	void SharpSignature::paramCheck(int location)
	{
		assert(m_Param.cArgs >= location + 1 && "ƒpƒ‰ƒ[ƒ^‚Ìˆø”‚Ì”‚É’£‚Á‚Ä‚¢‚Ü‚¹‚ñ");
	}
}