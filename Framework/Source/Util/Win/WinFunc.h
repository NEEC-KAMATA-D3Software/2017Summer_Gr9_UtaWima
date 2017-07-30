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
				//IDYESだったときのみtrueが返る
				return true;
			}
			return false;
		}
		
		/**
		* @brief			イミディエイトウィンドウに文字列を表示する　
		* @param msg		出直したい文字列
		* @detail			デバッグなしの場合は出力されない
		*/
		static void OutLog(const std::string& msg) {
			std::string temp = msg + "\n";
			OutputDebugString(temp.c_str());
		}
	private:

	};
}