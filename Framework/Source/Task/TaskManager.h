#pragma once

/**
* @file	TaskManager.h
* @brief	タスク管理クラス
* @author	高須優輝
* @date	2017/3/4
*/

#include<list>
#include<memory>
#include<functional>
#include<Source\Task\Render3DContainer.h>
#include<Source\Task\UpdateContainer.h>
#include<Source\Task\Render2DContainer.h>
#include<Source\Task\PhysicsContainer.h>

namespace framework {

	class Component;
	class UpdateComponent;
	class Render3DComponent;
	class Render2DComponent;
	class PhysicsComponent;

	class TaskManager
	{
	public:
		~TaskManager();

		void addTask(std::weak_ptr<UpdateComponent> task);

		void addTask(std::weak_ptr<Render3DComponent> task);

		void addTask(std::weak_ptr<Render2DComponent> task);

		void addTask(std::weak_ptr<PhysicsComponent> task);

		void removeTask(std::weak_ptr<UpdateComponent> task);

		void removeTask(std::weak_ptr<Render3DComponent> task);

		void removeTask(std::weak_ptr<Render2DComponent> task);

		void removeTask(std::weak_ptr<PhysicsComponent> task);

		void init();

		void physicsUpdate();

		void update();

		void draw();

		void clear();

		static TaskManager* getInstance();

	private:
		//インスタンス生成禁止
		TaskManager();

	private:
		static std::shared_ptr<TaskManager> instance;

		UpdateContainer m_UpdateTaskContainer;
		Render3DContainer m_Render3DContainer;
		Render2DContainer m_Render2DContainer;
		PhysicsContainer m_PhysicsContainer;
	};

}