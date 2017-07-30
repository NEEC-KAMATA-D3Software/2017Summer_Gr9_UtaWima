#pragma once
#include"Component.h"

namespace framework {

	class Render2DComponent : public framework::Component
	{
	public:
		Render2DComponent();
		~Render2DComponent();

		virtual void init();

		virtual void setup() {}

		virtual void draw() = 0;

		virtual void active() override;

		virtual void deActive() override;

	private:

	};



}
