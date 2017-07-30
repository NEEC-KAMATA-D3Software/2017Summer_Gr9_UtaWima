#pragma once

#include<Source\Component\Player\PlayerComponent.h>
#include<Source\State\State.h>
#include"PlayerStates.h"
#include<Source\Util\Type.h>
#include<Source\Component\Animation\AnimatorComponent.h>

class PlayerStepState : public State<component::PlayerComponent>
{
public:
	PlayerStepState();
	~PlayerStepState();


	/**
	* @brief					�؂�ւ���ď��߂ɌĂ΂��R�[���o�b�N���\�b�h
	*/
	virtual void entryAction(component::PlayerComponent* outerValue);

	/**
	* @brief					�}�C�t���[���A�b�v�f�[�g�ŌĂ΂��R�[���o�b�N���\�b�h
	*/
	virtual Trans inputAction(component::PlayerComponent* outerValue, std::shared_ptr<State<component::PlayerComponent>>* nextState);

	/**
	* @brief					���̃X�e�[�g�ɐ؂�ւ�钼�O�ɌĂ΂��R�[���o�b�N���\�b�h
	*/
	virtual void exitAction(component::PlayerComponent* outerValue);


private:
	std::weak_ptr<component::AnimatorComponent> m_Animator;
	const float m_Shift = 10.5f;

};

