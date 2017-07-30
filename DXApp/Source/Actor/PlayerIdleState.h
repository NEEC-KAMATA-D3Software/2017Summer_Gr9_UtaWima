#pragma once
#include<Source\State\State.h>
#include"PlayerStates.h"


namespace component {
	class PlayerComponent;
}

class PlayerIdleState : public State<component::PlayerComponent>
{
public:
	PlayerIdleState();
	~PlayerIdleState();

private:
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

};

