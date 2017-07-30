#include "Buttom.h"
#include<Framework.h>
#include<Source\Component\FadeComponent.h>

namespace component {

	Buttom::Buttom()
	{

	}

	Buttom::~Buttom()
	{
	}

	void Buttom::init()
	{

		m_pFade = framework::Entity::findGameObj("Fade").lock()->getComponent<FadeComponent>();
	}
	
	void Buttom::onEvent()
	{
		if (!m_pFade.lock()->isEnd())return;

		m_pFade.lock()->onStart();

		//m_pSceneChanger.lock()->changeSceneNotification();
	}
}