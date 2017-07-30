#pragma once
#include"TaskContainer.h"
#include<Source\Component\Physics\PhysicsComponent.h>

namespace framework {


	class PhysicsContainer :public TaskContainer<PhysicsComponent>
	{
	public:
		PhysicsContainer();
		~PhysicsContainer();

		void init();

		void physicsUpdate();

	private:

		/**
		* @brief		無効タスク削除
		*/
		virtual void checkDelete()override;

		/**
		* @brief			すべてのフィジックスコンポーネント同士で判定を行う
		*/
		void hitDecision();

		void hitObjLoop(std::function<void(std::weak_ptr<PhysicsComponent> a, std::weak_ptr<PhysicsComponent> b)> action);

	};



}