#include"DebugPointLightController.h"
#include<Source\Entity\Entity.h>
#include<Source\Component\Effect\PointLightComponent.h>
#include<Source\Device\Input\Input.h>
#include<Source\Util\Math\Math.h>

namespace component {

	DebugPointLightController::DebugPointLightController()
	{
	}

	DebugPointLightController::~DebugPointLightController()
	{
	}

	void DebugPointLightController::init()
	{
		m_pPointLight = m_Entity.lock()->getComponent<PointLightComponent>();
	}

	void DebugPointLightController::update()
	{
		if (m_pPointLight.expired())return;

		if (framework::Input::getGamePad().getButton(framework::ButtonCode::Y) && framework::Input::getGamePad().getButton(framework::ButtonCode::RightShouler)) {
			auto attrib = m_pPointLight.lock()->getAttrib();
			attrib->power += 0.01;
			attrib->power = util::clamp<float>(attrib->power, 0.0f, 1.0);
		}

		if (framework::Input::getGamePad().getButton(framework::ButtonCode::B) && framework::Input::getGamePad().getButton(framework::ButtonCode::RightShouler)) {
			auto attrib = m_pPointLight.lock()->getAttrib();
			attrib->near_ += 0.01;
			attrib->near_ = util::clamp<float>(attrib->near_, 0.0f, 1.0);
		}

		if (framework::Input::getGamePad().getButton(framework::ButtonCode::A) && framework::Input::getGamePad().getButton(framework::ButtonCode::RightShouler)) {
			auto attrib = m_pPointLight.lock()->getAttrib();
			attrib->far_ += 0.01;
			attrib->far_ = util::clamp<float>(attrib->far_, 0.0f, 1.0);
		}

		if (framework::Input::getGamePad().getButton(framework::ButtonCode::RightShouler))return;

		if (framework::Input::getGamePad().getButton(framework::ButtonCode::Y)) {
			auto attrib = m_pPointLight.lock()->getAttrib();
			attrib->power -= 0.01;
			attrib->power = util::clamp<float>(attrib->power, 0.0f, 1.0);
		}

		if (framework::Input::getGamePad().getButton(framework::ButtonCode::B)) {
			auto attrib = m_pPointLight.lock()->getAttrib();
			attrib->near_ -= 0.01;
			attrib->near_ = util::clamp<float>(attrib->near_, 0.0f, 1.0);
		}
		
		if (framework::Input::getGamePad().getButton(framework::ButtonCode::A)) {
			auto attrib = m_pPointLight.lock()->getAttrib();
			attrib->far_ -= 0.01;
			attrib->far_ = util::clamp<float>(attrib->far_, 0.0f, 1.0);
		}



	}

}