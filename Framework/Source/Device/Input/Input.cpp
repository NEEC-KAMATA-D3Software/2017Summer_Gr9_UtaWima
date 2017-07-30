#include"Input.h"


namespace framework {

	//TODO ƒpƒbƒh‚Ì”‚ğ•Ï‚¦‚ç‚ê‚é‚æ‚¤‚É‚·‚é
	GamePad Input::m_GamePad(4);

	Input::Input()
	{
	}

	Input::~Input()
	{
	}

	GamePad& Input::getGamePad()
	{
		return m_GamePad;
	}

	void Input::update()
	{
		m_GamePad.updateController();
	}




}