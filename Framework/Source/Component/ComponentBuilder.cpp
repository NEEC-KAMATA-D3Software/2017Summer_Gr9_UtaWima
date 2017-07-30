#include"ComponentBuilder.h"
#include<assert.h>


namespace framework {
	ComponentBuilder::ComponentBuilder()
	{
	}

	ComponentBuilder::~ComponentBuilder()
	{
	}

	void ComponentBuilder::registComponent(const std::string & name, IComponentCreator * componentRegister)
	{
		assert(m_BuildList.find(name) == m_BuildList.end() && "同じ種類のコンポーネントが登録されています");
		m_BuildList[name] = componentRegister;
	}

	std::shared_ptr<Component> ComponentBuilder::create(const std::string & name)
	{
		auto itr = m_BuildList.find(name);
		assert(itr != m_BuildList.end() && "コンポーネントが登録されていません");
		return std::shared_ptr<Component>(itr->second->create());
	}



}