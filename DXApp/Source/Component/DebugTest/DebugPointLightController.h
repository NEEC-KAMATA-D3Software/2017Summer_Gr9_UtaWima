#pragma once
#include<Source\Component\UpdateComponent.h>

namespace component {

	class PointLightComponent;

	class DebugPointLightController : public framework::UpdateComponent
	{
	public:
		DebugPointLightController();
		~DebugPointLightController();

		/**
		* @brief		������
		*/
		virtual void init();

		// UpdateComponent ����Čp������܂���
		virtual void update() override;

	private:
		std::weak_ptr<PointLightComponent> m_pPointLight;

	};



}