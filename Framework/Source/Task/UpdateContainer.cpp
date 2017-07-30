#include"UpdateContainer.h"
#include<Source\Component\SceneChangeComponent.h>


namespace framework {
	UpdateContainer::UpdateContainer()
	{
	}
	UpdateContainer::~UpdateContainer()
	{
	}
	void UpdateContainer::init()
	{
		//無効なウィーク削除
		TaskContainer::checkDelete();

		for (auto& task : m_Container) {
			if (task.expired())continue;
			task.lock()->setup();
		}

		m_pSceneChanger = framework::Entity::findGameObj("GlobalEvent").lock()->getComponent<component::SceneChangeComponent>();

	}
	void UpdateContainer::update()
	{
		checkDelete();

		callOrderSort();

		for (auto& task : m_Container) {
			if (task.expired())continue;
			task.lock()->update();
		}

		m_pSceneChanger.lock()->changeScene();
	}
}
