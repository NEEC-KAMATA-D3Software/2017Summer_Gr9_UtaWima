#include"Render3DComponent.h"
#include<Source\Task\TaskManager.h>
#include<Source\Entity\Entity.h>
#include<Source\Component\Component.h>
#include<Source\Util\Math\Transform.h>


namespace framework {

	Render3DComponent::Render3DComponent()
	{
	}

	Render3DComponent::~Render3DComponent()
	{
	}

	void Render3DComponent::init()
	{
	}

	void Render3DComponent::active()
	{
		auto wcom = m_Entity.lock()->getComponent<Render3DComponent>(this);

		framework::TaskManager::getInstance()->addTask(wcom);
		m_isActive = true;
	}

	void Render3DComponent::deActive()
	{
		auto wcom = m_Entity.lock()->getComponent<Render3DComponent>(this);

		framework::TaskManager::getInstance()->removeTask(wcom);
		m_isActive = false;
	}





}