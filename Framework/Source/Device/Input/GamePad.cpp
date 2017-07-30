#include"GamePad.h"
#include<Source\Util\WrapFunc.h>
#include<assert.h>

namespace framework {

	GamePad::GamePad()
		:GamePad(1)
	{
	}

	GamePad::GamePad(int controllerNum)
		: m_MaxControllers(controllerNum)
	{
		m_pControllers = std::make_unique<ControllerState[]>(m_MaxControllers);
		m_pPreControllers = std::make_unique<ControllerState[]>(m_MaxControllers);
		updateController();
	}

	GamePad::~GamePad()
	{
	}

	void GamePad::updateController()
	{
		padEach([&](DWORD i) {
			DWORD dwResult;
			m_pPreControllers[i].state = m_pControllers[i].state;
			m_pPreControllers[i].isConect = m_pControllers[i].isConect;
			dwResult = XInputGetState(i, &m_pControllers[i].state);
			m_pControllers[i].isConect = dwResult == ERROR_SUCCESS;
		});
	}

	bool GamePad::getButton(ButtonCode code, int padLocation)
	{
		//trueでなければ問題があるので何もしない
		if (!isConnect(padLocation))return false;

		WORD buttons = m_pControllers[padLocation].state.Gamepad.wButtons;
		
		return buttons & static_cast<unsigned short>(code);
	}

	bool GamePad::getButtonTrriger(ButtonCode code, int padLocation)
	{
		//trueでなければ問題があるので何もしない
		if (!isConnect(padLocation))return false;

		WORD buttons = m_pControllers[padLocation].state.Gamepad.wButtons;
		WORD preButtons = m_pPreControllers[padLocation].state.Gamepad.wButtons;

		bool isDown = buttons & static_cast<unsigned short>(code);
		bool isPreDown = preButtons & static_cast<unsigned short>(code);
		return isDown && !isPreDown;
	}

	util::Vec2 GamePad::getLeftStick(int padLocation)
	{
		//trueでなければ問題があるので何もしない
		if (!isConnect(padLocation))return util::Vec2();
		auto& pad = m_pControllers[padLocation].state.Gamepad;

		auto input = noizeCut(util::Vec2(pad.sThumbLX, pad.sThumbLY));

		return input;
	}

	util::Vec2 GamePad::getRightStick(int padLocation)
	{
		//trueでなければ問題があるので何もしない
		if (!isConnect(padLocation))return util::Vec2();
		auto& pad = m_pControllers[padLocation].state.Gamepad;

		auto input = noizeCut(util::Vec2(pad.sThumbRX, pad.sThumbRY));

		return input;
	}

	bool GamePad::isStickNoe(const util::Vec2 & vec)
	{
		//xy両方とも一定以上入力されていなければtrue
		if (vec.x == 0 && vec.y == 0)
			return true;
		return false;
	}

	const int GamePad::getConectNum()
	{
		int count = 0;
		padEach([&](DWORD i) {
			if (m_pControllers[i].isConect) {
				count++;
			}
		});
		return count;
	}

	bool GamePad::isConnect(int location)
	{
		assert(location < m_MaxControllers && "パッドの最大数を超えています");
		if (location > m_MaxControllers)return false;
		//接続されていなければ何もしない
		return m_pControllers[location].isConect;
	}

	void GamePad::padEach(const std::function<void(DWORD i)>& action)
	{
		util::foreach<DWORD>(m_MaxControllers, [&](int i) {
			action(i);
		});
	}

	util::Vec2 GamePad::noizeCut(const util::Vec2 & vec)
	{
		util::Vec2 result(0,0);
		
		//指定した以上の数値でなければ0を返す
		if (abs(vec.x) >= m_MaxHalfStickNum) {
			result.x = vec.x;
		}
		if (abs(vec.y) >= m_MaxHalfStickNum) {
			result.y = vec.y;
		}
		return result;
	}




}