#pragma once
#include<Source\State\State.h>
#include<Source\Component\Player\PlayerComponent.h>
#include<Source\Component\Animation\AnimatorComponent.h>


class PlayerDamageState : public State<component::PlayerComponent>
{
public:
	PlayerDamageState(const std::weak_ptr<framework::Entity>& other);
	~PlayerDamageState();

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
	bool isRelive();

	void reliveUpdate(component::PlayerComponent * outerValue);


private:
	std::weak_ptr<component::AnimatorComponent> m_Animator;
	//!�Փ˂�������
	std::weak_ptr<framework::Entity> m_ContactEntity;

	util::Vec3 m_Velocity;

	//!��΂����p�x
	float m_Y;

	int m_ReliveCount;

	util::Timer m_Timer;
};

