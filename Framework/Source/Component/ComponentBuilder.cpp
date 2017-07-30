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
		assert(m_BuildList.find(name) == m_BuildList.end() && "������ނ̃R���|�[�l���g���o�^����Ă��܂�");
		m_BuildList[name] = componentRegister;
	}

	std::shared_ptr<Component> ComponentBuilder::create(const std::string & name)
	{
		auto itr = m_BuildList.find(name);
		assert(itr != m_BuildList.end() && "�R���|�[�l���g���o�^����Ă��܂���");
		return std::shared_ptr<Component>(itr->second->create());
	}



}