#include"ComponentStateManager.h"


namespace framework {



	ComponentStateManager::ComponentStateManager()
	{
	}

	ComponentStateManager::~ComponentStateManager()
	{
	}

	void ComponentStateManager::registComponent(std::weak_ptr<Component> component)
	{
		//コンポーネントの状態を保持
		m_ComponentMemory[component.lock().get()] = component.lock()->isActive();
	}

	ComponentState ComponentStateManager::popComponentState(std::weak_ptr<Component> component)
	{
		auto isFind = m_ComponentMemory.find(component.lock().get());
		//コンポーネントが登録されていなければ通知する
		if (isFind == m_ComponentMemory.end())return ComponentState::notFind;
		
		bool isActive = m_ComponentMemory[component.lock().get()];
		//登録を破棄
		m_ComponentMemory.erase(isFind);

		return isActive ? ComponentState::active : ComponentState::deActive;
	}

	void ComponentStateManager::clear()
	{
		m_ComponentMemory.clear();
	}


}