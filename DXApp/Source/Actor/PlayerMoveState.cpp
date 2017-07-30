#include "PlayerMoveState.h"
#include<Source\Device\Input\Input.h>
#include<Source\Entity\Entity.h>
#include<Source\Component\CameraComponent.h>
#include<Source\Component\Player\PlayerComponent.h>
#include"PlayerIdleState.h"
#include<assert.h>
#include<Source\Component\Animation\AnimatorComponent.h>
#include<Source\Actor\PlayerAttackState.h>
#include<Source\Resource\ResourceManager.h>
#include<Source\Component\Physics\RigidBody.h>
#include<Source\Actor\PlayerSliderState.h>


PlayerMoveState::PlayerMoveState()
{
}

PlayerMoveState::~PlayerMoveState()
{
}

void PlayerMoveState::entryAction(component::PlayerComponent * outerValue)
{
	//移動ステータスに変更
	auto&& currentState = outerValue->getPlayerData().lock()->currentState = PlayerState::move;

	auto animator = outerValue->getGameObj().lock()->getComponent<component::AnimatorComponent>();
	assert(!animator.expired() && "プレイヤーにアニメーターがついていません");
	//移動モーションに切り替える
	animator.lock()->changeAnime(outerValue->getPlayerData().lock()->motionNames[currentState]);
}

Trans PlayerMoveState::inputAction(component::PlayerComponent * outerValue, std::shared_ptr<State<component::PlayerComponent>>* nextState)
{
	myUpdate(outerValue);

	auto vec = framework::Input::getGamePad().getLeftStick(outerValue->getPlayerData().lock()->controlID);

	if (!abs(vec.x)) {
		*nextState = std::make_shared<PlayerIdleState>();
		return Trans::Trans_Occured;
	}

	if (framework::Input::getGamePad().getButtonTrriger(framework::ButtonCode::A, outerValue->getPlayerData().lock()->controlID)) {
		*nextState = std::make_shared<PlayerSliderState>();
		return Trans::Trans_Occured;
	}

	//Bボタンが押されたらアタックへ遷移
	if (framework::Input::getGamePad().getButtonTrriger(framework::ButtonCode::B, outerValue->getPlayerData().lock()->controlID)) {
		*nextState = std::make_shared<PlayerAttackState>();
		return Trans::Trans_Occured;
	}

	*nextState = std::make_shared<PlayerMoveState>(*this);
	return Trans::Trans_Again;
}

void PlayerMoveState::exitAction(component::PlayerComponent * outerValue)
{

}


void PlayerMoveState::myUpdate(component::PlayerComponent * outerValue)
{
	using namespace framework;
	auto trans = outerValue->getGameObj().lock()->getTransform();
	auto vel = velocity(trans, outerValue->getPlayerData().lock()->controlID);

	trans->m_Position += vel;
	auto dir = util::Vec3(vel.x, 0, vel.z);
	trans->lookAt(dir);
}

util::Vec3 PlayerMoveState::velocity(util::Transform* trans, int controlID)
{
	//using namespace framework;
	//const float speed = 5.0f;

	//auto leftStick = Input::getGamePad().getLeftStick().normalize();
	//auto front = component::CameraComponent::getMainCamera().lock()->getGameObj().lock()->getTransform()->m_Position - trans->m_Position;

	//util::Mat4 rotete;
	//util::createMatrixFromFront(front, &rotete);

	////カメラの向いてる方向に回転させる

	//auto a = XMVector3Transform(XMLoadFloat3(&util::Vec3(leftStick.x, 0, leftStick.y)), rotete.toXMMatrix());

	//util::Vec3 b;
	//XMStoreFloat3(&b, a);

	//return util::Vec3(-b.x, 0, -b.z).normalize() *speed;



	using namespace framework;
	const float speed = 5.0f;

	auto leftStick = Input::getGamePad().getLeftStick(controlID).normalize() * speed;

	return util::Vec3(leftStick.x,0, 0);
}