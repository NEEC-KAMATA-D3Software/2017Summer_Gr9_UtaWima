#pragma once
#include<Windows.h>
#include<XInput.h>
#include<memory>
#include<functional>
#include<Source\Util\Type.h>

/**
* @file	GamePad.h
* @brief	�Q�[���p�b�h�N���X
* @authro	���{�D�P
* @date	2017/03/27
*/

namespace framework {

	enum class ButtonCode : unsigned short
	{
		A = XINPUT_GAMEPAD_A,
		B = XINPUT_GAMEPAD_B,
		Y = XINPUT_GAMEPAD_Y,
		X = XINPUT_GAMEPAD_X,
		//!�X�^�[�g�{�^�����������Ƃ�
		Start = XINPUT_GAMEPAD_START,
		//!�o�b�N�{�^�����������Ƃ�
		Back = XINPUT_GAMEPAD_BACK,
		//!�E�X�e�B�b�N���������񂾂Ƃ�
		RightThumb = XINPUT_GAMEPAD_RIGHT_THUMB,
		//!���X�e�B�b�N���������񂾂Ƃ�
		LeftThumb = XINPUT_GAMEPAD_LEFT_THUMB,
		//!RB
		RightShouler = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		//!LB
		LeftShouler = XINPUT_GAMEPAD_LEFT_SHOULDER,

	};

	struct ControllerState {
		//!�R���g���[���̏��
		XINPUT_STATE state;
		//!�ڑ�����Ă��邩�ǂ���
		bool isConect;
	};

	class GamePad{
	public:
		/**
		* @brief	���R���X�g���N�^
		* @dital	���ł̓Q�[���p�b�h�̐���4��
		*/
		GamePad();
		/**
		* @brief				�����t���R���X�g���N�^
		* @parma  controllerNum	�ڑ��ł���R���g���[���̍ő吔
		*/
		GamePad(int controllerNum);
		~GamePad();

		/**
		* @brief				�Q�[���p�b�h�̏�ԍX�V
		*/
		void updateController();

		/**
		* @brief				�{�^����������Ă��邩�m�F
		* @parma  code			������Ă��邩�m���߂����{�^��
		* @parma  padLocation	������Ă��邩�m���߂����{�^��
		*/
		bool getButton(ButtonCode code,int padLocation = 0);

		/**
		* @brief				�{�^����������Ă��邩�g���K�[����
		* @parma  code			������Ă��邩�m���߂����{�^��
		* @parma  padLocation	������Ă��邩�m���߂����{�^��
		*/
		bool getButtonTrriger(ButtonCode code, int padLocation = 0);

		/**
		* @brief				���X�e�B�b�N�̐��l�擾
		* @detail				-32768�`32767�͈̔�
		*/
		util::Vec2 getLeftStick(int padLocation = 0);

		/**
		* @brief				�E�X�e�B�b�N�̐��l�擾
		* @detail				-32768�`32767�͈̔�
		*/
		util::Vec2 getRightStick(int padLocation = 0);

		/**
		* @brief				���͂����邩�ǂ���
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