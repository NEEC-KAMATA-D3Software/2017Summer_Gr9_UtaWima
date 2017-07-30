#pragma once
#include<comutil.h>
#include<memory>
#include<vector>

namespace util {

	class SharpSignature
	{
	public:
		SharpSignature(int paramNum);
		~SharpSignature();

		SharpSignature(SharpSignature& otehr);

		template<typename T>
		void setParam(std::vector<T> param) {
			assert(param.size() == m_Param.cArgs && "パラメータのサイズが一致しません");

			util::foreach(param.size(), [&](int i) {
				m_ParamData.get()[i].vt = VT_I4;
				m_ParamData.get()[i].lVal = param[i];
			});
		}

		void setParam(bool param, int location);

		void setParam(int param, int location);

		void setParam(short param, int location);

		void setParam(float param, int location);

		void setParam(const double& param, int location);

		void setParam(const std::string& param, int location);

		template<typename T>
		void setParam(T *structParam, int location) {
			paramCheck(location);
			m_ParamData.get()[location].vt = VT_INT;
			m_ParamData.get()[location].intVal = (int)structParam;
		}

		DISPPARAMS* getParam();

	private:
		void paramCheck(int location);
	private:
		DISPPARAMS m_Param;
		std::shared_ptr<VARIANTARG> m_ParamData = nullptr;
	};

}
