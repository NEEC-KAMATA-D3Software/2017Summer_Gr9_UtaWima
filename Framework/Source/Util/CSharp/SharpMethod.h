#pragma once
#include<comutil.h>
#include<string>
#include<atlbase.h>
#include<Source\Util\CSharp\SharpSignature.h>
#include<Source\Util\Template\Template.h>

namespace util {

	template<typename T>
	struct CallStruct;

	template<>
	struct CallStruct<bool> {
		static bool callMethod(IDispatch* pDisp, DISPID methodID, SharpSignature signature) {
			VARIANT ret;
			VariantInit(&ret);
			pDisp->Invoke(methodID, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, signature.getParam(), &ret, NULL, NULL);
			return ret.boolVal;
		}
	};
	
	template<>
	struct CallStruct<int> {
		static int callMethod(IDispatch* pDisp, DISPID methodID, SharpSignature signature) {
			VARIANT ret;
			VariantInit(&ret);
			pDisp->Invoke(methodID, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, signature.getParam(), &ret, NULL, NULL);
			return ret.lVal;
		}
	};

	template<>
	struct CallStruct<short> {
		static short callMethod(IDispatch* pDisp, DISPID methodID, SharpSignature signature) {
			VARIANT ret;
			VariantInit(&ret);
			pDisp->Invoke(methodID, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, signature.getParam(), &ret, NULL, NULL);
			return ret.iVal;
		}
	};

	template<>
	struct CallStruct<float> {
		static float callMethod(IDispatch* pDisp, DISPID methodID, SharpSignature signature) {
			VARIANT ret;
			VariantInit(&ret);
			pDisp->Invoke(methodID, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, signature.getParam(), &ret, NULL, NULL);
			return ret.fltVal;
		}
	};

	template<>
	struct CallStruct<double> {
		static double callMethod(IDispatch* pDisp, DISPID methodID, SharpSignature signature) {
			VARIANT ret;
			VariantInit(&ret);
			pDisp->Invoke(methodID, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, signature.getParam(), &ret, NULL, NULL);
			return ret.dblVal;
		}
	};

	template<>
	struct CallStruct<std::string> {
		static std::string callMethod(IDispatch* pDisp, DISPID methodID, SharpSignature signature) {
			VARIANT ret;
			VariantInit(&ret);
			pDisp->Invoke(methodID, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, signature.getParam(), &ret, NULL, NULL);
			return _com_util::ConvertBSTRToString(ret.bstrVal);
		}
	};

	class SharpMethod
	{
	public:
		SharpMethod(IDispatch* pDisp,const std::string& methodName,int methodNum);
		~SharpMethod();

		/**
		* @brief		すべて同じ型のパラメータセット
		*/
		template<typename T>
		void setParam(std::vector<T> param) {
			m_Signature.setParam(param);
		}

		void setParam(bool param,int location) {
			m_Signature.setParam(param,location);
		}

		void setParam(int param, int location) {
			m_Signature.setParam(param, location);
		}

		void setParam(short param, int location) {
			m_Signature.setParam(param, location);
		}

		void setParam(float param, int location) {
			m_Signature.setParam(param, location);
		}

		void setParam(double param, int location) {
			m_Signature.setParam(param, location);
		}

		void setParam(const std::string& param, int location) {
			m_Signature.setParam(param, location);
		}

		template<typename T>
		void setParam(T* structParam, int location) {
			m_Signature.setParam<T>(structParam, location);
		}

		template<typename T>
		T callMethod(IDispatch* pDisp) {
			return CallStruct<T>::callMethod(pDisp,m_MethodID,m_Signature);
		}

	private:
		DISPID m_MethodID;
		SharpSignature m_Signature;
	};

}
