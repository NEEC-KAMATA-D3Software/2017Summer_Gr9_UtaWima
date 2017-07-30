#pragma once


namespace util {

	class Timer
	{
	public:
		Timer();
		Timer(int time);
		~Timer();

		void init();

		void update();

		bool isEnd();

		float rate();

		int getTime();

	private:
		const int m_MaxTime;
		int m_CurrentTime;
	};


}