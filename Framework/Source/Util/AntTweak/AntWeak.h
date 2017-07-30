#pragma once
#include<AntTweakBar.h>
#include<string>
#include<Source\Util\Type.h>
#include<typeinfo>
#include<stdint.h>
#include<vector>

namespace util {

	class AntWeak
	{
	public:

		/**
		* @brier	コンストラクタ
		*/
		AntWeak(const std::string& title);

		/**
		* @brier	コンストラクタ
		*/
		AntWeak(const std::string& title,util::Vec2& size);
		~AntWeak();
		
		/**
		* @brief		apiの初期化
		*/
		static void apiInit(TwGraphAPI module);

		/**
		* @brief		apiの狩猟処理
		*/
		static void apiEnd();

		/**
		* @brief		ラベルをセット
		*/
		void setLabel(const std::string& label);
	
		template<typename T>
		void addVarRw(const std::string& label, std::vector<T> value, TwType type) {
			TwAddVarRW(m_BarInstance,label.c_str(),type,&value[0],"Test");
		}

		template<typename T>
		void setParam(const std::string& key, std::vector<T> param, ETwParamValueType type) {
			TwSetParam(m_BarInstance, NULL, key.c_str(), type, param.size(), &param[0]);
		}
		/**
		* @brief		UIの描画
		*/
		void draw();

	private:
		std::string m_Title;
		TwBar * m_BarInstance;

	};



}