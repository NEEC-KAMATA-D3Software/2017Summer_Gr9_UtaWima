#pragma once
#include<Framework.h>

namespace component {

	class SceneChangeComponent : public framework::Component
	{
	public:
		SceneChangeComponent();
		~SceneChangeComponent();

		/**
		* @brief		初期化
		*/
		void init()override;

		/**
		* @brief		シーンの終わりを通知
		*/
		void changeSceneNotification();

		/**
		* @brief		シーン切り替え
		*/
		void changeScene();

	private:
		bool m_IsEnd;


	};
}