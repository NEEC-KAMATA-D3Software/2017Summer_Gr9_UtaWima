#include"UpdateComponent.h"
#include<Source\Task\TaskManager.h>
#include<Source\Entity\Entity.h>

namespace framework {

	UpdateComponent::UpdateComponent()
	{
	}

	UpdateComponent::~UpdateComponent()
	{
	}

	void UpdateComponent::init()
	{
	}

	void UpdateComponent::active()
	{
		auto wcom = m_Entity.lock()->getComponent<UpdateComponent>(this);

		TaskManager::getInstance()->addTask(wcom);
		m_isActive = true;
	}

	void UpdateComponent::deActive()
	{
		auto wcom = m_Entity.lock()->getComponent<UpdateComponent>(this);

		TaskManager::getInstance()->removeTask(wcom);
		m_isActive = false;
	}


}