#pragma once
#include"TaskContainer.h"
#include<Source\Component\UpdateComponent.h>

/**
* @file	    UpdateContainer.h
* @brief	アップデートタスクを格納するコンテナ
* @authro	高須優輝
* @date	2017/3/19
*/

namespace component {
	class SceneChangeComponent;
}


namespace framework {

	class UpdateContainer : public TaskContainer<UpdateComponent>
	{
	public:
		UpdateContainer();
		~UpdateContainer();

		void init();

		void update();

	private:
		std::weak_ptr<component::SceneChangeComponent> m_pSceneChanger;

	};

}