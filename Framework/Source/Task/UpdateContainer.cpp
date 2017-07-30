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
		//�����ȃE�B�[�N�폜
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
