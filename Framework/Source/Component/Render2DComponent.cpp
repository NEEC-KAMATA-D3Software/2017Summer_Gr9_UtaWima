#include"Render2DComponent.h"
#include<Source\Task\TaskManager.h>
#include<Source\Entity\Entity.h>


namespace framework {


	Render2DComponent::Render2DComponent()
	{
	}

	Render2DComponent::~Render2DComponent()
	{
	}
	void Render2DComponent::init()
	{
	}
	void Render2DComponent::active()
	{
		auto wcom = m_Entity.lock()->getComponent<Render2DComponent>(this);
		framework::TaskManager::getInstance()->addTask(wcom);
		m_isActive = true;
	}
	void Render2DComponent::deActive()
	{
		auto wcom = m_Entity.lock()->getComponent<Render2DComponent>(this);
		framework::TaskManager::getInstance()->removeTask(wcom);
		m_isActive = false;
	}
}