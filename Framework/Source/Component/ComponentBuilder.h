#pragma once
#include<unordered_map>
#include<string>
#include"Component.h"
#include<memory>

namespace framework {

	class IComponentCreator
	{
	public:
		virtual Component* create() = 0;
	};

	template<typename T>
	class ComponentCreator : public IComponentCreator
	{
	public:
		virtual T* create() override{
			return new T;
		};
	};

	class ComponentBuilder
	{
	public:
		ComponentBuilder();
		~ComponentBuilder();

		void registComponent(const std::string& name, IComponentCreator* componentCreator);

		std::shared_ptr<Component> create(const std::string& name);

		template<typename T>
		static IComponentCreator* componentRegister() {
			return new ComponentCreator<T>();
		}

	private:
		std::unordered_map<std::string, IComponentCreator* > m_BuildList;
	};
}