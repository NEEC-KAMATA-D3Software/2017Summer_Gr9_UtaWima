#pragma once
#include"GamePad.h"
#include<functional>
#include<list>
#include<utility>

namespace framework {

	class Input
	{
	public:
		~Input();

		static GamePad& getGamePad();

		static void update();

	private:
		Input();

	private:
		static GamePad m_GamePad;

	};


}