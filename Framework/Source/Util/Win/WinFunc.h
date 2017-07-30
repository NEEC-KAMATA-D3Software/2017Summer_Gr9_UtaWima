#pragma once
#include<windows.h>
#include<string>

namespace util {
	class WinFunc
	{
	public:
		WinFunc() {};
		~WinFunc() {};
		static void DrawMessageBox(HWND hWnd, LPCSTR title, LPCSTR message) {
			MessageBox(hWnd, message, title, MB_ICONINFORMATION);
		}

		static bool DrawQuestion(HWND hWnd, LPCSTR title, LPCSTR message) {
			if (MessageBox(hWnd, message, title, MB_YESNO | MB_ICONQUESTION) == IDYES) {
				//IDYES�������Ƃ��̂�true���Ԃ�
				return true;
			}
			return false;
		}
		
		/**
		* @brief			�C�~�f�B�G�C�g�E�B���h�E�ɕ������\������@
		* @param msg		�o��������������
		* @detail			�f�o�b�O�Ȃ��̏ꍇ�͏o�͂���Ȃ�
		*/
		static void OutLog(const std::string& msg) {
			std::string temp = msg + "\n";
			OutputDebugString(temp.c_str());
		}
	private:

	};
}