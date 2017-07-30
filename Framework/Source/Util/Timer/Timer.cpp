#include"Timer.h"


namespace util {


	Timer::Timer()
		:Timer(60)
	{
	}

	Timer::Timer(int time)
		: m_MaxTime(time)
	{
		init();
	}

	Timer::~Timer()
	{
	}

	void Timer::init()
	{
		m_CurrentTime = m_MaxTime;
	}

	void Timer::update()
	{
		m_CurrentTime = !isEnd() ? m_CurrentTime - 1 : 0;
	}

	bool Timer::isEnd()
	{
		return m_CurrentTime == 0;
	}

	float Timer::rate()
	{
		return m_CurrentTime / (float)m_MaxTime;
	}

	int Timer::getTime()
	{
		return m_CurrentTime;
	}

}