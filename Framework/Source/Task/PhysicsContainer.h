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
		* @brief		�����^�X�N�폜
		*/
		virtual void checkDelete()override;

		/**
		* @brief			���ׂẴt�B�W�b�N�X�R���|�[�l���g���m�Ŕ�����s��
		*/
		void hitDecision();

		void hitObjLoop(std::function<void(std::weak_ptr<PhysicsComponent> a, std::weak_ptr<PhysicsComponent> b)> action);

	};



}