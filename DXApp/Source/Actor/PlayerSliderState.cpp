#include"PlayerIdleState.h"
#include<Source\Entity\Entity.h>
#include<assert.h>
#include<Source\Actor\PlayerIdleState.h>
#include<Source\Resource\ResourceManager.h>
#include "PlayerSliderState.h"
#include<Source\Device\Input\Input.h>
#include<Source\Entity\Entity.h>
#include<Source\Actor\PlayerAttackState.h>
#include<Source\Resource\ResourceManager.h>

PlayerSliderState::PlayerSliderState()
{
}

PlayerSliderState::~PlayerSliderState()
{
}

void PlayerSliderState::entryAction(component::PlayerComponent * outerValue)
{
	//アイドルステータスに変更
	auto&& currentState = outerValue->getPlayerData().lock()->currentState = PlayerState::slide;

	m_Animator = outerValue->getGameObj().lock()->getComponent<component::AnimatorComponent>();
	assert(!m_Animator.expired() && "プレイヤーにアニメーターがついていません");
	//移動モーションに切り替える
	m_Animator.lock()->changeAnime(outerValue->getPlayerData().lock()->motionNames[currentState]);

	framework::ResourceManager::getInstance()->playSound("Slider");

}

Trans PlayerSliderState::inputAction(component::PlayerComponent * outerValue, std::shared_ptr<State<component::PlayerComponent>>* nextState)
{
	if (m_Animator.lock()->isEnd()) {
		*nextState = std::make_shared<PlayerIdleState>();
		return Trans::Trans_Occured;
	}

	////Bボタンが押されたらアタックへ遷移
	//if (framework::Input::getGamePad().getButtonTrriger(framework::ButtonCode::B, outerValue->getPlayerData().lock()->controlID)) {
	//	*nextState = std::make_shared<PlayerAttackState>();
	//	outerValue->activeCollision();
	//	return Trans::Trans_Occured;
	//}

	auto trans = outerValue->getGameObj().lock()->getTransform();

	int sign = -util::sign(trans->m_Rotation.y);

	trans->m_Position.x += sign * m_Shift * util::bezierCurve(m_Animator.lock()->rate(), 1, 0.8, 0);

	*nextState = std::make_shared<PlayerSliderState>(*this);
	return Trans::Trans_Again;
}

void PlayerSliderState::exitAction(component::PlayerComponent * outerValue)
{
}
