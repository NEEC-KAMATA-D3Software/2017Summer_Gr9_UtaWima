#pragma once


namespace util {

	class ProcessTimer
	{
	public:
		ProcessTimer();
		~ProcessTimer();

		void begin();

		void end();

		double getTime();

	private:
		double m_StartTime;
		double m_EndTime;
	};

}
