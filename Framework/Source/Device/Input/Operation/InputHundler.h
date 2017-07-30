#pragma once
#include<list>
#include<memory>
#include"ICommand.h"

namespace framework {

	class InputHundler
	{
	public:
		InputHundler();
		~InputHundler();

	private:
		//ƒ{ƒ^ƒ“‚ª‰Ÿ‚³‚ê‚½‚Æ‚«‚Ì“®ì
		ICommand* m_Xbutton;
		ICommand* m_Ybutton;
		ICommand* m_Bbutton;
		ICommand* m_Abutton;
	};


}