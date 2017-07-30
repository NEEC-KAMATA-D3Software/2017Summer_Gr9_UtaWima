#pragma once
#include<Framework.h>
#include<vector>
#include<Source\Util\Selector.h>
#include<memory>
#include<Source\Util\Timer\Timer.h>

namespace component {

	class SceneChangeComponent;
	class FadeComponent;


	class Menu : public framework::UpdateComponent
	{
	public:
		Menu();
		~Menu();

		void init()override;

		// UpdateComponent を介して継承されました
		virtual void update() override;

		void clear()override;

		/**
		* @brief		パラメータをセット
		* @param param  文字列パラメータ
		*/
		virtual void setParam(const std::vector<std::string>& param);

	private:
		/**
		* @brief		UIのスケールを変える
		*/
		void scaleChange();

	private:
		void menuUpdate();

		bool isUpTrriger(util::Vec2& vec);

		bool isDownTrriger(util::Vec2& vec);

	private:
		std::vector<framework::WeakEntity> m_MenuObj;
		framework::WeakEntity m_CurrentUI;
		framework::WeakEntity m_PreUI;
		std::unique_ptr<util::Selector> m_Selector;

		std::weak_ptr<SceneChangeComponent> m_pSceneChanger;
		std::weak_ptr<FadeComponent> m_pFade;

		bool m_IsPreUp;
		bool m_IsCurrentUp;
		bool m_IsPreDown;
		bool m_IsCurrentDown;
	};




}
