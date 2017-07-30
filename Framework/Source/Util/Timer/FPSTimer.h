#pragma once


namespace util {

	class FPSTimer
	{
	public:
		FPSTimer();
		FPSTimer(int fps);
		~FPSTimer();

		void update();
		int waiteTime();
		int getFPS();
	private:
		unsigned long m_StartTime;
		unsigned long m_EndTime;
		int m_Count;
		//!�ۂ�����FP�̒lS
		const int m_Fps;
		//!���݂�FPS
		int m_CurrentFps;
	};



}