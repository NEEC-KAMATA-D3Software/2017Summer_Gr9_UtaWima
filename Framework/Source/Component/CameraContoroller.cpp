#include"CameraContoroller.h"
#include<Source\Entity\Entity.h>
#include<Source\Util\Math\Math.h>
#include<Source\Device\Input\Input.h>


namespace component {

	CameraContoroller::CameraContoroller()
		:m_Speed(3)
	{
	}

	CameraContoroller::~CameraContoroller()
	{
	}

	void CameraContoroller::init()
	{
		m_Transform = util::Transform();
		m_Xrotate = 0;
		m_Yrotate = 0;
	}

	void CameraContoroller::update()
	{
		if (framework::Input::getGamePad().getRightStick().x > framework::Input::getGamePad().m_MaxHalfStickNum) {
			m_Yrotate = m_Speed;
			m_Xrotate = 0;
			rotateUpdate();
		}
		if (framework::Input::getGamePad().getRightStick().x < framework::Input::getGamePad().m_MinHalfStickNum) {
			m_Yrotate = -m_Speed;
			m_Xrotate = 0;
			rotateUpdate();
		}
		if (framework::Input::getGamePad().getRightStick().y > framework::Input::getGamePad().m_MaxHalfStickNum) {
			m_Xrotate = -m_Speed;
			m_Yrotate = 0;

			rotateUpdate();
		}
		if (framework::Input::getGamePad().getRightStick().y < framework::Input::getGamePad().m_MinHalfStickNum) {
			m_Xrotate = m_Speed;
			m_Yrotate = 0;

			rotateUpdate();
		}


		//auto trans = m_Entity.lock()->getTransform();
		//trans->rotate(util::Vec3(0, 1, 0), trans->getAngle() + m_Yrotate);
		//trans->rotate(util::Vec3(1, 0, 0), trans->getAngle() + m_Xrotate);
	
	}

	void CameraContoroller::rotateUpdate()
	{
		auto trans = m_Entity.lock()->getTransform();
		trans->m_Rotation += util::Vec3( m_Xrotate, m_Yrotate,0);
		//trans->rotate(this->m_Transform.m_Rotation);
		//auto vec = util::Vec2(m_Xrotate, m_Yrotate).normalize();
		//this->m_Transform.rotate(util::Vec3(vec.x, vec.y,0));
		//auto trans = m_Entity.lock()->getTransform();
		//trans->rotate(this->m_Transform.m_Rotation);
		//trans->m_Rotation.z = 0;
	}

}