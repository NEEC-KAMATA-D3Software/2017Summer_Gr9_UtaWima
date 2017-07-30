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
	* @brief					切り替わって初めに呼ばれるコールバックメソッド
	*/
	virtual void entryAction( component::PlayerComponent* outerValue);

	/**
	* @brief					マイフレームアップデートで呼ばれるコールバックメソッド
	*/
	virtual Trans inputAction( component::PlayerComponent* outerValue, std::shared_ptr<State<component::PlayerComponent>>* nextState);
	/**
	* @brief					次のステートに切り替わる直前に呼ばれるコールバックメソッド
	*/
	virtual void exitAction( component::PlayerComponent* outerValue);

private:
	void myUpdate(component::PlayerComponent * outerValue);

	util::Vec3 velocity(util::Transform* trans,int controlID);

private:

};
