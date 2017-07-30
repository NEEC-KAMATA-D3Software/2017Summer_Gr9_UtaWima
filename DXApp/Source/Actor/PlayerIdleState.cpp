#include"PlayerIdleState.h"
#include<Source\Device\Input\Input.h>
#include"PlayerMoveState.h"
#include<assert.h>
#include<Source\Component\Animation\AnimatorComponent.h>
#include<Source\Component\Player\PlayerComponent.h>
#include<Source\Entity\Entity.h>
#include<Source\Actor\PlayerAttackState.h>
#include<Source\Actor\PlayerStepState.h>
#include<Source\Actor\PlayerSliderState.h>

PlayerIdleState::PlayerIdleState()
{
}

PlayerIdleState::~PlayerIdleState()
{
}

void PlayerIdleState::entryAction(component::PlayerComponent * outerValue)
{
	//�A�C�h���X�e�[�^�X�ɕύX
	auto&& currentState = outerValue->getPlayerData().lock()->currentState = PlayerState::idle;

	auto animator = outerValue->getGameObj().lock()->getComponent<component::AnimatorComponent>();
	assert(!animator.expired() && "�v���C���[�ɃA�j���[�^�[�����Ă��܂���");
	//�ړ����[�V�����ɐ؂�ւ���
	animator.lock()->changeAnime(outerValue->getPlayerData().lock()->motionNames[currentState]);
}

Trans PlayerIdleState::inputAction(component::PlayerComponent * outerValue, std::shared_ptr<State<component::PlayerComponent>>* nextState)
{
	auto vec = framework::Input::getGamePad().getLeftStick(outerValue->getPlayerData().lock()->controlID);

	if (framework::Input::getGamePad().getButtonTrriger(framework::ButtonCode::A, outerValue->getPlayerData().lock()->controlID) && abs(vec.x)) {
		*nextState = std::make_shared<PlayerSliderState>();
		return Trans::Trans_Occured;
	}

	//���X�e�B�b�N�̓��͂������moveState�֑J�ڂ���
	if (abs(vec.x)) {
		*nextState = std::make_shared<PlayerMoveState>();
		return Trans::Trans_Occured;
	}
	//B�{�^���������ꂽ��A�^�b�N�֑J��
	if (framework::Input::getGamePad().getButtonTrriger(framework::ButtonCode::B, outerValue->getPlayerData().lock()->controlID)) {
		*nextState = std::make_shared<PlayerAttackState>();
		return Trans::Trans_Occured;
	}

	if (framework::Input::getGamePad().getButtonTrriger(framework::ButtonCode::A, outerValue->getPlayerData().lock()->controlID)) {
		*nextState = std::make_shared<PlayerStepState>();
		return Trans::Trans_Occured;
	}

	*nextState = std::make_shared<PlayerIdleState>(*this);
	return Trans::Trans_Again;
}

void PlayerIdleState::exitAction(component::PlayerComponent * outerValue)
{
}
