#pragma once
#include<Windows.h>
#include<XInput.h>
#include<memory>
#include<functional>
#include<Source\Util\Type.h>

/**
* @file	GamePad.h
* @brief	ゲームパッドクラス
* @authro	高須優輝
* @date	2017/03/27
*/

namespace framework {

	enum class ButtonCode : unsigned short
	{
		A = XINPUT_GAMEPAD_A,
		B = XINPUT_GAMEPAD_B,
		Y = XINPUT_GAMEPAD_Y,
		X = XINPUT_GAMEPAD_X,
		//!スタートボタンを押したとき
		Start = XINPUT_GAMEPAD_START,
		//!バックボタンを押したとき
		Back = XINPUT_GAMEPAD_BACK,
		//!右スティックを押し込んだとき
		RightThumb = XINPUT_GAMEPAD_RIGHT_THUMB,
		//!左スティックを押し込んだとき
		LeftThumb = XINPUT_GAMEPAD_LEFT_THUMB,
		//!RB
		RightShouler = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		//!LB
		LeftShouler = XINPUT_GAMEPAD_LEFT_SHOULDER,

	};

	struct ControllerState {
		//!コントローラの状態
		XINPUT_STATE state;
		//!接続されているかどうか
		bool isConect;
	};

	class GamePad{
	public:
		/**
		* @brief	基底コンストラクタ
		* @dital	基底ではゲームパッドの数は4個
		*/
		GamePad();
		/**
		* @brief				引数付きコンストラクタ
		* @parma  controllerNum	接続できるコントローラの最大数
		*/
		GamePad(int controllerNum);
		~GamePad();

		/**
		* @brief				ゲームパッドの状態更新
		*/
		void updateController();

		/**
		* @brief				ボタンが押されているか確認
		* @parma  code			押されているか確かめたいボタン
		* @parma  padLocation	押されているか確かめたいボタン
		*/
		bool getButton(ButtonCode code,int padLocation = 0);

		/**
		* @brief				ボタンが押されているかトリガー判定
		* @parma  code			押されているか確かめたいボタン
		* @parma  padLocation	押されているか確かめたいボタン
		*/
		bool getButtonTrriger(ButtonCode code, int padLocation = 0);

		/**
		* @brief				左スティックの数値取得
		* @detail				-32768～32767の範囲
		*/
		util::Vec2 getLeftStick(int padLocation = 0);

		/**
		* @brief				右スティックの数値取得
		* @detail				-32768～32767の範囲
		*/
		util::Vec2 getRightStick(int padLocation = 0);

		/**
		* @brief				入力があるかどうか
		*/
		bool isStickNoe(const util::Vec2& vec);

		const int getConectNum();

	public:
		static const short m_MaxStickNum = 32767;
		static const short m_MinStickNum = -32768;
		static const short m_MaxHalfStickNum = m_MaxStickNum/2;
		static const short m_MinHalfStickNum = m_MinStickNum/2;

	private:
		bool isConnect(int location);
		void padEach(const std::function<void(DWORD i)>& action);
		util::Vec2 noizeCut(const util::Vec2& vec);

	private:
		const unsigned int m_MaxControllers;
		std::unique_ptr<ControllerState[]> m_pControllers;
		std::unique_ptr<ControllerState[]> m_pPreControllers;
	};

	



}