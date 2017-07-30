#include"RulesComponent.h"
#include<Source\Component\SceneChangeComponent.h>
#include<Source\Application\Scene\SceneDelivery.h>
#include<Source\Component\Player\PlayerComponent.h>
#include<Source\Component\SpriteRenderComponent.h>
#include<Source\Util\Math\Math.h>

namespace component {


	RulesComponent::RulesComponent()
		:m_Timer(60 * 3),
		m_SpriteTimer(60 * 2)
	{
	}

	RulesComponent::~RulesComponent()
	{
	}

	void RulesComponent::init()
	{
		m_PlayerContainer = framework::Entity::findGameObjWithTags("Player");
		m_pSceneChanger = m_pSceneChanger = framework::Entity::findGameObj("GlobalEvent").lock()->getComponent<SceneChangeComponent>();

		m_pSpriteEntity = framework::Entity::findGameObj("Finish");
		m_pSprite = m_pSpriteEntity.lock()->getComponent<SpriteRenderComponent>();
		m_pSprite.lock()->setAlpha(0.0f);

		m_Timer.init();
		m_SpriteTimer.init();
	}

	void RulesComponent::update()
	{
		checkDelete();

		if (isPlayerOnes()) {
			m_SpriteTimer.update();
			m_pSprite.lock()->active();
			m_pSprite.lock()->setAlpha(util::bezierCurve(m_SpriteTimer.rate(), 0.0f, 1.5f, 2.0f, 0.0f));
			m_pSpriteEntity.lock()->getTransform()->scaling(util::bezierCurve(m_SpriteTimer.rate(), 2.5f, 1.5f, 1.5f, 0.0f));
		}

		if (isPlayerZero() || isPlayerOnes()) {
			m_Timer.update();
		}

		if (m_Timer.isEnd()) {
			if (m_PlayerContainer.front().expired()) {
				m_pSceneChanger.lock()->changeSceneNotification();
				return;
			}
			auto lastPlayer = m_PlayerContainer.front().lock()->getComponent<PlayerComponent>();
			SceneDelivery::getInstance()->addPlayer(lastPlayer.lock()->getPlayerData().lock()->controlID);
			m_pSceneChanger.lock()->changeSceneNotification();
		}



	}

	void RulesComponent::clear()
	{
		m_PlayerContainer.clear();
	}

	bool RulesComponent::isPlayerOnes()
	{
		return m_PlayerContainer.size() == 1;
	}

	bool RulesComponent::isPlayerZero()
	{
		return m_PlayerContainer.size() == 0;
	}

	void RulesComponent::checkDelete()
	{
		//アクティブでないもをリストから消去
		m_PlayerContainer.remove_if([](framework::WeakEntity entity) {
			return !entity.lock()->isActive();
		});
	}

}