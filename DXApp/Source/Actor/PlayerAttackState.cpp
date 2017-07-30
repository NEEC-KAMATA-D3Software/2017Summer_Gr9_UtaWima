#include"PlayerAttackState.h"
#include<Source\Entity\Entity.h>
#include<assert.h>
#include<Source\Actor\PlayerIdleState.h>
#include<Source\Resource\ResourceManager.h>
#include<Source\Actor\PlayerStepState.h>
#include<Source\Device\Input\Input.h>
#include<Source\Resource\ResourceManager.h>

PlayerAttackState::PlayerAttackState()
{
}

PlayerAttackState::~PlayerAttackState()
{
}

void PlayerAttackState::entryAction(component::PlayerComponent * outerValue)
{
	//攻撃モーションに変更
	auto&& currentState = outerValue->getPlayerData().lock()->currentState = PlayerState::attack;

	m_Animator = outerValue->getGameObj().lock()->getComponent<component::AnimatorComponent>();
	assert(!m_Animator.expired() && "プレイヤーにアニメーターがついていません");
	//移動モーションに切り替える

	m_Animator.lock()->changeAnime(outerValue->getPlayerData().lock()->motionNames[currentState]);
}

Trans PlayerAttackState::inputAction(component::PlayerComponent * outerValue, std::shared_ptr<State<component::PlayerComponent>>* nextState)
{
	if (m_Animator.lock()->isEnd()) {
		*nextState = std::make_shared<PlayerIdleState>();
		return Trans::Trans_Occured;
	}

	if (framework::Input::getGamePad().getButtonTrriger(framework::ButtonCode::A, outerValue->getPlayerData().lock()->controlID) && !isAttackFrame()) {
		*nextState = std::make_shared<PlayerStepState>();
		return Trans::Trans_Occured;
	}

	*nextState = std::make_shared<PlayerAttackState>(*this);
	return Trans::Trans_Again;
}

void PlayerAttackState::exitAction(component::PlayerComponent * outerValue)
{
	m_Animator.reset();
}

bool PlayerAttackState::isAttackFrame()
{
	//フレーム数が攻撃フレームに達している
	return m_Animator.lock()->getFrame() >= 26;
}

