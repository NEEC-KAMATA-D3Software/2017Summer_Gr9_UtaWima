#pragma once
#include<Framework.h>


namespace component {

	class NextButton : public framework::UpdateComponent
	{
	public:
		NextButton() {
		}
		~NextButton() {
		}

		// UpdateComponent ����Čp������܂���
		virtual void update() override {
			if (framework::Input::getGamePad().getButtonTrriger(framework::ButtonCode::A)) {
				//TODO ���g���C
				framework::Entity::findGameObj("GlobalEvent").lock()->getComponent<SceneChangeComponent>().lock()->changeSceneNotification();
				framework::ResourceManager::getInstance()->playSound("Enter");
				if (framework::Input::getGamePad().getConectNum() <= 2) {
					framework::Scene::m_NextSceneName = "GameMain";
				}
				if (framework::Input::getGamePad().getConectNum() == 3) {
					framework::Scene::m_NextSceneName = "GameMain3";
				}
				if (framework::Input::getGamePad().getConectNum() == 4) {
					framework::Scene::m_NextSceneName = "GameMain4";
				}
			}
			if (framework::Input::getGamePad().getButtonTrriger(framework::ButtonCode::B)) {
				//TODO �^�C�g��
				framework::Scene::m_NextSceneName = "TitleScene";
				framework::Entity::findGameObj("GlobalEvent").lock()->getComponent<SceneChangeComponent>().lock()->changeSceneNotification();
				framework::ResourceManager::getInstance()->playSound("Enter");
			}
		}

	private:




	};

}