#include"Input.h"


namespace framework {

	//TODO パッドの数を変えられるようにする
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