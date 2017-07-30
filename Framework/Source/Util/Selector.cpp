#include"Selector.h"


namespace util {

	Selector::Selector(const int selectNum)
		:m_SelectNum(selectNum),
		m_DownOffst(m_SelectNum - 1)
	{
		init();
	}

	Selector::~Selector()
	{
	}

	void Selector::init()
	{
		m_CurrentSelect = 0;
	}

	void Selector::enter()
	{
		m_CurrentSelect = (m_CurrentSelect+1) % m_SelectNum;
	}

	void Selector::back()
	{
		m_CurrentSelect = (m_CurrentSelect + m_DownOffst) % m_SelectNum;
	}

	const int Selector::getSelectNum()
	{
		return m_CurrentSelect;
	}
}