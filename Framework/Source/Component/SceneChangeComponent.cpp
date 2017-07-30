#include"SceneChangeComponent.h"

namespace component {

	SceneChangeComponent::SceneChangeComponent()
	{
	}

	SceneChangeComponent::~SceneChangeComponent()
	{
	}

	void SceneChangeComponent::init()
	{
		m_IsEnd = false;
	}

	void SceneChangeComponent::changeSceneNotification()
	{
		m_IsEnd = true;
	}

	void SceneChangeComponent::changeScene()
	{
		if (m_IsEnd)
			m_Entity.lock()->onEvent("ChangeScene");
	}
}