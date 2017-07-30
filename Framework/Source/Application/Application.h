#pragma once
#include<Windows.h>
#include<Source\Util\Timer\FPSTimer.h>
/**
* @file	    Application.h
* @brief	�A�v���P�[�V�����̊�{�N���X
* @authro	���{�D�P
* @date	2017/2/20
*/

namespace framework {

	void loadMaterial();

	class Application
	{
	public:
		Application(HINSTANCE instance);
		~Application();

		virtual void init();

		int run();


		static bool m_Exit;
	private:
		/**
		* @brief	�A�b�v�f�[�g�R�[���o�b�N
		*/
		virtual void update() = 0;

		/**
		* @brief	�`��R�[���o�b�N
		*/
		virtual void draw() = 0;

	private:
		/**
		* @brief			���b�Z�[�W����
		* @param msg		���b�Z�[�W�擾�i�[���邽�߂̕ϐ�
		* @return			���b�Z�[�W�擾�ɐ����������ǂ���
		*/
		bool messageProcess(MSG* msg);

	protected:
		/**
		* @brief			�t���[�����[�N���ŗ��p����K�{���\�[�X��ǂݍ���
		*/
		void importMaterial();

	protected:
		HINSTANCE m_hInstance;
		HWND m_hWnd;
		util::FPSTimer m_FpsTimer;
	};

}

