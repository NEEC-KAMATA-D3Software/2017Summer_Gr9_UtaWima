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
		//ボタンが押されたときの動作
		ICommand* m_Xbutton;
		ICommand* m_Ybutton;
		ICommand* m_Bbutton;
		ICommand* m_Abutton;
	};


}