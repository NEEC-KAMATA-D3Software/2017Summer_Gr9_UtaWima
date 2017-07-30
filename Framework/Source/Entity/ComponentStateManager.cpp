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
		//�R���|�[�l���g�̏�Ԃ�ێ�
		m_ComponentMemory[component.lock().get()] = component.lock()->isActive();
	}

	ComponentState ComponentStateManager::popComponentState(std::weak_ptr<Component> component)
	{
		auto isFind = m_ComponentMemory.find(component.lock().get());
		//�R���|�[�l���g���o�^����Ă��Ȃ���Βʒm����
		if (isFind == m_ComponentMemory.end())return ComponentState::notFind;
		
		bool isActive = m_ComponentMemory[component.lock().get()];
		//�o�^��j��
		m_ComponentMemory.erase(isFind);

		return isActive ? ComponentState::active : ComponentState::deActive;
	}

	void ComponentStateManager::clear()
	{
		m_ComponentMemory.clear();
	}


}