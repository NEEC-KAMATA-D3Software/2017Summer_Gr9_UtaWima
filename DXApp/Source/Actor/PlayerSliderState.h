#pragma once
#include<Source\Component\Player\PlayerComponent.h>
#include<Source\State\State.h>
#include"PlayerStates.h"
#include<Source\Util\Type.h>
#include<Source\Component\Animation\AnimatorComponent.h>

class PlayerSliderState : public State<component::PlayerComponent>
{
public:
	PlayerSliderState();
	~PlayerSliderState();

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
	std::weak_ptr<component::AnimatorComponent> m_Animator;
	const float m_Shift = 5.5f;
};