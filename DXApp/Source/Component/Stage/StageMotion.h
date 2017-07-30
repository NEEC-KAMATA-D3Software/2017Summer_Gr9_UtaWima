#pragma once
#include<Framework.h>
#include<Source\Component\Animation\AnimatorComponent.h>

namespace component {
	class StageMotion : public framework::Component
	{
	public:
		StageMotion() {}
		~StageMotion() {}

		void init() override {
			auto animator = m_Entity.lock()->getComponent<component::AnimatorComponent>();

			animator.lock()->changeAnime("Stage1");
		}

	private:

	};
}