#include "DebugContoroller.h"
#include<Source\Device\Input\Input.h>
#include<Source\Entity\Entity.h>

namespace component {

	DebugContoroller::DebugContoroller()
	{
	}

	DebugContoroller::~DebugContoroller()
	{
		
	}

	void DebugContoroller::init()
	{
		m_Animator = m_Entity.lock()->getComponent<AnimatorComponent>();
	}

	void DebugContoroller::update()
	{
		using namespace framework;
		const float speed = 20.0f;

		auto leftStick = Input::getGamePad().getLeftStick();

		if (leftStick.x >= Input::getGamePad().m_MaxHalfStickNum) {
			auto trans = m_Entity.lock()->getTransform();
			trans->m_Position.x += speed;
		}
		if (leftStick.x <= -Input::getGamePad().m_MaxHalfStickNum) {
			auto trans = m_Entity.lock()->getTransform();
			trans->m_Position.x -= speed;
		}
		if (leftStick.y >= Input::getGamePad().m_MaxHalfStickNum&&
			!Input::getGamePad().getButton(ButtonCode::RightShouler)) {
			auto trans = m_Entity.lock()->getTransform();
			trans->m_Position.y += speed;
		}
		if (leftStick.y <= -Input::getGamePad().m_MaxHalfStickNum&&
			!Input::getGamePad().getButton(ButtonCode::RightShouler)) {
			auto trans = m_Entity.lock()->getTransform();
			trans->m_Position.y -= speed;
		}

		if (leftStick.y >= Input::getGamePad().m_MaxHalfStickNum&&
			Input::getGamePad().getButton(ButtonCode::RightShouler)) {
			auto trans = m_Entity.lock()->getTransform();
			trans->m_Position.z -= speed;
		}

		if (leftStick.y <= -Input::getGamePad().m_MaxHalfStickNum&&
			Input::getGamePad().getButton(ButtonCode::RightShouler)) {
			auto trans = m_Entity.lock()->getTransform();
			trans->m_Position.z += speed;
		}

	/*	if (Input::getGamePad().getButton(ButtonCode::B)) {
			if (!m_Animator.expired()) {
				if (m_Animator.lock()->isEnd()) {
					m_Animator.lock()->changeMotion("Nail_Attack");
					m_Animator.lock()->onNotLoop();
				}
			}
		}
*/

	}
}
