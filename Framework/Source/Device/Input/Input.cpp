#include"Input.h"


namespace framework {

	//TODO �p�b�h�̐���ς�����悤�ɂ���
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