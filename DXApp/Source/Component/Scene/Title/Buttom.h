#pragma once
#include<Framework.h>

namespace component {

	class	FadeComponent;

	class Buttom : public framework::Component
	{
	public:
		Buttom();
		~Buttom();

		void init()override;

		void onEvent();
	private:
		std::weak_ptr<FadeComponent> m_pFade;
	};

}
