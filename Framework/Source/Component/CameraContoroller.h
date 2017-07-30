#pragma once
#include"UpdateComponent.h"
#include<Source\Util\Math\Transform.h>

namespace component {

	class CameraContoroller : public framework::UpdateComponent
	{
	public:
		CameraContoroller();
		~CameraContoroller();

		virtual void init()override;

		virtual void update() override;

	private:
		void rotateUpdate();

	private:
		const float m_Speed;
		util::Transform m_Transform;

		float m_Xrotate;
		float m_Yrotate;

	};


}

