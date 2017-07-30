#include"PhysicsContainer.h"
#include<Source\Util\WrapFunc.h>


namespace framework {

	PhysicsContainer::PhysicsContainer()
	{
	}

	PhysicsContainer::~PhysicsContainer()
	{
	}

	void PhysicsContainer::init()
	{

	}

	void PhysicsContainer::physicsUpdate()
	{
		checkDelete();

		hitDecision();

		//コールバック呼び出し
		for (auto comp : m_Container) {
			comp.lock()->collisionUpdate();
		}
	}

	void PhysicsContainer::checkDelete()
	{
		TaskContainer::checkDelete();

		for (auto comp : m_Container) {
			comp.lock()->checkDelete();
		}
	}

	void PhysicsContainer::hitDecision(){

		hitObjLoop([&](std::weak_ptr<PhysicsComponent> a, std::weak_ptr<PhysicsComponent> b) {
			a.lock()->hitCheck(b);
		});
	}

	void PhysicsContainer::hitObjLoop(std::function<void(std::weak_ptr<PhysicsComponent>a, std::weak_ptr<PhysicsComponent>b)> action)
	{
		for (auto comp1 : m_Container) {
			for (auto comp2 : m_Container) {
				//同じものを比較していたら都バス
				if (comp1._Get() == comp2._Get()) continue;
				action(comp1, comp2);
			}
		}

	}

}