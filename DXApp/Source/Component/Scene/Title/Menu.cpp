#include"Menu.h"
#include<stdio.h>
#include<Source\Util\Type.h>
#include"Buttom.h"
#include<Source\Component\FadeComponent.h>
#include<Source\Application\Application.h>

namespace component {

	Menu::Menu()
	{
	}

	Menu::~Menu()
	{
	}

	void Menu::init()
	{
		auto entityList = framework::Entity::findGameObjWithTags("Menu");

		for (auto& entity : entityList) {
			m_MenuObj.emplace_back(entity);
		}
		m_Selector->init();

		m_CurrentUI = m_MenuObj[0];
		m_CurrentUI.lock()->getTransform()->scaling(2);

		m_pSceneChanger = framework::Entity::findGameObj("GlobalEvent").lock()->getComponent<SceneChangeComponent>();
		m_pFade = framework::Entity::findGameObj("Fade").lock()->getComponent<FadeComponent>();

		m_IsPreUp = false;
		m_IsPreDown = false;
		m_IsCurrentUp = false;
		m_IsCurrentDown = false;
	}


	void Menu::update()
	{
		//フェードが終了していれば遷移する
		if (m_pFade.lock()->isEnd() && m_pFade.lock()->isOutFade()) {
			m_pSceneChanger.lock()->changeSceneNotification();
		}

		menuUpdate();
	}

	void Menu::clear()
	{
		m_MenuObj.clear();
	}

	void Menu::setParam(const std::vector<std::string>& param)
	{
		m_Selector = std::make_unique<util::Selector>(std::atoi(param[0].c_str()));
	}

	void Menu::scaleChange()
	{
		m_PreUI = m_CurrentUI;
		m_CurrentUI = m_MenuObj[m_Selector->getSelectNum()];
		m_CurrentUI.lock()->getTransform()->scaling(2);

		if (!m_PreUI.expired()) {
			m_PreUI.lock()->getTransform()->scaling(1);
		}
	}

	void Menu::menuUpdate()
	{
		auto vec = framework::Input::getGamePad().getLeftStick();
		if (isUpTrriger(vec)) {
			m_Selector->enter();
			scaleChange();
			framework::ResourceManager::getInstance()->playSound("Move");
		}

		if (isDownTrriger(vec)) {
			m_Selector->back();
			scaleChange();
			framework::ResourceManager::getInstance()->playSound("Move");
		}

		if (framework::Input::getGamePad().getButton(framework::ButtonCode::A)) {
			auto Buttom = m_CurrentUI.lock()->getComponent<component::Buttom>();
			if (m_Selector->getSelectNum() == 0) {
				//コントローラの数を検出してシーンを変える
				if (framework::Input::getGamePad().getConectNum() <= 2) {
					framework::Scene::m_NextSceneName = "GameMain";
				}
				if (framework::Input::getGamePad().getConectNum() == 3) {
					framework::Scene::m_NextSceneName = "GameMain3";
				}
				if (framework::Input::getGamePad().getConectNum() == 4) {
					framework::Scene::m_NextSceneName = "GameMain4";
				}

				Buttom.lock()->onEvent();
			}
			if (m_Selector->getSelectNum() == 1)
				framework::Application::m_Exit = true;
			framework::ResourceManager::getInstance()->playSound("Enter");
		}
	}

	bool Menu::isUpTrriger(util::Vec2& vec)
	{
	 	m_IsPreUp = m_IsCurrentUp;
		m_IsCurrentUp = vec.y > framework::Input::getGamePad().m_MaxHalfStickNum;
		return !m_IsPreUp && m_IsCurrentUp;
	}

	bool Menu::isDownTrriger(util::Vec2& vec)
	{
		m_IsPreDown = m_IsCurrentDown;
		m_IsCurrentDown = vec.y < framework::Input::getGamePad().m_MinHalfStickNum;
		return  !m_IsPreDown && m_IsCurrentDown;
	}

}