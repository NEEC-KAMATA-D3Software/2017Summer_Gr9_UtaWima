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
		* @brief		‰Šú‰»
		*/
		virtual void init();

		// UpdateComponent ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		virtual void update() override;

	private:
		std::weak_ptr<PointLightComponent> m_pPointLight;

	};



}