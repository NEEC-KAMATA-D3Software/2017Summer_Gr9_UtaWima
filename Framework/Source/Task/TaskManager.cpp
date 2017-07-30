#include"TaskManager.h"
#include<Source\Component\Render2DComponent.h>
#include<Source\Component\Render3DComponent.h>
#include<Source\Component\UpdateComponent.h>


namespace framework {

	std::shared_ptr<TaskManager> TaskManager::instance = std::shared_ptr<TaskManager>(new TaskManager());

	TaskManager::TaskManager()
	{
	}

	TaskManager::~TaskManager()
	{
	}

	void TaskManager::addTask(std::weak_ptr<UpdateComponent> task)
	{
		m_UpdateTaskContainer.addTask(task);
	}

	void TaskManager::addTask(std::weak_ptr<Render3DComponent> task)
	{
		m_Render3DContainer.addTask(task);
	}

	void TaskManager::addTask(std::weak_ptr<Render2DComponent> task)
	{
		m_Render2DContainer.addTask(task);
	}

	void TaskManager::addTask(std::weak_ptr<PhysicsComponent> task)
	{
		m_PhysicsContainer.addTask(task);
	}

	void TaskManager::removeTask(std::weak_ptr<UpdateComponent> task)
	{
		m_UpdateTaskContainer.removeTask(task);
	}

	void TaskManager::removeTask(std::weak_ptr<Render3DComponent> task)
	{
		m_Render3DContainer.removeTask(task);
	}

	void TaskManager::removeTask(std::weak_ptr<Render2DComponent> task)
	{
		m_Render2DContainer.removeTask(task);
	}

	void TaskManager::removeTask(std::weak_ptr<PhysicsComponent> task)
	{
		m_PhysicsContainer.removeTask(task);
	}

	void TaskManager::init()
	{
		m_UpdateTaskContainer.init();
		m_Render3DContainer.init();
		m_Render2DContainer.init();
		m_PhysicsContainer.init();
	}

	void TaskManager::physicsUpdate()
	{
		m_PhysicsContainer.physicsUpdate();
	}

	void TaskManager::update()
	{
		m_UpdateTaskContainer.update();
	}

	void TaskManager::draw()
	{
		m_Render3DContainer.draw();
		m_Render2DContainer.draw();
	}

	void TaskManager::clear()
	{
		m_UpdateTaskContainer.clear();
		m_Render3DContainer.clear();
		m_Render2DContainer.clear();
		m_PhysicsContainer.clear();
	}

	TaskManager * TaskManager::getInstance()
	{
		return instance.get();
	}



}