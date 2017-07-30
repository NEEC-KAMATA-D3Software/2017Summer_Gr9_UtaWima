#include"FadeComponent.h"


namespace component {


	FadeComponent::FadeComponent()
	{
	}

	FadeComponent::~FadeComponent()
	{
	}

	void FadeComponent::init()
	{
		m_pTimer->init();
		m_OnStart = false;
		//デフォルトはフェードイン onStartで切り替わるので逆で初期化
		m_IsFadeIn = false;
		m_pSprite = m_Entity.lock()->getComponent<SpriteRenderComponent>();
		onStart();
	}

	void FadeComponent::update()
	{
		if (!m_OnStart)return;

		if (m_pTimer->isEnd()) {
			m_OnStart = false;
			return;
		}

		timerUpdate();

		if (m_IsFadeIn)
			m_pSprite.lock()->setAlpha(util::bezierCurve(m_pTimer->rate(),0.0f,1.0f,1.0f));
		else
			m_pSprite.lock()->setAlpha(1 - util::bezierCurve(m_pTimer->rate(), 0.0f, 1.0f, 1.0f));
	}

	void FadeComponent::setParam(const std::vector<std::string>& param)
	{
		m_pTimer = std::make_unique < util::Timer >(atoi(param[0].c_str()));
	}

	void FadeComponent::onStart()
	{
		if (!isStart())return;
		m_OnStart = true;
		m_IsFadeIn = !m_IsFadeIn;
		m_pTimer->init();
	}		

	float FadeComponent::fadeRate()
	{
		return m_pTimer->rate();
	}

	bool FadeComponent::isStart()
	{
		return !m_OnStart;
	}

	bool FadeComponent::isOutFade()
	{
		return !m_IsFadeIn;
	}

	bool FadeComponent::isInFade()
	{
		return m_IsFadeIn;
	}

	bool FadeComponent::isEnd()
	{
		return m_pTimer->isEnd();
	}

	void FadeComponent::timerUpdate()
	{
		m_pTimer->update();
	}

}