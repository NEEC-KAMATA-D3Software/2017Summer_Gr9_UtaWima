#include"FPSTimer.h"
#include<Windows.h>

namespace util {


	FPSTimer::FPSTimer()
		:FPSTimer(60)
	{
		m_StartTime = 0;
		m_EndTime = 0;
		m_CurrentFps = 0;
		m_Count = 0;
	}

	FPSTimer::FPSTimer(int fps)
		:m_Fps(fps)
	{
	}

	FPSTimer::~FPSTimer()
	{
	}

	void FPSTimer::update()
	{
		//•½‹ÏƒTƒ“ƒvƒ‹‰ñ”
		const int aveN = 60;
		if (m_Count == 0) {
			m_StartTime = GetTickCount();
		}

		if(m_Count == aveN){
			unsigned long time = GetTickCount();
			m_CurrentFps = 1000.0f / ((time - m_StartTime) / (float)aveN);
			m_Count = 0;
			m_StartTime = time;
		}
		m_Count += 1;
	}

	int FPSTimer::waiteTime()
	{
		//!‚©‚©‚Á‚½ŠÔ
		auto tookTime = GetTickCount() - m_StartTime;
		return m_Count * 1000.0f / m_Fps - tookTime;
	}

	int FPSTimer::getFPS()
	{
		return m_CurrentFps;
	}

}