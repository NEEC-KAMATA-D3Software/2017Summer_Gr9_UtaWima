#include"PlayerStepState.h"
#include<Source\Entity\Entity.h>
#include<assert.h>
#include<Source\Actor\PlayerIdleState.h>
#include<Source\Device\Input\Input.h>
#include<Source\Component\Physics\CircleCollider.h>
#include<Source\Resource\ResourceManager.h>

PlayerStepState::PlayerStepState()
{
}

PlayerStepState::~PlayerStepState()
{
}

void PlayerStepState::entryAction(component::PlayerComponent * outerValue)
{
	//移動ステータスに変更
	auto&& currentState = outerValue->getPlayerData().lock()->currentState = PlayerState::step;

	m_Animator = outerValue->getGameObj().lock()->getComponent<component::AnimatorComponent>();
	assert(!m_Animator.expired() && "プレイヤーにアニメーターがついていません");
	//移動モーションに切り替える
	//TODO モーションがないのでとりあえずダメージにしておく
	m_Animator.lock()->changeAnime(outerValue->getPlayerData().lock()->motionNames[currentState]);

	auto myEntity = outerValue->getGameObj();
	auto attackEntity = myEntity.lock()->findChildIf([&](framework::WeakEntity entity) {
		return entity.lock()->getName() == myEntity.lock()->getName() + "AttackEntity";
	});

	auto attackCollider = attackEntity.lock()->getComponent<component::CircleCollider>();
	attackCollider.lock()->deActive();

	framework::ResourceManager::getInstance()->playSound("Step");
}

Trans PlayerStepState::inputAction(component::PlayerComponent * outerValue, std::shared_ptr<State<component::PlayerComponent>>* nextState)
{
	if (m_Animator.lock()->isEnd()) {
		*nextState = std::make_shared<PlayerIdleState>();
		return Trans::Trans_Occured;
	}
	auto trans  = outerValue->getGameObj().lock()->getTransform();

	int sign = util::sign(trans->m_Rotation.y);

	trans->m_Position.x += sign * m_Shift * util::bezierCurve(m_Animator.lock()->rate(),1,0.8,0);

	*nextState = std::make_shared<PlayerStepState>(*this);
	return Trans::Trans_Again;
}

void PlayerStepState::exitAction(component::PlayerComponent * outerValue)
{
}
