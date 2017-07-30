#pragma once

#include<Source\State\State.h>
#include"PlayerStates.h"
#include<Source\Util\Type.h>
#include<Source\Component\Player\PlayerComponent.h>

namespace util {
	class Transform;
}


class PlayerMoveState : public State<component::PlayerComponent>
{
public:
	PlayerMoveState();
	~PlayerMoveState();

	/**
	* @brief					�؂�ւ���ď��߂ɌĂ΂��R�[���o�b�N���\�b�h
	*/
	virtual void entryAction( component::PlayerComponent* outerValue);

	/**
	* @brief					�}�C�t���[���A�b�v�f�[�g�ŌĂ΂��R�[���o�b�N���\�b�h
	*/
	virtual Trans inputAction( component::PlayerComponent* outerValue, std::shared_ptr<State<component::PlayerComponent>>* nextState);
	/**
	* @brief					���̃X�e�[�g�ɐ؂�ւ�钼�O�ɌĂ΂��R�[���o�b�N���\�b�h
	*/
	virtual void exitAction( component::PlayerComponent* outerValue);

private:
	void myUpdate(component::PlayerComponent * outerValue);

	util::Vec3 velocity(util::Transform* trans,int controlID);

private:

};
