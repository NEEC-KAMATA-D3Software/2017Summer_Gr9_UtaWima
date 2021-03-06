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
		* @brief		初期化
		*/
		virtual void init();

		// UpdateComponent を介して継承されました
		virtual void update() override;

	private:
		std::weak_ptr<PointLightComponent> m_pPointLight;

	};



}