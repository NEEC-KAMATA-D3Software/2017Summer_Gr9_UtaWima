#pragma once
#include<Framework.h>

namespace component {

	class SceneChangeComponent : public framework::Component
	{
	public:
		SceneChangeComponent();
		~SceneChangeComponent();

		/**
		* @brief		������
		*/
		void init()override;

		/**
		* @brief		�V�[���̏I����ʒm
		*/
		void changeSceneNotification();

		/**
		* @brief		�V�[���؂�ւ�
		*/
		void changeScene();

	private:
		bool m_IsEnd;


	};
}