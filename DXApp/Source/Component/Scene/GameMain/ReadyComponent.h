#pragma once
#include<Framework.h>
#include<vector>

namespace component {

	class SpriteRenderComponent;
	class FadeComponent;

	class ReadyComponent : public framework::UpdateComponent 
	{
	public:
		ReadyComponent();
		~ReadyComponent();

		void init()override;

		// UpdateComponent を介して継承されました
		virtual void update() override;

		bool isTimerEnd();

		bool isCountEnd();

		//Goのタイミングで動けるようにするため
		bool isGo();

		void clear()override;

	private:
		void count();

		void finish();

		void incrementAction();

		void firstAction();

		void scaleChange();

		void alphaChange();

		std::weak_ptr<SpriteRenderComponent> findRender(const std::string& name, std::list < std::weak_ptr<SpriteRenderComponent>> renders);

	private:
		util::Timer m_Timer;
		int  m_Counter;
		bool m_IsFirst;

		std::weak_ptr<SpriteRenderComponent> m_pCurrentRenderer;
		std::vector < std::weak_ptr<SpriteRenderComponent>> m_Renderers;
		std::weak_ptr<FadeComponent> m_pFade;
	};



}