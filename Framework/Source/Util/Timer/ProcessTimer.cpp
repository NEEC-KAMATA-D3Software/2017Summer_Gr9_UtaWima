#include"ProcessTimer.h"
#include<Windows.h>

namespace util {

	ProcessTimer::ProcessTimer()
	{
	}

	ProcessTimer::~ProcessTimer()
	{
	}

	void ProcessTimer::begin()
	{
		m_StartTime = GetTickCount();
	}

	void ProcessTimer::end()
	{
		m_EndTime = GetTickCount();
	}

	double ProcessTimer::getTime()
	{
		return m_EndTime - m_StartTime;
	}
}