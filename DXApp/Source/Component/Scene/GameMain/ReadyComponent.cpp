#include"ReadyComponent.h"
#include<Source\Component\SpriteRenderComponent.h>
#include<Source\Component\FadeComponent.h>
#include<Source\Util\Math\Math.h>
#include<Source\Application\Scene\SceneDelivery.h>

namespace component {
	ReadyComponent::ReadyComponent()
		:m_Timer(60)
	{
	}

	ReadyComponent::~ReadyComponent()
	{
	}

	void ReadyComponent::init()
	{
		//GameMainが始まったらランキング用データを消去
		SceneDelivery::getInstance()->clear();
		m_Timer.init();
		m_Counter = 0;
		m_IsFirst = true;

		m_pFade = framework::Entity::findGameObj("Fade").lock()->getComponent<FadeComponent>();

		std::list<framework::WeakEntity> entityList;
		entityList.emplace_back(framework::Entity::findGameObj("3"));
		entityList.emplace_back(framework::Entity::findGameObj("2"));
		entityList.emplace_back(framework::Entity::findGameObj("1"));
		entityList.emplace_back(framework::Entity::findGameObj("GO"));

		for (auto& e : entityList) {
			auto sprite = e.lock()->getComponent<SpriteRenderComponent>();
			sprite.lock()->deActive();
			m_Renderers.emplace_back(sprite);
		}

		m_pCurrentRenderer = m_Renderers[0];
	}

	void ReadyComponent::update()
	{
		if (!m_pFade.lock()->isEnd() && m_pFade.lock()->isInFade()) return;
		if (m_Timer.isEnd()) {
			m_Timer.init();
		}
		firstAction();
		scaleChange();
		alphaChange();
		count();
		finish();
	}

	bool ReadyComponent::isTimerEnd()
	{
		return m_Timer.isEnd();
	}

	bool ReadyComponent::isCountEnd()
	{
		return m_Counter == 4;
	}

	bool ReadyComponent::isGo()
	{
		return m_Counter >= 3;
	}

	void ReadyComponent::clear()
	{
		m_Renderers.clear();
	}

	void ReadyComponent::count()
	{
		if (isCountEnd())return;

		m_Timer.update();

		if (m_Timer.isEnd()) {
			m_Counter++;
			incrementAction();
		}
	}

	void ReadyComponent::finish()
	{
		if (m_Counter == 3) {
			if (m_Timer.isEnd()) {
				if (!framework::ResourceManager::getInstance()->isPlaying("Signal2")) {
					framework::ResourceManager::getInstance()->playSound("Signal2");
				}
			}
		}

		if (isCountEnd()) {
			m_pCurrentRenderer.lock()->deActive();
		}
	}

	void ReadyComponent::incrementAction()
	{
		if (isCountEnd())return;
		m_pCurrentRenderer.lock()->deActive();
		m_pCurrentRenderer = m_Renderers[m_Counter];
		m_pCurrentRenderer.lock()->active();

		if (m_Counter != 3)
			framework::ResourceManager::getInstance()->playSound("Signal1");
	}

	void ReadyComponent::firstAction()
	{
		if (!m_IsFirst)return;

		m_pCurrentRenderer.lock()->active();

		m_IsFirst = false;

		framework::ResourceManager::getInstance()->playSound("Signal1");
	}

	void ReadyComponent::scaleChange()
	{
		auto entity = m_pCurrentRenderer.lock()->getGameObj();
		auto trans = entity.lock()->getTransform();
		trans->scaling(util::lerp(m_Timer.rate(), 1.0f, 1.5f));
	}

	void ReadyComponent::alphaChange()
	{
		m_pCurrentRenderer.lock()->setAlpha(util::bezierCurve(m_Timer.rate(), 0.0f, 1.0f, 1.0f));
	}

	std::weak_ptr<SpriteRenderComponent> ReadyComponent::findRender(const std::string & name, std::list < std::weak_ptr<SpriteRenderComponent>> renders)
	{
		auto find = std::find_if(renders.begin(), renders.end(), [&](std::weak_ptr<SpriteRenderComponent>& render) {
			return render.lock()->getSpriteName() == name;
		});
		return *find;
	}

}