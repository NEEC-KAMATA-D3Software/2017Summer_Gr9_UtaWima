#pragma once
#include<Source\Component\UpdateComponent.h>
#include<Source\Entity\Entity.h>

namespace component {

	class Test : public framework::UpdateComponent
	{
	public:
		Test() {};
		~Test() {};

		void init()override {

		}

		virtual void update() override {


			float r = 10.0f;

			auto trans = m_Entity.lock()->getTransform();

			trans->m_Rotation += util::Vec3(0, -r, 0);
			//trans->rotate(util::Vec3(1,0,0),90);
			//trans->rotate(util::Vec3(0, -r, 0));
		};

	};
}