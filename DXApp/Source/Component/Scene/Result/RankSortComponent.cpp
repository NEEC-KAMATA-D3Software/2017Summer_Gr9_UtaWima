#include"RankSortComponent.h"
#include<Source\Application\Scene\SceneDelivery.h>
#include<Source\Component\SpriteRenderComponent.h>
#include<Source\Component\FadeComponent.h>

namespace component {

	RankSortComponent::RankSortComponent()
		:m_LastPos(540),
		m_FirstPos(-512),
		m_MediumPos(m_LastPos + 150),
		m_Timer(60)
	{
	}

	RankSortComponent::~RankSortComponent()
	{
	}

	void RankSortComponent::init()
	{
		m_pFade = framework::Entity::findGameObj("Fade").lock()->getComponent<FadeComponent>();
		m_IsFirst = true;
	}

	void RankSortComponent::componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer)
	{
		connectComponent(componentInitalizer);
	}

	void RankSortComponent::connectComponent(std::vector<std::weak_ptr<Component>>* componentInitalizer)
	{
		//!ランキングの数
		const int RankingNum = 4;

		m_RankList.reserve(RankingNum);
		//表示位置オブジェクト取得
		m_RankList.emplace_back(framework::Entity::findGameObj("1P"));
		m_RankList.emplace_back(framework::Entity::findGameObj("2P"));
		m_RankList.emplace_back(framework::Entity::findGameObj("3P"));
		m_RankList.emplace_back(framework::Entity::findGameObj("4P"));

		auto rank = SceneDelivery::getInstance()->getData();

		RankConverter converter;
		util::foreach(rank.size(), [&](int i) {

			std::vector<std::string> param = { converter.get(rank[i]) ,"3" };

			auto sprite = m_RankList[i].lock()->addComponent<SpriteRenderComponent>();
			sprite.lock()->setParam(param);

			componentInitalizer->emplace_back(sprite);
		});

		m_Timer.init();
	}

	void RankSortComponent::firstAction()
	{
		if (!m_IsFirst)return;
		m_IsFirst = false;

		framework::ResourceManager::getInstance()->playSound("C");
		framework::ResourceManager::getInstance()->playSound("C2");
	}

	void RankSortComponent::update()
	{
		if (!m_pFade.lock()->isEnd() && m_pFade.lock()->isInFade()) return;
		if (m_Timer.isEnd())return;
		firstAction();

		m_Timer.update();

		m_RankList[0].lock()->getTransform()->m_Position.x = util::bezierCurve(m_Timer.rate(), m_LastPos, m_MediumPos, m_LastPos, m_FirstPos);
		m_RankList[1].lock()->getTransform()->m_Position.x = util::bezierCurve(m_Timer.rate(), m_LastPos, m_MediumPos, m_LastPos - 200, m_FirstPos);
		m_RankList[2].lock()->getTransform()->m_Position.x = util::bezierCurve(m_Timer.rate(), m_LastPos, m_MediumPos, m_LastPos - 300, m_FirstPos);
		m_RankList[3].lock()->getTransform()->m_Position.x = util::bezierCurve(m_Timer.rate(), m_LastPos, m_MediumPos, m_LastPos - 400, m_FirstPos);
	}


}

