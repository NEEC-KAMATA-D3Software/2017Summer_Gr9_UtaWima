#pragma once
#include<Windows.h>
#include<string>
#include<Source\Util\Type.h>
#include"../../Util/Type.h"

namespace framework {

	using Proc = LRESULT (CALLBACK *)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	class WindowFactory
	{
	public:
		WindowFactory(HINSTANCE hInstance);
		~WindowFactory();

		HWND createWnd(LPCSTR className, LPCSTR title, util::Vec2 size);
		HWND createWnd(LPCSTR className, LPCSTR title, util::Vec2 pos, util::Vec2 size);

		//���݂̐ݒ�ŃN���X��o�^����
		void regist();

		//�^�C�g�����Z�b�g
		//TEXT�}�N�����g���ė��p����Ƃ悢
		void setClassName(LPCSTR className);

		void setProc(Proc proc);

		void setStyle(UINT style);

		void setMenuName(LPSTR menuName);

		void setMenuName(UINT nemeName);

		//�A�C�R���Z�b�g
		void setIcon();

		//�J�[�\�����Z�b�g
		void setCursor();

		//�E�B���h�E�N���C�A���g�̈�擾
		util::Vec2 getWindowSize();

	private:
		/**
		* @brief		�E�B���h�E�̕����l�������N���C�A���g�̈�̌v�Z
		*/
		util::Vec2 clientSize(HWND hwnd,util::Vec2& size);

	private:
		WNDCLASS m_WndClass;
	};

}