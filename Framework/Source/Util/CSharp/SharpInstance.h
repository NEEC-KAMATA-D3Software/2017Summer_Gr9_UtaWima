#pragma once
#include<comutil.h>
#include<string>
#include<atlbase.h>
#include<memory>
#include<unordered_map>
#include<Source\Util\CSharp\SharpMethod.h>

namespace util {

	class SharpInstance
	{
	public:
		SharpInstance(CComPtr<IUnknown> instance);
		~SharpInstance();

		void addMethod(const std::string& methodName,int paramNum);

		template<typename T>
		void setParam(const std::string& methodName,std::vector<T> param) {
			m_Methods[methodName]->setParam(param);
		}

		void setParam(const std::string& methodName,bool param, int location) {
			m_Methods[methodName]->setParam(param, location);
		}

		void setParam(const std::string& methodName, int param, int location) {
			m_Methods[methodName]->setParam(param, location);
		}

		void setParam(const std::string& methodName, short param, int location) {
			m_Methods[methodName]->setParam(param, location);
		}

		void setParam(const std::string& methodName, float param, int location) {
			m_Methods[methodName]->setParam(param, location);
		}

		void setParam(const std::string& methodName, double param, int location) {
			m_Methods[methodName]->setParam(param, location);
		}

		void setParam(const std::string& methodName, const std::string& param, int location) {
			m_Methods[methodName]->setParam(param, location);
		}

		template<typename T>
		void setParam(const std::string& methodName, T* structParam, int location) {
			m_Methods[methodName]->setParam<T>(structParam, location);
		}

		template<typename T>
		T callMethod(const std::string& methodName) {
			assert(m_Methods.find(methodName) != m_Methods.end() && "“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
			return m_Methods[methodName]->callMethod<T>(m_Interface);
		}

	private:
		CComPtr<IUnknown> m_Instance;
		CComPtr<IDispatch> m_Interface;
		using MethodContainer =	std::unordered_map<std::string, std::shared_ptr<SharpMethod>>;
		MethodContainer m_Methods;
	};

}