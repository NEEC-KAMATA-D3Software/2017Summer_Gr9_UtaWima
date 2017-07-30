#pragma once
#include<list>
#include<memory>
#include<vector>
#include<Source\Entity\Entity.h>
#include<algorithm>

/**
* @file	TaskContainer.h
* @brief	�^�X�N���i�[����R���e�i
* @author	���{�D�P
* @date	2017/3/4
*/


namespace framework {

	template<typename T>
	class TaskContainer
	{
	public:
		TaskContainer() {};
		virtual ~TaskContainer() {};

		virtual void addTask(std::weak_ptr<T> task) {
			//moveTask�����s���Ă��炷��active���ĂԂ�delete���X�g�Ɏc���Ă��܂����߂����Ńf���[�g���X�g����폜����
			m_DeleteContainer.remove_if([&](std::weak_ptr<T> deleteTask) {
				return deleteTask._Get() == task._Get();
			});

			auto find = std::find_if(m_Container.begin(), m_Container.end(), [&](std::weak_ptr<T> inTask) {
				return inTask._Get() == task._Get();
			});

			//�܂܂�Ă����
			if (find != m_Container.end())
				return;

			m_Container.emplace_back(task);
		};

		virtual void removeTask(std::weak_ptr<T> task) {

			m_DeleteContainer.emplace_back(task);
		};

		void clear() {
			m_Container.clear();
			m_DeleteContainer.clear();
		};

	protected:
		/**
		* @brief		�����^�X�N�폜
		*/
		virtual void checkDelete() {

			m_Container.remove_if([&](std::weak_ptr<T> _task) {
				return _task.expired();
			});

			//�폜�Ώۂ��f���[�g
			for (auto deleteTask : m_DeleteContainer) {
				m_Container.remove_if([&](std::weak_ptr<T> _task) {
					////�����|�C���^���폜
					//return deleteTask.lock().get() == _task.lock().get();
					if (deleteTask.lock().get() == _task.lock().get()) {
						return true;
					}
					return false;
				});
			}
			m_DeleteContainer.clear();
		}

		/**
		* @brief			�R���e�i�ɓ����Ă���R���|�[�l���g����G���e�B�e�B�[�����ׂĎ��o��
		*/
		std::vector<std::weak_ptr<Entity>> getContainerEntity() {
			std::vector<std::weak_ptr<Entity>> result;

			result.reserve(m_Container.size());
			for (auto comp : m_Container) {
				result.emplace_back(comp.lock()->getGameObj());
			}
		}

		void callOrderSort() {
			m_Container.sort([](const std::weak_ptr<T>& left, const std::weak_ptr<T>& right) {
				return left.lock()->getCallOrder() < right.lock()->getCallOrder();
			});
		}

	protected:
		std::list <std::weak_ptr<T>> m_Container;
		//!�폜�p�R���e�i
		std::list <std::weak_ptr<T>> m_DeleteContainer;

	};

}

