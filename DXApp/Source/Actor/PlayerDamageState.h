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
	* @brief					切り替わって初めに呼ばれるコールバックメソッド
	*/
	virtual void entryAction(component::PlayerComponent* outerValue);

	/**
	* @brief					マイフレームアップデートで呼ばれるコールバックメソッド
	*/
	virtual Trans inputAction(component::PlayerComponent* outerValue, std::shared_ptr<State<component::PlayerComponent>>* nextState);

	/**
	* @brief					次のステートに切り替わる直前に呼ばれるコールバックメソッド
	*/
	virtual void exitAction(component::PlayerComponent* outerValue);

private:
	bool isRelive();

	void reliveUpdate(component::PlayerComponent * outerValue);


private:
	std::weak_ptr<component::AnimatorComponent> m_Animator;
	//!衝突した相手
	std::weak_ptr<framework::Entity> m_ContactEntity;

	util::Vec3 m_Velocity;

	//!飛ばされる角度
	float m_Y;

	int m_ReliveCount;

	util::Timer m_Timer;
};

