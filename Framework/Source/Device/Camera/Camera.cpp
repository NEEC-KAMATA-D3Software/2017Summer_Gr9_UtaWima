#include"Camera.h"


namespace framework {


	Camera::Camera()
		:Camera(util::Vec3(),util::Vec3())
	{
	}

	Camera::Camera(util::Vec3 eyePos, util::Vec3 lookPos)
		: m_EyePosition(eyePos),
		m_LookPosition(lookPos)
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::setEyePosition(util::Vec3 eyePos)
	{
		m_EyePosition = eyePos;
	}

	void Camera::setLookAt(util::Vec3 lookPos)
	{
		m_LookPosition = lookPos;
	}

	util::Mat4 Camera::toViewMatrix()
	{
		util::Vec4 upDirection = { 0.0f,1.0f,0.0f,1.0f };
		util::Mat4 viewMat = XMMatrixLookAtLH(
			m_EyePosition.toXMVector(),
			m_LookPosition.toXMVector(),
			upDirection.toXMVector());

		m_ViewMatrix = XMMatrixTranspose(viewMat.toXMMatrix());
		return m_ViewMatrix;
	}

	util::Mat4 Camera::getViewMatrix()
	{
		return m_ViewMatrix;
	}



}