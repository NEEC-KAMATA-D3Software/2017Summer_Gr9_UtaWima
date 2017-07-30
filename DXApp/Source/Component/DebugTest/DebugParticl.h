#pragma once
#include<Source\Component\UpdateComponent.h>
#include<Source\Util\Math\Math.h>
#include<Source\Util\Timer\Timer.h>
#include<Source\Entity\Entity.h>

/**
* @file	DebugParticl.h
* @brief	ディレクショナルライトを操作するコンポーネント
* @authro	高須優輝
* @date	2017/04/14
*/


namespace component {

	class DebugParticl : public framework::UpdateComponent
	{
	public:
		DebugParticl():m_Timer(60 * 2)
		{
			m_Min = -200;
		
			m_Max = 200;
		};

		~DebugParticl() {};

		// UpdateComponent を介して継承されました
		virtual void update() override {
		
			m_Timer.update();

			auto trans = m_Entity.lock()->getTransform();

			trans->m_Position.z = util::lerp(m_Timer.rate(), m_Min, m_Max);

			if (m_Timer.isEnd()) {
				m_Timer.init();
				float temp = m_Max;
				m_Max = m_Min;
				m_Min = temp;
			}
		
		};

	private:
		util::Timer m_Timer;
		float m_Max,m_Min;

	};

}