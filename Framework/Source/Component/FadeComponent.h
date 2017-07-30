#pragma once
#include<Framework.h>
#include<memory>
#include<Source\Component\SpriteRenderComponent.h>

namespace component {

	class FadeComponent : public framework::UpdateComponent
	{
	public:
		FadeComponent();
		~FadeComponent();

		void init()override;

		// UpdateComponent ����Čp������܂���
		virtual void update() override;

		/**
		* @brief		�p�����[�^���Z�b�g
		* @param param  ������p�����[�^
		*/
		virtual void setParam(const std::vector<std::string>& param);

		void onStart();

		float fadeRate();

		bool isStart();

		bool isOutFade();

		bool isInFade();

		bool isEnd();

	private:
		void timerUpdate();

	private:
		std::weak_ptr<SpriteRenderComponent> m_pSprite;
		std::unique_ptr<util::Timer> m_pTimer;
		bool m_OnStart;
		bool m_IsFadeIn;

	};


}