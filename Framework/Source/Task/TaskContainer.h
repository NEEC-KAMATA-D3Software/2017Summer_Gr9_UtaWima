#pragma once
#include<list>
#include<memory>
#include<vector>
#include<Source\Entity\Entity.h>
#include<algorithm>

/**
* @file	TaskContainer.h
* @brief	タスクを格納するコンテナ
* @author	高須優輝
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
			//moveTaskを実行してからすぐactiveを呼ぶとdeleteリストに残ってしまうためここでデリートリストから削除する
			m_DeleteContainer.remove_if([&](std::weak_ptr<T> deleteTask) {
				return deleteTask._Get() == task._Get();
			});

			auto find = std::find_if(m_Container.begin(), m_Container.end(), [&](std::weak_ptr<T> inTask) {
				return inTask._Get() == task._Get();
			});

			//含まれていれば
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
		* @brief		無効タスク削除
		*/
		virtual void checkDelete() {

			m_Container.remove_if([&](std::weak_ptr<T> _task) {
				return _task.expired();
			});

			//削除対象をデリート
			for (auto deleteTask : m_DeleteContainer) {
				m_Container.remove_if([&](std::weak_ptr<T> _task) {
					////同じポインタを削除
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
		* @brief			コンテナに入っているコンポーネントからエンティティーをすべて取り出す
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
		//!削除用コンテナ
		std::list <std::weak_ptr<T>> m_DeleteContainer;

	};

}

